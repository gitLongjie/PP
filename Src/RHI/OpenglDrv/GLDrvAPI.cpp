#include "RHI/OpenglDrv/GLDrvAPI.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Core/Logger.h"

#include "RHI/RHI.h"
#include "RHI/OpenglDrv/GLRHICommond.h"

namespace PPRHI {

    bool GLDrvAPI::InitEnv(const DriverSettings& setting) {
        auto glfwErrorCallback = [](int error, const char* description) {
            LOGE("code={}, description={}", error, description);
        };

        glfwSetErrorCallback(glfwErrorCallback);

        if (GLFW_TRUE != glfwInit()) {
            return false; //exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, setting.contextMajorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, setting.contextMinorVersion);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, setting.samples);

        if (setting.debugProfile) {
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        }

        glewExperimental = GL_TRUE;
        return true;
    }

    bool GLDrvAPI::InitRenderEnv(const DriverSettings& setting) {
        // Initialize GLEW to setup the OpenGL Function pointers
        //glfwMakeContextCurrent(reinterpret_cast<GLFWwindow*>(setting.windowHandle));
        GLenum err = glewInit();
        if (GLEW_OK != err) {
            const GLubyte* errorString = glewGetErrorString(err);
            LOGE("glewInit={1}, err={2}", err, reinterpret_cast<const char*>(errorString));
            return false;
        }
        glfwSwapInterval(1);
        return true;
    }

    void GLDrvAPI::UpdateRenderSize(const DriverSettings& setting) {
        int width = 0, height = 0;
        //glfwGetFramebufferSize(reinterpret_cast<GLFWwindow*>(setting.windowHandle), &width, &height);
        glViewport(0, 0, width, height);

        glViewport(0, 0, width, height);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Screen::set_width_height(width, height);
    }

    void GLDrvAPI::UninitEnv() {
        glfwTerminate();
    }

    void GLDrvAPI::PollEvents() {
        glfwPollEvents();
    }

    std::shared_ptr<RHICommond> GLDrvAPI::CreateVAOCommand(
        Core::Mesh* mesh,
        unsigned int vaoHandle,
        unsigned int vboHandle, 
        unsigned int eboHandle
    ) {
        auto endCommand = std::make_shared<GLVAOCreateCommand>(mesh, vaoHandle, vboHandle, eboHandle);
        return endCommand;
    }

    std::shared_ptr<RHICommond> GLDrvAPI::DrawArrayCommand(unsigned int vaoHandle, unsigned int vaoStart, unsigned int vaoCount) {
        auto endCommand = std::make_shared<GLDrawArrayCommand>(vaoHandle, vaoStart, vaoCount);
        return endCommand;
    }

    std::shared_ptr<RHICommond> GLDrvAPI::DrawElementsCommand(unsigned int vaoHandle, unsigned int eboHandle, unsigned int eboCount) {
        auto endCommand = std::make_shared<GLDrawElementsCommand>(vaoHandle, eboHandle, eboCount);
        return endCommand;
    }

    std::shared_ptr<RHICommond> GLDrvAPI::DeleteVAOCommand(unsigned int vaoHandle, unsigned int vboHandle, unsigned int eboHandle) {
        auto endCommand = std::make_shared<GLVAODeleteCommand>(vaoHandle, vboHandle, eboHandle);
        return endCommand;
    }

    std::shared_ptr<RHICommond> GLDrvAPI::CreateShaderCommand(unsigned int shaderHandle, unsigned int type, const std::string& shader) {
        auto endCommand = std::make_shared<GLShaderCreateCommand>(shaderHandle, type, shader);
        return endCommand;
    }

    std::shared_ptr<RHICommond> GLDrvAPI::DeleteShaderCommand(unsigned int shaderHandle) {
        auto endCommand = std::make_shared<GLShaderDeleteCommand>(shaderHandle);
        return endCommand;
    }

    std::shared_ptr<RHICommond> GLDrvAPI::CreateProgramCommand(unsigned int programHandle, const std::vector<unsigned int>& shaders) {
        auto endCommand = std::make_shared<GLProgramCreateCommand>(programHandle, shaders);
        return endCommand;
    }

    std::shared_ptr<RHICommond> GLDrvAPI::UseProgramCommand(unsigned int programHandle, bool use) {
        auto endCommand = std::make_shared<GLProgramUseCommand>(programHandle, use);
        return endCommand;
    }

    std::shared_ptr<RHICommond> GLDrvAPI::DeleteProgramCommand(unsigned int programHandle) {
        auto endCommand = std::make_shared<GLProgramDeleteCommand>(programHandle);
        return endCommand;
    }

    std::shared_ptr<RHICommond> GLDrvAPI::CreateEndCommand() {
        auto endCommand = std::make_shared<GLEndCommand>();
        return endCommand;
    }

}
