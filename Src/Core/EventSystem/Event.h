#pragma once

#include "Core/Constant.h"
#include "Core/EventSystem/Constent.h"
#include "Core/Math/Size.h"
#include "Core/Platform.h"

namespace PPEngine {
    namespace Core {
        namespace EventSystem {
            class IEvent {
            public:
                virtual ~IEvent() = default;
                virtual EventType GetType() const = 0;
            };

            template <typename T = EventType, T value = EventType::Unknow>
            class Event : public IEvent {
            public:
                

            public:
                Event() :timestamp_(Platform::GetTickCount64()) {}
                ~Event() = default;
                EventType GetType() const {
                    return Type();
                }


                static T Type() {
                    return value;
                }

            private:
                uint64 timestamp_{ 0 };
            };

            class MouseMoveEvent : public Event<EventType, EventType::MouseMove> {
            public:
                MouseMoveEvent() = default;
                MouseMoveEvent(const Math::Point2d& pt);
                ~MouseMoveEvent() override = default;

                const Math::Point2d& GetPoint() const {
                    return pt_;
                }
            private:
                Math::Point2d pt_;
            };
        }
    }
}
