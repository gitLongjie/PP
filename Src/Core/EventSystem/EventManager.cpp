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

            void EventManager::Dispatch() {
                if (eventQueue_.empty()) {
                    return;
                }

                do 
                {
                    //std::shared_ptr<Event>  eventQueue_.front();
                    eventQueue_.pop();

                } while (!eventQueue_.empty());
            }

        }
    }
}
