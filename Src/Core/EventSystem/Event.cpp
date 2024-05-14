#include "Core/EventSystem/Event.h"

#include "Core/Platform.h"

namespace PPEngine {
    namespace Core {
        namespace EventSystem {

            Event::Event(Type t) 
                : t_(t)
                , timestamp_(Platform::GetTickCount64()) {

            }

            MouseMoveEvent::MouseMoveEvent(const Math::Point2d& pt)
                : Event(Event::Type::MouseMove)
                , pt_(pt) {
            }

        }
    }
}