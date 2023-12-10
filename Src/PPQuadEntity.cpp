#include "PPQuadEntity.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>

#include "PPCamera.h"


static const char* shaderCodeVertex = R"(
#version 460 core
layout(std140, binding = 0) uniform PerFrameData
{
	uniform mat4 view;
	uniform mat4 proj;
};

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;
layout (location=2) in vec2 auv;

layout (location=0) out vec3 color;
layout (location=1) out vec2 uv;

uniform mat4 uModel;

void main()
{
	gl_Position = proj * view * uModel * vec4(aPos, 1.0f);
    color = aColor;
	uv = auv;
}
)";

static const char* shaderCodeFragment = R"(
#version 460 core
layout (location=0) in vec3 color;
layout (location=1) in vec2 uv;

layout (location=0) out vec4 out_FragColor;

uniform sampler2D texture0;
void main()
{
	out_FragColor = texture(texture0, uv);
};
)";

PPQuadEntity::PPQuadEntity()
    : PPEntity() {

}

PPQuadEntity::~PPQuadEntity() {

}

void PPQuadEntity::OnInit() {
    const float vertices[] = {
        //    ---- 位置 ----  ---- 颜色 ----     - 纹理坐标 -
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // 左下
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // 右下
         0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // 右上
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f  // 左上
    };
    const unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glCreateBuffers(1, &vbo);
    const int t = sizeof(vertices);
    glNamedBufferStorage(vbo, sizeof(vertices), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(vbo, 0, sizeof(vertices), vertices);

    glCreateBuffers(1, &ebo);
    glNamedBufferStorage(ebo, sizeof(indices), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(ebo, 0, sizeof(indices), indices);

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float) * 8);
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
    glEnableVertexArrayAttrib(vao, 2);
    glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6);
    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribBinding(vao, 1, 0);
    glVertexArrayAttribBinding(vao, 2, 0);

    

    shaderVertex = new PPGLShader(GL_VERTEX_SHADER, shaderCodeVertex);
    shaderFragment = new PPGLShader(GL_FRAGMENT_SHADER, shaderCodeFragment);
    shaderProgram = new PPGLProgram(*shaderVertex, *shaderFragment);

    //glCreateBuffers(1, &model);
    //glNamedBufferStorage(model, sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT);
    //glBindBufferRange(GL_UNIFORM_BUFFER, 0, model, 0, sizeof(glm::mat4));

    //glCreateBuffers(1, &modelMatrices);
    //glNamedBufferStorage(modelMatrices, sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT);
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, modelMatrices);

    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

}

void PPQuadEntity::OnUpdate() {

}

void PPQuadEntity::OnRender(PPCamera* camera) {
    shaderProgram->useProgram();

 /*   float ratio = 3840.0f / 1200.0f * 0.4;

    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(ratio, 1.0f, 1.0f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * 0.3f, glm::vec3(0.0f, 0.0f, -1.0f));*/

    //glm::mat4 tras = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -.50f));
    //glNamedBufferSubData(model, 0, sizeof(glm::mat4), glm::value_ptr(m_model));



  
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //glBindBuffer(GL_VERTEX_ARRAY, ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //const glm::mat4 Matrices = scale * rotate;// glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -1.5f)), (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    //glUniformMatrix4fv(Model, 1, GL_FALSE, glm::value_ptr(Matrices));

    //glDrawArrays(GL_TRIANGLES, 0, 3);
}

void PPQuadEntity::OnRelease() {
    if (nullptr != shaderProgram) {
        delete shaderProgram;
        shaderProgram = nullptr;
    }

    if (nullptr != shaderFragment) {
        delete shaderFragment;
        shaderFragment = nullptr;
    }

    if (nullptr != shaderVertex) {
        delete shaderVertex;
        shaderVertex = nullptr;
    }

    //glDeleteBuffers(1, &modelMatrices);

    glDeleteVertexArrays(1, &ebo);
    glDeleteVertexArrays(1, &vbo);
    glDeleteVertexArrays(1, &vao);


}

