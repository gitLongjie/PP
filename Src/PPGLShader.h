#pragma once

#include <GL/glew.h>

class PPGLShader
{
public:
	explicit PPGLShader(const char* fileName);
	PPGLShader(GLenum type, const char* text, const char* debugFileName = "");
	~PPGLShader();
	GLenum getType() const { return type_; }
	GLuint getHandle() const { return handle_; }

private:
	GLenum type_;
	GLuint handle_;
};

class PPGLProgram
{
public:
	PPGLProgram(const PPGLShader& a);
	PPGLProgram(const PPGLShader& a, const PPGLShader& b);
	PPGLProgram(const PPGLShader& a, const PPGLShader& b, const PPGLShader& c);
	PPGLProgram(const PPGLShader& a, const PPGLShader& b, const PPGLShader& c, const PPGLShader& d, const PPGLShader& e);
	~PPGLProgram();

	void useProgram() const;
	GLuint getHandle() const { return handle_; }

private:
	GLuint handle_;
};

GLenum GLShaderTypeFromFileName(const char* fileName);

class PPGLBuffer
{
public:
	PPGLBuffer(GLsizeiptr size, const void* data, GLbitfield flags);
	~PPGLBuffer();

	GLuint getHandle() const { return handle_; }

private:
	GLuint handle_;
};
