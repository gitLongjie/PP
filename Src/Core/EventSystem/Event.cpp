#include "Core/EventSystem/Event.h"

#include "Core/Platform.h"

namespace Core {
    namespace EventSystem {
        MoveEvent::MoveEvent(const Math::Point2d& pt)
            : pt_(pt) {
        }

    }
}
