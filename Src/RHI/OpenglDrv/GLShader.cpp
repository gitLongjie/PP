#include "RHI/OpenglDrv/GLShader.h"

#include <cassert>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Core/StringUtil.h"
#include "RHI/OpenglDrv/GLHandleMapper.h"
#include "RHI/IRHIAPI.h"
#include "RHI/RHI.h"

namespace PPRHI {

	static GLenum GLShaderTypeFromFileName(const char* fileName) {
		if (Core::EndsWith(fileName, ".vert"))
			return GL_VERTEX_SHADER;

		if (Core::EndsWith(fileName, ".frag"))
			return GL_FRAGMENT_SHADER;

		if (Core::EndsWith(fileName, ".geom"))
			return GL_GEOMETRY_SHADER;

		if (Core::EndsWith(fileName, ".tesc"))
			return GL_TESS_CONTROL_SHADER;

		if (Core::EndsWith(fileName, ".tese"))
			return GL_TESS_EVALUATION_SHADER;

		if (Core::EndsWith(fileName, ".comp"))
			return GL_COMPUTE_SHADER;

		assert(false);

		return 0;
	}


    std::string ReadShaderFile(const char* fileName) {
        FILE* file = fopen(fileName, "r");

        if (!file) {
            printf("I/O error. Cannot open shader file '%s'\n", fileName);
            return std::string();
        }

        fseek(file, 0L, SEEK_END);
        const auto bytesinfile = ftell(file);
        fseek(file, 0L, SEEK_SET);

        char* buffer = (char*)alloca(bytesinfile + 1);
        const size_t bytesread = fread(buffer, 1, bytesinfile, file);
        fclose(file);

        buffer[bytesread] = 0;

        static constexpr unsigned char BOM[] = { 0xEF, 0xBB, 0xBF };

        if (bytesread > 3) {
            if (!memcmp(buffer, BOM, 3))
                memset(buffer, ' ', 3);
        }

        std::string code(buffer);

        while (code.find("#include ") != code.npos) {
            const auto pos = code.find("#include ");
            const auto p1 = code.find('<', pos);
            const auto p2 = code.find('>', pos);
            if (p1 == code.npos || p2 == code.npos || p2 <= p1) {
                printf("Error while loading shader program: %s\n", code.c_str());
                return std::string();
            }
            const std::string name = code.substr(p1 + 1, p2 - p1 - 1);
            const std::string include = ReadShaderFile(name.c_str());
            code.replace(pos, p2 - pos + 1, include.c_str());
        }

        return code;
    }

    GLShader::Source::Source(const char* fileName)
        : GLShader::Source::Source(GLShaderTypeFromFileName(fileName), ReadShaderFile(fileName)) {}

    GLShader::Source::Source(unsigned int type, const std::string& text)
        : type_(type) {
        if (0 != shaderHandle_) {
            return;
        }

        shaderHandle_ = GLHandleMapper::GeneratorShaderHandle();
        IRHIAPI* api = RHI::Get()->GetRHIAPI();
        auto task = api->CreateShaderCommand(shaderHandle_, type, text);
        RHI::Get()->Push(task);
    }

    GLShader::Source::~Source() {
        IRHIAPI* api = RHI::Get()->GetRHIAPI();
        auto task = api->DeleteShaderCommand(shaderHandle_);
        RHI::Get()->Push(task);
    }


    GLShader::GLShader(const std::shared_ptr<Source>& a) {
        sources_.push_back(a);
        Compile();
    }

    GLShader::GLShader(const std::shared_ptr<Source>& a, const std::shared_ptr<Source>& b) {
        sources_.push_back(a);
        sources_.push_back(b);
        Compile();
    }

    GLShader::GLShader(const std::shared_ptr<Source>& a, const std::shared_ptr<Source>& b, const std::shared_ptr<Source>& c) {
        sources_.push_back(a);
        sources_.push_back(b);
        sources_.push_back(c);
        Compile();
    }

    GLShader::GLShader(const std::shared_ptr<Source>& a, const std::shared_ptr<Source>& b, const std::shared_ptr<Source>& c,
        const std::shared_ptr<Source>& d) {
        sources_.push_back(a);
        sources_.push_back(b);
        sources_.push_back(d);
        Compile();
    }

    GLShader::GLShader(const std::shared_ptr<Source>& a, const std::shared_ptr<Source>& b, const std::shared_ptr<Source>& c,
        const std::shared_ptr<Source>& d, const std::shared_ptr<Source>& e) {
        sources_.push_back(a);
        sources_.push_back(b);
        sources_.push_back(d);
        sources_.push_back(e);
        Compile();
    }

    GLShader::~GLShader() {
        IRHIAPI* api = RHI::Get()->GetRHIAPI();
        auto task = api->DeleteProgramCommand(programHandle_);
        RHI::Get()->Push(task);
    }

    std::shared_ptr<GLShader::Source> GLShader::CreateSource(const char* fileName) {
        std::shared_ptr<GLShader::Source> source(new GLShader::Source(fileName));
        return source;
    }

    std::shared_ptr<GLShader::Source> GLShader::CreateSource(unsigned int type, const std::string& text) {
        std::shared_ptr<GLShader::Source> source(new GLShader::Source(type, text));
        return source;
    }

    void GLShader::Use() {
        assert(0 != programHandle_);

        IRHIAPI* api = RHI::Get()->GetRHIAPI();
        auto task = api->UseProgramCommand(programHandle_, true);
        RHI::Get()->Push(task);
    }

    void GLShader::Compile() {
        if (0 != programHandle_) {
            return;
        }

        programHandle_ = GLHandleMapper::GeneratorProgramHandle();

        std::vector<unsigned int> shaders;
        for (auto& source : sources_) {
            shaders.push_back(source->shaderHandle_);
        }
        IRHIAPI* api = RHI::Get()->GetRHIAPI();
        auto task = api->CreateProgramCommand(programHandle_, shaders);
        RHI::Get()->Push(task);
    }
}
