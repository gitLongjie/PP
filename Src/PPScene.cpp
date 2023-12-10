#include "PPScene.h"

#include "PPEntity.h"
#include "PPSourceQuadEntity.h"
#include "PPBackgroundQuadEntity.h"

PPScene::PPScene(PPCamera* camera)
    : m_camera(camera) {

}

PPScene::~PPScene() {

}

void PPScene::Init() {
    PPEntity* entity = new PPSourceQuadEntity;
    m_entities.push_back(entity);
    entity = new PPBackgroundQuadEntity;
    m_entities.push_back(entity);
}

void PPScene::Update() {
    for (auto& entity : m_entities) {
        entity->Init();
    }

    for (auto& entity : m_entities) {
        entity->Update();
    }
}

void PPScene::Render() {
    glEnable(GL_DEPTH_TEST);
    for (auto& entity : m_entities) {
        entity->Render(m_camera);
    }
}

void PPScene::Release() {
    for (auto& entity : m_entities) {
        entity->Release();
    }

    for (auto itor = m_entities.begin(); itor != m_entities.end(); ++itor) {
        delete *itor;
    }
    m_entities.clear();
}

