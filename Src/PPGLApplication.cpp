#include "PPGLApplication.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "PPViewport.h"
#include "Core/Logger.h"


PPGLApplication::PPGLApplication(int width, int height, const char* title)
    : PPApplication()
    , m_width(width)
    , m_height(height)
    , m_title(title) {

}

PPGLApplication::~PPGLApplication() {

}

bool PPGLApplication::Create(int width, int height, const char* title) {
    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (nullptr == m_window) {
        return false;
    }

    glfwMakeContextCurrent(m_window);

    auto KeyboardEventCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        reinterpret_cast<PPGLApplication*>(PPGLApplication::Get())->InputKeyBoardCallback(window, key, scancode, action, mods);
    };

    // Set the required callback functions
    glfwSetKeyCallback(m_window, KeyboardEventCallback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    glfwSwapInterval(1);

    // Define the viewport dimensions
    //glViewport(0, 0, width, height);
    return true;
}

void PPGLApplication::InputKeyBoardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

bool PPGLApplication::Initialize() {
    if (!PPApplication::Initialize()) {
        return false;
    }

 /*   if (!Create(m_width, m_height, m_title.c_str())) {
        LOGE("create failed");
    }*/

    m_scene = new PPScene(&m_camera);
    m_scene->Init();
    return true;
}

int PPGLApplication::RunLoop() {
    if (nullptr == m_window) {
        LOGE("No window to run");
        return -1;
    }

    m_camera.Init();
    while (!glfwWindowShouldClose(m_window)) {
        m_framePerSecondCounter.OnFrame(m_deltaSeconds);

        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        const float ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_camera.Update(ratio);

        m_scene->Update();
        m_scene->Render();

        SwapBuffers();
    }
    m_camera.Uninit();

    return 0;
}

void PPGLApplication::Uninitialize() {
    if (nullptr != m_scene) {
        m_scene->Release();
        delete m_scene;
        m_scene = nullptr;
    }

    if (nullptr != m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    PPApplication::Uninitialize();
}

void PPGLApplication::SwapBuffers() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
    
    GLenum err = glGetError();
    assert(GL_NO_ERROR == err);

    const double newTimeStamp = glfwGetTime();
    m_deltaSeconds = static_cast<float>(newTimeStamp - m_timeStamp);
    m_timeStamp = newTimeStamp;
}


