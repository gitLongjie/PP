#include "Editor/Context.h"

#include "RHI/RHI.h"
#include "RHI/DriverSettings.h"
#include "Context.h"

namespace Editor {

    Context::Context() noexcept {
           
    }

    bool Context::Initialize() {
        windowSettings_.title = "PPEngine Editor";
        windowSettings_.width = 1280;
        windowSettings_.height = 720;
        windowSettings_.maximized = true;

        PPRHI::DriverSettings& driverSettings = PPRHI::RHI::Get()->GetDriverSettings();

        //   window_ = std::make_unique<PPWindow::Window>(driverSettings, windowSettings_);

        return true;
    }

    void Context::Uninitialize() {
      
    }

    bool Context::IsRunning() const {
        return false;// window_->ShouldClose();
    }

    void Context::SwapBuffers() const {
        // window_->SwapBuffers();
    }
}
