//
// Copyright 2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

#include "common/mathutil.h"

#include <string.h>

namespace angle
{

namespace priv
{

template <typename T>
inline T *OffsetDataPointer(uint8_t *data, size_t y, size_t z, size_t rowPitch, size_t depthPitch)
{
    return reinterpret_cast<T*>(data + (y * rowPitch) + (z * depthPitch));
}

template <typename T>
inline const T *OffsetDataPointer(const uint8_t *data, size_t y, size_t z, size_t rowPitch, size_t depthPitch)
{
    return reinterpret_cast<const T*>(data + (y * rowPitch) + (z * depthPitch));
}

}  // namespace priv

template <typename type, size_t componentCount>
inline void LoadToNative(const ImageLoadContext &context, size_t width, size_t height, size_t depth,
                         const uint8_t *input, size_t inputRowPitch, size_t inputDepthPitch,
                         uint8_t *output, size_t outputRowPitch, size_t outputDepthPitch)
{
    const size_t rowSize = width * sizeof(type) * componentCount;
    const size_t layerSize = rowSize * height;
    const size_t imageSize = layerSize * depth;

    if (layerSize == inputDepthPitch && layerSize == outputDepthPitch)
    {
        ASSERT(rowSize == inputRowPitch && rowSize == outputRowPitch);
        memcpy(output, input, imageSize);
    }
    else if (rowSize == inputRowPitch && rowSize == outputRowPitch)
    {
        for (size_t z = 0; z < depth; z++)
        {
            const type *source = priv::OffsetDataPointer<type>(input, 0, z, inputRowPitch, inputDepthPitch);
            type *dest = priv::OffsetDataPointer<type>(output, 0, z, outputRowPitch, outputDepthPitch);

            memcpy(dest, source, layerSize);
        }
    }
    else
    {
        for (size_t z = 0; z < depth; z++)
        {
            for (size_t y = 0; y < height; y++)
            {
                const type *source = priv::OffsetDataPointer<type>(input, y, z, inputRowPitch, inputDepthPitch);
                type *dest = priv::OffsetDataPointer<type>(output, y, z, outputRowPitch, outputDepthPitch);
                memcpy(dest, source, width * sizeof(type) * componentCount);
            }
        }
    }
}

template <typename type>
inline void LoadToNative3To4Impl(const ImageLoadContext &context,
                                 const uint32_t fourthComponentBits,
                                 size_t width,
                                 size_t height,
                                 size_t depth,
                                 const uint8_t *input,
                                 size_t inputRowPitch,
                                 size_t inputDepthPitch,
                                 uint8_t *output,
                                 size_t outputRowPitch,
                                 size_t outputDepthPitch)
{
    const type fourthValue = gl::bitCast<type>(fourthComponentBits);

    for (size_t z = 0; z < depth; z++)
    {
        for (size_t y = 0; y < height; y++)
        {
            const type *source =
                priv::OffsetDataPointer<type>(input, y, z, inputRowPitch, inputDepthPitch);
            type *dest =
                priv::OffsetDataPointer<type>(output, y, z, outputRowPitch, outputDepthPitch);
            for (size_t x = 0; x < width; x++)
            {
                memcpy(&dest[x * 4], &source[x * 3], sizeof(type) * 3);
                dest[x * 4 + 3] = fourthValue;
            }
        }
    }
}

template <typename type, uint32_t fourthComponentBits>
inline void LoadToNative3To4(const ImageLoadContext &context,
                             size_t width,
                             size_t height,
                             size_t depth,
                             const uint8_t *input,
                             size_t inputRowPitch,
                             size_t inputDepthPitch,
                             uint8_t *output,
                             size_t outputRowPitch,
                             size_t outputDepthPitch)
{
    LoadToNative3To4Impl<type>(context, fourthComponentBits, width, height, depth, input,
                               inputRowPitch, inputDepthPitch, output, outputRowPitch,
                               outputDepthPitch);
}

inline void LoadToNativeUbyte3To4Impl(const ImageLoadContext &context,
                                      const uint8_t fourthValue,
                                      size_t width,
                                      size_t height,
                                      size_t depth,
                                      const uint8_t *input,
                                      size_t inputRowPitch,
                                      size_t inputDepthPitch,
                                      uint8_t *output,
                                      size_t outputRowPitch,
                                      size_t outputDepthPitch)
{
    ASSERT(IsLittleEndian());
    uint32_t fourthValue32 = static_cast<uint32_t>(fourthValue) << 24;

    // To prevent undefined behavior, if the output address is not aligned by 4, the copy would be
    // done using the default function instead.
    if (reinterpret_cast<uintptr_t>(output) % 4 != 0)
    {
        LoadToNative3To4Impl<uint8_t>(context, fourthValue, width, height, depth, input,
                                      inputRowPitch, inputDepthPitch, output, outputRowPitch,
                                      outputDepthPitch);
        return;
    }

    for (size_t z = 0; z < depth; z++)
    {
        for (size_t y = 0; y < height; y++)
        {
            const uint8_t *source8 =
                priv::OffsetDataPointer<uint8_t>(input, y, z, inputRowPitch, inputDepthPitch);
            uint8_t *dest8 =
                priv::OffsetDataPointer<uint8_t>(output, y, z, outputRowPitch, outputDepthPitch);

            // If the uint8_t addresses are not aligned to 4 bytes, there may be undefined behavior
            // if they are used to copy 32-bit data. In that case, pixels are copied to the output
            // one at a time until 4-byte alignment has been achieved for the source.
            size_t pixelIndexStart = 0;
            size_t srcIndexStart   = 0;
            size_t destIndexStart  = 0;

            uint32_t source4Mod = reinterpret_cast<uintptr_t>(source8) % 4;
            while (source4Mod != 0 && pixelIndexStart < width)
            {
                dest8[destIndexStart]     = source8[srcIndexStart];
                dest8[destIndexStart + 1] = source8[srcIndexStart + 1];
                dest8[destIndexStart + 2] = source8[srcIndexStart + 2];
                dest8[destIndexStart + 3] = fourthValue;

                srcIndexStart += 3;
                source4Mod = (source4Mod + 3) % 4;
                destIndexStart += 4;
                pixelIndexStart++;
            }

            // In the following loop, 4 RGB pixels will be read in each iteration. If the remaining
            // pixels are not a multiple of 4, the rest at the end of the row will be copied one at
            // a time.
            const uint32_t *source32 = reinterpret_cast<const uint32_t *>(source8 + srcIndexStart);
            uint32_t *dest32         = reinterpret_cast<uint32_t *>(dest8 + destIndexStart);

            size_t remainingWidth = width - pixelIndexStart;
            size_t pixelPackIndex = 0;
            if (remainingWidth >= 4)
            {
                const uint32_t *source32Index = source32;

                size_t fourByteCopyThreshold = remainingWidth - 4;
                for (; pixelPackIndex <= fourByteCopyThreshold; pixelPackIndex += 4)
                {
                    // Three 32-bit values from the input contain 4 RGB pixels in total. This
                    // translates to four 32-bits on the output.
                    // (RGBR GBRG BRGB -> RGBA RGBA RGBA RGBA)
                    uint32_t newPixelData0 = source32Index[0];
                    uint32_t newPixelData1 = source32Index[1];
                    uint32_t newPixelData2 = source32Index[2];

                    uint32_t rgbaPixel0 = (newPixelData0 & 0x00FFFFFF) | fourthValue32;
                    uint32_t rgbaPixel1 =
                        (newPixelData0 >> 24) | ((newPixelData1 & 0x0000FFFF) << 8) | fourthValue32;
                    uint32_t rgbaPixel2 = (newPixelData1 >> 16) |
                                          ((newPixelData2 & 0x000000FF) << 16) | fourthValue32;
                    uint32_t rgbaPixel3 = (newPixelData2 >> 8) | fourthValue32;

                    dest32[pixelPackIndex]     = rgbaPixel0;
                    dest32[pixelPackIndex + 1] = rgbaPixel1;
                    dest32[pixelPackIndex + 2] = rgbaPixel2;
                    dest32[pixelPackIndex + 3] = rgbaPixel3;

                    source32Index += 3;
                }
            }

            // We should copy the remaining pixels at the end one by one.
            size_t pixelIndexEnd = pixelPackIndex + pixelIndexStart;
            if (pixelIndexEnd == width)
            {
                continue;
            }
            ASSERT(pixelIndexEnd < width);

            size_t srcIndexEnd  = pixelIndexEnd * 3;
            size_t destIndexEnd = pixelIndexEnd * 4;
            for (; pixelIndexEnd < width; pixelIndexEnd++)
            {
                dest8[destIndexEnd]     = source8[srcIndexEnd];
                dest8[destIndexEnd + 1] = source8[srcIndexEnd + 1];
                dest8[destIndexEnd + 2] = source8[srcIndexEnd + 2];
                dest8[destIndexEnd + 3] = fourthValue;

                srcIndexEnd += 3;
                destIndexEnd += 4;
            }
        }
    }
}

template <>
inline void LoadToNative3To4<uint8_t, 0xFF>(const ImageLoadContext &context,
                                            size_t width,
                                            size_t height,
                                            size_t depth,
                                            const uint8_t *input,
                                            size_t inputRowPitch,
                                            size_t inputDepthPitch,
                                            uint8_t *output,
                                            size_t outputRowPitch,
                                            size_t outputDepthPitch)
{
    LoadToNativeUbyte3To4Impl(context, 0xFF, width, height, depth, input, inputRowPitch,
                              inputDepthPitch, output, outputRowPitch, outputDepthPitch);
}

template <size_t componentCount>
inline void Load32FTo16F(const ImageLoadContext &context, size_t width, size_t height, size_t depth,
                         const uint8_t *input, size_t inputRowPitch, size_t inputDepthPitch,
                         uint8_t *output, size_t outputRowPitch, size_t outputDepthPitch)
{
    const size_t elementWidth = componentCount * width;

    for (size_t z = 0; z < depth; z++)
    {
        for (size_t y = 0; y < height; y++)
        {
            const float *source = priv::OffsetDataPointer<float>(input, y, z, inputRowPitch, inputDepthPitch);
            uint16_t *dest = priv::OffsetDataPointer<uint16_t>(output, y, z, outputRowPitch, outputDepthPitch);

            for (size_t x = 0; x < elementWidth; x++)
            {
                dest[x] = gl::float32ToFloat16(source[x]);
            }
        }
    }
}

template <typename type,
          size_t inputComponentCount,
          size_t outputComponentCount,
          bool normalized>
inline void LoadToFloat(const ImageLoadContext &context, size_t width, size_t height, size_t depth,
                         const uint8_t *input, size_t inputRowPitch, size_t inputDepthPitch,
                         uint8_t *output, size_t outputRowPitch, size_t outputDepthPitch) {
    typedef std::numeric_limits<type> NL;

    for (size_t z = 0; z < depth; z++)
    {
        for (size_t y = 0; y < height; y++)
        {
            const type *source_line = priv::OffsetDataPointer<type>(input, y, z, inputRowPitch, inputDepthPitch);
            float *dest_line = priv::OffsetDataPointer<float>(output, y, z, outputRowPitch, outputDepthPitch);

            for (size_t x = 0; x < width; x++)
            {
                const type *source_pixel = source_line + x * inputComponentCount;
                float *dest_pixel = dest_line + x * outputComponentCount;

                for (size_t i = 0; i < inputComponentCount; i++)
                {
                    float result = 0;
                    if (normalized)
                    {
                        if (NL::is_signed)
                        {
                            result = static_cast<float>(source_pixel[i]) / static_cast<float>(NL::max());
                            result = result >= -1.0f ? result : -1.0f;
                        }
                        else
                        {
                            result = static_cast<float>(source_pixel[i]) / static_cast<float>(NL::max());
                        }
                    }
                    else
                    {
                        result = static_cast<float>(source_pixel[i]);
                    }
                    dest_pixel[i] = result;
                }

                for (size_t j = inputComponentCount; j < outputComponentCount; j++)
                {
                    dest_pixel[j] = j == 3 ? 1.0f : 0.0f;
                }
            }
        }
    }
}

template <size_t blockWidth, size_t blockHeight, size_t blockDepth, size_t blockSize>
inline void LoadCompressedToNative(const ImageLoadContext &context, size_t width, size_t height,
                                   size_t depth, const uint8_t *input, size_t inputRowPitch,
                                   size_t inputDepthPitch, uint8_t *output, size_t outputRowPitch,
                                   size_t outputDepthPitch)
{
    const size_t columns = (width + (blockWidth - 1)) / blockWidth;
    const size_t rows = (height + (blockHeight - 1)) / blockHeight;
    const size_t layers = (depth + (blockDepth - 1)) / blockDepth;

    const size_t inputLayerSize = inputRowPitch * rows;
    const size_t inputImageSize = inputDepthPitch * layers;

    const size_t outputLayerSize = outputRowPitch * rows;
    const size_t outputImageSize = outputDepthPitch * layers;

    if (inputImageSize == outputImageSize)
    {
        ASSERT(inputRowPitch == outputRowPitch);
        ASSERT(inputLayerSize == outputLayerSize && inputLayerSize == inputDepthPitch && outputLayerSize == outputDepthPitch);
        memcpy(output, input, inputImageSize);
    }
    else
    {
        // Note: this path should technically never be hit, but it is with the d3d backend.  Once
        // the issue is fixed, this path should be removed.
        // http://anglebug.com/8345
        for (size_t z = 0; z < layers; ++z)
        {
            for (size_t y = 0; y < rows; ++y)
            {
                const uint8_t *source = priv::OffsetDataPointer<uint8_t>(input, y, z, inputRowPitch, inputDepthPitch);
                uint8_t *dest = priv::OffsetDataPointer<uint8_t>(output, y, z, outputRowPitch, outputDepthPitch);
                memcpy(dest, source, columns * blockSize);
            }
        }
    }
}

template <typename type, uint32_t firstBits, uint32_t secondBits, uint32_t thirdBits, uint32_t fourthBits>
inline void Initialize4ComponentData(size_t width, size_t height, size_t depth,
                                     uint8_t *output, size_t outputRowPitch, size_t outputDepthPitch)
{
    type writeValues[4] =
    {
        gl::bitCast<type>(firstBits),
        gl::bitCast<type>(secondBits),
        gl::bitCast<type>(thirdBits),
        gl::bitCast<type>(fourthBits),
    };

    for (size_t z = 0; z < depth; z++)
    {
        for (size_t y = 0; y < height; y++)
        {
            type *destRow = priv::OffsetDataPointer<type>(output, y, z, outputRowPitch, outputDepthPitch);
            for (size_t x = 0; x < width; x++)
            {
                type* destPixel = destRow + x * 4;

                // This could potentially be optimized by generating an entire row of initialization
                // data and copying row by row instead of pixel by pixel.
                memcpy(destPixel, writeValues, sizeof(type) * 4);
            }
        }
    }
}

template <size_t blockWidth, size_t blockHeight>
inline void LoadASTCToRGBA8(const ImageLoadContext &context,
                            size_t width,
                            size_t height,
                            size_t depth,
                            const uint8_t *input,
                            size_t inputRowPitch,
                            size_t inputDepthPitch,
                            uint8_t *output,
                            size_t outputRowPitch,
                            size_t outputDepthPitch)
{
    LoadASTCToRGBA8Inner(context, width, height, depth, blockWidth, blockHeight, input, inputRowPitch,
                         inputDepthPitch, output, outputRowPitch, outputDepthPitch);
}

template <uint32_t indexBits, uint32_t redBlueBits, uint32_t greenBits, uint32_t alphaBits>
inline void LoadPalettedToRGBA8(const ImageLoadContext &context,
                                size_t width,
                                size_t height,
                                size_t depth,
                                const uint8_t *input,
                                size_t inputRowPitch,
                                size_t inputDepthPitch,
                                uint8_t *output,
                                size_t outputRowPitch,
                                size_t outputDepthPitch)
{
    static_assert(indexBits == 4 || indexBits == 8);
    static_assert(redBlueBits == 4 || redBlueBits == 5 || redBlueBits == 8);
    static_assert(greenBits == 4 || greenBits == 5 || greenBits == 6 || greenBits == 8);
    static_assert(alphaBits == 0 || alphaBits == 1 || alphaBits == 4 || alphaBits == 8);
    constexpr uint32_t colorBits = 2 * redBlueBits + greenBits + alphaBits;
    static_assert(colorBits == 16 || colorBits == 24 || colorBits == 32);

    LoadPalettedToRGBA8Impl(context, width, height, depth,
                            indexBits, redBlueBits, greenBits, alphaBits,
                            input, inputRowPitch, inputDepthPitch,
                            output, outputRowPitch, outputDepthPitch);
}

// Temporary overload functions; need to have no-context overloads of the following functions used
// by Chromium.  A Chromium change will switch to the with-context overloads, and then these can be
// removed.
inline void LoadEACR11ToR8(size_t width,
                           size_t height,
                           size_t depth,
                           const uint8_t *input,
                           size_t inputRowPitch,
                           size_t inputDepthPitch,
                           uint8_t *output,
                           size_t outputRowPitch,
                           size_t outputDepthPitch)
{
    LoadEACR11ToR8({}, width, height, depth, input, inputRowPitch, inputDepthPitch, output,
                   outputRowPitch, outputDepthPitch);
}

inline void LoadEACR11SToR8(size_t width,
                            size_t height,
                            size_t depth,
                            const uint8_t *input,
                            size_t inputRowPitch,
                            size_t inputDepthPitch,
                            uint8_t *output,
                            size_t outputRowPitch,
                            size_t outputDepthPitch)
{
    LoadEACR11SToR8({}, width, height, depth, input, inputRowPitch, inputDepthPitch, output,
                    outputRowPitch, outputDepthPitch);
}

inline void LoadEACRG11ToRG8(size_t width,
                             size_t height,
                             size_t depth,
                             const uint8_t *input,
                             size_t inputRowPitch,
                             size_t inputDepthPitch,
                             uint8_t *output,
                             size_t outputRowPitch,
                             size_t outputDepthPitch)
{
    LoadEACRG11ToRG8({}, width, height, depth, input, inputRowPitch, inputDepthPitch, output,
                     outputRowPitch, outputDepthPitch);
}

inline void LoadEACRG11SToRG8(size_t width,
                              size_t height,
                              size_t depth,
                              const uint8_t *input,
                              size_t inputRowPitch,
                              size_t inputDepthPitch,
                              uint8_t *output,
                              size_t outputRowPitch,
                              size_t outputDepthPitch)
{
    LoadEACRG11SToRG8({}, width, height, depth, input, inputRowPitch, inputDepthPitch, output,
                      outputRowPitch, outputDepthPitch);
}

inline void LoadETC2RGB8ToRGBA8(size_t width,
                                size_t height,
                                size_t depth,
                                const uint8_t *input,
                                size_t inputRowPitch,
                                size_t inputDepthPitch,
                                uint8_t *output,
                                size_t outputRowPitch,
                                size_t outputDepthPitch)
{
    LoadETC2RGB8ToRGBA8({}, width, height, depth, input, inputRowPitch, inputDepthPitch, output,
                        outputRowPitch, outputDepthPitch);
}

inline void LoadETC2SRGB8ToRGBA8(size_t width,
                                 size_t height,
                                 size_t depth,
                                 const uint8_t *input,
                                 size_t inputRowPitch,
                                 size_t inputDepthPitch,
                                 uint8_t *output,
                                 size_t outputRowPitch,
                                 size_t outputDepthPitch)
{
    LoadETC2SRGB8ToRGBA8({}, width, height, depth, input, inputRowPitch, inputDepthPitch, output,
                         outputRowPitch, outputDepthPitch);
}

inline void LoadETC2RGBA8ToRGBA8(size_t width,
                                 size_t height,
                                 size_t depth,
                                 const uint8_t *input,
                                 size_t inputRowPitch,
                                 size_t inputDepthPitch,
                                 uint8_t *output,
                                 size_t outputRowPitch,
                                 size_t outputDepthPitch)
{
    LoadETC2RGBA8ToRGBA8({}, width, height, depth, input, inputRowPitch, inputDepthPitch, output,
                         outputRowPitch, outputDepthPitch);
}

inline void LoadETC2RGB8A1ToRGBA8(size_t width,
                                  size_t height,
                                  size_t depth,
                                  const uint8_t *input,
                                  size_t inputRowPitch,
                                  size_t inputDepthPitch,
                                  uint8_t *output,
                                  size_t outputRowPitch,
                                  size_t outputDepthPitch)
{
    LoadETC2RGB8A1ToRGBA8({}, width, height, depth, input, inputRowPitch, inputDepthPitch, output,
                          outputRowPitch, outputDepthPitch);
}

inline void LoadETC2SRGBA8ToSRGBA8(size_t width,
                                   size_t height,
                                   size_t depth,
                                   const uint8_t *input,
                                   size_t inputRowPitch,
                                   size_t inputDepthPitch,
                                   uint8_t *output,
                                   size_t outputRowPitch,
                                   size_t outputDepthPitch)
{
    LoadETC2SRGBA8ToSRGBA8({}, width, height, depth, input, inputRowPitch, inputDepthPitch, output,
                           outputRowPitch, outputDepthPitch);
}

inline void LoadETC2SRGB8A1ToRGBA8(size_t width,
                                   size_t height,
                                   size_t depth,
                                   const uint8_t *input,
                                   size_t inputRowPitch,
                                   size_t inputDepthPitch,
                                   uint8_t *output,
                                   size_t outputRowPitch,
                                   size_t outputDepthPitch)
{
    LoadETC2SRGB8A1ToRGBA8({}, width, height, depth, input, inputRowPitch, inputDepthPitch, output,
                           outputRowPitch, outputDepthPitch);
}

} // namespace angle
