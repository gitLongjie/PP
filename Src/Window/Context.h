#pragma once

#include <vector>
#include <string>
#include <unordered_set>

#include "Core/Math/Rect.h"
#include "Window/Control.h"


namespace PPEngine {
    namespace Window {

        class Context {
        public:
            struct DefaultInfo {
                unsigned long disabledColor_{ 0 };
                unsigned long fontColor_{ 0 };
                unsigned long selectedBkColor_{ 0 };
            };

        public:
            virtual ~Context() = default;

            virtual void Invalidate(Core::Math::Rect& rect);
            virtual void RemovePostPaint(Control* control);
            virtual void SetFocus(Control* control, bool focusWnd);
            virtual Control* GetFocus() const { return focusControl_; }
            virtual void DrawLine(const glm::vec2& start, const glm::vec2& end, int32_t size, unsigned long color, int nStyle = 0);
            virtual void DrawRect(const Core::Math::Rect& rectPaint, int32_t size, unsigned long color);
            virtual void DrawRoundRect(const Core::Math::Rect& rectPaint, int32_t size, int32_t width, int32_t height, unsigned long color);
            virtual void DrawImageString(const Core::Math::Rect& rectPaint, const Core::Math::Rect& rect, const std::string& image);
            virtual void DrawColor(const Core::Math::Rect& rect, unsigned long color);
            virtual void DrawGradient(const Core::Math::Rect& rect, unsigned long color1,
                unsigned long color2, bool vertical, int32_t steps);
            //virtual void GenerateRoundClip(const Core::Math::Rect& rect, const Core::Math::Rect& rcItem, int width, int height);

            void SetDefaultTextColor(unsigned long color, bool shared);
            unsigned long GetDefaultTextColor() const { return defaultInfo_.fontColor_; }
            void SetDisabledColor(unsigned long color, bool shared);
            unsigned long GetDisabledColor() const { return defaultInfo_.fontColor_; }
            

            bool IsUpdateNeeded() const { return needUpdate_; }
            void NeedUpdate() { needUpdate_ = true; }

            void InitControl(Control* control, Control* parent);
            void UninitContorl(Control* control);
            bool Attach(Control::Ptr control);
            

        protected:
            std::vector<Control*> postPaintControls_;
            Control* focusControl_{ nullptr };
            bool needUpdate_{ false };
            bool noActivate_{ false };

            Control::Ptr control_{ nullptr };
            std::unordered_set<Control*> controls_;
            DefaultInfo defaultInfo_;
        };
    }
}