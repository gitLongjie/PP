#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window/GLFWImpl/GLFWImpl.h"



#include "Core/Logger.h"

namespace PPEngine {
    namespace Window {
        GLFWImpl::GLFWImpl() noexcept {
        }

        GLFWImpl::~GLFWImpl() {
            if (nullptr != window_) {
                glfwDestroyWindow(window_);
                window_ = nullptr;
            }
        }

        bool GLFWImpl::Create(const WindowSettings& settings, PPRHI::DriverSettings& driverSetting) {
            glfwWindowHint(GLFW_RESIZABLE, settings.resizable);
            glfwWindowHint(GLFW_DECORATED, settings.decorated);
            glfwWindowHint(GLFW_FOCUSED, settings.focused);
            glfwWindowHint(GLFW_MAXIMIZED, settings.maximized);
            glfwWindowHint(GLFW_FLOATING, settings.floating);
            glfwWindowHint(GLFW_VISIBLE, settings.visible);
            glfwWindowHint(GLFW_AUTO_ICONIFY, settings.autoIconify);
            //glfwWindowHint(GLFW_REFRESH_RATE, settings.refreshRate);
            glfwWindowHint(GLFW_SAMPLES, settings.samples);

            window_ = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, nullptr);
            if (!window_) {
                ERRORLOG("glfwCreateWindow failed");
                return false;
            }

            //driverSetting.windowHandle = window_;

            glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                }
            );

            return true;
        }

        bool GLFWImpl::ShouldClose() const {
            if (nullptr == window_) {
                WARNLOG("window_ is nullptr");
                return false;
            }
            return !glfwWindowShouldClose(window_);
        }

        void GLFWImpl::MakeCurrentContext() const {
            glfwMakeContextCurrent(window_);
        }

        void GLFWImpl::SwapBuffers() const {
            glfwSwapBuffers(window_);
        }

    }
}