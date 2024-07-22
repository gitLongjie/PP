#pragma once

#include "Core/Constant.h"

namespace Core {
    namespace EventSystem {
        enum class EventType : uint32 {
            Unknow = 0,
            MouseMove,
            MouseClick,
        };
    }
}
