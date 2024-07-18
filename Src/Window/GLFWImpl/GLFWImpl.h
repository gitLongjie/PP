#pragma once

#include <glfw/glfw3.h>

#include "Window/WindowImpl.h"

namespace Window {
    class GLFWImpl : public WindowImpl {
    public:
        explicit GLFWImpl() noexcept;
        ~GLFWImpl() override;

        bool Create(const WindowSettings& settings, PPRHI::DriverSettings& driverSetting) override;
        bool ShouldClose() const override;
        void MakeCurrentContext() const override;
        void SwapBuffers() const override;

        //Core::WindowHandle GetWindowHandle() const override { return nullptr; /* window_;*/ }

    private:
        GLFWwindow* window_{ nullptr };
    };
}