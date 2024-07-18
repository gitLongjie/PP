#pragma once

#include <vector>

#include "RHI/IRHIAPI.h"

namespace PPRHI {
    class GLDrvAPI : public IRHIAPI {
    public:
        explicit GLDrvAPI() noexcept = default;
        ~GLDrvAPI() override = default;

        bool InitEnv(const DriverSettings& setting) override;
        bool InitRenderEnv(const DriverSettings& setting) override;
        void UpdateRenderSize(const DriverSettings& setting) override;
        void UninitEnv() override;
        void PollEvents() override;

        std::shared_ptr<RHICommond> CreateVAOCommand(
            Core::Mesh* mesh,
            unsigned int vaoHandle,
            unsigned int vboHandle,
            unsigned int eboHandle
        ) override;
        std::shared_ptr<RHICommond> DrawArrayCommand(
            unsigned int vaoHandle,
            unsigned int vaoStart,
            unsigned int vaoCount
        ) override;
        std::shared_ptr<RHICommond> DrawElementsCommand(
            unsigned int vaoHandle,
            unsigned int eboHandle,
            unsigned int eboCount
        ) override;
        std::shared_ptr<RHICommond> DeleteVAOCommand(
            unsigned int vaoHandle,
            unsigned int vboHandle,
            unsigned int eboHandle
        ) override;
        std::shared_ptr<RHICommond> CreateShaderCommand(
            unsigned int shaderHandle,
            unsigned int type,
            const std::string& shader
        ) override;
        std::shared_ptr<RHICommond> DeleteShaderCommand(
            unsigned int shaderHandle
        ) override;
        std::shared_ptr<RHICommond> CreateProgramCommand(
            unsigned int programHandle,
            const std::vector<unsigned int>& shaders
        ) override;
        std::shared_ptr<RHICommond> UseProgramCommand(
            unsigned int programHandle,
            bool use
        );
        std::shared_ptr<RHICommond> DeleteProgramCommand(
            unsigned int programHandle
        ) override;
        std::shared_ptr<RHICommond> CreateEndCommand() override;
    };
}
