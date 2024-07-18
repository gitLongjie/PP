#pragma once

#include <string>

#include "Application/PPApplication.h"
#include "PPFramesPerSecondCounter.h"
#include "PPCamera.h"
#include "PPScene.h"

class PPGLApplication : public PPApplication {
public:
    PPGLApplication(int width, int height, const char* title);
    ~PPGLApplication();

public:
    bool Initialize() override;
    int RunLoop() override;
    void Uninitialize() override;

protected:
    void SwapBuffers();
    //struct GLFWwindow* GetWindow() const override { return m_window; }
    bool Create(int width, int height, const char* title);

private:
    void InputKeyBoardCallback(struct GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    int m_width{ 1 };
    int m_height{ 1 };
    std::string m_title{ "PPGLApplication" };
    struct GLFWwindow* m_window;

    PPFramesPerSecondCounter m_framePerSecondCounter;
    PPCamera m_camera;
    PPScene* m_scene{ nullptr };
};