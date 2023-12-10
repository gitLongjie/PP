#include "PPEntity.h"

PPEntity::PPEntity() {

}

PPEntity::~PPEntity() {

}

void PPEntity::Init() {
    if (m_isInited) {
        return ;
    }
    OnInit();
    m_isInited = true;
}

void PPEntity::Update() {
    if (!m_isInited) {
        return;
    }
    OnUpdate();
}

void PPEntity::Render(PPCamera* camera) {
    if (!m_isInited) {
        return;
    }
    OnRender(camera);
}

void PPEntity::Release() {
    if (!m_isInited) {
        return ;
    }
    OnRelease();
    m_isInited = false;
}

void PPEntity::OnInit() {

}

void PPEntity::OnUpdate() {

}

void PPEntity::OnRender(PPCamera* camera) {

}

void PPEntity::OnRelease() {

}
