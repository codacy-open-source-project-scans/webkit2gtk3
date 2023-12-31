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

function initializeTextEncoderStream()
{
    "use strict";

    const startAlgorithm = () => {
        return @Promise.@resolve();
    };
    const transformAlgorithm = (chunk) => {
        const encoder = @getByIdDirectPrivate(this, "textEncoderStreamEncoder");
        let buffer;
        try {
            buffer = encoder.@encode(chunk);
        } catch (e) {
            return @Promise.@reject(e);
        }
        if (buffer) {
            const transformStream = @getByIdDirectPrivate(this, "textEncoderStreamTransform");
            const controller = @getByIdDirectPrivate(transformStream, "controller");
            @transformStreamDefaultControllerEnqueue(controller, buffer);
        }
        return @Promise.@resolve();
    };
    const flushAlgorithm = () => {
        const encoder = @getByIdDirectPrivate(this, "textEncoderStreamEncoder");
        const buffer = encoder.@flush();
        if (buffer) {
            const transformStream = @getByIdDirectPrivate(this, "textEncoderStreamTransform");
            const controller = @getByIdDirectPrivate(transformStream, "controller");
            @transformStreamDefaultControllerEnqueue(controller, buffer);
        }
        return @Promise.@resolve();
    };

    const [transform, readable, writable] = @createTransformStream(startAlgorithm, transformAlgorithm, flushAlgorithm);
    @putByIdDirectPrivate(this, "textEncoderStreamTransform", transform);
    @putByIdDirectPrivate(this, "textEncoderStreamEncoder", new @TextEncoderStreamEncoder());
    @putByIdDirectPrivate(this, "readable", readable);
    @putByIdDirectPrivate(this, "writable", writable);

    return this;
}

@getter
function encoding()
{
    "use strict";

    if (!@getByIdDirectPrivate(this, "textEncoderStreamTransform"))
        throw @makeThisTypeError("TextEncoderStream", "encoding");

    return "utf-8";
}

@getter
function readable()
{
    "use strict";

    const transform = @getByIdDirectPrivate(this, "textEncoderStreamTransform");
    if (!transform)
        throw @makeThisTypeError("TextEncoderStream", "readable");

    return @getByIdDirectPrivate(this, "readable");
}

@getter
function writable()
{
    "use strict";

    const transform = @getByIdDirectPrivate(this, "textEncoderStreamTransform");
    if (!transform)
        throw @makeThisTypeError("TextEncoderStream", "writable");

    return @getByIdDirectPrivate(this, "writable");
}
