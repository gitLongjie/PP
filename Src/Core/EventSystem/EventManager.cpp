#include "Core/EventSystem/EventManager.h"

namespace PPEngine {
    namespace Core {
        EventSystem::EventManager* Core::Singleton<EventSystem::EventManager>::instance_ = nullptr;
        namespace EventSystem {

            bool EventManager::Initialize() {
                return true;
            }

            void EventManager::Uninitialize() {

            }
        }
    }
}
