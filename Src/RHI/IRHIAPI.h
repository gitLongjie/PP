#pragma once

#include <memory>
#include <vector>

#include "Core/Constant.h"
#include "Core/Mesh.h"

#include "RHI/DriverSettings.h"
#include "RHI/RHICommond.h"

namespace PPRHI {
    class IRHIAPI {
    public:
        virtual ~IRHIAPI() = default;

        virtual bool InitEnv(const DriverSettings& setting) = 0;
        virtual bool InitRenderEnv(const DriverSettings& setting) = 0;
        virtual void UpdateRenderSize(const DriverSettings& setting) = 0;
        virtual void UninitEnv() = 0;
        virtual void PollEvents() = 0;

        virtual std::shared_ptr<RHICommond> CreateVAOCommand(
            Core::Mesh* mesh,
            unsigned int vaoHandle,
            unsigned int vboHandle,
            unsigned int eboHandle
        ) = 0;
        virtual std::shared_ptr<RHICommond> DrawArrayCommand(
            unsigned int vaoHandle,
            unsigned int vaoStart,
            unsigned int vaoCount
        ) = 0;
        virtual std::shared_ptr<RHICommond> DrawElementsCommand(
            unsigned int vaoHandle,
            unsigned int eboHandle,
            unsigned int eboCount
        ) = 0;
        virtual std::shared_ptr<RHICommond> DeleteVAOCommand(
            unsigned int vaoHandle,
            unsigned int vboHandle,
            unsigned int eboHandle
        ) = 0; 
        virtual std::shared_ptr<RHICommond> CreateShaderCommand(
            unsigned int shaderHandle,
            unsigned int type,
            const std::string& shader
        ) = 0;
        virtual std::shared_ptr<RHICommond> DeleteShaderCommand(
            unsigned int shaderHandle
        ) = 0;
        virtual std::shared_ptr<RHICommond> CreateProgramCommand(
            unsigned int programHandle,
            const std::vector<unsigned int>& shaders
        ) = 0;
        virtual std::shared_ptr<RHICommond> UseProgramCommand(
            unsigned int programHandle,
            bool use
        ) = 0;
        virtual std::shared_ptr<RHICommond> DeleteProgramCommand(
            unsigned int programHandle
        ) = 0;
        virtual std::shared_ptr<RHICommond> CreateEndCommand() = 0;
    };
}
