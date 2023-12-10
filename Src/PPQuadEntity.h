#pragma once

#include "PPEntity.h"

#include "PPGLShader.h"

#include <glm.hpp>

class PPQuadEntity : public PPEntity {
public:
    PPQuadEntity();
    ~PPQuadEntity();

protected:
    void OnInit() override;
    void OnUpdate() override;
    void OnRender(PPCamera* camera) override;
    void OnRelease() override;

protected:
 //   unsigned int modelMatrices{ 0 };

private:
    unsigned int vao{ 0 };
    unsigned int vbo{ 0 };
    unsigned int ebo{ 0 };

 



protected:

    PPGLShader* shaderVertex{ nullptr };
    PPGLShader* shaderFragment{ nullptr };
    PPGLProgram* shaderProgram{ nullptr };
};