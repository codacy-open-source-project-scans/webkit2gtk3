/*
 * Copyright (C) 2015-2020 Apple Inc. All rights reserved.
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

#pragma once

#include "APIObject.h"
#include "WebURLSchemeHandler.h"
#include <WebCore/ContentSecurityPolicy.h>
#include <WebCore/ShouldRelaxThirdPartyCookieBlocking.h>
#include <wtf/Forward.h>
#include <wtf/GetPtr.h>
#include <wtf/HashMap.h>
#include <wtf/RobinHoodHashSet.h>
#include <wtf/text/WTFString.h>

#if PLATFORM(IOS_FAMILY)
OBJC_PROTOCOL(_UIClickInteractionDriving);
#include <wtf/RetainPtr.h>
#endif

namespace WebKit {
class BrowsingContextGroup;
class VisitedLinkStore;
class WebPageGroup;
class WebPageProxy;
class WebPreferences;
class WebProcessPool;
class WebUserContentControllerProxy;
class WebsiteDataStore;

struct GPUProcessPreferencesForWebProcess;
struct NetworkProcessPreferencesForWebProcess;

#if ENABLE(WK_WEB_EXTENSIONS)
class WebExtensionController;
#endif

#if PLATFORM(IOS_FAMILY)
enum class AttributionOverrideTesting : uint8_t {
    NoOverride,
    UserInitiated,
    AppInitiated
};
#endif

}

namespace API {

class ApplicationManifest;
class WebsitePolicies;

class PageConfiguration : public ObjectImpl<Object::Type::PageConfiguration> {
public:
    static Ref<PageConfiguration> create();

    explicit PageConfiguration();
    virtual ~PageConfiguration();

    Ref<PageConfiguration> copy() const;

    // FIXME: The configuration properties should return their default values
    // rather than nullptr.

    WebKit::BrowsingContextGroup& browsingContextGroup();

    WebKit::WebProcessPool* processPool();
    void setProcessPool(RefPtr<WebKit::WebProcessPool>&&);

    WebKit::WebUserContentControllerProxy* userContentController();
    void setUserContentController(RefPtr<WebKit::WebUserContentControllerProxy>&&);

#if ENABLE(WK_WEB_EXTENSIONS)
    const WTF::URL& requiredWebExtensionBaseURL() const;
    void setRequiredWebExtensionBaseURL(WTF::URL&&);

    WebKit::WebExtensionController* webExtensionController() const;
    void setWebExtensionController(RefPtr<WebKit::WebExtensionController>&&);

    WebKit::WebExtensionController* weakWebExtensionController() const;
    void setWeakWebExtensionController(WebKit::WebExtensionController*);
#endif

    WebKit::WebPageGroup* pageGroup();
    void setPageGroup(RefPtr<WebKit::WebPageGroup>&&);

    WebKit::WebPreferences* preferences() const;
    void setPreferences(RefPtr<WebKit::WebPreferences>&&);

    WebKit::WebPageProxy* relatedPage() const;
    void setRelatedPage(RefPtr<WebKit::WebPageProxy>&&);

    WebKit::WebPageProxy* pageToCloneSessionStorageFrom() const;
    void setPageToCloneSessionStorageFrom(WebKit::WebPageProxy*);

    WebKit::VisitedLinkStore* visitedLinkStore();
    void setVisitedLinkStore(RefPtr<WebKit::VisitedLinkStore>&&);

    WebKit::WebsiteDataStore* websiteDataStore();
    RefPtr<WebKit::WebsiteDataStore> protectedWebsiteDataStore();
    void setWebsiteDataStore(RefPtr<WebKit::WebsiteDataStore>&&);

    WebsitePolicies* defaultWebsitePolicies() const;
    void setDefaultWebsitePolicies(RefPtr<WebsitePolicies>&&);

#if PLATFORM(IOS_FAMILY)
    bool canShowWhileLocked() const { return m_data.canShowWhileLocked; }
    void setCanShowWhileLocked(bool canShowWhileLocked) { m_data.canShowWhileLocked = canShowWhileLocked; }

    const RetainPtr<_UIClickInteractionDriving>& clickInteractionDriverForTesting() const { return m_data.clickInteractionDriverForTesting; }
    void setClickInteractionDriverForTesting(RetainPtr<_UIClickInteractionDriving>&& driver) { m_data.clickInteractionDriverForTesting = WTFMove(driver); }
#endif
    bool initialCapitalizationEnabled() { return m_data.initialCapitalizationEnabled; }
    void setInitialCapitalizationEnabled(bool initialCapitalizationEnabled) { m_data.initialCapitalizationEnabled = initialCapitalizationEnabled; }

    std::optional<double> cpuLimit() const { return m_data.cpuLimit; }
    void setCPULimit(double cpuLimit) { m_data.cpuLimit = cpuLimit; }

    bool waitsForPaintAfterViewDidMoveToWindow() const { return m_data.waitsForPaintAfterViewDidMoveToWindow; }
    void setWaitsForPaintAfterViewDidMoveToWindow(bool shouldSynchronize) { m_data.waitsForPaintAfterViewDidMoveToWindow = shouldSynchronize; }

    bool drawsBackground() const { return m_data.drawsBackground; }
    void setDrawsBackground(bool drawsBackground) { m_data.drawsBackground = drawsBackground; }

    bool isControlledByAutomation() const { return m_data.controlledByAutomation; }
    void setControlledByAutomation(bool controlledByAutomation) { m_data.controlledByAutomation = controlledByAutomation; }

    const WTF::String& overrideContentSecurityPolicy() const { return m_data.overrideContentSecurityPolicy; }
    void setOverrideContentSecurityPolicy(const WTF::String& overrideContentSecurityPolicy) { m_data.overrideContentSecurityPolicy = overrideContentSecurityPolicy; }

#if PLATFORM(COCOA)
    const WTF::Vector<WTF::String>& additionalSupportedImageTypes() const { return m_data.additionalSupportedImageTypes; }
    void setAdditionalSupportedImageTypes(WTF::Vector<WTF::String>&& additionalSupportedImageTypes) { m_data.additionalSupportedImageTypes = WTFMove(additionalSupportedImageTypes); }

    bool clientNavigationsRunAtForegroundPriority() const { return m_data.clientNavigationsRunAtForegroundPriority; }
    void setClientNavigationsRunAtForegroundPriority(bool value) { m_data.clientNavigationsRunAtForegroundPriority = value; }
#endif

#if ENABLE(APPLICATION_MANIFEST)
    ApplicationManifest* applicationManifest() const;
    void setApplicationManifest(RefPtr<ApplicationManifest>&&);
#endif

    RefPtr<WebKit::WebURLSchemeHandler> urlSchemeHandlerForURLScheme(const WTF::String&);
    void setURLSchemeHandlerForURLScheme(Ref<WebKit::WebURLSchemeHandler>&&, const WTF::String&);
    const HashMap<WTF::String, Ref<WebKit::WebURLSchemeHandler>>& urlSchemeHandlers() { return m_data.urlSchemeHandlers; }

    const Vector<WTF::String>& corsDisablingPatterns() const { return m_data.corsDisablingPatterns; }
    void setCORSDisablingPatterns(Vector<WTF::String>&& patterns) { m_data.corsDisablingPatterns = WTFMove(patterns); }

    const HashSet<WTF::String>& maskedURLSchemes() const { return m_data.maskedURLSchemes; }
    void setMaskedURLSchemes(HashSet<WTF::String>&& schemes) { m_data.maskedURLSchemes = WTFMove(schemes); }

    bool userScriptsShouldWaitUntilNotification() const { return m_data.userScriptsShouldWaitUntilNotification; }
    void setUserScriptsShouldWaitUntilNotification(bool value) { m_data.userScriptsShouldWaitUntilNotification = value; }

    bool crossOriginAccessControlCheckEnabled() const { return m_data.crossOriginAccessControlCheckEnabled; }
    void setCrossOriginAccessControlCheckEnabled(bool enabled) { m_data.crossOriginAccessControlCheckEnabled = enabled; }

    const WTF::String& processDisplayName() const { return m_data.processDisplayName; }
    void setProcessDisplayName(const WTF::String& name) { m_data.processDisplayName = name; }

    bool loadsSubresources() const { return m_data.loadsSubresources; }
    void setLoadsSubresources(bool loads) { m_data.loadsSubresources = loads; }

    const std::optional<MemoryCompactLookupOnlyRobinHoodHashSet<WTF::String>>& allowedNetworkHosts() const { return m_data.allowedNetworkHosts; }
    void setAllowedNetworkHosts(std::optional<MemoryCompactLookupOnlyRobinHoodHashSet<WTF::String>>&& hosts) { m_data.allowedNetworkHosts = WTFMove(hosts); }

#if ENABLE(APP_BOUND_DOMAINS)
    bool ignoresAppBoundDomains() const { return m_data.ignoresAppBoundDomains; }
    void setIgnoresAppBoundDomains(bool shouldIgnore) { m_data.ignoresAppBoundDomains = shouldIgnore; }
    
    bool limitsNavigationsToAppBoundDomains() const { return m_data.limitsNavigationsToAppBoundDomains; }
    void setLimitsNavigationsToAppBoundDomains(bool limits) { m_data.limitsNavigationsToAppBoundDomains = limits; }
#endif

    void setMediaCaptureEnabled(bool value) { m_data.mediaCaptureEnabled = value; }
    bool mediaCaptureEnabled() const { return m_data.mediaCaptureEnabled; }

    void setHTTPSUpgradeEnabled(bool enabled) { m_data.httpsUpgradeEnabled = enabled; }
    bool httpsUpgradeEnabled() const { return m_data.httpsUpgradeEnabled; }

#if PLATFORM(MAC)
    bool showsURLsInToolTips() const { return m_data.showsURLsInToolTips; }
    void setShowsURLsInToolTips(bool shows) { m_data.showsURLsInToolTips = shows; }

    bool serviceControlsEnabled() const { return m_data.serviceControlsEnabled; }
    void setServiceControlsEnabled(bool enabled) { m_data.serviceControlsEnabled = enabled; }

    bool imageControlsEnabled() const { return m_data.imageControlsEnabled; }
    void setImageControlsEnabled(bool enabled) { m_data.imageControlsEnabled = enabled; }

    bool contextMenuQRCodeDetectionEnabled() const { return m_data.contextMenuQRCodeDetectionEnabled; }
    void setContextMenuQRCodeDetectionEnabled(bool enabled) { m_data.contextMenuQRCodeDetectionEnabled = enabled; }
#endif

    double incrementalRenderingSuppressionTimeout() const { return m_data.incrementalRenderingSuppressionTimeout; }
    void setIncrementalRenderingSuppressionTimeout(double timeout) { m_data.incrementalRenderingSuppressionTimeout = timeout; }

    bool allowsJavaScriptMarkup() const { return m_data.allowsJavaScriptMarkup; }
    void setAllowsJavaScriptMarkup(bool allows) { m_data.allowsJavaScriptMarkup = allows; }

    bool convertsPositionStyleOnCopy() const { return m_data.convertsPositionStyleOnCopy; }
    void setConvertsPositionStyleOnCopy(bool converts) { m_data.convertsPositionStyleOnCopy = converts; }

    bool allowsMetaRefresh() const { return m_data.allowsMetaRefresh; }
    void setAllowsMetaRefresh(bool allows) { m_data.allowsMetaRefresh = allows; }

    bool allowUniversalAccessFromFileURLs() const { return m_data.allowUniversalAccessFromFileURLs; }
    void setAllowUniversalAccessFromFileURLs(bool allow) { m_data.allowUniversalAccessFromFileURLs = allow; }

    bool allowTopNavigationToDataURLs() const { return m_data.allowTopNavigationToDataURLs; }
    void setAllowTopNavigationToDataURLs(bool allow) { m_data.allowTopNavigationToDataURLs = allow; }

    bool needsStorageAccessFromFileURLsQuirk() const { return m_data.needsStorageAccessFromFileURLsQuirk; }
    void setNeedsStorageAccessFromFileURLsQuirk(bool needs) { m_data.needsStorageAccessFromFileURLsQuirk = needs; }

    bool legacyEncryptedMediaAPIEnabled() const { return m_data.legacyEncryptedMediaAPIEnabled; }
    void setLegacyEncryptedMediaAPIEnabled(bool enabled) { m_data.legacyEncryptedMediaAPIEnabled = enabled; }

    bool allowMediaContentTypesRequiringHardwareSupportAsFallback() const { return m_data.allowMediaContentTypesRequiringHardwareSupportAsFallback; }
    void setAllowMediaContentTypesRequiringHardwareSupportAsFallback(bool allow) { m_data.allowMediaContentTypesRequiringHardwareSupportAsFallback = allow; }

    bool colorFilterEnabled() const { return m_data.colorFilterEnabled; }
    void setColorFilterEnabled(bool enabled) { m_data.colorFilterEnabled = enabled; }

    bool incompleteImageBorderEnabled() const { return m_data.incompleteImageBorderEnabled; }
    void setIncompleteImageBorderEnabled(bool enabled) { m_data.incompleteImageBorderEnabled = enabled; }

    bool shouldDeferAsynchronousScriptsUntilAfterDocumentLoad() const { return m_data.shouldDeferAsynchronousScriptsUntilAfterDocumentLoad; }
    void setShouldDeferAsynchronousScriptsUntilAfterDocumentLoad(bool defer) { m_data.shouldDeferAsynchronousScriptsUntilAfterDocumentLoad = defer; }

    bool undoManagerAPIEnabled() const { return m_data.undoManagerAPIEnabled; }
    void setUndoManagerAPIEnabled(bool enabled) { m_data.undoManagerAPIEnabled = enabled; }

    bool mainContentUserGestureOverrideEnabled() const { return m_data.mainContentUserGestureOverrideEnabled; }
    void setMainContentUserGestureOverrideEnabled(bool enabled) { m_data.mainContentUserGestureOverrideEnabled = enabled; }

    bool invisibleAutoplayForbidden() const { return m_data.invisibleAutoplayForbidden; }
    void setInvisibleAutoplayForbidden(bool forbidden) { m_data.invisibleAutoplayForbidden = forbidden; }

    bool attachmentElementEnabled() const { return m_data.attachmentElementEnabled; }
    void setAttachmentElementEnabled(bool enabled) { m_data.attachmentElementEnabled = enabled; }

    bool attachmentWideLayoutEnabled() const { return m_data.attachmentWideLayoutEnabled; }
    void setAttachmentWideLayoutEnabled(bool enabled) { m_data.attachmentWideLayoutEnabled = enabled; }

    bool allowsInlinePredictions() const { return m_data.allowsInlinePredictions; }
    void setAllowsInlinePredictions(bool allows) { m_data.allowsInlinePredictions = allows; }

    void setShouldRelaxThirdPartyCookieBlocking(WebCore::ShouldRelaxThirdPartyCookieBlocking value) { m_data.shouldRelaxThirdPartyCookieBlocking = value; }
    WebCore::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking() const { return m_data.shouldRelaxThirdPartyCookieBlocking; }

    void setAttributedBundleIdentifier(WTF::String&& identifier) { m_data.attributedBundleIdentifier = WTFMove(identifier); }
    const WTF::String& attributedBundleIdentifier() const { return m_data.attributedBundleIdentifier; }

#if PLATFORM(IOS_FAMILY)
    WebKit::AttributionOverrideTesting appInitiatedOverrideValueForTesting() const { return m_data.appInitiatedOverrideValueForTesting; }
    void setAppInitiatedOverrideValueForTesting(WebKit::AttributionOverrideTesting appInitiatedOverrideValueForTesting) { m_data.appInitiatedOverrideValueForTesting = appInitiatedOverrideValueForTesting; }
#endif

#if HAVE(TOUCH_BAR)
    bool requiresUserActionForEditingControlsManager() const { return m_data.requiresUserActionForEditingControlsManager; }
    void setRequiresUserActionForEditingControlsManager(bool value) { m_data.requiresUserActionForEditingControlsManager = value; }
#endif

    bool isLockdownModeExplicitlySet() const;
    bool lockdownModeEnabled() const;
    
    void setAllowTestOnlyIPC(bool enabled) { m_data.allowTestOnlyIPC = enabled; }
    bool allowTestOnlyIPC() const { return m_data.allowTestOnlyIPC; }

    void setPortsForUpgradingInsecureSchemeForTesting(uint16_t upgradeFromInsecurePort, uint16_t upgradeToSecurePort) { m_data.portsForUpgradingInsecureSchemeForTesting = { upgradeFromInsecurePort, upgradeToSecurePort }; }
    std::optional<std::pair<uint16_t, uint16_t>> portsForUpgradingInsecureSchemeForTesting() const { return m_data.portsForUpgradingInsecureSchemeForTesting; }

    void setDelaysWebProcessLaunchUntilFirstLoad(bool);
    bool delaysWebProcessLaunchUntilFirstLoad() const;

    void setContentSecurityPolicyModeForExtension(WebCore::ContentSecurityPolicyModeForExtension mode) { m_data.contentSecurityPolicyModeForExtension = mode; }
    WebCore::ContentSecurityPolicyModeForExtension contentSecurityPolicyModeForExtension() const { return m_data.contentSecurityPolicyModeForExtension; }

#if ENABLE(GPU_PROCESS)
    WebKit::GPUProcessPreferencesForWebProcess preferencesForGPUProcess() const;
#endif
    WebKit::NetworkProcessPreferencesForWebProcess preferencesForNetworkProcess() const;

private:
    struct Data {
        Ref<WebKit::BrowsingContextGroup> browsingContextGroup;
        RefPtr<WebKit::WebProcessPool> processPool { };
        RefPtr<WebKit::WebUserContentControllerProxy> userContentController { };
#if ENABLE(WK_WEB_EXTENSIONS)
        WTF::URL requiredWebExtensionBaseURL { };
        RefPtr<WebKit::WebExtensionController> webExtensionController { };
        WeakPtr<WebKit::WebExtensionController> weakWebExtensionController { };
#endif
        RefPtr<WebKit::WebPageGroup> pageGroup { };
        RefPtr<WebKit::WebPreferences> preferences { };
        RefPtr<WebKit::WebPageProxy> relatedPage { };
        WeakPtr<WebKit::WebPageProxy> pageToCloneSessionStorageFrom { };
        RefPtr<WebKit::VisitedLinkStore> visitedLinkStore { };

        RefPtr<WebKit::WebsiteDataStore> websiteDataStore { };
        RefPtr<WebsitePolicies> defaultWebsitePolicies { };

#if PLATFORM(IOS_FAMILY)
        bool canShowWhileLocked { false };
        WebKit::AttributionOverrideTesting appInitiatedOverrideValueForTesting { WebKit::AttributionOverrideTesting::NoOverride };
        RetainPtr<_UIClickInteractionDriving> clickInteractionDriverForTesting { };
#endif
        bool initialCapitalizationEnabled { true };
        bool waitsForPaintAfterViewDidMoveToWindow { true };
        bool drawsBackground { true };
        bool controlledByAutomation { false };
        bool allowTestOnlyIPC { false };
        std::optional<bool> delaysWebProcessLaunchUntilFirstLoad { };
        std::optional<double> cpuLimit { };
        std::optional<std::pair<uint16_t, uint16_t>> portsForUpgradingInsecureSchemeForTesting { };

        WTF::String overrideContentSecurityPolicy { };

#if PLATFORM(COCOA)
        WTF::Vector<WTF::String> additionalSupportedImageTypes { };
        bool clientNavigationsRunAtForegroundPriority { true };
#endif

#if ENABLE(APPLICATION_MANIFEST)
        RefPtr<ApplicationManifest> applicationManifest { };
#endif

        HashMap<WTF::String, Ref<WebKit::WebURLSchemeHandler>> urlSchemeHandlers { };
        Vector<WTF::String> corsDisablingPatterns { };
        HashSet<WTF::String> maskedURLSchemes { };
        bool userScriptsShouldWaitUntilNotification { true };
        bool crossOriginAccessControlCheckEnabled { true };
        WTF::String processDisplayName { };
        bool loadsSubresources { true };
        std::optional<MemoryCompactLookupOnlyRobinHoodHashSet<WTF::String>> allowedNetworkHosts { };

#if ENABLE(APP_BOUND_DOMAINS)
        bool ignoresAppBoundDomains { false };
        bool limitsNavigationsToAppBoundDomains { false };
#endif

        bool mediaCaptureEnabled { false };
        bool httpsUpgradeEnabled { true };

#if PLATFORM(MAC)
        bool showsURLsInToolTips { false };
        bool serviceControlsEnabled { false };
        bool imageControlsEnabled { false };
        bool contextMenuQRCodeDetectionEnabled { false };
#endif

        double incrementalRenderingSuppressionTimeout { 5 };
        bool allowsJavaScriptMarkup { true };
        bool convertsPositionStyleOnCopy { false };
        bool allowsMetaRefresh { true };
        bool allowUniversalAccessFromFileURLs { false };
        bool allowTopNavigationToDataURLs { false };
        bool needsStorageAccessFromFileURLsQuirk { true };
        bool legacyEncryptedMediaAPIEnabled { true };
        bool allowMediaContentTypesRequiringHardwareSupportAsFallback { true };
        bool colorFilterEnabled { false };
        bool incompleteImageBorderEnabled { false };
        bool shouldDeferAsynchronousScriptsUntilAfterDocumentLoad { true };
        bool undoManagerAPIEnabled { false };
        bool mainContentUserGestureOverrideEnabled { false };
        bool invisibleAutoplayForbidden { false };
        bool attachmentElementEnabled { false };
        bool attachmentWideLayoutEnabled { false };
        bool allowsInlinePredictions { false };

        WebCore::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking { WebCore::ShouldRelaxThirdPartyCookieBlocking::No };
        WTF::String attributedBundleIdentifier { };

#if HAVE(TOUCH_BAR)
        bool requiresUserActionForEditingControlsManager { false };
#endif

        WebCore::ContentSecurityPolicyModeForExtension contentSecurityPolicyModeForExtension { WebCore::ContentSecurityPolicyModeForExtension::None };
    };

    // All data members should be added to the Data structure to avoid breaking PageConfiguration::copy().
    Data m_data;
};

} // namespace API
