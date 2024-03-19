#include "Window/Button.h"

#include "Core/StringUtil.h"
#include "Window/Context.h"

namespace PPEngine {
    namespace Window {
        Button::Button() {
        }

        Button::~Button() {
        }

        Control::Ptr Button::Create() {
            return std::make_shared<Button>();
        }

        void Button::SetAttribute(const char* name, const char* value) {
            if (strcmp(name, "normalimage") == 0) { SetNormalImage(value); }
            else if (strcmp(name, "hotimage") == 0) { SetHotImage(value); }
            else if (strcmp(name, "pushedimage") == 0) { SetPushedImage(value); }
            else if (strcmp(name, "focusedimage") == 0) { SetFoucesedImage(value); }
            else if (strcmp(name, "disabledimage") == 0) { SetDisableImage(value); }
            //else if (strcmp(name, "foreimage") == 0) SetFoucesedImage(value);
            else if (strcmp(name, "hotforeimage") == 0) { SetHotForeImage(value); }
            else if (strcmp(name, "hotbkcolor") == 0) {
                uint32 uColor = Core::StringToColor16(value);
                SetHotBkColor(uColor);
            }
            else if (strcmp(name, "hottextcolor") == 0) {
                uint32 uColor = Core::StringToColor16(value);
                SetHotTextColor(uColor);
            }
            else if (strcmp(name, "pushedtextcolor") == 0) {
                uint32 uColor = Core::StringToColor16(value);
                SetPushedTextColor(uColor);
            }
            else if (strcmp(name, "foucedtextcolor") == 0) {
                uint32 uColor = Core::StringToColor16(value);
                SetFoucedTextColor(uColor);
            }
            else {
                Label::SetAttribute(name, value);
            }
        }

        void Button::SetNormalImage(const char* normalImage) {
            normalImage_ = normalImage;
            Invalidate();
        }

        void Button::SetHotImage(const char* hotImage) {
            hotImage_ = hotImage;
            Invalidate();
        }

        void Button::SetHotForeImage(const char* hotForeImage) {
            hotForeImage_ = hotForeImage;
            Invalidate();
        }

        void Button::SetPushedImage(const char* pushedImage) {
            pushedImage_ = pushedImage;
            Invalidate();
        }

        void Button::SetPushedForeImage(const char* pushedForeImage) {
            pushedForeImage_ = pushedForeImage;
            Invalidate();
        }

        void Button::SetFoucesedImage(const char* foucesedImage) {
            foucesedImage_ = foucesedImage;
            Invalidate();
        }

        void Button::SetDisableImage(const char* disableImage) {
            disableImage_ = disableImage;
            Invalidate();
        }

        void Button::OnDrawText() {
            if (0 == textColor_) {
                textColor_ = context_->GetDefaultTextColor();
            }

            if (0 == disabledTextColor_) {
                disabledTextColor_ = context_->GetDisabledColor();
            }

            Core::Math::Rect rect1 = rect_;
            glm::vec2 rectMin = rect1.GetMin();
            glm::vec2 rectMax = rect1.GetMax();

            glm::vec2 paddingMin = textPadding_.GetMin();
            glm::vec2 paddingMax = textPadding_.GetMax();

            rectMin += paddingMin;
            rectMax -= paddingMax;
            Core::Math::Rect drawRect(rectMin, rectMax);

            if (!IsEnabledEffect()) {
                if (text_.empty()) { return; }

                if (IsEnabled()) {
                    context_->DrawUIText(drawRect, text_, textColor_, font_, 0);
                }
            }

        }

    }
}
