#include "Window/Label.h"

#include "Core/StringUtil.h"
#include "Window/Context.h"

namespace PPEngine {
    namespace Window {
        Label::Label() {
        }

        Label::~Label() {
        }

        void Label::SetAttribute(const char* name, const char* value) {
            if (0 == strcmp("align", name)) {
                if (0 == strcmp("left", value)) {
                    align_ = Align::Left;
                } else if (0 == strcmp("center", value)) {
                    align_ = Align::Center;
                } else if (0 == strcmp("right", value)) {
                    align_ = Align::Right;
                } else if (0 == strcmp("top", value)) {
                    align_ = Align::Top;
                } else if (0 == strcmp("bottom", value)) {
                    align_ = Align::Bottom;
                }
            } else if (0 == strcmp("endellipsis", name)) {
                if (0 == strcmp("true", value)) {
                    align_ = Align::EndEllipsis;
                }
            } else if (0 == strcmp("font", name)) {
                SetFont(atoi(value));
            } else if (0 == strcmp("textcolor", name)) {
                SetTextColor(Core::StringToColor16(value));
            } else if (0 == strcmp("disabledtextcolor", name)) {
                SetDisabledTextColor(Core::StringToColor16(value));
            } else if (0 == strcmp("textpadding", name)) {
                Core::Math::Rect rtPadding = Core::Math::Rect::FromString(value);
                SetTextPadding(rtPadding);
            } else if (0 == strcmp("showhtml", name)) {
                SetShowHtml(0 == strcmp("true", value));
            } else if (0 == strcmp("enabledeffect", name)) {
                SetEnabledEffect(0 == strcmp("true", value));
            } else if (0 == strcmp("rhaa", name)) {
                SetTextRenderingHintAntiAlias(atoi(value));
            } else if (0 == strcmp("transshadow", name)) {
                SetTransShadow(atoi(value));
            } else if (0 == strcmp("transtext", name)) {
                SetTransText(atoi(value));
            } else if (0 == strcmp("transshadow1", name)) {
                SetTransShadow1(atoi(value));
            } else if (0 == strcmp("transtext1", name)) {
                SetTransText1(atoi(value));
            } else if (0 == strcmp("gradientangle", name)) {
                SetGradientAngle(atoi(value));
            } else if (0 == strcmp("enabledstroke", name)) {
                SetEnabledStroke(0 == strcmp("true", value));
            } else if (0 == strcmp("enabledshadow", name)) {
                SetEnabledShadow(0 == strcmp("true", value));
            } else if (0 == strcmp("transstroke", name)) {
                SetTransStroke(atoi(value));
            } else if (0 == strcmp("gradientlength", name)) {
                SetGradientLength(atoi(value));
            } else if (0 == strcmp("shadowoffset", name)) {
                Core::Math::Size offset = Core::Math::FromString(value);
                SetShadowOffset(offset);
            } else if (0 == strcmp("textcolor1", name)) {
                SetTextColor1(Core::StringToColor16(value));
            } else if (0 == strcmp("textshadowcolora", name)) {
                SetTextShadowColorA(Core::StringToColor16(value));
            } else if (0 == strcmp("textshadowcolorb", name)) {
                SetTextShadowColorB(Core::StringToColor16(value));
            } else if (0 == strcmp("strokecolor", name)) {
                SetStrokeColor(Core::StringToColor16(value));
            } else {
               Control::SetAttribute(name, value);
            }
        }

        void Label::SetRect(const Core::Math::Rect& rect) {
        }

        void Label::SetEnabledEffect(bool enable) {
            enabledEffect_ = enable;
            Invalidate();
        }

        void Label::OnDrawText() {
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
