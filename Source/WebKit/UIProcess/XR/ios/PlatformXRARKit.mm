/*
 * Copyright (C) 2023 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "config.h"
#import "PlatformXRARKit.h"

#if ENABLE(WEBXR) && USE(ARKITXR_IOS)

#import "APIUIClient.h"
#import "WKARPresentationSession.h"
#import "WebPageProxy.h"

#import "ARKitSoftLink.h"

namespace WebKit {

ARKitCoordinator::ARKitCoordinator()
{
    ASSERT(RunLoop::isMain());
}

void ARKitCoordinator::getPrimaryDeviceInfo(DeviceInfoCallback&& callback)
{
    RELEASE_LOG(XR, "ARKitCoordinator::getPrimaryDeviceInfo");
    ASSERT(RunLoop::isMain());

    if (![getARWorldTrackingConfigurationClass() isSupported]) {
        RELEASE_LOG_ERROR(XR, "ARKitCoordinator: WorldTrackingConfiguration is not supported");
        return callback(std::nullopt);
    }

    NSArray<ARVideoFormat *> *supportedVideoFormats = [getARWorldTrackingConfigurationClass() supportedVideoFormats];
    if (![supportedVideoFormats count])
        return callback(std::nullopt);

    CGSize recommendedResolution = supportedVideoFormats[0].imageResolution;

    XRDeviceInfo deviceInfo;
    deviceInfo.identifier = m_deviceIdentifier;
    deviceInfo.supportsOrientationTracking = true;
    deviceInfo.supportsStereoRendering = false;
    deviceInfo.recommendedResolution = WebCore::IntSize(recommendedResolution.width, recommendedResolution.height);
    deviceInfo.arFeatures.append(PlatformXR::SessionFeature::ReferenceSpaceTypeUnbounded);
    deviceInfo.arFeatures.append(PlatformXR::SessionFeature::ReferenceSpaceTypeLocalFloor);
    deviceInfo.arFeatures.append(PlatformXR::SessionFeature::ReferenceSpaceTypeLocal);
    deviceInfo.arFeatures.append(PlatformXR::SessionFeature::ReferenceSpaceTypeViewer);

    callback(WTFMove(deviceInfo));
}

void ARKitCoordinator::requestPermissionOnSessionFeatures(WebPageProxy& page, const WebCore::SecurityOriginData& securityOriginData, PlatformXR::SessionMode mode, const PlatformXR::Device::FeatureList& granted, const PlatformXR::Device::FeatureList& consentRequired, const PlatformXR::Device::FeatureList& consentOptional, const PlatformXR::Device::FeatureList& requiredFeaturesRequested, const PlatformXR::Device::FeatureList& optionalFeaturesRequested, FeatureListCallback&& callback)
{
    RELEASE_LOG(XR, "ARKitCoordinator::requestPermissionOnSessionFeatures");
    if (mode == PlatformXR::SessionMode::Inline) {
        callback(granted);
        return;
    }

    page.uiClient().requestPermissionOnXRSessionFeatures(page, securityOriginData, mode, granted, consentRequired, consentOptional, requiredFeaturesRequested, optionalFeaturesRequested, [callback = WTFMove(callback)](std::optional<Vector<PlatformXR::SessionFeature>> userGranted) mutable {
        callback(WTFMove(userGranted));
    });
}

void ARKitCoordinator::startSession(WebPageProxy& page, WeakPtr<SessionEventClient>&& sessionEventClient, const WebCore::SecurityOriginData&, PlatformXR::SessionMode mode, const PlatformXR::Device::FeatureList&)
{
    RELEASE_LOG(XR, "ARKitCoordinator::startSession");
    ASSERT(RunLoop::isMain());
    ASSERT(mode == PlatformXR::SessionMode::ImmersiveAr);

    WTF::switchOn(m_state,
        [&](Idle&) {
            createSessionIfNeeded();

            auto* presentingViewController = page.uiClient().presentingViewController();
            if (!presentingViewController) {
                RELEASE_LOG_ERROR(XR, "ARKitCoordinator: failed to obtain presenting ViewController from page.");
                return;
            }

            auto presentationSessionDesc = adoptNS([WKARPresentationSessionDescriptor new]);
            [presentationSessionDesc setPresentingViewController:presentingViewController];

            auto presentationSession = adoptNS(createPresesentationSession(m_session.get(), presentationSessionDesc.get()));

            m_state = Active {
                .pageIdentifier = page.webPageID(),
                .sessionEventClient = WTFMove(sessionEventClient),
                .presentationSession = WTFMove(presentationSession),
                .renderThread = Thread::create("ARKitCoordinator session renderer", [this] { renderLoop(); }),
                .renderSemaphore = Box<BinarySemaphore>::create(),
            };
        },
        [&](Active&) {
            RELEASE_LOG_ERROR(XR, "ARKitCoordinator: an existing immersive session is active");
            if (sessionEventClient)
                sessionEventClient->sessionDidEnd(m_deviceIdentifier);
        },
        [&](Terminating&) { });
}

void ARKitCoordinator::endSessionIfExists(WebPageProxy& page)
{
    RELEASE_LOG(XR, "ARKitCoordinator::endSessionIfExists");
    ASSERT(RunLoop::isMain());

    WTF::switchOn(m_state,
        [&](Idle&) { },
        [&](Active& active) {
            if (active.pageIdentifier != page.webPageID()) {
                RELEASE_LOG(XR, "ARKitCoordinator: trying to end an immersive session owned by another page");
                return;
            }

            if (active.onFrameUpdate)
                active.onFrameUpdate({ });

            active.renderSemaphore->signal();

            m_state = Terminating { WTFMove(active.sessionEventClient) };
        },
        [&](Terminating&) { });
}

void ARKitCoordinator::scheduleAnimationFrame(WebPageProxy& page, PlatformXR::Device::RequestFrameCallback&& onFrameUpdateCallback)
{
    RELEASE_LOG(XR, "ARKitCoordinator::scheduleAnimationFrame");
    WTF::switchOn(m_state,
        [&](Idle&) {
            RELEASE_LOG(XR, "ARKitCoordinator: trying to schedule frame update for an inactive session");
            onFrameUpdateCallback({ });
        },
        [&](Active& active) {
            if (active.pageIdentifier != page.webPageID()) {
                RELEASE_LOG(XR, "ARKitCoordinator: trying to schedule frame update for session owned by another page");
                return;
            }

            active.onFrameUpdate = WTFMove(onFrameUpdateCallback);
            active.renderSemaphore->signal();
        },
        [&](Terminating&) {
            RELEASE_LOG(XR, "ARKitCoordinator: trying to schedule frame for terminating session");
            onFrameUpdateCallback({ });
        });
}

void ARKitCoordinator::submitFrame(WebPageProxy& page)
{
    RELEASE_LOG(XR, "ARKitCoordinator::submitFrame");
    ASSERT(RunLoop::isMain());
    WTF::switchOn(m_state,
        [&](Idle&) {
            RELEASE_LOG(XR, "ARKitCoordinator: trying to submit frame update for an inactive session");
        },
        [&](Active& active) {
            if (active.pageIdentifier != page.webPageID()) {
                RELEASE_LOG(XR, "ARKitCoordinator: trying to submit frame update for session owned by another page");
                return;
            }
        },
        [&](Terminating&) {
            RELEASE_LOG(XR, "ARKitCoordinator: trying to submit frame update for a terminating session");
        });
}

void ARKitCoordinator::createSessionIfNeeded()
{
    ASSERT(RunLoop::isMain());
    RELEASE_LOG(XR, "ARKitCoordinator::createSessionIfNeeded");

    if (m_session)
        return;

    m_session = adoptNS([WebKit::allocARSessionInstance() init]);
}

void ARKitCoordinator::currentSessionHasEnded()
{
    ASSERT(RunLoop::isMain());
    RELEASE_LOG(XR, "ARKitCoordinator::currentSessionHasEnded");

    if (auto* terminating = std::get_if<Terminating>(&m_state)) {
        auto& sessionEventClient = terminating->sessionEventClient;
        if (sessionEventClient)
            sessionEventClient->sessionDidEnd(m_deviceIdentifier);
    }

    RELEASE_LOG(XR, "... immersive session ended");
    m_state = Idle { };
}

void ARKitCoordinator::renderLoop()
{
    for (;;) {
        auto* maybeActive = std::get_if<Active>(&m_state);
        if (!maybeActive)
            break;

        auto& active = *maybeActive;
        active.renderSemaphore->wait();
        if (!active.onFrameUpdate)
            break;

        @autoreleasepool {
            [active.presentationSession startFrame];

            PlatformXR::Device::FrameData frameData = { };
            callOnMainRunLoop([callback = WTFMove(active.onFrameUpdate), frameData = WTFMove(frameData)]() mutable {
                callback(WTFMove(frameData));
            });

            [active.presentationSession present];
        }
    }

    callOnMainRunLoop([this]() {
        currentSessionHasEnded();
    });
}

} // namespace WebKit

#endif // ENABLE(WEBXR) && USE(ARKITXR_IOS)
