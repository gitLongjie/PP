#include "RHI.h"

#include "Core/Logger.h"
#include "Core/SpinLock.h"
#include "OpenGLDrv/GLDrvAPI.h"

#include "RHI/RHICommondList.h"

PPRHI::RHI* Core::Singleton<PPRHI::RHI>::instance_ = nullptr;

namespace PPRHI {

    RHI::RHI() noexcept {
    }

    RHI::~RHI() {
            
    }

    bool RHI::Initialize() {
        rhiAPI_ = std::make_unique<GLDrvAPI>();
        if (!rhiAPI_->InitEnv(driverSettings_)) {
            LOGE("RHI init evn failed");
            return false;
        }

        rhiCommonList_ = RHICommonList::Create();
        if (!rhiCommonList_->Start()) {
            LOGE("RHICommonList start failed");
            return false;
        }
        return true;
    }

    void RHI::Uninitialize() {
        if (rhiCommonList_) {
            rhiCommonList_->Stop();
            rhiCommonList_.reset();
        }

        if (!rhiAPI_) {
            LOGE("RHI is not initialized");
            return;
        }
        rhiAPI_->UninitEnv();
    }

    bool RHI::InitRenderEnv() {
        return rhiAPI_->InitRenderEnv(driverSettings_);
    }

    void RHI::UpdaeRenderSize() {
        rhiAPI_->UpdateRenderSize(driverSettings_);
    }

    void RHI::UpdateScreenSize() {
        auto task = UpdateRenderSizeCommand::Create();
        rhiCommonList_->Push(task);
        task->Wait();
    }

    void RHI::EndFrame() {
        auto task = rhiAPI_->CreateEndCommand();
        rhiCommonList_->Push(task);
        task->Wait();
    }

    void RHI::PollEvents() {
        rhiAPI_->PollEvents();
    }

    void RHI::StartRender() {
        auto task = StartRenderCommand::Create();
        rhiCommonList_->Push(task);
        task->Wait();
    }

    void RHI::StopRender() {
        auto task = EndRenderCommand::Create();
        rhiCommonList_->Push(task);
        task->Wait();
    }

    void RHI::Push(std::shared_ptr<class RHICommond> commond) {
        rhiCommonList_->Push(commond);
    }

}
