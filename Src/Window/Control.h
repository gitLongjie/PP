#pragma once

#include <memory>
#include <string>

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
            virtual void SetInternVisible(bool visible);

            virtual void OnDraw(const Core::Math::Rect& rect);

            void Invalidate();
            bool IsUpdateNeeded() const;
            void NeedUpdate();
            void NeedParentUpdate();
            bool IsEnabled() const { return enabled_; }
            void SetEnabled(bool enable = true);

            bool IsFocused() const { return focused_; }
            void SetFocuse();

            const Core::Math::Rect& GetRect() const { return rect_; }
            unsigned long GetColor(unsigned long color);
            void SetBkColor(unsigned long color);
            void SetBkColor2(unsigned long color);
            void SetBkColor3(unsigned long color);
            void SetBorderSize(const Core::Math::Rect& size);
            void SetBorderSize(int32_t size);
            void SetBorderRound(const Core::Math::Size& size);
            void SetBorderColor(unsigned long color);
            unsigned long GetBorderColor() const { return borderColor_; }
            void SetFocusBorderColor(unsigned long color);
            unsigned long SetFocusBorderColor() const { return focusBorderColor_; }

        protected:
            virtual void OnDrawBkColor();
            virtual void OnDrawBkImage();
            virtual void OnDrawStatusImage();
            virtual void OnDrawText();
            virtual void OnDrawBorder();

        protected:
            class Context* context_{ nullptr };
            Control* parent_{ nullptr };

            Core::Math::Rect rect_;
            Core::Math::Rect rectPaint_;
            Core::Math::Size cXY_{ 0.0f, 0.0f };
            Core::Math::Size cxyFixed_{ 0.0f, 0.0f };
            Core::Math::Size cxyMin_{ 0.0f, 0.0f };
            Core::Math::Size cxyMax_{ 0.0f, 0.0f };

            bool float_{ false };
            bool focused_{ false };

            bool visible_{ true };
            bool internVisible_{ true };
            bool enabled_{ true };

            bool updateNeeded_ { true };
            bool isHSL_{ false };
            unsigned long bkColor_{ 0 };
            unsigned long bkColor2_{ 0 };
            unsigned long bkColor3_{ 0 };
            unsigned long borderColor_{ 0 };
            unsigned long focusBorderColor_{ 0 };

            Core::Math::Rect borderRect_;
            int32_t borderSize_{ 0 };
            Core::Math::Size borderRound_{ 0.0f, 0.0f };

            std::string bkImage_;
        };
    }
}
