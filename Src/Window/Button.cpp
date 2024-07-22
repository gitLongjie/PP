#include "Window/Button.h"

#include <functional>

#include "Core/StringUtil.h"
#include "Core/EventSystem/EventManager.h"
#include "Core/EventSystem/Invoke.h"
#include "Window/Context.h"

namespace Window {
    Button::Button() {
        using namespace Core::EventSystem;
        //EventManager::Get()->Attach<int>(&Button::Test1, this);
       // EventManager::Get()->Attach<const MouseMoveEvent&>(MouseMoveEvent(), &Button::Test, this);
        //Core::EventSystem::InvokeImpl<Button> t(&Button::Test1);
        //Core::EventSystem::EventManager::Get()->Bind(this, std::mem_fn())
    }

    Button::~Button() {
    }

    Control* Button::Create() {
        return new Button;
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

    void Button::FixRect(Core::Math::Rect rect) {
        Label::FixRect(rect);
    }

    Core::Math::Size Button::EstimateSize(const Core::Math::Size& availableSize) {
        if (cxyFixed_.y == 0) return Core::Math::Size(cxyFixed_.x, Core::FontManager::Get()->GetFont(GetFont())->GetTEXTMETRIC().tmHeight + 8);
        return Label::EstimateSize(availableSize);
        //return Core::Math::Size();
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
        if (text_.empty()) {
            return;
        }

        if (IsFocused()) {
            state_ = state_ | State::FOCUSED;
        } else {
            state_ = state_ & ~State::FOCUSED;
        }

        if (!IsEnabled()) {
            state_ = state_ & ~State::DISABLED;
        } else {
            state_ = state_ | State::DISABLED;
        }

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

        if (IsShowHtml()) {
            context_->DrawHtmlText(drawRect, text_, textColor_, font_, 0);
        } else {
            context_->DrawUIText(drawRect, text_, textColor_, font_, 0);
        }
    }

    void Button::OnDrawStatusImage() {
        if (IsFocused()) state_ |= State::FOCUSED;
        else state_ &= ~State::FOCUSED;
        if (!IsEnabled()) state_ |= State::DISABLED;
        else state_ &= ~State::DISABLED;

        if ((state_ & State::DISABLED) != 0) {
            if (!disableImage_.empty()) {
                context_->DrawImageString(rectPaint_, rect_, disableImage_);
            } else goto Label_ForeImage;
        } else if ((state_ & State::PUSHED) != 0) {
            if (!pushedImage_.empty()) {
                context_->DrawImageString(rectPaint_, rect_, pushedImage_);
            }
        } else if ((state_ & State::HOT) != 0) {
            if (!hotImage_.empty()) {
                context_->DrawImageString(rectPaint_, rect_, hotImage_);
            } else if (hotBKColor_ != 0) {
                context_->DrawColor(rectPaint_, hotBKColor_);
                return;
            }
        } else if ((state_ & State::FOCUSED) != 0) {
            if (!foucesedImage_.empty()) {
                context_->DrawImageString(rectPaint_, rect_, foucesedImage_);
            }
        }else if (!normalImage_.empty()) {
            context_->DrawImageString(rectPaint_, rect_, normalImage_);
            /* if (!DrawImage(hDC, (LPCTSTR)m_sNormalImage)) m_sNormalImage.Empty();
            else goto Label_ForeImage;*/
        }

    //    if (!m_sForeImage.IsEmpty())
    //        goto Label_ForeImage;

        return;

    Label_ForeImage:
        if (!hotForeImage_.empty()) {
            //if (!DrawImage(hDC, (LPCTSTR)m_sForeImage)) m_sForeImage.Empty();
        }
    }

    

}
