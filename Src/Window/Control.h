#pragma once

#include <memory>

#include "Core/Math/Size.h"
#include "Core/Math/Rect.h"

namespace PPEngine {
    namespace Window {
        class Control {
        public:
            using Ptr = std::shared_ptr<Control>;

        public:
            Control();
            virtual ~Control();

            void SetContext(class Context* context, Control* parent) {
                context_ = context;
                parent_ = parent;
            }
            class Context* GetContext() const { return context_;}
            Control* GetParent() const { return parent_; }

            virtual bool IsVisible() const { return visible_ && internVisible_; }
            virtual void SetVisible(bool visible = true);
            virtual void SetAttribute(const char* name, const char* value);
            virtual void SetRect(const Core::Math::Rect& rect);

            void Invalidate();
            bool IsUpdateNeeded() const;
            void NeedUpdate();
            void NeedParentUpdate();
            bool IsEnabled() const { return enabled_; }
            void SetEnabled(bool enable = true);

            const Core::Math::Rect& GetRect() const { return rect_; }

        private:
            class Context* context_{ nullptr };
            Control* parent_{ nullptr };

            Core::Math::Rect rect_;
            Core::Math::Size cXY_{ 0.0f, 0.0f };
            Core::Math::Size cxyFixed_{ 0.0f, 0.0f };
            Core::Math::Size cxyMin_{ 0.0f, 0.0f };
            Core::Math::Size cxyMax_{ 0.0f, 0.0f };

            bool float_{ false };
            bool focused_{ false };

            bool visible_{ false };
            bool internVisible_{ false };
            bool enabled_{ false };

            bool updateNeeded_ { false };
        };
    }
}
