#include "Core/EventSystem/Event.h"

#include "Core/Platform.h"

namespace PPEngine {
    namespace Core {
        namespace EventSystem {
            MouseMoveEvent::MouseMoveEvent(const Math::Point2d& pt)
                : pt_(pt) {
            }

        }
    }
}