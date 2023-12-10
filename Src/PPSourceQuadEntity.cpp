#include "PPSourceQuadEntity.h"

#include <stb/stb_image.h>
#include <glm/ext.hpp>
#include <glm.hpp>
#include <GLFW/glfw3.h>

PPSourceQuadEntity::PPSourceQuadEntity() {

}

PPSourceQuadEntity::~PPSourceQuadEntity() {

}

void PPSourceQuadEntity::OnInit() {
    int w, h, comp;
    const uint8_t* img = stbi_load("D:/test/source.png", &w, &h, &comp, 3);

    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glTextureParameteri(texture, GL_TEXTURE_MAX_LEVEL, 0);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(texture, 1, GL_RGB8, w, h);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTextureSubImage2D(texture, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, img);
    glBindTextures(0, 1, &texture);

    stbi_image_free((void*)img);
    img = nullptr;

    PPQuadEntity::OnInit();

    GLuint programId = shaderProgram->getHandle();
    model = glGetUniformLocation(programId, "uModel");
}


void PPSourceQuadEntity::OnUpdate() {

}

void PPSourceQuadEntity::OnRender(PPCamera* camera) {
    glBindTextures(0, 1, &texture);


    float ratio = 3840.0f / 1200.0f * 0.6;
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(ratio, 1.0f, 1.0f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * 0.3f, glm::vec3(0.0f, 0.0f, -1.0f));
    glm::mat4 tras = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0001f));

    //glm::mat4 tras = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -.50f));
    //glNamedBufferSubData(model, 0, sizeof(glm::mat4), glm::value_ptr(m_model));



    const glm::mat4 Matrices = scale *tras;// glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -1.5f)), (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
  //  glNamedBufferSubData(modelMatrices, 0, sizeof(glm::mat4), glm::value_ptr(Matrices)); // &Matrices);
    PPQuadEntity::OnRender(camera);

    GLuint programId = shaderProgram->getHandle();
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Matrices));
}

void PPSourceQuadEntity::OnRelease() {
    PPQuadEntity::OnRelease();
    glDeleteTextures(1, &texture);
}


