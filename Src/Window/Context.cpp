#include "Window/Context.h"

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

        void Context::DrawImageString(const Core::Math::Rect& rectPaint, const Core::Math::Rect& rect, const std::string& image) {
        }

        void Context::DrawColor(const Core::Math::Rect& rect, unsigned long color1) {

        }

        void Context::DrawGradient(const Core::Math::Rect& rect, unsigned long color1, unsigned long color2, bool vertical, int32_t steps) {

        }

        void Context::GenerateRoundClip(const Core::Math::Rect& rect, const Core::Math::Rect& rcItem, int width, int height) {
        }

    }
}
