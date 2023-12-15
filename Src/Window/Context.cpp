#include "Window/Context.h"
#include "Window/Control.h"

namespace PPEngine {
    namespace Window {

        void Context::Invalidate(Core::Math::Rect& rect) {}

        void Context::RemovePostPaint(Control* control) {
            const auto& itor = std::find_if(postPaintControls_.begin(), postPaintControls_.end(), [control](Control* c) {
                return c == control;
            });
            if (itor != postPaintControls_.end()) {
                postPaintControls_.erase(itor);
            }
        }

        void Context::SetFocus(Control* control, bool focusWnd) {

        }

        void Context::DrawLine(const glm::vec2& start, const glm::vec2& end, int32_t size, unsigned long color, int nStyle) {
        }

        void Context::DrawRect(const Core::Math::Rect& rectPaint, int32_t size, unsigned long color) {
        }

        void Context::DrawRoundRect(const Core::Math::Rect& rectPaint, int32_t size, int32_t width, int32_t height, unsigned long color) {
        }

        void Context::DrawImageString(const Core::Math::Rect& rectPaint, const Core::Math::Rect& rect, const std::string& image) {
        }

        void Context::DrawColor(const Core::Math::Rect& rect, unsigned long color1) {

        }

        void Context::DrawGradient(const Core::Math::Rect& rect, unsigned long color1, unsigned long color2, bool vertical, int32_t steps) {

        }

        void Context::GenerateRoundClip(const Core::Math::Rect& rect, const Core::Math::Rect& rcItem, int width, int height) {
        }

        void Context::InitControl(Control* control, Control* parent) {
            if (nullptr == control) { return; }

            control->SetContext(this, parent);
            controls_.insert(control);
        }

        void Context::UninitContorl(Control* control) {
            controls_.erase(control);
        }

        bool Context::Attach(Window::Control::Ptr control) {
            if (nullptr == control) {
                return false;
            }

            control->SetContext(this, nullptr);
            control_ = control;
            return true;
        }

    }
}
