#pragma once

#include "Core/Constent.h"

#include "RHI/DriverSettings.h"
#include "Window/WindowSettings.h"

namespace PPEngine {
    namespace Window {
        class WindowImpl {
        public:
            virtual ~WindowImpl() = default;

            virtual bool Create(const WindowSettings& settings, PPRHI::DriverSettings& driverSetting) = 0;
            virtual bool ShouldClose() const = 0;
            virtual void MakeCurrentContext() const = 0;
            virtual void SwapBuffers() const = 0;

            //virtual Core::WindowHandle 
            // GetWindowHandle() const = 0;
        };
    }
}