// GENERATED FILE - DO NOT EDIT.
// Generated by gen_vk_internal_shaders.py.
//
// Copyright 2018 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// shaders/gen/ImageCopy.frag.00000021.inc:
//   Pre-generated shader for the ANGLE Vulkan back-end.

#pragma once
constexpr uint8_t kImageCopy_frag_00000021[] = {
    0x1f,0x8b,0x08,0x00,0x00,0x00,0x00,0x00,0x02,0xff,0x85,0x98,0x6d,0x68,0x96,0x55,
    0x18,0xc7,0xcf,0x79,0x9e,0xed,0xd9,0xd4,0x69,0x66,0x9a,0xf3,0x6d,0x6c,0xbe,0x65,
    0x6d,0xcd,0x97,0x6c,0x9a,0xfa,0xa8,0xcb,0x97,0x8d,0x9a,0xce,0x4d,0x0a,0xcc,0x4c,
    0x34,0x2b,0x35,0x9d,0xb5,0x05,0x45,0x8a,0x4e,0xdd,0x2a,0x1d,0x91,0xd2,0x87,0x28,
    0xcd,0x14,0x82,0x88,0xc2,0xc0,0x4a,0x3f,0xa4,0x95,0x23,0xcc,0xa2,0x5c,0x50,0x0e,
    0xca,0x85,0x7e,0xea,0x63,0x24,0x69,0xa9,0x11,0x75,0xae,0xfb,0xfc,0xae,0xed,0x6a,
    0xbd,0x0d,0x0e,0xf7,0x7d,0xfe,0xd7,0xf5,0xbf,0x5e,0xfe,0xe7,0xe5,0xde,0x96,0x4e,
    0x8d,0xc9,0x73,0xce,0xbb,0xbe,0x2e,0xdf,0xd5,0x7b,0x97,0xfc,0x5c,0xef,0x52,0x4e,
    0x5e,0xfb,0xb9,0x4c,0xf2,0xac,0xaa,0x59,0x5a,0x53,0xde,0xf4,0xc4,0x9a,0xf2,0xa9,
    0xb7,0x4f,0x12,0xfb,0x00,0x97,0x4e,0xfc,0xc4,0x76,0x9d,0xcb,0x73,0x39,0xe1,0x29,
    0x63,0xe3,0xaa,0x75,0x0d,0x82,0x8f,0x0f,0xe3,0x42,0x18,0x03,0x83,0x9f,0xe0,0x79,
    0x12,0x23,0xbc,0x8d,0x4f,0x62,0x0a,0xc7,0xb9,0x6a,0x97,0xeb,0xca,0x62,0x3a,0x37,
    0x86,0xa7,0x62,0x1e,0x2c,0xdf,0x60,0x29,0xb0,0x81,0x06,0x4b,0x83,0x0d,0x36,0x58,
    0x0e,0x58,0xa1,0xc1,0x72,0xc1,0x46,0x1a,0x2c,0x03,0x56,0x6c,0xb0,0x3c,0xb0,0xb1,
    0x06,0xcb,0x07,0x9b,0x60,0xb0,0x3e,0x60,0x65,0x06,0xeb,0x0b,0x36,0xc9,0x60,0xfd,
    0xc0,0xa6,0x1a,0xac,0x00,0x6c,0xba,0xc1,0xfa,0x83,0xcd,0x4a,0x74,0x4a,0x77,0xf7,
    0x2b,0x9a,0x2d,0x0f,0xcf,0xd1,0xe8,0xa3,0xf3,0x12,0x33,0x17,0x9d,0x8b,0x98,0xdf,
    0x10,0x58,0xa9,0xc4,0x9e,0x4e,0xb4,0x91,0xf7,0x1b,0xc3,0x5b,0x86,0x3e,0x4b,0x83,
    0x7f,0x86,0x7c,0x3f,0x1c,0x1e,0x97,0x1d,0x1c,0x3c,0x06,0x18,0x5c,0x34,0x3b,0x5c,
    0xb5,0x2a,0xab,0x73,0xd1,0xeb,0x95,0x97,0x87,0xcd,0x1d,0x12,0xe6,0xc5,0xc4,0x90,
    0xb5,0x19,0x1a,0xe6,0x25,0xcc,0x53,0xcc,0xc7,0xa2,0x69,0x4e,0x82,0xe7,0xb8,0x71,
    0xf8,0x0a,0x3e,0x93,0xf9,0x78,0xc3,0xbf,0x09,0x7f,0xe1,0x4b,0xfc,0x52,0xe2,0xb9,
    0xa4,0x9f,0x81,0x89,0x06,0x25,0x8c,0xe2,0xee,0xfa,0xff,0x7b,0x14,0x77,0x3f,0x73,
    0xdc,0xad,0xac,0x53,0x19,0xf9,0x65,0x5e,0x0e,0x56,0x4a,0x3f,0x13,0xa9,0x47,0xfc,
    0x27,0x61,0x2b,0x31,0xf6,0x0a,0xd3,0xcf,0xb4,0x6e,0x6e,0xb4,0xcb,0x73,0x06,0xf5,
    0xaa,0xff,0x3c,0xf6,0x9a,0xce,0x17,0xa3,0xb5,0xf4,0x7b,0x2f,0x5c,0x89,0x37,0x2c,
    0x44,0x5a,0xc6,0x9c,0x23,0xf7,0xb7,0xa7,0xe4,0xbc,0x8f,0xf7,0x65,0xf4,0x20,0xf3,
    0xe5,0xbd,0x72,0xae,0xe8,0x35,0x5f,0xcd,0xb9,0x10,0xfe,0x83,0xd4,0x5c,0x4c,0x0d,
    0x0f,0xf3,0x9e,0x36,0xfe,0xeb,0x59,0x03,0x9d,0x37,0xb0,0x2e,0xe2,0xff,0x14,0x35,
    0x5a,0xff,0x66,0xce,0x89,0xce,0xf7,0xb3,0xcf,0x75,0x7e,0x84,0x33,0xa3,0xf3,0x13,
    0xbd,0x34,0x3f,0xcd,0x19,0xd0,0x3d,0xf6,0x95,0xa9,0x5f,0xe6,0xdf,0x27,0xb3,0xe6,
    0x39,0x52,0xff,0x79,0x72,0xeb,0x1e,0x3a,0xcf,0xbd,0x92,0x36,0xfe,0xb3,0x82,0x60,
    0xdb,0x9b,0x9b,0x67,0x96,0x85,0xaa,0xc4,0x2f,0xeb,0x23,0x66,0x87,0xfa,0xce,0x0e,
    0xef,0x9b,0xde,0x68,0x9c,0xa3,0xf3,0x39,0x61,0xbe,0x77,0xe7,0x81,0x6c,0x45,0x58,
    0xb5,0x14,0x6b,0xe3,0x88,0x7f,0x25,0x20,0xb2,0x96,0x59,0xf6,0xf6,0x04,0xf6,0x6f,
    0x6d,0xc8,0x23,0x7b,0xf7,0x66,0xb0,0x09,0x66,0xdd,0x1a,0x38,0x17,0xb7,0x60,0xaf,
    0x0c,0x11,0x64,0x5f,0x4d,0x66,0xef,0x4d,0x24,0x9e,0xf8,0x4c,0x01,0xdf,0x11,0x7c,
    0x64,0x7e,0x1b,0xbc,0x29,0xf0,0xa6,0x71,0x47,0x94,0xb3,0x0f,0xb3,0xec,0xb9,0x3b,
    0xc0,0xdf,0x0a,0x3e,0x03,0x12,0x5d,0x22,0x26,0x7b,0xf1,0xd7,0x50,0x79,0x96,0x7a,
    0xae,0x05,0xff,0x99,0xdc,0x29,0x59,0xfa,0x91,0xf7,0xba,0xc0,0x93,0x75,0x98,0x4b,
    0x4e,0xf9,0xd9,0xca,0xda,0x54,0x82,0xd7,0x07,0x75,0xa4,0xa6,0xd1,0x3e,0x62,0xea,
    0x77,0x35,0xc4,0xd0,0x58,0xf2,0xbc,0x1c,0xb4,0x10,0xbf,0xb9,0x3e,0xfa,0xe4,0xc2,
    0x99,0x65,0x7a,0x98,0x4f,0x0f,0xf3,0x4c,0x0f,0x0b,0xc0,0xb5,0x87,0x85,0x60,0xda,
    0x43,0xb5,0xe9,0x61,0x61,0x72,0xcf,0xb9,0x04,0x93,0xbc,0x55,0xa6,0x87,0xbb,0xc9,
    0xed,0x4d,0x0f,0x35,0xe0,0xda,0xc3,0x38,0x1f,0x31,0xf5,0x93,0x1e,0x34,0x56,0xb5,
    0xe9,0xa1,0xd2,0x47,0x9f,0x2c,0x9c,0x2a,0xd3,0x43,0x2d,0x3d,0x2c,0x36,0x3d,0x2c,
    0x01,0xd7,0x1e,0xea,0xc0,0xb4,0x87,0xa5,0xa6,0x87,0xba,0xa4,0x1e,0x97,0x60,0x92,
    0xb7,0x9e,0x7d,0x24,0x79,0xef,0x21,0x77,0xa5,0xef,0x39,0xfb,0x52,0xa3,0xfa,0x2e,
    0x35,0x35,0xde,0x89,0x5f,0x35,0xbc,0x7a,0xea,0x91,0xfb,0xe1,0x7e,0xee,0x06,0xdd,
    0x73,0x0f,0x50,0xf3,0x0a,0xb3,0xe7,0x56,0x82,0x37,0xb3,0xe7,0x56,0x81,0x25,0x71,
    0x03,0x26,0xf7,0xc5,0x1a,0x78,0xab,0xe1,0x89,0xa6,0x0f,0x81,0xab,0xee,0x8f,0xc0,
    0x75,0x06,0x5b,0x0b,0xe6,0x13,0x1d,0x32,0xc9,0x5d,0xb3,0x0e,0xdf,0xb5,0xc4,0xd0,
    0x1c,0x8f,0x92,0x63,0xbd,0xc9,0xb1,0x01,0x7c,0x65,0xe8,0x55,0xee,0xca,0x8d,0xf4,
    0xb4,0x8e,0xfb,0x69,0x83,0x59,0x8f,0x4d,0xf0,0x1b,0xcc,0x7a,0x3c,0x06,0xae,0xeb,
    0xf1,0x38,0x98,0xae,0x47,0x93,0x59,0x0f,0xb1,0x35,0x86,0xb1,0x0d,0x8d,0xb7,0x99,
    0xd8,0xdb,0x89,0xdd,0x6c,0x62,0xef,0x00,0xd7,0xd8,0x3b,0xc1,0x34,0x76,0xab,0x89,
    0x2d,0xb6,0x96,0x30,0x5a,0x89,0xdd,0x82,0x46,0x12,0xe7,0x59,0xfa,0x92,0xfb,0xe5,
    0x6d,0x62,0x3d,0x07,0x3e,0x83,0x75,0x57,0x5e,0x2b,0xeb,0x2e,0x3e,0xbb,0xc8,0xb9,
    0x0d,0xff,0x16,0xf2,0xb6,0x99,0xbc,0xe2,0xb3,0x3b,0x8c,0x36,0xf8,0xbb,0x4d,0xde,
    0xe7,0x4d,0xde,0xda,0x70,0x3b,0xcb,0xbd,0xfe,0x02,0xd8,0x46,0x73,0x7f,0xa5,0x58,
    0x3b,0xb1,0xef,0x81,0xa7,0xe3,0x99,0x10,0x4b,0xf0,0xbd,0x70,0xf7,0x24,0xb1,0xfa,
    0x24,0x6b,0xf5,0x22,0x71,0xf6,0x72,0x87,0xe6,0xf2,0x0d,0x49,0xd3,0x93,0xd6,0xd4,
    0x46,0x4f,0xc2,0x59,0xe0,0x23,0xa7,0x15,0xfe,0x6e,0x7c,0x9b,0xf0,0x6d,0x34,0xf5,
    0x3f,0xf9,0x0f,0xf5,0x3f,0xfd,0x3f,0xf5,0x6f,0x86,0xa7,0xa3,0x85,0xfa,0xb7,0xc0,
    0xdd,0x6c,0xea,0xdf,0x4a,0x9c,0x2d,0xff,0x52,0xbf,0xd6,0xd4,0x64,0xea,0x9f,0xef,
    0x63,0x0f,0x6d,0xf0,0x1b,0x93,0xfd,0x16,0xbf,0x15,0x2f,0x61,0x6f,0x0d,0x91,0x64,
    0xbe,0x0f,0x2c,0xeb,0x7b,0xf6,0xd9,0xab,0xec,0xb3,0xfd,0x66,0x9f,0x1d,0x00,0xd7,
    0x7d,0xf6,0x1a,0x98,0xee,0xb3,0x43,0x66,0xbd,0xc5,0x76,0x30,0x8c,0x43,0xd4,0x76,
    0x10,0xbd,0xa4,0xee,0xd7,0xc9,0xe9,0xe0,0x5d,0xe3,0xfd,0xb7,0xf0,0x2e,0xb1,0x7e,
    0x84,0x23,0xcf,0xe3,0xe4,0xfa,0x09,0x5e,0x7f,0x38,0x57,0x4d,0x2e,0xb1,0x5d,0x0c,
    0xe3,0x12,0xbc,0x4b,0xc9,0x79,0x88,0xb9,0x7e,0x81,0x57,0xe8,0x62,0xbf,0x99,0x84,
    0x1f,0x71,0xf9,0xde,0x16,0x04,0xbd,0x32,0x09,0x2f,0xae,0xd1,0x70,0xec,0x23,0xd1,
    0xf6,0x1a,0x31,0x2f,0x1a,0xfe,0xcf,0xc4,0x94,0xef,0xb3,0xf5,0x91,0x9a,0xfe,0x08,
    0x51,0xae,0x80,0x5d,0x26,0xf6,0x42,0x1f,0xe3,0x5f,0x82,0x7b,0x91,0xfb,0x5f,0xb4,
    0x2f,0xf5,0xd1,0xbe,0xcf,0xdc,0x57,0xc2,0x79,0x13,0x8d,0x3c,0xfd,0x16,0xf8,0xbf,
    0x6a,0xe4,0x7c,0xcc,0x29,0x4f,0xd5,0x28,0xe5,0x23,0x4f,0x35,0xea,0xe7,0x7b,0x34,
    0x12,0x5b,0x3a,0x8c,0x0c,0x3c,0x79,0xaa,0x46,0xf9,0xf0,0xac,0x46,0x7d,0x7c,0xc4,
    0xad,0x46,0x7d,0x7d,0x8f,0x46,0x62,0x57,0x8d,0x0a,0x88,0x99,0xf6,0x3d,0xfc,0x5c,
    0x62,0xaa,0x46,0xea,0x23,0x35,0xa9,0x46,0x82,0xa9,0x46,0xd5,0x3e,0xc6,0x97,0xba,
    0x72,0xa9,0x55,0x35,0x2a,0xf7,0xd1,0x5e,0x4a,0x7e,0xd5,0xe8,0x1d,0x34,0x4a,0xd1,
    0x6f,0x51,0x2f,0x8d,0x06,0x91,0x73,0x90,0xd1,0x68,0xb0,0x8f,0x3c,0xd5,0x68,0x94,
    0xd1,0x48,0x6c,0x43,0xc2,0x28,0x84,0x57,0x68,0x34,0x1a,0x0e,0xcf,0x6a,0x34,0xc2,
    0x47,0xdc,0x6a,0x34,0xd2,0x68,0x34,0xc2,0x68,0x54,0x44,0xcc,0x21,0x46,0xa3,0xa1,
    0xc4,0x54,0x8d,0xd4,0x67,0x94,0xd1,0xa8,0xc8,0x68,0x74,0x97,0x8f,0xf1,0xa5,0xae,
    0xa1,0xd4,0xaa,0x1a,0x4d,0xf6,0xd1,0x2e,0x5a,0xa5,0xc8,0xa9,0x67,0xef,0x10,0x7b,
    0x51,0xfc,0x6a,0xd8,0x6b,0x4d,0x70,0x8a,0xcc,0x99,0x7f,0x97,0x33,0x7f,0xc4,0x9c,
    0xf9,0xf7,0xc0,0xf5,0xcc,0xbf,0x0f,0xa6,0x67,0xfe,0x98,0x39,0x87,0x62,0x3b,0x9a,
    0x9c,0xd9,0x98,0xf7,0xb8,0xf9,0x6e,0x7d,0x48,0xec,0x13,0x26,0xf6,0x47,0xe0,0x1a,
    0xfb,0x63,0x30,0x8d,0xdd,0x6e,0x62,0x8b,0xed,0x64,0x18,0xa7,0x88,0x7d,0xca,0x7c,
    0xaf,0x3f,0x23,0xf6,0x69,0xf3,0xbd,0xfe,0x1c,0xfc,0x53,0xbe,0xff,0x5f,0x80,0xad,
    0x37,0xf9,0xbe,0x04,0x5f,0x41,0xbe,0x0e,0x93,0x4f,0x6c,0x67,0xc2,0xe8,0x20,0xdf,
    0x19,0x73,0x66,0x2b,0x7c,0xfc,0xbb,0xa0,0xc6,0xfc,0xce,0xa6,0x7e,0x1d,0x46,0xeb,
    0x45,0x3e,0xfa,0x9c,0x82,0x73,0xc6,0xd4,0xf3,0x35,0xf5,0x54,0x98,0x7a,0xbe,0x01,
    0xd7,0x7a,0x3a,0x4d,0x3d,0x62,0x3b,0x1b,0x46,0x27,0x79,0xce,0x9a,0x7a,0xa6,0x53,
    0xcf,0x22,0xb3,0xf6,0xea,0xd7,0x69,0xea,0x59,0xec,0xa3,0x4f,0x07,0x9c,0xb3,0xa6,
    0x9e,0x6f,0xa9,0xe7,0x88,0xa9,0xe7,0x3b,0x70,0xad,0xa7,0xcb,0xd4,0x23,0xb6,0x73,
    0x61,0x74,0x91,0xe7,0x9c,0xa9,0x67,0x86,0x8f,0x7f,0x27,0x49,0x3e,0xfd,0x46,0xa9,
    0x5f,0x97,0xa9,0xa7,0xce,0x47,0x9f,0x4e,0x38,0xe7,0xf0,0x6d,0xc7,0xf7,0xa4,0x39,
    0xeb,0x9f,0xa0,0x77,0xda,0xe4,0x99,0xea,0x7b,0x70,0xd7,0x8b,0xdb,0x6e,0xf2,0x2c,
    0xf1,0x31,0x57,0x17,0x9c,0x93,0xf8,0x1e,0xc3,0xf7,0x28,0xdf,0x5a,0xf1,0xfd,0x80,
    0x78,0x32,0xf4,0x7f,0x43,0x29,0xf3,0xad,0x55,0xce,0x31,0x13,0xbf,0xd6,0xc7,0x1c,
    0xed,0xf0,0x25,0xde,0xec,0xc0,0xb8,0x80,0xed,0xf7,0xb0,0x4b,0xa6,0x87,0xf1,0x27,
    0x08,0x36,0x9d,0x78,0xcc,0x12,0x00,0x00
};

// Generated from:
//
// #version 450 core
//
// #extension GL_EXT_samplerless_texture_functions : require
//
// layout(set = 0, binding = 0)uniform utexture2DArray src;
// layout(location = 0)out vec4 dst;
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
// float sRGBToLinear(float sRGB)
// {
//
//     if(sRGB <= 0.04045)
//     {
//         return sRGB / 12.92;
//     }
//     else
//     {
//         return pow((sRGB + 0.055f)/ 1.055f, 2.4f);
//     }
// }
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
//            vec4 dstValue = vec4(srcValue);
//
//     dstValue /= 255.0;
//
//     if(params . dstIsSRGB)
//     {
//
//         dstValue . r = sRGBToLinear(dstValue . r);
//         dstValue . g = sRGBToLinear(dstValue . g);
//         dstValue . b = sRGBToLinear(dstValue . b);
//     }
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
