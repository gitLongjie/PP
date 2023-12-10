#include "PPCamera.h"

#include <gl/glew.h>
#include <gtc/matrix_transform.hpp>


bool PPCamera::Init() {
    glCreateBuffers(1, &m_preFrameBuffer);
    glNamedBufferStorage(m_preFrameBuffer, sizeof(PerFrameData), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_preFrameBuffer, 0, sizeof(PerFrameData));
    return true;
}

void PPCamera::Uninit() {
    glDeleteBuffers(1, &m_preFrameBuffer);
}

glm::mat4 PPCamera::GetViewMatrix() const {
    return glm::lookAt(m_position, m_center, m_up);
    /*const glm::mat4 t = glm::translate(glm::mat4(1.0f), m_position);
    const glm::mat4 r = glm::mat4_cast(m_orientation);*/
    //return r * t;
}

void PPCamera::Update(float aspectRatio) {
    m_projection = glm::perspective(45.0f, aspectRatio, 0.1f, 1000.0f);

    glm::mat4 view = GetViewMatrix();
    PerFrameData perFrameData = { view, m_projection };
    glNamedBufferSubData(m_preFrameBuffer, 0, sizeof(PPCamera::PerFrameData), &perFrameData);
}

