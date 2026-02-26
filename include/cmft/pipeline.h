/*
 * Copyright 2026 The cmft contributors. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#ifndef CMFT_PIPELINE_H_HEADER_GUARD
#define CMFT_PIPELINE_H_HEADER_GUARD

#include "image.h"
#include "cubemapfilter.h"
#include "clcontext.h"

#include <stdint.h>

namespace cmft
{
    #define CMFT_PIPELINE_MAX_OUTPUTS 16

    struct PipelineFilterType
    {
        enum Enum
        {
            None,
            Radiance,
            Irradiance,
            ShCoeffs,
        };
    };

    struct PipelineOutputFile
    {
        PipelineOutputFile()
        {
            m_fileType      = ImageFileType::DDS;
            m_textureFormat = TextureFormat::BGRA8;
            m_outputType    = OutputType::LatLong;
            m_fileName      = NULL;
        }

        ImageFileType::Enum m_fileType;
        TextureFormat::Enum m_textureFormat;
        OutputType::Enum m_outputType;
        const char* m_fileName;
    };

    struct PipelineRequest
    {
        PipelineRequest()
        {
            // Input.
            m_inputFilePath = NULL;
            m_inputFacePosX = NULL;
            m_inputFaceNegX = NULL;
            m_inputFacePosY = NULL;
            m_inputFaceNegY = NULL;
            m_inputFacePosZ = NULL;
            m_inputFaceNegZ = NULL;

            // Image operations.
            m_inputGammaPowNumerator    = 1.0f;
            m_inputGammaPowDenominator  = 1.0f;
            m_outputGammaPowNumerator   = 1.0f;
            m_outputGammaPowDenominator = 1.0f;
            m_generateMipMapChain       = false;

            // Cubemap rotate/flip.
            m_imageOpPosX = 0;
            m_imageOpPosY = 0;
            m_imageOpPosZ = 0;
            m_imageOpNegX = 0;
            m_imageOpNegY = 0;
            m_imageOpNegZ = 0;

            // Filter parameters.
            m_filterType    = PipelineFilterType::None;
            m_srcFaceSize   = 0;
            m_excludeBase   = false;
            m_mipCount      = 9;
            m_glossScale    = 10;
            m_glossBias     = 1;
            m_dstFaceSize   = 0;
            m_lightingModel = LightingModel::Phong;
            m_edgeFixup     = EdgeFixup::None;
            m_processingMode = RadianceFilterProcessing::Auto;

            // Processing devices.
            m_numCpuProcessingThreads = UINT32_MAX;
            m_useOpenCL               = true;
            m_clVendor                = CMFT_CL_VENDOR_ANY_GPU;
            m_vendorStrPart           = NULL;
            m_deviceType              = CMFT_CL_DEVICE_TYPE_GPU;
            m_deviceIndex             = 0;

            // Output.
            m_outputFilesNum = 0;

            // Misc.
            m_silent = false;

            // Encode.
            m_encodeRGBM = false;
        }

        // Input.
        const char* m_inputFilePath;
        const char* m_inputFacePosX;
        const char* m_inputFaceNegX;
        const char* m_inputFacePosY;
        const char* m_inputFaceNegY;
        const char* m_inputFacePosZ;
        const char* m_inputFaceNegZ;

        // Image operations.
        float m_inputGammaPowNumerator;
        float m_inputGammaPowDenominator;
        float m_outputGammaPowNumerator;
        float m_outputGammaPowDenominator;
        bool m_generateMipMapChain;

        // Cubemap rotate/flip.
        uint32_t m_imageOpPosX;
        uint32_t m_imageOpPosY;
        uint32_t m_imageOpPosZ;
        uint32_t m_imageOpNegX;
        uint32_t m_imageOpNegY;
        uint32_t m_imageOpNegZ;

        // Filter parameters.
        PipelineFilterType::Enum m_filterType;
        uint32_t m_srcFaceSize;
        bool m_excludeBase;
        uint32_t m_mipCount;
        uint32_t m_glossScale;
        uint32_t m_glossBias;
        uint32_t m_dstFaceSize;
        LightingModel::Enum m_lightingModel;
        EdgeFixup::Enum m_edgeFixup;
        RadianceFilterProcessing::Enum m_processingMode;

        // Processing devices.
        uint32_t m_numCpuProcessingThreads;
        bool m_useOpenCL;
        uint32_t m_clVendor;
        const char* m_vendorStrPart;
        uint32_t m_deviceType;
        uint32_t m_deviceIndex;

        // Output.
        uint32_t m_outputFilesNum;
        PipelineOutputFile m_outputFiles[CMFT_PIPELINE_MAX_OUTPUTS];

        // Misc.
        bool m_silent;

        // Encode.
        bool m_encodeRGBM;
    };

    struct PipelineResult
    {
        PipelineResult()
        {
            m_success = false;
            m_outputFilesProcessed = 0;
        }

        bool m_success;
        uint32_t m_outputFilesProcessed;
    };

    int32_t pipelineRun(const PipelineRequest& _request, PipelineResult* _result = NULL);

} // namespace cmft

#endif // CMFT_PIPELINE_H_HEADER_GUARD

/* vim: set sw=4 ts=4 expandtab: */
