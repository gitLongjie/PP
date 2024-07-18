#include "Renderer/MeshRender.h"

#include "RHI/OpenglDrv/GLHandleMapper.h"

namespace Renderer {
    MeshRender::MeshRender(Core::Mesh* mesh)
        : mesh_(mesh) {
        PPRHI::RHI* rhi = PPRHI::RHI::Get();
        rhiApi_ = rhi->GetRHIAPI();
    }

    MeshRender::~MeshRender() {
        if (0 == vaoHandle_) {
            return;
        }
        auto task = rhiApi_->DeleteVAOCommand(vaoHandle_, vboHandle_, eboHandle_);
        PPRHI::RHI::Get()->Push(task);
    }

    void MeshRender::Render() {
        if (nullptr == mesh_) {
            return;
        }

        if (0 == vaoHandle_) {
            vaoHandle_ = PPRHI::GLHandleMapper::GeneratorVAOHandle();
            vboHandle_ = PPRHI::GLHandleMapper::GeneratorVBOHandle();
            eboHandle_ = PPRHI::GLHandleMapper::GeneratorEBOHandle();
            auto task = rhiApi_->CreateVAOCommand(mesh_, vaoHandle_, vboHandle_, eboHandle_);
            PPRHI::RHI::Get()->Push(task);
        } else {
            auto task = rhiApi_->DrawElementsCommand(vaoHandle_, eboHandle_, mesh_->GetIndexCount());
            PPRHI::RHI::Get()->Push(task);
        }
    }
}
