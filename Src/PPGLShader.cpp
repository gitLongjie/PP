#include "PPGLShader.h"
#include "PPUtils.h"

#include <assert.h>
#include <stdio.h>
#include <string>

PPGLShader::PPGLShader(const char* fileName)
: PPGLShader(GLShaderTypeFromFileName(fileName), ReadShaderFile(fileName).c_str(), fileName)
{}

PPGLShader::PPGLShader(GLenum type, const char* text, const char* debugFileName)
	: type_(type)
	, handle_(glCreateShader(type))
{
	glShaderSource(handle_, 1, &text, nullptr);
	glCompileShader(handle_);

	char buffer[8192];
	GLsizei length = 0;
	glGetShaderInfoLog(handle_, sizeof(buffer), &length, buffer);

	if (length)
	{
		printf("%s (File: %s)\n", buffer, debugFileName);
		PrintShaderSource(text);
		assert(false);
	}
}

PPGLShader::~PPGLShader()
{
	glDeleteShader(handle_);
}

void printProgramInfoLog(GLuint handle)
{
	char buffer[8192];
	GLsizei length = 0;
	glGetProgramInfoLog(handle, sizeof(buffer), &length, buffer);
	if (length)
	{
		printf("%s\n", buffer);
		assert(false);
	}
}

PPGLProgram::PPGLProgram(const PPGLShader& a)
	: handle_(glCreateProgram())
{
	glAttachShader(handle_, a.getHandle());
	glLinkProgram(handle_);
	printProgramInfoLog(handle_);
}

PPGLProgram::PPGLProgram(const PPGLShader& a, const PPGLShader& b)
: handle_(glCreateProgram())
{
	glAttachShader(handle_, a.getHandle());
	glAttachShader(handle_, b.getHandle());
	glLinkProgram(handle_);
	printProgramInfoLog(handle_);
}

PPGLProgram::PPGLProgram(const PPGLShader& a, const PPGLShader& b, const PPGLShader& c)
: handle_(glCreateProgram())
{
	glAttachShader(handle_, a.getHandle());
	glAttachShader(handle_, b.getHandle());
	glAttachShader(handle_, c.getHandle());
	glLinkProgram(handle_);
	printProgramInfoLog(handle_);
}

PPGLProgram::PPGLProgram(const PPGLShader& a, const PPGLShader& b, const PPGLShader& c, const PPGLShader& d, const PPGLShader& e)
: handle_(glCreateProgram())
{
	glAttachShader(handle_, a.getHandle());
	glAttachShader(handle_, b.getHandle());
	glAttachShader(handle_, c.getHandle());
	glAttachShader(handle_, d.getHandle());
	glAttachShader(handle_, e.getHandle());
	glLinkProgram(handle_);
	printProgramInfoLog(handle_);
}

PPGLProgram::~PPGLProgram()
{
	glDeleteProgram(handle_);
}

void PPGLProgram::useProgram() const
{
	glUseProgram(handle_);
}

GLenum GLShaderTypeFromFileName(const char* fileName)
{
	if (EndsWith(fileName, ".vert"))
		return GL_VERTEX_SHADER;

	if (EndsWith(fileName, ".frag"))
		return GL_FRAGMENT_SHADER;

	if (EndsWith(fileName, ".geom"))
		return GL_GEOMETRY_SHADER;

	if (EndsWith(fileName, ".tesc"))
		return GL_TESS_CONTROL_SHADER;

	if (EndsWith(fileName, ".tese"))
		return GL_TESS_EVALUATION_SHADER;

	if (EndsWith(fileName, ".comp"))
		return GL_COMPUTE_SHADER;

	assert(false);

	return 0;
}

PPGLBuffer::PPGLBuffer(GLsizeiptr size, const void* data, GLbitfield flags)
{
	glCreateBuffers(1, &handle_);
	glNamedBufferStorage(handle_, size, data, flags);
}

PPGLBuffer::~PPGLBuffer()
{
	glDeleteBuffers(1, &handle_);
}
