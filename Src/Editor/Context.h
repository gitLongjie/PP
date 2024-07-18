#pragma once

#include <memory>

#include "Window/Window.h"
#include "Window/WindowSettings.h"

namespace Editor {
    class Context {
    public:
        explicit Context() noexcept;
        ~Context() = default;

        bool Initialize();
        void Uninitialize();

        bool IsRunning() const;
        void SwapBuffers() const;

    //     PPWindow::Window* GetWindow() const { return window_.get(); }

    private:
        Window::WindowSettings windowSettings_;
        std::unique_ptr<Window::Window> window_;
    };
}
