#pragma once

#include <glm.hpp>
#include <gtc/quaternion.hpp>

class PPCamera {
public:
    PPCamera() = default;

    bool Init();
    void Uninit();
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const { return m_projection; }

    void Update(float aspectRatio);

public:
    glm::vec3 m_position{ 0.0f, 0.0f, 1.0f };
    glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 m_center{ 0.0f, 0.0f, 0.0f };
    glm::quat m_orientation{ 0.0f, 0.0f, 0.0f, 1.0f };

    glm::mat4 m_projection{ 1.0f };

    unsigned int m_preFrameBuffer{ 0 };
public:
    struct PerFrameData {
        glm::mat4 view;
        glm::mat4 proj;
    };
};
