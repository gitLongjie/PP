#pragma once

#include <memory>

#include "Core/Constant.h"
#include "Window/WindowImpl.h"
#include "Window/WindowSettings.h"

#include "RHI/DriverSettings.h"

namespace PPEngine {
    namespace Window {
        class Window {
        public:
            explicit Window(PPRHI::DriverSettings& driverSetting, const WindowSettings& settings, std::unique_ptr<WindowImpl> impl) noexcept;

            bool ShouldClose() const;
            void MakeCurrentContext() const;
            void SwapBuffers() const;

            //Core::WindowHandle GetWindowHandle() const;

        private:
            PPRHI::DriverSettings& driverSetting_;
            const WindowSettings& settings_;
            std::unique_ptr<WindowImpl> impl_;
        };
    }
}