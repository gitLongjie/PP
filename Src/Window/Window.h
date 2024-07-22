#pragma once

#include <memory>

#include "Core/Object/Object.h"
#include "Window/WindowImpl.h"
#include "Window/WindowSettings.h"

#include "RHI/DriverSettings.h"

namespace Window {
    class Window : public Core::Object {
    public:
        explicit Window(PPRHI::DriverSettings& driverSetting,
            const WindowSettings& settings, WindowImpl* impl) noexcept;


        bool ShouldClose() const;
        void MakeCurrentContext() const;
        void SwapBuffers() const;

        //Core::WindowHandle GetWindowHandle() const;
    protected:
        ~Window() override;

    private:
        PPRHI::DriverSettings& driverSetting_;
        const WindowSettings& settings_;
        WindowImpl* impl_;

        IMPLEMENT_OBJECT_REFCOUN(Window)
    };
}
