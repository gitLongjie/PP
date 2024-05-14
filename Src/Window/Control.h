#pragma once

#include <memory>
#include <string>
#include <functional>

#include "Core/Xml/tinyxml2.h"

#include "Core/Math/Size.h"
#include "Core/Math/Rect.h"
#include "Core/Constent.h"

namespace PPEngine {
    namespace Window {
        class Control : public std::enable_shared_from_this<Control> {
        public:
            using Ptr = std::shared_ptr<Control>;

        public:
            Control();
            virtual ~Control();
            static Control::Ptr Create();


            void SetContext(class Context* context, Control* parent) {
                context_ = context;
                parent_ = parent;
            }
            class Context* GetContext() const { return context_; }
            Control* GetParent() const { return parent_; }

            bool Serialize(tinyxml2::XMLElement* root);

            virtual const char* GetClass() const { return "Control"; }
            virtual void InitControl(Control* parent);
            virtual bool IsVisible() const { return visible_ && internVisible_; }
            virtual void SetVisible(bool visible = true);
            virtual void SetAttribute(const char* name, const char* value);
            virtual void FixRect(Core::Math::Rect rect);
            virtual void SetInternVisible(bool visible);

            virtual void OnDraw(const Core::Math::Rect& rect);
            virtual Core::Math::Size EstimateSize(const Core::Math::Size& availableSize) {
                return cxyFixed_;
            }

            void SetFloat(bool value) { float_ = value; }
            bool IsFloat() const { return float_; }
            void Invalidate();
            bool IsUpdateNeeded() const;
            void NeedUpdate();
            void NeedParentUpdate();
            bool IsEnabled() const { return enabled_; }
            void SetEnabled(bool enable = true);

            bool IsFocused() const { return focused_; }
            void SetFocuse();

            const Core::Math::Rect& GetRect() const { return rect_; }
            unsigned long GetColor(unsigned long color);
            void SetBkColor(unsigned long color);
            void SetBkColor2(unsigned long color);
            void SetBkColor3(unsigned long color);
            void SetBorderSize(const Core::Math::Rect& size);
            void SetBorderSize(int32_t size);
            void SetBorderRound(const Core::Math::Size& size);
            void SetBorderColor(unsigned long color);
            void SetBkImage(const std::string& bkImage);
            const std::string& GetBkImage() const { return bkImage_; }
            unsigned long GetBorderColor() const { return borderColor_; }
            void SetFocusBorderColor(unsigned long color);
            unsigned long SetFocusBorderColor() const { return focusBorderColor_; }

            void SetPadding(const Core::Math::Rect& padding);
            const Core::Math::Rect GetPadding() const { return rcPadding_; }
            void SetFixedXY(const Core::Math::Size& cxy);
            const Core::Math::Size& GetFixedXY() const { return cXY_; }
            void SetFixed(const Core::Math::Size& size);
            const Core::Math::Size& GetFixed() const { return cxyFixed_; }
            void SetFixedWidth(float width);
            float GetFixedWidth() const { return cxyFixed_.x; }
            void SetFixedHeight(float height);
            float GetFixedHeight() const { return cxyFixed_.y; }
            void SetMinWidth(float width);
            float GetMinWidth() const { return cxyMin_.x; }
            void SetMinHeight(float height);
            float GetMinHeight() const { return cxyMin_.y; }
            void SetMaxWidth(float width);
            float GetMaxWidth() const { return cxyMax_.x; }
            void SetMaxHeight(float height);
            float GetMaxHeight() const { return cxyMax_.y; }
            /*virtual void SetRelativePos(const Core::Math::Size& szMove, const Core::Math::Size& szZoom);
            virtual void SetRelativeParentSize(const Core::Math::Size& sz);*/
           // virtual TRelativePosUI GetRelativePos() const;
            bool IsRelativePos() const { return false; }


        protected:
            virtual void OnDrawBkColor();
            virtual void OnDrawBkImage();
            virtual void OnDrawStatusImage();
            virtual void OnDrawText();
            virtual void OnDrawBorder();
            virtual void CreateControl(tinyxml2::XMLElement* root);

        protected:
            class Context* context_{ nullptr };
            Control* parent_{ nullptr };

            Core::Math::Rect rect_;
            Core::Math::Rect rectPaint_;
            Core::Math::Rect rcPadding_;
            Core::Math::Size cXY_{ 0.0f, 0.0f };
            Core::Math::Size cxyFixed_{ 0.0f, 0.0f };
            Core::Math::Size cxyMin_{ 0.0f, 0.0f };
            Core::Math::Size cxyMax_{ INT32_MAX, INT32_MAX };

            bool float_{ false };
            bool focused_{ false };

            bool visible_{ true };
            bool internVisible_{ true };
            bool enabled_{ true };

            bool updateNeeded_ { true };
            bool isHSL_{ false };
            unsigned long bkColor_{ 0 };
            unsigned long bkColor2_{ 0 };
            unsigned long bkColor3_{ 0 };
            unsigned long borderColor_{ 0 };
            unsigned long focusBorderColor_{ 0 };

            Core::Math::Rect borderRect_;
            int32_t borderSize_{ 0 };
            Core::Math::Size borderRound_{ 0.0f, 0.0f };

            std::string bkImage_;
        };

        using ControlCreator = std::function<Control::Ptr()>;
        void RegisterControlCreator(const char* type, ControlCreator);
        Control::Ptr CreateControl(const char* type);
    }
}
