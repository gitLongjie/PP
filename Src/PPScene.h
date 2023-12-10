#pragma once

#include <vector>

class PPCamera;;

class PPScene {
public:
    PPScene(PPCamera* camera);
    ~PPScene();

    virtual void Init();
    virtual void Update();
    virtual void Render();
    virtual void Release();

    PPCamera* GetCamera() const { return m_camera; }

private:
    std::vector<class PPEntity*> m_entities;
    PPCamera* m_camera{ nullptr };
};