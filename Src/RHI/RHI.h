#pragma once

#include <memory>

#include "Core/Constant.h"
#include "Core/Singleton.h"

#include "RHI/DriverSettings.h"
#include "RHI/IRHIAPI.h"

namespace PPRHI {
    class RHI : public Core::Singleton<RHI> {
    public:
        explicit RHI() noexcept;
        ~RHI() override;

        bool Initialize() override;
        void Uninitialize() override;

        bool InitRenderEnv();
        void UpdaeRenderSize();

        void UpdateScreenSize();
        void EndFrame();
        void PollEvents();
        void StartRender();
        void StopRender();

        void Push(std::shared_ptr<class RHICommond> commond);

        IRHIAPI* GetRHIAPI() const noexcept { return rhiAPI_.get(); }

        const DriverSettings& GetDriverSettings() const noexcept { return driverSettings_; }
        DriverSettings& GetDriverSettings() noexcept { return driverSettings_; }

    private:
        DriverSettings driverSettings_;
        std::unique_ptr<IRHIAPI> rhiAPI_;
        std::shared_ptr<class RHICommonList> rhiCommonList_;
    };
}
