#include "Window/Window.h"

#include "Window/GLFWImpl/GLFWImpl.h"

namespace PPEngine {
    namespace Window {
        
        Window::Window(PPRHI::DriverSettings& driverSetting, const WindowSettings& settings, std::unique_ptr<WindowImpl> impl) noexcept
            : driverSetting_(driverSetting)
            , settings_(settings)
            , impl_(std::move(impl)) {
            impl_->Create(settings_, driverSetting_);
        }

        bool Window::ShouldClose() const {
            return impl_->ShouldClose();
        }

        void Window::MakeCurrentContext() const {
            impl_->MakeCurrentContext();
        }

        void Window::SwapBuffers() const {
            impl_->SwapBuffers();
        }

        /*Core::WindowHandle Window::GetWindowHandle() const {
            return impl_->GetWindowHandle();
        }*/

    }
}