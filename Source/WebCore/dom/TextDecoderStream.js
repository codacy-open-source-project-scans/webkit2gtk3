/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

function initializeTextDecoderStream()
{
    "use strict";

    const label = arguments.length >= 1 ? arguments[0] : "utf-8";
    const options = arguments.length >= 2 ? arguments[1] : { };

    const startAlgorithm = () => {
        return @Promise.@resolve();
    };
    const transformAlgorithm = (chunk) => {
        const decoder = @getByIdDirectPrivate(this, "textDecoderStreamDecoder");
        let buffer;
        try {
            buffer = decoder.@decode(chunk);
        } catch (e) {
            return @Promise.@reject(e);
        }
        if (buffer) {
            const transformStream = @getByIdDirectPrivate(this, "textDecoderStreamTransform");
            const controller = @getByIdDirectPrivate(transformStream, "controller");
            @transformStreamDefaultControllerEnqueue(controller, buffer);
        }
        return @Promise.@resolve();
    };
    const flushAlgorithm = () => {
        const decoder = @getByIdDirectPrivate(this, "textDecoderStreamDecoder");
        let buffer;
        try {
            buffer = decoder.@flush();
        } catch (e) {
            return @Promise.@reject(e);
        }
        if (buffer) {
            const transformStream = @getByIdDirectPrivate(this, "textDecoderStreamTransform");
            const controller = @getByIdDirectPrivate(transformStream, "controller");
            @transformStreamDefaultControllerEnqueue(controller, buffer);
        }
        return @Promise.@resolve();
    };

    const [transform, readable, writable] = @createTransformStream(startAlgorithm, transformAlgorithm, flushAlgorithm);
    @putByIdDirectPrivate(this, "textDecoderStreamTransform", transform);
    @putByIdDirectPrivate(this, "readable", readable);
    @putByIdDirectPrivate(this, "writable", writable);

    const fatal = !!options.fatal;
    const ignoreBOM = !!options.ignoreBOM;
    const decoder = new @TextDecoderStreamDecoder(label, fatal, ignoreBOM);

    @putByIdDirectPrivate(this, "fatal", fatal);
    @putByIdDirectPrivate(this, "ignoreBOM", ignoreBOM);
    @putByIdDirectPrivate(this, "encoding", decoder.@encoding());
    @putByIdDirectPrivate(this, "textDecoderStreamDecoder", decoder);

    return this;
}

@getter
function encoding()
{
    "use strict";

    if (!@getByIdDirectPrivate(this, "textDecoderStreamTransform"))
        throw @makeThisTypeError("TextDecoderStream", "encoding");

    return @getByIdDirectPrivate(this, "encoding");
}

@getter
function fatal()
{
    "use strict";

    if (!@getByIdDirectPrivate(this, "textDecoderStreamTransform"))
        throw @makeThisTypeError("TextDecoderStream", "fatal");

    return @getByIdDirectPrivate(this, "fatal");
}

@getter
function ignoreBOM()
{
    "use strict";

    if (!@getByIdDirectPrivate(this, "textDecoderStreamTransform"))
        throw @makeThisTypeError("TextDecoderStream", "ignoreBOM");

    return @getByIdDirectPrivate(this, "ignoreBOM");
}

@getter
function readable()
{
    "use strict";

    const transform = @getByIdDirectPrivate(this, "textDecoderStreamTransform");
    if (!transform)
        throw @makeThisTypeError("TextDecoderStream", "readable");

    return @getByIdDirectPrivate(this, "readable");
}

@getter
function writable()
{
    "use strict";

    const transform = @getByIdDirectPrivate(this, "textDecoderStreamTransform");
    if (!transform)
        throw @makeThisTypeError("TextDecoderStream", "writable");

    return @getByIdDirectPrivate(this, "writable");
}
