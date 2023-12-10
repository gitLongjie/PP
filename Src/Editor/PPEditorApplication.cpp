#include "PPEditorApplication.h"

#include <assert.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "RHI/RHI.h"
#include "RHI/OpenglDrv/GLShader.h"

#include "Core/Mesh.h"
#include "Renderer/MeshRender.h"


static const char* shaderCodeVertex = R"(
#version 410 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;
layout (location=2) in vec2 auv;

layout (location=0) out vec3 color;
layout (location=1) out vec2 uv;

void main()
{
	gl_Position = vec4(aPos, 1.0);
    color = aColor;
	uv = auv;
}
)";

static const char* shaderCodeFragment = R"(
#version 410 core
layout (location=0) in vec3 color;
layout (location=1) in vec2 uv;

layout (location=0) out vec4 out_FragColor;

void main()
{
	out_FragColor = vec4(color, 1.0);
};
)";

namespace PPEngine {
    namespace PPEditor {

        PPEditorApplication::PPEditorApplication(Core::MainLoop& mainLoop) noexcept
            : PPApplication()
            , mainLoop_(mainLoop){

        }

        PPEditorApplication::~PPEditorApplication() {

        }

        bool PPEditorApplication::Initialize() {
            if (!PPApplication::Initialize()) {
                return false;
            }
            return true;
           // return context_.Initialize();
        }

        int PPEditorApplication::RunLoop() {
            return mainLoop_.Run();
            PPRHI::RHI* rhi = PPRHI::RHI::Get();
            assert(nullptr != rhi);

            const float vertices[] = {
                //    ---- 位置 ----  ---- 颜色 ----     - 纹理坐标 -
                -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // 左下
                 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // 右下
                 0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // 右上
                -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f  // 左上
            };
            unsigned int indices[] = {
                0, 1, 2,
                2, 3, 0
            };

            Core::Mesh::Vertex vertexs[4] = {
                { { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, {0.0f, 1.0f}, { 0.0f, 0.0f, 0.0f } },
                { {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, {1.0f, 1.0f}, { 0.0f, 0.0f, 0.0f } },
                { {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, {1.0f, 0.0f}, { 0.0f, 0.0f, 0.0f } },
                { { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f } }
            };

            Core::Mesh mesh;
            int count = sizeof(Core::Mesh::Vertex);
            mesh.SetVertices(vertexs, 4, false);
            mesh.SetIndices(indices, 6, false);
            {
                Renderer::MeshRender meshRender(&mesh);
                rhi->StartRender();
                auto vertexSource = PPRHI::GLShader::CreateSource(GL_VERTEX_SHADER, shaderCodeVertex);
                auto fragmSource = PPRHI::GLShader::CreateSource(GL_FRAGMENT_SHADER, shaderCodeFragment);
                PPRHI::GLShader shader(vertexSource, fragmSource);

                while (context_.IsRunning()) {
                    rhi->UpdateScreenSize();
                    shader.Use();
                    meshRender.Render();
                    rhi->EndFrame();
                    rhi->PollEvents();
                }
            }
            rhi->StopRender();
            return PPApplication::RunLoop();
        }

        void PPEditorApplication::Uninitialize() {
            //context_.Uninitialize();
            PPApplication::Uninitialize();
        }

    }
}