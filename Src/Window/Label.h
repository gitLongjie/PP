#pragma once

#include "Window/Control.h"

namespace PPEngine {
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
            void FixRect(Core::Math::Rect rect) override;

            void SetText(const char* text) { SetText(std::string(text)); }
            void SetText(const std::string& text);
            const std::string& GetText() const { return text_; }

            void SetFont(int32_t font) { font_ = font; }
            int32_t GetFont() const { return font_;}
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
            void SetTextRenderingHintAntiAlias(int32_t textRenderingHintAntiAlias) { textRenderingHintAntiAlias_ = textRenderingHintAntiAlias; }
            int32_t GetTextRenderingHintAntiAlias() const { return textRenderingHintAntiAlias_; }
            void SetTransShadow(int32_t transShadow) { transShadow_ = transShadow; }
            int32_t GetTransShadow() const { return transShadow_; }
            void SetTransText(int32_t transText) { transText_ = transText;}
            int32_t GetTransText() const { return transText_; }
            void SetTransShadow1(int32_t transShadow) { transShadow1_ = transShadow; }
            int32_t GetTransShadow1() const { return transShadow1_; }
            void SetTransText1(int32_t transText) { transText1_ = transText;}
            int32_t GetTransText1() const { return transText1_; }
            void SetGradientAngle(int32_t gradientAngle) { gradientAngle_ = gradientAngle; }
            int32_t GetGradientAngle() const { return gradientAngle_; }
            void SetEnabledStroke(bool enabledStroke) { enabledStroke_ = enabledStroke; }
            bool IsEnabledStroke() const { return enabledStroke_; }
            void SetEnabledShadow(bool enabledShadow) { enabledShadow_ = enabledShadow; }
            bool IsEnabledShadow() const { return enabledShadow_; }
            void SetTransStroke(int32_t transStroke) { transStroke_ = transStroke; }
            int32_t IsTransStroke() const { return transStroke_; }
            void SetGradientLength(int32_t gradientLength) { gradientLength_ = gradientLength; }
            int32_t GetGradientLength() const { return gradientLength_; }
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
            int32_t font_;
            unsigned long textColor_{ 0 };
            unsigned long disabledTextColor_{ 0 };
            Core::Math::Rect textPadding_;
            bool showHtml_{ false };
            bool enabledEffect_{ false };
            int32_t textRenderingHintAntiAlias_{ 0 };
            int32_t transShadow_{ 0 };
            int32_t transText_{ 0 };
            int32_t transShadow1_{ 0 };
            int32_t transText1_{ 0 };
            int32_t gradientAngle_{ 0 };
            bool enabledStroke_{ false };
            bool enabledShadow_{ false };
            int32_t transStroke_{ false };
            int32_t gradientLength_{ 0 };
            Core::Math::Size shadowOffset_;
            unsigned long textColor1_{ 0 };
            unsigned long textShadowColorA_{ 0 };
            unsigned long textShadowColorB_{ 0 };
            unsigned long strokeColor_{ 0 };

            std::string text_;
        };
    }
}
