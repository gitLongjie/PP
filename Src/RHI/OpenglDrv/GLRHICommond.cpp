#include "RHI/OpenglDrv/GLRHICommond.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "Core/Logger.h"
#include "RHI/RHI.h"
#include "GLHandleMapper.h"

namespace PPEngine {
    namespace PPRHI {
        GLVAOCreateCommand::GLVAOCreateCommand(
            Core::Mesh* mesh,
            unsigned int vaoHandle,
            unsigned int vboHandle,
            unsigned int eboHandle
        )
            : RHICommond(RHICommond::Type::CREATE_VAO)
            , vaoHandle_(vaoHandle)
            , vboHandle_(vboHandle)
            , eboHandle_(eboHandle) {
            const Core::Mesh::Vertex* vertex = mesh->GetVertices();
            unsigned int count = mesh->GetVertexCount() * sizeof(Core::Mesh::Vertex);

            vertex_ = new unsigned char[count] {0};
            vertexCount_ = mesh->GetVertexCount();
            memcpy(vertex_, vertex, count);

            const unsigned int* indices = mesh->GetIndices();
            if (nullptr == indices) {
                return;
            }

            count = mesh->GetIndexCount();
            indices_ = new unsigned int[count] {0};
            indicesCount_ = mesh->GetIndexCount();
            memcpy(indices_, indices, count * sizeof(unsigned int));
        }

        GLVAOCreateCommand::~GLVAOCreateCommand() {
            if (nullptr != vertex_) {
                delete[] vertex_;
            }

            if (nullptr != indices_) {
                delete[] indices_;
            }
        }

        void GLVAOCreateCommand::OnExecute() {
            glCreateVertexArrays(1, &vao_);
            glBindVertexArray(vao_);

            glCreateBuffers(1, &vbo_);
            const unsigned int vertexCount = vertexCount_ * sizeof(Core::Mesh::Vertex);
            glNamedBufferStorage(vbo_, vertexCount, nullptr, GL_DYNAMIC_STORAGE_BIT);
            glNamedBufferSubData(vbo_, 0, vertexCount, vertex_);
            GLHandleMapper::MapVBOHandle(vboHandle_, vbo_);

            glVertexArrayVertexBuffer(vao_, 0, vbo_, 0, sizeof(Core::Mesh::Vertex));

            glEnableVertexArrayAttrib(vao_, 0);
            glVertexArrayAttribFormat(vao_, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Core::Mesh::Vertex, position_));
            glVertexArrayAttribBinding(vao_, 0, 0);

            glEnableVertexArrayAttrib(vao_, 1);
            glVertexArrayAttribFormat(vao_, 1, 4, GL_FLOAT, GL_FALSE, offsetof(Core::Mesh::Vertex, color_));
            glVertexArrayAttribBinding(vao_, 1, 0);

            glEnableVertexArrayAttrib(vao_, 2);
            glVertexArrayAttribFormat(vao_, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Core::Mesh::Vertex, uv_));
            glVertexArrayAttribBinding(vao_, 2, 0);

            glEnableVertexArrayAttrib(vao_, 3);
            glVertexArrayAttribFormat(vao_, 3, 3, GL_FLOAT, GL_FALSE, offsetof(Core::Mesh::Vertex, normal_));
            glVertexArrayAttribBinding(vao_, 3, 0);
            GLHandleMapper::MapVAOHandle(vaoHandle_, vao_);

            if (nullptr == indices_) {
                return;
            }

            glCreateBuffers(1, &ebo_);
            const unsigned int indicesCount = indicesCount_ * sizeof(unsigned int);
            glNamedBufferStorage(ebo_, indicesCount, nullptr, GL_DYNAMIC_STORAGE_BIT);
            glNamedBufferSubData(ebo_, 0, indicesCount, indices_);
            GLHandleMapper::MapEBOHandle(eboHandle_, ebo_);
        }

        GLDrawArrayCommand::GLDrawArrayCommand(unsigned int vaoHandle, unsigned int vaoStart, unsigned int vaoCount)
            : RHICommond(RHICommond::Type::DRAW_ARRAYS)
            , vaoHandle_(vaoHandle)
            , vaoStart_(vaoStart)
            , vaoCount_(vaoCount) {}

        void GLDrawArrayCommand::OnExecute() {
            unsigned int vao = GLHandleMapper::GetVAO(vaoHandle_);
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, vaoStart_, vaoCount_);
            glBindVertexArray(0);
        }

        GLDrawElementsCommand::GLDrawElementsCommand(unsigned int vaoHandle, unsigned int eboHandle, unsigned int eboCount)
            : RHICommond(RHICommond::Type::DRAW_ELEMENTS)
            , vaoHandle_(vaoHandle)
            , eboHandle_(eboHandle)
            , eboCount_(eboCount) {}

        void GLDrawElementsCommand::OnExecute() {
            unsigned int vao = GLHandleMapper::GetVAO(vaoHandle_);
            unsigned int ebo = GLHandleMapper::GetEBO(eboHandle_);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, 1);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glDrawElements(GL_TRIANGLES, eboCount_, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        GLVAODeleteCommand::GLVAODeleteCommand(unsigned int vaoHandle, unsigned int vboHandle, unsigned int eboHandle)
            : RHICommond(RHICommond::Type::DELETE_VAO)
            , vaoHandle_(vaoHandle)
            , vboHandle_(vboHandle)
            , eboHandle_(eboHandle) {}

        void GLVAODeleteCommand::OnExecute() {
            if (0 != eboHandle_) {
                unsigned int ebo = GLHandleMapper::GetEBO(eboHandle_);
                if (0 != ebo && glIsBuffer(ebo)) {
                    GLHandleMapper::UnmapEBOHandle(eboHandle_);
                    glDeleteBuffers(1, &ebo);
                }
            }

            if (0 != vboHandle_) {
                unsigned int vbo = GLHandleMapper::GetVBO(vboHandle_);
                if (0 != vbo && glIsBuffer(vbo)) {
                    GLHandleMapper::UnmapVBOHandle(vboHandle_);
                    glDeleteBuffers(1, &vbo);
                }
            }

            if (0 != vaoHandle_) {
                unsigned int vao = GLHandleMapper::GetVAO(vaoHandle_);
                if (0 != vao && glIsVertexArray(vao)) {
                    GLHandleMapper::UnmapVAOHandle(vaoHandle_);
                    glDeleteVertexArrays(1, &vao);
                }
            }
        }

        GLShaderCreateCommand::GLShaderCreateCommand(unsigned int shaderHandle, unsigned int type, const std::string& shader)
            : RHICommond(RHICommond::Type::CREATE_SHADER)
            , shaderHandle_(shaderHandle)
            , type_(type)
            , shader_(shader) {}

        void GLShaderCreateCommand::OnExecute() {
            unsigned int handle = glCreateShader(type_);
            const char* shaderSource = shader_.c_str();
            glShaderSource(handle, 1, &shaderSource, nullptr);
            glCompileShader(handle);

            char buffer[8192];
            GLsizei length = 0;
            glGetShaderInfoLog(handle, sizeof(buffer), &length, buffer);

            if (length) {
                ERRORLOG(" CompileShader error {}", buffer);
                assert(false);
                return;
            }
            GLHandleMapper::MapShaderHandle(shaderHandle_, handle);
        }

        GLShaderDeleteCommand::GLShaderDeleteCommand(unsigned int shaderHandle)
            : RHICommond(RHICommond::Type::DELETE_SHADER)
            , shaderHandle_(shaderHandle) {}

        void GLShaderDeleteCommand::OnExecute() {
            if (0 != shaderHandle_) {
                unsigned int shader = GLHandleMapper::GetShader(shaderHandle_);
                if (0 != shader && glIsShader(shader)) {
                    GLHandleMapper::UnmapShaderHandle(shaderHandle_);
                    glDeleteShader(shader);
                }
            }
        }

        GLProgramCreateCommand::GLProgramCreateCommand(unsigned int programHandle, std::vector<unsigned int> shaders)
            : RHICommond(RHICommond::Type::CREATE_PROGRAM)
            , programHandle_(programHandle)
            , shaders_(shaders) {}

        void GLProgramCreateCommand::OnExecute() {
            if (shaders_.empty()) {
                return;
            }

            unsigned int program = glCreateProgram();
            for (auto shader : shaders_) {
                unsigned int shaderHandle = GLHandleMapper::GetShader(shader);
                if (0 != shaderHandle && glIsShader(shaderHandle)) {
                    glAttachShader(program, shaderHandle);
                }
            }

            glLinkProgram(program);
            char buffer[8192];
            GLsizei length = 0;
            glGetProgramInfoLog(program, sizeof(buffer), &length, buffer);
            if (length) {
                ERRORLOG(" LinkProgram error {}", buffer);
                glDeleteProgram(program);
                assert(false);
                return;
            }
            GLHandleMapper::MapProgramHandle(programHandle_, program);
        }

        GLProgramUseCommand::GLProgramUseCommand(unsigned int programHandle, bool use)
            : RHICommond(RHICommond::Type::DELETE_PROGRAM)
            , programHandle_(programHandle)
            , use_(use) {}

        void GLProgramUseCommand::OnExecute() {
            assert(0 != programHandle_);
            unsigned int program = GLHandleMapper::GetProgram(programHandle_);
            glUseProgram(program);
        }

        GLProgramDeleteCommand::GLProgramDeleteCommand(unsigned int programHandle)
            : RHICommond(RHICommond::Type::DELETE_PROGRAM)
            , programHandle_(programHandle) {}

        void GLProgramDeleteCommand::OnExecute() {
            if (0 != programHandle_) {
                unsigned int program = GLHandleMapper::GetProgram(programHandle_);
                if (0 != program && glIsProgram(program)) {
                    GLHandleMapper::UnmapProgramHandle(programHandle_);
                    glDeleteProgram(program);
                }
            }
        }

        GLEndCommand::GLEndCommand()
            : RHICommond(RHICommond::Type::END_FRAME) {

        }

        void GLEndCommand::OnExecute() {
            const DriverSettings& setting = RHI::Get()->GetDriverSettings();
            //glfwSwapBuffers(reinterpret_cast<GLFWwindow*>(setting.windowHandle));
        }

        

}
}