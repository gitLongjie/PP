#pragma once

#include "PPQuadEntity.h"

class PPSourceQuadEntity : public PPQuadEntity {
    public:
    PPSourceQuadEntity();
    virtual ~PPSourceQuadEntity();

protected:
    void OnInit() override;
    void OnUpdate() override;
    void OnRender(PPCamera* camera) override;
    void OnRelease() override;


private:
    unsigned int texture{ 0 };
    unsigned int model{ 0 };
};