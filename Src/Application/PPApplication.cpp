#include "PPApplication.h"

#include <assert.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "Core/Logger.h"
#include "Core/Font.h"
#include "Core/Platform.h"
#include "Core/EventSystem/EventManager.h"
#include "RHI/RHI.h"


PPApplication* s_ppApplication = nullptr;

const GLuint WIDTH = 800, HEIGHT = 600;

PPApplication::PPApplication() {
    assert(nullptr == s_ppApplication);
    s_ppApplication = this;

    Core::Logger::Init();
}

PPApplication::~PPApplication() {
    Core::Logger::Shotdown();
    assert(nullptr != s_ppApplication);
    s_ppApplication = nullptr;
}

PPApplication* PPApplication::Get() {
    return s_ppApplication;
}

bool PPApplication::Initialize() {
    if (!Core::Platform::Init()) {
        return false;
    }

    if (!Core::FontManager::Init()) {
        return false;
    }

    if (!PPRHI::RHI::Init()) {
        return false;
    }

    if (!Core::EventSystem::EventManager::Init()) {
        return false;
    }

    /*   auto glfwErrorCallback = [](int error, const char* description) {
        LOGE("code={%1}, description={%2}", error, description);
        };
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit()) {
        LOGE("Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);*/

    return true;
}

int PPApplication::RunLoop() {

    return 0;
}

void PPApplication::Uninitialize() {
    Core::EventSystem::EventManager::Shotdown();
    PPRHI::RHI::Shotdown();
    Core::FontManager::Shotdown();
    Core::Platform::Shotdown();
}
