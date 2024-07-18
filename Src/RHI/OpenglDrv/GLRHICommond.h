#pragma once

#include <vector>
#include <string>

#include "RHI/RHICommond.h"

#include "Core/Mesh.h"

namespace PPRHI {
    class GLVAOCreateCommand : public RHICommond {
    public:
        GLVAOCreateCommand(
            Core::Mesh* mesh,
            unsigned int vaoHandle,
            unsigned int vboHandle,
            unsigned int eboHandle
        );
        ~GLVAOCreateCommand() override;

    protected:
        void OnExecute() override;

    private:
        unsigned char* vertex_{ nullptr };
        unsigned int vertexCount_{ 0 };
        unsigned int* indices_{ nullptr };
        unsigned int indicesCount_{ 0 };

        unsigned int vaoHandle_{ 0 };
        unsigned int vao_{ 0 };
        unsigned int vboHandle_{ 0 };
        unsigned int vbo_{ 0 };
        unsigned int eboHandle_{ 0 };
        unsigned int ebo_{ 0 };
    };
        
    class GLDrawArrayCommand : public RHICommond {
    public:
        GLDrawArrayCommand(
            unsigned int vaoHandle,
            unsigned int vaoStart,
            unsigned int vaoCount
        );
        ~GLDrawArrayCommand() override = default;

    protected:
        void OnExecute() override;

    private:
        unsigned int vaoHandle_{ 0 };
        unsigned int vaoStart_{ 0 };
        unsigned int vaoCount_{ 0 };
    };
        
    class GLDrawElementsCommand : public RHICommond {
    public:
        GLDrawElementsCommand(
            unsigned int vaoHandle,
            unsigned int eboHandle,
            unsigned int eboCount
        );
        ~GLDrawElementsCommand() override = default;

    protected:
        void OnExecute() override;

    private:
        unsigned int vaoHandle_{ 0 };
        unsigned int eboHandle_{ 0 };
        unsigned int eboCount_{ 0 };
    };
        
    class GLVAODeleteCommand : public RHICommond {
    public:
        GLVAODeleteCommand(
            unsigned int vaoHandle,
            unsigned int vboHandle,
            unsigned int eboHandle
        );
        ~GLVAODeleteCommand() override = default;

    protected:
        void OnExecute() override;

    private:
        unsigned int vaoHandle_{ 0 };
        unsigned int vboHandle_{ 0 };
        unsigned int eboHandle_{ 0 };
    };

    class GLShaderCreateCommand : public RHICommond {
    public:
        GLShaderCreateCommand(
            unsigned int shaderHandle,
            unsigned int type,
            const std::string& shader
        );
        ~GLShaderCreateCommand() override = default;

    protected:
        void OnExecute() override;

    private:
        unsigned int shaderHandle_{ 0 };
        std::string shader_;
        unsigned int type_{ 0 };
    };

    class GLShaderDeleteCommand : public RHICommond {
    public:
        GLShaderDeleteCommand(
            unsigned int shaderHandle
        );
        ~GLShaderDeleteCommand() override = default;

    protected:
        void OnExecute() override;

    private:
        unsigned int shaderHandle_{ 0 };
    };

    class GLProgramCreateCommand : public RHICommond {
    public:
        GLProgramCreateCommand(
            unsigned int programHandle,
            std::vector<unsigned int> shaders
        );
        ~GLProgramCreateCommand() override = default;

    protected:
        void OnExecute() override;

    private:
        unsigned int programHandle_{ 0 };
        std::vector<unsigned int> shaders_;
    };
        
    class GLProgramUseCommand : public RHICommond {
    public:
        GLProgramUseCommand(
            unsigned int programHandle,
            bool use
        );
        ~GLProgramUseCommand() override = default;

    protected:
        void OnExecute() override;

    private:
        unsigned int programHandle_{ 0 };
        bool use_;
    };

    class GLProgramDeleteCommand : public RHICommond {
    public:
        GLProgramDeleteCommand(
            unsigned int programHandle
        );
        ~GLProgramDeleteCommand() override = default;

    protected:
        void OnExecute() override;

    private:
        unsigned int programHandle_{ 0 };
    };

        
    class GLEndCommand : public RHICommond {
    public:
        GLEndCommand();
        virtual ~GLEndCommand() = default;

    protected:
        void OnExecute() override;
    };
}
