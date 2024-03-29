#include <metal_stdlib>
#include <simd/simd.h>
#ifdef __clang__
#pragma clang diagnostic ignored "-Wall"
#endif
using namespace metal;
struct Inputs {
};
struct Outputs {
    half4 sk_FragColor [[color(0)]];
};
fragment Outputs fragmentMain(Inputs _in [[stage_in]], bool _frontFacing [[front_facing]], float4 _fragCoord [[position]], half4 FramebufferFragColor [[color(0)]]
) {
    Outputs _out;
    (void)_out;
    _out.sk_FragColor = FramebufferFragColor;
    return _out;
}
