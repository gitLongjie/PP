#pragma once

#include "Core/Object/Object.h"

#include "RHI/DriverSettings.h"
#include "Window/WindowSettings.h"

namespace Window {
    class WindowImpl : public Core::Object {
    public:
        virtual ~WindowImpl() = default;

        virtual bool Create(const WindowSettings& settings, PPRHI::DriverSettings& driverSetting) = 0;
        virtual bool ShouldClose() const = 0;
        virtual void MakeCurrentContext() const = 0;
        virtual void SwapBuffers() const = 0;
    };
}
