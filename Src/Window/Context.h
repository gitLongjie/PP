#pragma once

#include <vector>

#include "Core/Math/Rect.h"

namespace PPEngine {
    namespace Window {
        class Control;

        class Context {
        public:
            virtual ~Context() = default;

            virtual void Invalidate(Core::Math::Rect& rect);
            virtual void RemovePostPaint(Control* control);
            virtual void SetFocus(Control* control, bool focusWnd);
            virtual Control* GetFocus() const { return focusControl_; }
            virtual void DrawColor(const Core::Math::Rect& rect, unsigned long color);
            virtual void DrawGradient(const Core::Math::Rect& rect, unsigned long color1,
                unsigned long color2, bool vertical, int32_t steps);

            bool IsUpdateNeeded() const { return needUpdate_; }
            void NeedUpdate() { needUpdate_ = true; }

            

        protected:
            std::vector<Control*> postPaintControls_;
            Control* focusControl_{ nullptr };
            bool needUpdate_{ false };
            bool noActivate_{ false };
        };
    }
}