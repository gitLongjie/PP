#pragma once

#include <vector>
#include <string>

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
            virtual void DrawLine(const Core::Math::Rect& rectPaint, int32_t size, unsigned long color, int nStyle = 0);
            virtual void DrawRect(const Core::Math::Rect& rectPaint, int32_t size, unsigned long color);
            virtual void DrawRoundRect(const Core::Math::Rect& rectPaint, int32_t size, int32_t width, int32_t height, unsigned long color);
            virtual void DrawImageString(const Core::Math::Rect& rectPaint, const Core::Math::Rect& rect, const std::string& image);
            virtual void DrawColor(const Core::Math::Rect& rect, unsigned long color);
            virtual void DrawGradient(const Core::Math::Rect& rect, unsigned long color1,
                unsigned long color2, bool vertical, int32_t steps);
            virtual void GenerateRoundClip(const Core::Math::Rect& rect, const Core::Math::Rect& rcItem, int width, int height);

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