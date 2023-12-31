// GENERATED FILE - DO NOT EDIT.
// Generated by gen_vk_internal_shaders.py.
//
// Copyright 2018 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// shaders/gen/ImageCopy.frag.00000025.inc:
//   Pre-generated shader for the ANGLE Vulkan back-end.

#pragma once
constexpr uint8_t kImageCopy_frag_00000025[] = {
    0x1f,0x8b,0x08,0x00,0x00,0x00,0x00,0x00,0x02,0xff,0x85,0x96,0xd9,0x4f,0x5d,0x55,
    0x14,0xc6,0xcf,0x9d,0x29,0xc8,0x50,0xa0,0x45,0x1a,0xa9,0x76,0x20,0xd1,0x50,0xe1,
    0x26,0x62,0x45,0xb0,0x4d,0xb0,0x62,0xb5,0xa2,0x2d,0x74,0x50,0xe1,0xc1,0xa1,0x44,
    0x5a,0x85,0xaa,0xa0,0xb1,0x8d,0x8d,0x81,0x5a,0x78,0xb0,0x55,0x21,0x0d,0xa9,0x46,
    0x21,0x51,0x78,0x50,0xb0,0x7d,0x30,0xf1,0xc9,0x18,0xa3,0x6d,0x6c,0xeb,0xf0,0xe7,
    0x18,0x87,0xc4,0xc4,0xbd,0xf6,0xfd,0xad,0xe6,0xcb,0x8d,0xa6,0x24,0x3b,0xe7,0xec,
    0x6f,0x4d,0xdf,0xb7,0xf6,0x3a,0xfb,0x92,0x49,0x6f,0x2b,0x24,0x49,0x2a,0xa9,0x4c,
    0x2a,0x92,0xdf,0x92,0xd2,0xdf,0xfa,0x24,0x1d,0x90,0x24,0xa9,0x4a,0xf2,0xf1,0xb9,
    0xb7,0xff,0x60,0x7f,0xfb,0xe4,0x1b,0x23,0xed,0x9d,0xf7,0x17,0xcd,0x5e,0x93,0x64,
    0xa2,0x9f,0xd9,0x6a,0x93,0x42,0x92,0x0d,0x4f,0x5b,0xe3,0x2f,0x1c,0x3f,0x61,0x78,
    0x75,0x58,0x97,0xc3,0xaa,0x0b,0x7e,0x86,0x17,0x2c,0x47,0x78,0xab,0x8e,0x39,0x2d,
    0x26,0x49,0x1e,0x4b,0x72,0x49,0x03,0xf5,0xb6,0xf1,0x74,0x2c,0x05,0x56,0x21,0x58,
    0x1a,0xac,0x4e,0xb0,0x0c,0x58,0xa3,0x60,0x59,0xb0,0xdb,0x05,0xcb,0x81,0xdd,0x21,
    0x58,0x1e,0xec,0x2e,0xc1,0x0a,0x60,0xdb,0x05,0xab,0x00,0xbb,0x5b,0xb0,0x75,0x60,
    0x3b,0x04,0xab,0x04,0x2b,0x0a,0x56,0x05,0xd6,0x29,0xd8,0x6d,0x60,0x5d,0x82,0x55,
    0x83,0x3d,0x14,0xfb,0x94,0xb9,0xa9,0xd7,0x7a,0xb6,0x27,0x3c,0xb7,0xd2,0x1f,0xdf,
    0x6f,0x91,0xbd,0xf5,0x79,0x33,0xfb,0x86,0x10,0x95,0x8e,0xf6,0x4c,0xec,0x8d,0xbd,
    0x6f,0x08,0x3e,0x79,0x74,0x5a,0x5f,0x9b,0xc2,0xbe,0x80,0x7e,0xb3,0x6f,0x0c,0x9e,
    0x95,0xd8,0xcd,0x56,0x85,0x96,0x6c,0xc4,0xb2,0x91,0x6f,0x0a,0x2d,0x3d,0xec,0xab,
    0x25,0x57,0x0d,0xfe,0x5e,0xab,0x9e,0x5c,0x49,0xe4,0x55,0x77,0xb3,0xaf,0x5e,0xd3,
    0x56,0xfd,0x2d,0x96,0xfb,0x58,0xfd,0x46,0xfa,0xdd,0x40,0xfd,0xc6,0x58,0xa7,0x84,
    0xb5,0xa1,0x6d,0x23,0x7c,0xcc,0xbf,0x09,0x5b,0x41,0xec,0x2d,0xa2,0x67,0x33,0xf6,
    0xfa,0x38,0x37,0xe9,0xd8,0xcb,0x36,0x78,0x7b,0x9f,0x3d,0xae,0x8d,0xd9,0xf1,0xfd,
    0x4e,0xb4,0x9b,0xee,0xdd,0xe4,0xb0,0xbc,0xcd,0x21,0x63,0x2f,0xfb,0x14,0x39,0xca,
    0x9f,0x56,0xfb,0x61,0xde,0x7b,0xd1,0x62,0xfb,0x3d,0x65,0x35,0xfb,0xca,0xf6,0xfb,
    0x98,0x73,0x8b,0x7f,0x02,0xee,0x79,0x38,0x3c,0xc5,0x7b,0x46,0xfc,0x07,0x39,0x0b,
    0xdf,0x1f,0x11,0xff,0xe7,0xe1,0xa8,0xfe,0xc7,0xe8,0x95,0xd9,0x4f,0xe3,0x9b,0x15,
    0xfb,0x34,0xdf,0x80,0xef,0xcf,0x97,0xf5,0xfe,0x22,0x33,0x6d,0xfc,0x2e,0x91,0xfb,
    0x34,0xfa,0x2e,0x71,0x0f,0x64,0x62,0xef,0x72,0x91,0x57,0x16,0x7d,0x86,0xfd,0x19,
    0x10,0xeb,0xef,0x2e,0xe6,0xae,0x96,0xfe,0xee,0x0f,0x8c,0x6a,0xf8,0xd6,0x6b,0x59,
    0xde,0xcb,0x13,0xcc,0xef,0x7a,0xec,0xbd,0x21,0x43,0x13,0xdf,0xfb,0x06,0x66,0x61,
    0x17,0x3e,0xcd,0xe0,0x67,0x82,0x8f,0xed,0x37,0x11,0xd7,0x4c,0x9c,0xcd,0xc2,0x9d,
    0xc4,0xb5,0x10,0xe7,0xf3,0x6b,0xf8,0x5a,0xf0,0xd9,0x22,0xf7,0x84,0xcd,0xc7,0x1f,
    0x81,0x79,0x2b,0x7c,0xfe,0x0e,0xfe,0x7e,0x5f,0xb4,0xa2,0xc7,0xde,0x07,0x42,0x5c,
    0x9e,0x3b,0x63,0x13,0xbe,0xef,0xd0,0xaf,0x7b,0xc0,0x07,0xc3,0xce,0x38,0x7d,0x0d,
    0xe6,0x7e,0x7f,0x85,0x1c,0x9e,0xcb,0x9e,0xbf,0x07,0x2f,0xf3,0xfb,0x11,0x9f,0x1c,
    0x31,0xdb,0x45,0xc3,0x0e,0x34,0xb4,0x89,0x86,0x7b,0xc1,0x5d,0x43,0x3b,0x98,0x6b,
    0x28,0x8a,0x06,0xb3,0x75,0x70,0x77,0x59,0xdd,0x0e,0xd1,0x70,0x1f,0xb5,0x53,0xa2,
    0xa1,0x13,0xdc,0x35,0x7c,0x03,0xe6,0x7e,0xa6,0xc1,0x73,0x15,0x45,0xc3,0x15,0x7c,
    0x5a,0x89,0xe9,0x10,0x0d,0x0f,0xa0,0x61,0xa7,0x68,0xe8,0x02,0x77,0x0d,0x0f,0x82,
    0xb9,0x86,0x1e,0xd1,0x60,0xb6,0xee,0x38,0x77,0xa5,0xba,0xdd,0xcc,0x51,0x21,0xe6,
    0x2b,0xd5,0xbe,0x22,0xdf,0xa3,0x71,0x74,0xdf,0x1e,0xe1,0x78,0x15,0xbf,0x22,0x71,
    0xdd,0xf0,0xb1,0x6f,0xf6,0x11,0xbe,0x57,0x9f,0xb9,0x47,0xe1,0xdc,0x27,0x33,0xb7,
    0x17,0x7c,0x8a,0x99,0x7b,0x1c,0xec,0x2a,0x71,0xf6,0x0d,0xf7,0x13,0xb7,0x8f,0x38,
    0xeb,0xe9,0x93,0xe0,0xde,0xf7,0xfd,0xc4,0x26,0x82,0x1d,0x00,0x4b,0xc5,0xf7,0x7c,
    0xfc,0xfe,0x07,0xf0,0x3d,0x40,0x0e,0xaf,0x71,0x90,0x1a,0x83,0x52,0xe3,0x10,0xf8,
    0x73,0x81,0x99,0xdd,0x5f,0x87,0xd1,0x34,0xc0,0x9d,0x71,0x48,0xce,0xe3,0x69,0xe2,
    0x8f,0xc8,0x79,0x3c,0x03,0xee,0xe7,0xf1,0x2c,0x98,0x9f,0xc7,0xb0,0x9c,0x87,0xd9,
    0x86,0xc2,0x1a,0xa5,0xc7,0x43,0xe8,0xa8,0x8f,0xf5,0x4b,0xb5,0x33,0x91,0x7b,0x45,
    0xbc,0x97,0x5e,0x04,0x3b,0x2c,0xdf,0x7a,0x1a,0x9d,0x66,0x3f,0x4a,0x9c,0xaf,0xb3,
    0x21,0x97,0xe1,0x23,0xc4,0x1e,0x8d,0xb9,0xd6,0x45,0x5d,0x2f,0x91,0x67,0x84,0xfb,
    0x26,0x27,0xf7,0xa4,0x9d,0xfb,0x30,0x9c,0x46,0x45,0xef,0x71,0xf4,0x1e,0x13,0xbd,
    0x2f,0x83,0xbb,0xde,0x57,0xc0,0x5c,0xef,0xb8,0xe8,0x35,0xdb,0x98,0xfd,0x0f,0x44,
    0xee,0x31,0xd1,0xfb,0xaa,0xe8,0xfd,0x8a,0x5c,0xaf,0x81,0x6f,0x85,0x93,0xc7,0x8d,
    0x33,0x8b,0xe6,0xf3,0x3a,0x35,0x47,0xf1,0x1f,0xa3,0xee,0xa4,0xd4,0x35,0x9f,0x89,
    0xb0,0x26,0x89,0x9f,0x90,0xba,0x6f,0xfe,0x47,0x9f,0xdf,0xba,0x45,0x9f,0x4f,0x12,
    0xe7,0x6b,0x96,0x3e,0x9f,0x22,0xf6,0xa4,0xf4,0xf9,0x6d,0xf2,0x9c,0xfa,0x9f,0x3e,
    0x3b,0xa7,0x49,0x34,0x59,0xcc,0x35,0x62,0xc6,0x89,0x9f,0x28,0x3b,0x93,0x61,0xf1,
    0xfd,0x89,0xb3,0x1c,0x22,0x6e,0x32,0xfe,0xb6,0x94,0x7e,0xa7,0xa6,0xb0,0xfb,0xf9,
    0x9d,0xe1,0xfc,0xa6,0xe5,0xfc,0xde,0x05,0xf7,0xf3,0x3b,0x0b,0xe6,0xe7,0x37,0x2b,
    0x7d,0x34,0xdb,0x4c,0x58,0xe7,0xe0,0x31,0x83,0x4e,0xab,0xf5,0x1e,0xf5,0xa6,0xd0,
    0x59,0xa0,0x67,0xae,0x73,0x96,0x98,0x73,0xc2,0xe7,0x7d,0xf8,0x9c,0x17,0x3e,0x1f,
    0x80,0x3b,0x9f,0x0f,0xc1,0x9c,0xcf,0xbc,0xf0,0x31,0xdb,0x5c,0x58,0x0b,0xe4,0x9e,
    0x93,0x7b,0xe0,0x02,0x5c,0x32,0xdc,0xc1,0xc6,0xf1,0x5b,0xc1,0xfd,0x7e,0x9b,0x27,
    0x76,0x41,0xee,0x85,0x8f,0xe0,0x75,0x51,0xee,0x85,0x8f,0xc1,0xaf,0x91,0xff,0x13,
    0xb0,0x41,0xe1,0xfa,0x29,0x78,0x1f,0x5c,0x97,0x84,0xab,0xd9,0x16,0xc3,0x5a,0xa2,
    0xde,0xa2,0xf0,0xfa,0x8e,0x98,0x29,0xf9,0x6d,0x70,0xbf,0x25,0xce,0xda,0xfc,0xae,
    0xe3,0xb3,0x40,0xcc,0xa2,0xf0,0xf9,0x0c,0x3e,0x2d,0xc2,0xe7,0x73,0x70,0xe7,0xb3,
    0x22,0x7c,0xcc,0xb6,0x1c,0xd6,0x0a,0x75,0x96,0x85,0xcf,0xf7,0xc4,0x5c,0xe7,0x0c,
    0x8d,0x8f,0xfb,0xad,0x08,0x9f,0x1b,0xf8,0x2c,0x11,0xb3,0x2c,0x7c,0xbe,0x80,0xcf,
    0xb4,0xf0,0xf9,0x12,0xdc,0xf9,0xac,0x09,0x1f,0xb3,0xad,0x46,0xdf,0x52,0x9d,0x55,
    0xe1,0xf3,0x03,0xff,0xb3,0xdc,0x90,0x79,0x72,0xbf,0x35,0xe1,0xf3,0x2b,0x3e,0x2b,
    0xc4,0xac,0x96,0x9d,0xf1,0xbc,0xf8,0xfe,0xc2,0x3c,0xcc,0x11,0xb7,0x56,0x36,0xa7,
    0xb3,0xe2,0xfb,0x33,0xf3,0x3d,0x43,0x9c,0xe5,0xd9,0x1d,0x98,0x5c,0xc6,0xf6,0x4f,
    0x38,0xb5,0xae,0xb0,0xfe,0x05,0x43,0xa1,0xf9,0xc7,0x28,0x0e,0x00,0x00
};

// Generated from:
//
// #version 450 core
//
// #extension GL_EXT_samplerless_texture_functions : require
//
// layout(set = 0, binding = 0)uniform utexture2DArray src;
// layout(location = 0)out ivec4 dst;
//
// layout(push_constant)uniform PushConstants {
//
//     ivec2 srcOffset;
//     ivec2 dstOffset;
//     int srcMip;
//     int srcLayer;
//
//     bool flipX;
//     bool flipY;
//
//     bool premultiplyAlpha;
//     bool unmultiplyAlpha;
//
//     bool dstHasLuminance;
//     bool dstIsAlpha;
//
//     bool srcIsSRGB;
//     bool dstIsSRGB;
//
//     int dstDefaultChannelsMask;
//     bool rotateXY;
// } params;
//
// void main()
// {
//     ivec2 dstSubImageCoords = ivec2(gl_FragCoord . xy)- params . dstOffset;
//
//     ivec2 srcSubImageCoords = dstSubImageCoords;
//
//     if(params . flipX)
//     {
//         srcSubImageCoords . x = - srcSubImageCoords . x;
//     }
//     if(params . flipY)
//     {
//         srcSubImageCoords . y = - srcSubImageCoords . y;
//     }
//     if(params . rotateXY)
//     {
//         srcSubImageCoords . xy = srcSubImageCoords . yx;
//     }
//
//           uvec4 srcValue = texelFetch(src, ivec3(params . srcOffset + srcSubImageCoords, params . srcLayer), params . srcMip);
//
//     if(params . premultiplyAlpha)
//     {
//         srcValue . rgb *= srcValue . a;
//     }
//     else if(params . unmultiplyAlpha && srcValue . a > 0)
//     {
//         srcValue . rgb /= srcValue . a;
//     }
//
//            ivec4 dstValue = ivec4(srcValue);
//
//     if(params . dstHasLuminance)
//     {
//         dstValue . rg = dstValue . ra;
//     }
//     else if(params . dstIsAlpha)
//     {
//         dstValue . r = dstValue . a;
//     }
//     else
//     {
//         int defaultChannelsMask = params . dstDefaultChannelsMask;
//         if((defaultChannelsMask & 2)!= 0)
//         {
//             dstValue . g = 0;
//         }
//         if((defaultChannelsMask & 4)!= 0)
//         {
//             dstValue . b = 0;
//         }
//         if((defaultChannelsMask & 8)!= 0)
//         {
//             dstValue . a = 1;
//         }
//     }
//
//     dst = dstValue;
// }
