#pragma once

#include "Window/Control.h"

namespace Window {
    class Label : public Control {
    public:
        enum class Align {
            Left,
            Center,
            Right,
            Top,
            Bottom,
            EndEllipsis
        };
    public:
        Label();
        ~Label() override;

        static Control::Ptr Create();

        const char* GetClass() const { return "Label"; }
        void SetAttribute(const char* name, const char* value) override;

        void SetText(const char* text) { SetText(std::string(text)); }
        void SetText(const std::string& text);
        const std::string& GetText() const { return text_; }

        void SetFont(int32 font) { font_ = font; }
        int32 GetFont() const { return font_;}
        void SetTextColor(unsigned long color) { textColor_ = color; }
        unsigned long GetTextColor() const { return textColor_; }
        void SetDisabledTextColor(unsigned long color) { disabledTextColor_ = color; }
        unsigned long GetDisabledTextColor() const { return disabledTextColor_; }
        void SetTextPadding(Core::Math::Rect& padding) { textPadding_ = padding; }
        const Core::Math::Rect& GetTextPadding() const { return textPadding_; }
        void SetShowHtml(bool showHtml) { showHtml_ = showHtml; }
        bool IsShowHtml() const { return showHtml_; }
        void SetEnabledEffect(bool enable);
        bool IsEnabledEffect() const { return enabledEffect_; }
        void SetTextRenderingHintAntiAlias(int32 textRenderingHintAntiAlias) { textRenderingHintAntiAlias_ = textRenderingHintAntiAlias; }
        int32 GetTextRenderingHintAntiAlias() const { return textRenderingHintAntiAlias_; }
        void SetTransShadow(int32 transShadow) { transShadow_ = transShadow; }
        int32 GetTransShadow() const { return transShadow_; }
        void SetTransText(int32 transText) { transText_ = transText;}
        int32 GetTransText() const { return transText_; }
        void SetTransShadow1(int32 transShadow) { transShadow1_ = transShadow; }
        int32 GetTransShadow1() const { return transShadow1_; }
        void SetTransText1(int32 transText) { transText1_ = transText;}
        int32 GetTransText1() const { return transText1_; }
        void SetGradientAngle(int32 gradientAngle) { gradientAngle_ = gradientAngle; }
        int32 GetGradientAngle() const { return gradientAngle_; }
        void SetEnabledStroke(bool enabledStroke) { enabledStroke_ = enabledStroke; }
        bool IsEnabledStroke() const { return enabledStroke_; }
        void SetEnabledShadow(bool enabledShadow) { enabledShadow_ = enabledShadow; }
        bool IsEnabledShadow() const { return enabledShadow_; }
        void SetTransStroke(int32 transStroke) { transStroke_ = transStroke; }
        int32 IsTransStroke() const { return transStroke_; }
        void SetGradientLength(int32 gradientLength) { gradientLength_ = gradientLength; }
        int32 GetGradientLength() const { return gradientLength_; }
        void SetShadowOffset(const Core::Math::Size& offset) { shadowOffset_ = offset; }
        void SetTextColor1(unsigned long color) { textColor1_ = color; }
        unsigned long GetTextColor1() const { return textColor1_; }
        void SetTextShadowColorA(unsigned long color) { textShadowColorA_ = color; }
        unsigned long GetTextShadowColorA() const { return textShadowColorA_; }
        void SetTextShadowColorB(unsigned long color) { textShadowColorB_ = color; }
        unsigned long GetTextShadowColorB() const { return textShadowColorB_; }
        void SetStrokeColor(unsigned long color) { strokeColor_ = color; }
        unsigned long GetStrokeColor() const { return strokeColor_; }

    protected:
        void OnDrawText() override;

    protected:
        Align align_;
        int32 font_{ 0 };
        unsigned long textColor_{ 0 };
        unsigned long disabledTextColor_{ 0 };
        Core::Math::Rect textPadding_;
        bool showHtml_{ false };
        bool enabledEffect_{ false };
        int32 textRenderingHintAntiAlias_{ 0 };
        int32 transShadow_{ 0 };
        int32 transText_{ 0 };
        int32 transShadow1_{ 0 };
        int32 transText1_{ 0 };
        int32 gradientAngle_{ 0 };
        bool enabledStroke_{ false };
        bool enabledShadow_{ false };
        int32 transStroke_{ false };
        int32 gradientLength_{ 0 };
        Core::Math::Size shadowOffset_;
        unsigned long textColor1_{ 0 };
        unsigned long textShadowColorA_{ 0 };
        unsigned long textShadowColorB_{ 0 };
        unsigned long strokeColor_{ 0 };

        std::string text_;
    };
}
