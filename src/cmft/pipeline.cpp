/*
 * Copyright 2026 The cmft contributors. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#include "common/config.h"
#include "common/utils.h"
#include "common/cl.h"

#include <cmft/pipeline.h>
#include <cmft/print.h>

#include <ctype.h>  // toupper
#include <stdio.h>  // FILE, fopen, fwrite, sprintf
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE
#include <string.h> // strlen

namespace
{
    static inline bool hasValue(const char* _str)
    {
        return (NULL != _str && '\0' != _str[0]);
    }

    static void outputShCoeffs(const char* _pathName, double _shCoeffs[cmft::SH_COEFF_NUM][3])
    {
        // Get base name.
        char baseName[128];
        if (!cmft::basename(baseName, 128, _pathName))
        {
            strcpy(baseName, "cmft");
        }
        baseName[0] = (char)toupper(baseName[0]);

        char baseNameUpper[128];
        cmft::strtoupper(baseNameUpper, baseName);

        // File content.
        char content[10240];
        sprintf(content,
               "#ifndef CMFT_%s_H_HEADER_GUARD\n"
               "#define CMFT_%s_H_HEADER_GUARD\n"
               "\n"
               "static const float s_shCoeffs%s[25][3] =\n"
               "{\n"
               "    /* Band 0 */ { %21.18f, %21.18f, %21.18f },\n"
               "    /* Band 1 */ { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f },\n"
               "    /* Band 2 */ { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f },\n"
               "    /* Band 3 */ { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f },\n"
               "    /* Band 4 */ { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }, { %21.18f, %21.18f, %21.18f }\n"
               "};\n"
               "\n"
               "#endif // CMFT_%s_H_HEADER_GUARD\n"
               , baseNameUpper
               , baseNameUpper
               , baseName
               , _shCoeffs[ 0][0], _shCoeffs[ 0][1], _shCoeffs[ 0][2]
               , _shCoeffs[ 1][0], _shCoeffs[ 1][1], _shCoeffs[ 1][2]
               , _shCoeffs[ 2][0], _shCoeffs[ 2][1], _shCoeffs[ 2][2]
               , _shCoeffs[ 3][0], _shCoeffs[ 3][1], _shCoeffs[ 3][2]
               , _shCoeffs[ 4][0], _shCoeffs[ 4][1], _shCoeffs[ 4][2]
               , _shCoeffs[ 5][0], _shCoeffs[ 5][1], _shCoeffs[ 5][2]
               , _shCoeffs[ 6][0], _shCoeffs[ 6][1], _shCoeffs[ 6][2]
               , _shCoeffs[ 7][0], _shCoeffs[ 7][1], _shCoeffs[ 7][2]
               , _shCoeffs[ 8][0], _shCoeffs[ 8][1], _shCoeffs[ 8][2]
               , _shCoeffs[ 9][0], _shCoeffs[ 9][1], _shCoeffs[ 9][2]
               , _shCoeffs[10][0], _shCoeffs[10][1], _shCoeffs[10][2]
               , _shCoeffs[11][0], _shCoeffs[11][1], _shCoeffs[11][2]
               , _shCoeffs[12][0], _shCoeffs[12][1], _shCoeffs[12][2]
               , _shCoeffs[13][0], _shCoeffs[13][1], _shCoeffs[13][2]
               , _shCoeffs[14][0], _shCoeffs[14][1], _shCoeffs[14][2]
               , _shCoeffs[15][0], _shCoeffs[15][1], _shCoeffs[15][2]
               , _shCoeffs[16][0], _shCoeffs[16][1], _shCoeffs[16][2]
               , _shCoeffs[17][0], _shCoeffs[17][1], _shCoeffs[17][2]
               , _shCoeffs[18][0], _shCoeffs[18][1], _shCoeffs[18][2]
               , _shCoeffs[19][0], _shCoeffs[19][1], _shCoeffs[19][2]
               , _shCoeffs[20][0], _shCoeffs[20][1], _shCoeffs[20][2]
               , _shCoeffs[21][0], _shCoeffs[21][1], _shCoeffs[21][2]
               , _shCoeffs[22][0], _shCoeffs[22][1], _shCoeffs[22][2]
               , _shCoeffs[23][0], _shCoeffs[23][1], _shCoeffs[23][2]
               , _shCoeffs[24][0], _shCoeffs[24][1], _shCoeffs[24][2]
               , baseNameUpper
               );

        // Append *.c extension.
        char filePath[CMFT_PATH_LEN];
        strcpy(filePath, _pathName);
        strcat(filePath, ".c");

        // Open file.
        FILE* fp = fopen(filePath, "wb");
        if (NULL == fp)
        {
            WARN("Could not open file %s for writing.", filePath);
            return;
        }
        cmft::ScopeFclose cleanup(fp);

        // Write content.
        size_t write;
        CMFT_UNUSED(write);
        write = fwrite(&content, strlen(content), 1, fp);
        DEBUG_CHECK(write == 1, "Error writing sh coeffs file content.");
        FERROR_CHECK(fp);
    }
}

namespace cmft
{
    int32_t pipelineRun(const PipelineRequest& _request, PipelineResult* _result)
    {
        if (NULL != _result)
        {
            _result->m_success = false;
            _result->m_outputFilesProcessed = 0;
        }

        const uint32_t outputFilesNum = CMFT_MIN(_request.m_outputFilesNum, uint32_t(CMFT_PIPELINE_MAX_OUTPUTS));
        if (0 == outputFilesNum)
        {
            WARN("There are no valid specified outputs! Execution will not terminate.");
            return EXIT_FAILURE;
        }

        if (_request.m_silent)
        {
            setWarningPrintf(NULL);
            setInfoPrintf(NULL);
        }

        Image image;
        Image imageFaceList[6];

        bool imageLoaded = false;

        // Load image.
        if (hasValue(_request.m_inputFilePath))
        {
            imageLoaded = imageLoad   (image, _request.m_inputFilePath, TextureFormat::RGBA32F)
                       || imageLoadStb(image, _request.m_inputFilePath, TextureFormat::RGBA32F)
                       ;
        }
        else
        {
            if (hasValue(_request.m_inputFacePosX)
            &&  hasValue(_request.m_inputFaceNegX)
            &&  hasValue(_request.m_inputFacePosY)
            &&  hasValue(_request.m_inputFaceNegY)
            &&  hasValue(_request.m_inputFacePosZ)
            &&  hasValue(_request.m_inputFaceNegZ))
            {
                imageLoaded = imageLoad(imageFaceList[0], _request.m_inputFacePosX, TextureFormat::RGBA32F)
                           && imageLoad(imageFaceList[1], _request.m_inputFaceNegX, TextureFormat::RGBA32F)
                           && imageLoad(imageFaceList[2], _request.m_inputFacePosY, TextureFormat::RGBA32F)
                           && imageLoad(imageFaceList[3], _request.m_inputFaceNegY, TextureFormat::RGBA32F)
                           && imageLoad(imageFaceList[4], _request.m_inputFacePosZ, TextureFormat::RGBA32F)
                           && imageLoad(imageFaceList[5], _request.m_inputFaceNegZ, TextureFormat::RGBA32F)
                           ;

                if (imageLoaded)
                {
                    INFO("Assembling cubemap from image list.");
                    imageCubemapFromFaceList(image, imageFaceList);
                }

                for (uint8_t ii = 0; ii < 6; ++ii)
                {
                    imageUnload(imageFaceList[ii]);
                }
            }
        }

        if (!imageLoaded)
        {
            WARN("Invalid input!\n");
            return EXIT_FAILURE;
        }

        int32_t result = EXIT_SUCCESS;

        // Assemble cubemap.
        if (!imageIsCubemap(image))
        {
            if (imageIsCubeCross(image))
            {
                INFO("Converting cube cross to cubemap.");
                imageCubemapFromCross(image);
            }
            else if (imageIsLatLong(image))
            {
                INFO("Converting latlong image to cubemap.");
                imageCubemapFromLatLong(image);
            }
            else if (imageIsHStrip(image))
            {
                INFO("Converting hstrip image to cubemap.");
                imageCubemapFromStrip(image);
            }
            else if (imageIsVStrip(image))
            {
                INFO("Converting vstrip image to cubemap.");
                imageCubemapFromStrip(image);
            }
            else if (imageIsOctant(image))
            {
                INFO("Converting octant image to cubemap.");
                imageCubemapFromOctant(image);
            }
            else
            {
                INFO("Image is not cubemap(6 faces), cubecross(ratio 3:4 or 4:3), latlong(ratio 2:1), hstrip(ratio 6:1), vstrip(ration 1:6)");
            }
        }

        if (!imageIsCubemap(image))
        {
            INFO("Conversion failed. Exiting...");
            imageUnload(image);
            return EXIT_FAILURE;
        }

        // Resize if requested.
        if (0 != _request.m_srcFaceSize && image.m_width != _request.m_srcFaceSize)
        {
            INFO("Resizing source image from %ux%u to %ux%u."
                , image.m_width
                , image.m_height
                , _request.m_srcFaceSize
                , _request.m_srcFaceSize
                );
            imageResize(image, _request.m_srcFaceSize, _request.m_srcFaceSize);
        }

        // Transform cubemap if requested.
        imageTransform(image
                     , IMAGE_FACE_POSITIVEX | _request.m_imageOpPosX
                     , IMAGE_FACE_NEGATIVEX | _request.m_imageOpNegX
                     , IMAGE_FACE_POSITIVEY | _request.m_imageOpPosY
                     , IMAGE_FACE_NEGATIVEY | _request.m_imageOpNegY
                     , IMAGE_FACE_POSITIVEZ | _request.m_imageOpPosZ
                     , IMAGE_FACE_NEGATIVEZ | _request.m_imageOpNegZ
                     );

        // Apply gamma on input image.
        imageApplyGamma(image, _request.m_inputGammaPowNumerator / _request.m_inputGammaPowDenominator);

        // Filter cubemap.
        if (PipelineFilterType::Radiance == _request.m_filterType)
        {
            ClContext* clContext = NULL;

            int32_t clLoaded = 0;
            if (_request.m_useOpenCL)
            {
                // Dynamically load OpenCL lib.
                clLoaded = cmft::clLoad();
                if (clLoaded)
                {
                    clContext = clInit(_request.m_clVendor
                                     , _request.m_deviceType
                                     , _request.m_deviceIndex
                                     , _request.m_vendorStrPart
                                     );
                }
            }

            // Start filter.
            imageRadianceFilter(image
                              , _request.m_dstFaceSize
                              , _request.m_lightingModel
                              , _request.m_excludeBase
                              , uint8_t(_request.m_mipCount)
                              , uint8_t(_request.m_glossScale)
                              , uint8_t(_request.m_glossBias)
                              , _request.m_edgeFixup
                              , uint8_t(_request.m_numCpuProcessingThreads)
                              , clContext
                              );

            clDestroy(clContext);

            // Unload OpenCL lib.
            if (clLoaded)
            {
                cmft::clUnload();
            }
        }
        else if (PipelineFilterType::Irradiance == _request.m_filterType)
        {
            imageIrradianceFilterSh(image, _request.m_dstFaceSize);
        }
        else if (PipelineFilterType::ShCoeffs == _request.m_filterType)
        {
            double shCoeffs[SH_COEFF_NUM][3];
            imageShCoeffs(shCoeffs, image);

            uint32_t outputsProcessed = 0;
            for (uint32_t ii = 0; ii < outputFilesNum; ++ii)
            {
                const char* fileName = _request.m_outputFiles[ii].m_fileName;
                if (!hasValue(fileName))
                {
                    continue;
                }

                INFO("Saving spherical harmonics coefficients to %s.c", fileName);
                outputShCoeffs(fileName, shCoeffs);
                outputsProcessed++;
            }

            imageUnload(image);
            INFO("Done.");

            if (NULL != _result)
            {
                _result->m_success = true;
                _result->m_outputFilesProcessed = outputsProcessed;
            }
            return EXIT_SUCCESS;
        }
        else if (PipelineFilterType::None == _request.m_filterType)
        {
            if (0 != _request.m_dstFaceSize && image.m_width != _request.m_dstFaceSize)
            {
                INFO("Resizing destination image from %ux%u to %ux%u."
                    , image.m_width
                    , image.m_height
                    , _request.m_dstFaceSize
                    , _request.m_dstFaceSize
                    );
                imageResize(image, _request.m_dstFaceSize, _request.m_dstFaceSize);
            }
        }

        // Generate mip map chain if requested.
        if (_request.m_generateMipMapChain)
        {
            imageGenerateMipMapChain(image);
        }

        // Apply gamma on output image.
        imageApplyGamma(image, _request.m_outputGammaPowNumerator / _request.m_outputGammaPowDenominator);

        // Encode RGBM (using --rgbm arg).
        if (_request.m_encodeRGBM)
        {
            INFO("Encoding RGBM");
            imageEncodeRGBM(image);
        }

        // Save output images.
        uint32_t outputsProcessed = 0;
        for (uint32_t outputIdx = 0; outputIdx < outputFilesNum; ++outputIdx)
        {
            const PipelineOutputFile& output = _request.m_outputFiles[outputIdx];
            if (!hasValue(output.m_fileName))
            {
                continue;
            }

            OutputType::Enum ot = output.m_outputType;
            ImageFileType::Enum ft = output.m_fileType;
            TextureFormat::Enum tf = output.m_textureFormat;

            // Encode RGBM (using texture format).
            if (tf == TextureFormat::RGBM)
            {
                INFO("Encoding RGBM");
                imageEncodeRGBM(image);
                tf = TextureFormat::BGRA8; // Change file format to BGRA8 for saving.
            }

            if (!imageSave(image, output.m_fileName, ft, ot, tf, true))
            {
                result = EXIT_FAILURE;
            }

            outputsProcessed++;
        }

        // Cleanup.
        imageUnload(image);

        INFO("Done.");

        if (NULL != _result)
        {
            _result->m_success = (EXIT_SUCCESS == result);
            _result->m_outputFilesProcessed = outputsProcessed;
        }

        return result;
    }

} // namespace cmft

/* vim: set sw=4 ts=4 expandtab: */
