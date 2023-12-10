#pragma once

class PPCamera;

class PPEntity {
public:
    PPEntity();
    ~PPEntity();

    void Init();
    void Update();
    void Render(PPCamera* camera);
    void Release();

    bool IsInited() const { return m_isInited; }

protected:
    virtual void OnInit();
    virtual void OnUpdate();
    virtual void OnRender(PPCamera* camera);
    virtual void OnRelease();

private:
    bool m_isInited{ false };
};