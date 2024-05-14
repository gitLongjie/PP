#pragma once

#include "Core/Constent.h"
#include "Core/Math/Size.h"

namespace PPEngine {
    namespace Core {
        namespace EventSystem {
            class Event {
            public:
                enum class Type : uint32 {
                    Unknow = 0,
                    MouseMove,
                };

            public:
                Event(Type t);
                virtual ~Event() = default;

                Type GetType() const { return t_; }

            private:
                Type t_{Type::Unknow};
                uint64 timestamp_{ 0 };
            };

            class MouseMoveEvent : public Event {
            public:
                MouseMoveEvent(const Math::Point2d& pt);
                ~MouseMoveEvent() override = default;

            private:
                Math::Point2d pt_;
            };
        }
    }
}
