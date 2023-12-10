#pragma once

#include "PPQuadEntity.h"

class PPBackgroundQuadEntity : public PPQuadEntity {
    public:
    PPBackgroundQuadEntity();
    virtual ~PPBackgroundQuadEntity();

protected:
    void OnInit() override;
    void OnUpdate() override;
    void OnRender(PPCamera* camera) override;
    void OnRelease() override;


private:
    unsigned int texture{ 0 };
    unsigned int model{ 0 };
};