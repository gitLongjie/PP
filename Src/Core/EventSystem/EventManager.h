#pragma once


#include <queue>
#include <memory>

#include "Core/Constent.h"
#include "Core/Singleton.h"
#include "Core/EventSystem/Event.h"

namespace PPEngine {
    namespace Core {
        namespace EventSystem {
            class EventManager : public Singleton<EventManager> {
            public:
                EventManager() = default;
                ~EventManager() override = default;

            public:
                bool Initialize() override;
                void Uninitialize() override;

               /* template<typename ET, typename Sender>
                bool Send(const ET& event, Sender* sender) {
                    return sender->OnHandlerEvent(event);
                }*/

                void Dispatch();

            private:
                std::queue<std::shared_ptr<Event>> eventQueue_;
            };
        }
    }
}
