/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
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

#if ENABLE(UI_SIDE_COMPOSITING)

#include <wtf/ArgumentCoder.h>
#include <wtf/text/WTFString.h>

namespace IPC {
class Decoder;
class Encoder;
}

namespace WebCore {
class ScrollingStateFrameScrollingNode;
class ScrollingStateOverflowScrollingNode;
class ScrollingStateOverflowScrollProxyNode;
class ScrollingStateFixedNode;
class ScrollingStateStickyNode;
class ScrollingStatePositionedNode;
class ScrollingStateTree;
}

namespace WebKit {

class RemoteScrollingCoordinatorTransaction {
public:
    enum class FromDeserialization : bool { No, Yes };
    RemoteScrollingCoordinatorTransaction();
    RemoteScrollingCoordinatorTransaction(std::unique_ptr<WebCore::ScrollingStateTree>&&, bool, FromDeserialization = FromDeserialization::Yes);
    RemoteScrollingCoordinatorTransaction(RemoteScrollingCoordinatorTransaction&&);
    RemoteScrollingCoordinatorTransaction& operator=(RemoteScrollingCoordinatorTransaction&&);
    ~RemoteScrollingCoordinatorTransaction();

    std::unique_ptr<WebCore::ScrollingStateTree>& scrollingStateTree() { return m_scrollingStateTree; }
    const std::unique_ptr<WebCore::ScrollingStateTree>& scrollingStateTree() const { return m_scrollingStateTree; }

    bool clearScrollLatching() const { return m_clearScrollLatching; }

#if !defined(NDEBUG) || !LOG_DISABLED
    String description() const;
    void dump() const;
#endif

private:
    std::unique_ptr<WebCore::ScrollingStateTree> m_scrollingStateTree;
    
    // Data encoded here should be "imperative" (valid just for one transaction). Stateful things should live on scrolling tree nodes.
    // Maybe RequestedScrollData should move here.
    bool m_clearScrollLatching { false };
};

} // namespace WebKit

namespace IPC {
template<> struct ArgumentCoder<WebCore::ScrollingStateFrameScrollingNode> {
    static void encode(Encoder&, const WebCore::ScrollingStateFrameScrollingNode&);
    static std::optional<Ref<WebCore::ScrollingStateFrameScrollingNode>> decode(Decoder&);
};
template<> struct ArgumentCoder<WebCore::ScrollingStateOverflowScrollingNode> {
    static void encode(Encoder&, const WebCore::ScrollingStateOverflowScrollingNode&);
    static std::optional<Ref<WebCore::ScrollingStateOverflowScrollingNode>> decode(Decoder&);
};
template<> struct ArgumentCoder<WebCore::ScrollingStateOverflowScrollProxyNode> {
    static void encode(Encoder&, const WebCore::ScrollingStateOverflowScrollProxyNode&);
    static std::optional<Ref<WebCore::ScrollingStateOverflowScrollProxyNode>> decode(Decoder&);
};
template<> struct ArgumentCoder<WebCore::ScrollingStateFixedNode> {
    static void encode(Encoder&, const WebCore::ScrollingStateFixedNode&);
    static std::optional<Ref<WebCore::ScrollingStateFixedNode>> decode(Decoder&);
};
template<> struct ArgumentCoder<WebCore::ScrollingStateStickyNode> {
    static void encode(Encoder&, const WebCore::ScrollingStateStickyNode&);
    static std::optional<Ref<WebCore::ScrollingStateStickyNode>> decode(Decoder&);
};
template<> struct ArgumentCoder<WebCore::ScrollingStatePositionedNode> {
    static void encode(Encoder&, const WebCore::ScrollingStatePositionedNode&);
    static std::optional<Ref<WebCore::ScrollingStatePositionedNode>> decode(Decoder&);
};
}

#endif // ENABLE(UI_SIDE_COMPOSITING)
