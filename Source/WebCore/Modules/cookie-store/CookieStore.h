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

#pragma once

#include "EventTarget.h"
#include <optional>
#include <wtf/Forward.h>
#include <wtf/IsoMalloc.h>

namespace WebCore {

struct CookieInit;
struct CookieStoreDeleteOptions;
struct CookieStoreGetOptions;
class DeferredPromise;

class CookieStore final : public RefCounted<CookieStore>, public EventTarget {
    WTF_MAKE_ISO_ALLOCATED(CookieStore);
public:
    static Ref<CookieStore> create();
    ~CookieStore();

    void get(const String& name, Ref<DeferredPromise>&&);
    void get(CookieStoreGetOptions&&, Ref<DeferredPromise>&&);

    void getAll(const String& name, Ref<DeferredPromise>&&);
    void getAll(CookieStoreGetOptions&&, Ref<DeferredPromise>&&);

    void set(const String& name, const String& value, Ref<DeferredPromise>&&);
    void set(CookieInit&&, Ref<DeferredPromise>&&);

    void remove(const String& name, Ref<DeferredPromise>&&);
    void remove(CookieStoreDeleteOptions&&, Ref<DeferredPromise>&&);

    using RefCounted::ref;
    using RefCounted::deref;

private:
    CookieStore();

    // EventTarget
    EventTargetInterface eventTargetInterface() const final;
    ScriptExecutionContext* scriptExecutionContext() const final;
    void refEventTarget() final { ref(); }
    void derefEventTarget() final { deref(); }
};

}
