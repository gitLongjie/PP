#pragma once

#include "PPFramesPerSecondCounter.h"
#include "PPCamera.h"
#include "PPGLShader.h"

class PPViewport {
public:
    PPViewport();

public:
    bool Init();
    void Uninit();
    void BeginRender(struct GLFWwindow* window);
    void Render(struct GLFWwindow* window);
    void EndRender(struct GLFWwindow* window);

private:
    double m_timeStamp{ 0.0 };
    float m_deltaSeconds = 0.0f;

    struct PerFrameData {
        glm::mat4 mvp;
    };

    PPFramesPerSecondCounter m_framePerSecondCounter;
    PPCamera m_camera;

    unsigned int vao{ 0 };
    unsigned int texture{ 0 };

    PPGLShader shaderVertex;
    PPGLShader shaderFragment;
    PPGLProgram shaderProgram;
};
