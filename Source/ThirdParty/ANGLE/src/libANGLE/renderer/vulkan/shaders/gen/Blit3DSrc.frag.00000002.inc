// GENERATED FILE - DO NOT EDIT.
// Generated by gen_vk_internal_shaders.py.
//
// Copyright 2018 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// shaders/gen/Blit3DSrc.frag.00000002.inc:
//   Pre-generated shader for the ANGLE Vulkan back-end.

#pragma once
constexpr uint8_t kBlit3DSrc_frag_00000002[] = {
    0x1f,0x8b,0x08,0x00,0x00,0x00,0x00,0x00,0x02,0xff,0x5d,0x94,0x4b,0x68,0x5d,0x65,
    0x10,0xc7,0xbf,0xef,0x9c,0xfb,0x48,0x73,0x2f,0x36,0x21,0x9a,0xc6,0x98,0x67,0x5b,
    0xa2,0x36,0xa1,0x76,0xa1,0x62,0x10,0x45,0xa1,0xa0,0x8b,0x42,0xab,0x82,0xe9,0x22,
    0x9b,0x0b,0x76,0x61,0x40,0x85,0x28,0xdd,0x64,0x51,0xc1,0x08,0xdd,0x64,0xd1,0x4d,
    0x16,0x2e,0xea,0xa3,0xa5,0xd6,0x36,0x55,0xb4,0x2e,0x6c,0xdd,0x89,0xda,0xa2,0xc5,
    0x47,0x50,0xa2,0x0d,0x4a,0xd4,0x60,0xc0,0x17,0x01,0x5b,0xa2,0xb5,0x06,0x67,0xbe,
    0xf3,0x9b,0x76,0xbc,0x81,0xc9,0xb9,0xf3,0xff,0xcf,0xcc,0x7f,0xe6,0x3b,0xdf,0x9c,
    0x3c,0xdb,0x52,0x0d,0x21,0x86,0xd6,0xd0,0x12,0x46,0x62,0x48,0x7f,0xed,0x21,0x0b,
    0xfa,0xb3,0x16,0x2a,0xe9,0xf9,0xd0,0xae,0xc7,0x76,0x6d,0x7f,0xf6,0xb9,0x27,0xb6,
    0xdf,0x79,0xd7,0x0e,0xe5,0x6f,0x08,0x79,0x8a,0x53,0x6e,0xa3,0x78,0x25,0x79,0xaa,
    0x3d,0xd5,0x78,0xf2,0x69,0xc5,0x3b,0xc5,0xf6,0x8a,0x35,0xc4,0x26,0xc4,0x26,0xc5,
    0xa6,0xc4,0xa6,0xc5,0x66,0xc4,0x66,0xc5,0xda,0xa4,0x86,0xe6,0x54,0xb5,0xbe,0xfc,
    0xea,0x4c,0x7a,0x5a,0x2f,0x84,0x87,0x43,0x39,0xf4,0x16,0xad,0x84,0x2d,0x3c,0x0d,
    0x8b,0x60,0x2d,0x0e,0xcb,0xc0,0xda,0x1c,0x96,0x83,0x75,0x39,0xac,0x04,0xd6,0xe3,
    0xb0,0x32,0xd8,0x80,0xc3,0x2a,0x60,0x5b,0x1d,0x56,0x05,0xbb,0xcd,0x61,0x2d,0x60,
    0x23,0x0e,0xdb,0x00,0xb6,0x23,0xcd,0x95,0x5f,0xeb,0x4f,0x67,0xd4,0x33,0xe9,0x63,
    0x1e,0xf5,0x1b,0xf8,0x11,0x7f,0x02,0xdf,0xe2,0x27,0xf1,0x73,0xfc,0x29,0xfc,0x12,
    0xfe,0x34,0x7e,0x19,0x7f,0x06,0xbf,0x82,0x3f,0x8b,0x6f,0x67,0x7c,0x5c,0x9e,0x9b,
    0x9d,0xbe,0xfa,0x83,0xce,0x9f,0x6b,0xe2,0xe7,0xe0,0xb5,0x9f,0x0e,0xf9,0x9f,0x25,
    0x3f,0x4f,0xfd,0x64,0xe9,0x3d,0xe7,0x49,0x4b,0xcf,0x6e,0x93,0xc4,0x57,0xd1,0x56,
    0xee,0x26,0xf1,0x6b,0x70,0x01,0xbe,0xce,0x3b,0x2e,0xe1,0xdf,0x48,0x7c,0x29,0xc5,
    0x95,0x24,0xa7,0x38,0x0b,0xc5,0xef,0xc5,0xef,0x04,0xd3,0x7a,0x3d,0xd4,0x8b,0x69,
    0xae,0xfa,0xb5,0xf7,0x62,0xd6,0x83,0x55,0x78,0xd6,0x9c,0x69,0xfd,0x3e,0xde,0x4f,
    0x2f,0xf5,0xd5,0xef,0x07,0x1b,0x6e,0xaa,0xaf,0xf1,0x83,0x70,0x55,0xc7,0x0f,0x31,
    0x8f,0xf9,0xc3,0xf0,0x1a,0x3f,0x42,0x7c,0x2d,0xcd,0x90,0x85,0x3b,0x88,0xab,0x71,
    0x1f,0x7c,0xde,0x28,0xf7,0xc7,0xfc,0x9d,0x4d,0x7d,0xec,0x66,0x0e,0xad,0xbb,0x07,
    0xae,0x07,0x7f,0x8c,0x3b,0x51,0x67,0x8e,0x31,0xf6,0x2d,0x77,0xf9,0xe3,0xbc,0x07,
    0xe3,0x1b,0x4d,0xfc,0x3e,0xce,0xdd,0xf8,0x09,0x78,0xf3,0x27,0x9b,0xe2,0xf7,0xb3,
    0x5f,0xc6,0x4f,0x35,0xf1,0x07,0x38,0x3b,0xe3,0xa7,0x9b,0xf8,0x83,0x62,0x0f,0x38,
    0x7e,0xa6,0x89,0x3f,0x24,0xf6,0xbc,0xe3,0x67,0xe1,0xf5,0x9e,0xbc,0xc4,0x59,0xf8,
    0xf8,0xc3,0xf8,0x37,0xcb,0xc9,0x1c,0xe3,0xcc,0xb3,0xf0,0xff,0x3f,0x3e,0x67,0xe9,
    0xcc,0x5e,0xe7,0xf7,0x31,0x34,0xd4,0x3f,0x0e,0xd6,0x2d,0x99,0x27,0x88,0x3b,0x09,
    0x76,0x82,0x38,0xf5,0xe7,0xc0,0x6e,0x11,0xc5,0x37,0xa9,0x71,0xb7,0x6c,0x5d,0xc6,
    0x19,0x06,0x7a,0xf9,0x4b,0x10,0xdd,0xc5,0xfb,0xb8,0xdb,0x1f,0x70,0x7f,0x1f,0x11,
    0x54,0xfb,0xff,0x10,0x2c,0x38,0xec,0x23,0x30,0xed,0x75,0x8f,0x60,0x7a,0x97,0x3e,
    0x26,0x56,0xb9,0x07,0x05,0xd3,0x7b,0x78,0x8e,0xbb,0x3a,0x40,0x7d,0x8d,0x3b,0x0f,
    0xfe,0x22,0x79,0x9f,0x90,0x7b,0xde,0xe5,0x7d,0x4a,0xde,0x90,0xcb,0xbb,0x00,0xfe,
    0x02,0x79,0x9f,0x91,0x7b,0x81,0x3c,0xbd,0xc7,0x9f,0x93,0x37,0x4c,0x9e,0x9e,0xef,
    0x17,0xe0,0x27,0x25,0x46,0xef,0xf6,0x97,0x60,0x7a,0xb7,0xd7,0xe4,0x04,0x16,0x98,
    0xed,0x8a,0xc4,0x2b,0x37,0x2f,0xb6,0xc0,0xb9,0xcc,0xbb,0x99,0xbf,0x42,0x33,0xa4,
    0x7b,0x53,0x4a,0xd8,0xd7,0xe0,0x8f,0x8a,0xa7,0x3d,0x6d,0x8d,0x05,0x66,0x71,0x7f,
    0x4b,0x0d,0xab,0xa5,0xcf,0xcb,0x12,0xa5,0x71,0xb7,0xc7,0x22,0xa6,0x4c,0xce,0xbc,
    0x9b,0xe1,0x1b,0x66,0x18,0x75,0x33,0x7c,0x0b,0x6e,0x33,0x5c,0x04,0xb3,0x19,0x96,
    0xdc,0x0c,0xca,0x2d,0x8a,0x2d,0xa1,0xbb,0xe8,0x66,0xf8,0x0e,0xed,0xe8,0x66,0xf8,
    0x1e,0xdc,0x66,0xb8,0x35,0x16,0x98,0xc5,0xe9,0x0c,0x56,0x6b,0xc9,0xcd,0xb0,0x2d,
    0x16,0x31,0x0b,0xe4,0x2c,0xba,0x19,0x7e,0x60,0x86,0x9d,0x6e,0x86,0x1f,0xc1,0x6d,
    0x86,0x9f,0xc0,0x6c,0x86,0x15,0x37,0x83,0x72,0xcb,0x62,0x2b,0xe8,0x2e,0xa7,0x6f,
    0x4b,0xa1,0xfb,0x33,0xda,0xdb,0xe2,0xf5,0x5d,0xd1,0x1e,0x2d,0x76,0xc5,0xf5,0x38,
    0x4c,0xdc,0x12,0x79,0xcb,0xf4,0xa8,0xdf,0xa6,0x97,0xe9,0xf1,0x30,0x3d,0xea,0x7e,
    0xbe,0x02,0xfe,0x0c,0x67,0xf3,0x2a,0x98,0x9d,0xdf,0x6b,0xd4,0xf4,0xbb,0x70,0x04,
    0xac,0xd8,0x85,0x4a,0xda,0xfb,0xa3,0xc4,0x1e,0xa1,0x86,0xd6,0xd7,0xfd,0x7b,0x83,
    0xfd,0x55,0x5f,0x77,0xf5,0x14,0x7b,0xfa,0xb8,0xd4,0xd2,0x1d,0x7d,0x8b,0x98,0x53,
    0x6e,0xde,0xb7,0xa9,0x77,0xd4,0x7d,0x1f,0x6c,0x4f,0xde,0x61,0x86,0x71,0xb7,0x27,
    0xa7,0xc1,0x6d,0xbf,0xde,0xa5,0xc6,0x69,0xd7,0xf3,0x7b,0xe0,0x7e,0x8e,0x33,0x60,
    0x7e,0x8e,0xb3,0xc4,0x9e,0x61,0x8e,0x31,0x89,0xd5,0xef,0xf7,0xfb,0xf4,0x7a,0xd6,
    0x9d,0xe7,0x2f,0xf4,0xb2,0xdb,0x9d,0xe7,0xaf,0xe0,0xe7,0x24,0x46,0xfd,0xdf,0xc0,
    0x06,0xdc,0x3d,0xf8,0x1d,0x7c,0x88,0x7b,0xb0,0xea,0xee,0x81,0x72,0x7f,0x88,0xad,
    0xf2,0x6e,0xf5,0xf7,0xfd,0x12,0xb3,0x97,0x1e,0xf4,0xbd,0x1b,0xb7,0xea,0x74,0xfe,
    0x44,0x67,0xdc,0xe9,0x5c,0x02,0x37,0x9d,0x35,0xa7,0x73,0x29,0xdd,0x99,0x90,0x30,
    0xad,0x75,0x19,0x9d,0x86,0xd3,0x31,0x6e,0xcd,0xe9,0x5c,0x41,0x67,0x9f,0xd3,0xf9,
    0x07,0xdc,0x74,0xd6,0x9d,0x8e,0x72,0x57,0xc5,0xd6,0xa9,0x75,0x15,0x9d,0x09,0xa7,
    0x63,0xdc,0xba,0xd3,0x89,0xb1,0xd0,0x19,0x75,0x3a,0x59,0x2c,0x70,0xd3,0x29,0xc7,
    0xeb,0x3a,0xca,0xe5,0x62,0x8a,0x69,0x2d,0xfd,0xad,0x3a,0x93,0x4e,0xc7,0x38,0x7d,
    0x9a,0x4e,0x0b,0x3a,0xfb,0x9d,0xce,0x86,0x58,0xe0,0xa6,0x53,0x77,0x3a,0xca,0xb5,
    0x8a,0xd5,0xa9,0xd5,0x8a,0xce,0x94,0xd3,0x31,0xae,0xee,0x74,0x36,0xa2,0x73,0xc0,
    0xe9,0xb4,0xc5,0x02,0x37,0x9d,0x0e,0xa7,0xa3,0x5c,0xbb,0x58,0x07,0xb5,0xda,0xd1,
    0x99,0x76,0x3a,0xc6,0x75,0x38,0x9d,0x4e,0x74,0x0e,0x3a,0x9d,0x4d,0xb1,0xc0,0x4d,
    0xa7,0xdb,0xe9,0x28,0xd7,0x25,0xd6,0x4d,0xad,0x2e,0x74,0x66,0x9c,0x8e,0x71,0xdd,
    0x4e,0xa7,0x17,0x9d,0x43,0x4e,0xa7,0x2f,0x16,0xb8,0xe9,0x0c,0x3a,0x1d,0xe5,0xfa,
    0xc5,0x06,0xa9,0xd5,0x8f,0xce,0xac,0xd3,0x31,0x4e,0x9f,0xff,0xca,0x56,0xde,0x23,
    0xf6,0x1f,0x52,0xc4,0x1f,0x8c,0xf8,0x0c,0x00,0x00
};

// Generated from:
//
// #version 450 core
//
// #extension GL_GOOGLE_include_directive : require
//
// layout(set = 0, binding = 0)uniform utexture3D color;
//
// layout(location = 0)out uvec4 colorOut0;
// layout(location = 1)out uvec4 colorOut1;
// layout(location = 2)out uvec4 colorOut2;
// layout(location = 3)out uvec4 colorOut3;
// layout(location = 4)out uvec4 colorOut4;
// layout(location = 5)out uvec4 colorOut5;
// layout(location = 6)out uvec4 colorOut6;
// layout(location = 7)out uvec4 colorOut7;
//
// layout(set = 0, binding = 2)uniform sampler blitSampler;
//
// #line 1 "shaders/src/BlitResolve.inc"
//
// layout(push_constant)uniform PushConstants {
//
//             vec2 offset;
//     vec2 stretch;
//     vec2 invSrcExtent;
//     int srcLayer;
//     int samples;
//     float invSamples;
//
//     int outputMask;
//
//     bool flipX;
//     bool flipY;
//     bool rotateXY;
// } params;
//
//         vec2 getSrcImageCoords()
// {
//
//             vec2 srcImageCoords = vec2(gl_FragCoord . xy);
//
//     srcImageCoords *= params . stretch;
//
//     srcImageCoords -= params . offset;
//
//     if(params . flipX)
//         srcImageCoords . x = - srcImageCoords . x;
//     if(params . flipY)
//         srcImageCoords . y = - srcImageCoords . y;
//     if(params . rotateXY)
//         srcImageCoords . xy = srcImageCoords . yx;
//
//     return srcImageCoords;
// }
//
// void broadcastColor(uvec4 colorValue)
// {
//
//     if((params . outputMask &(1 << 0))!= 0)
//     {
//         colorOut0 = colorValue;
//     }
//     if((params . outputMask &(1 << 1))!= 0)
//     {
//         colorOut1 = colorValue;
//     }
//     if((params . outputMask &(1 << 2))!= 0)
//     {
//         colorOut2 = colorValue;
//     }
//     if((params . outputMask &(1 << 3))!= 0)
//     {
//         colorOut3 = colorValue;
//     }
//     if((params . outputMask &(1 << 4))!= 0)
//     {
//         colorOut4 = colorValue;
//     }
//     if((params . outputMask &(1 << 5))!= 0)
//     {
//         colorOut5 = colorValue;
//     }
//     if((params . outputMask &(1 << 6))!= 0)
//     {
//         colorOut6 = colorValue;
//     }
//     if((params . outputMask &(1 << 7))!= 0)
//     {
//         colorOut7 = colorValue;
//     }
// }
//
// #line 58 "shaders/src/Blit3DSrc.frag"
//
// void main()
// {
//             vec2 srcImageCoordsXY = getSrcImageCoords();
//     vec3 srcImageCoords = vec3(srcImageCoordsXY, params . srcLayer);
//
//             uvec4 colorValue = texture(usampler3D(color, blitSampler), vec3((srcImageCoords . xy)* params . invSrcExtent, srcImageCoords . z));
//
//     broadcastColor(colorValue);
// }
