#pragma once

#include <vector>
#include <string>
#include <unordered_set>

#include "Core/Xml/tinyxml2.h"
#include "Core/Math/Rect.h"
#include "Core/Font.h"
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

            virtual bool Serialize(tinyxml2::XMLElement* root);

            virtual void AddFont(int id, Core::Font::Ptr font, bool shared);

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
            virtual void DrawUIText(const Core::Math::Rect& rect, const std::string& text, unsigned long color, int32_t font, uint32_t style);
            //virtual void GenerateRoundClip(const Core::Math::Rect& rect, const Core::Math::Rect& rcItem, int width, int height);

            void SetDefaultTextColor(unsigned long color, bool shared);
            unsigned long GetDefaultTextColor() const { return defaultInfo_.fontColor_; }
            void SetDisabledColor(unsigned long color, bool shared);
            unsigned long GetDisabledColor() const { return defaultInfo_.fontColor_; }
            

            bool IsUpdateNeeded() const { return needUpdate_; }
            void NeedUpdate() { needUpdate_ = true; }

            void InitControl(Control::Ptr control, Control* parent);
            void UninitContorl(Control::Ptr control);
            bool Attach(Control::Ptr control);

            void ParseAttribute(tinyxml2::XMLElement* root);
            void ParseFontAttribute(tinyxml2::XMLElement* root);

            const Core::Math::Size& GetSize() const { return size_; }
            virtual void SetInitSize(const Core::Math::Size& size);

            const Core::Math::Size& GetRoundCorner() const { return roundCorner_; }
            void SetRoundCorner(const Core::Math::Size& roundCorner) { roundCorner_ = roundCorner; }

            const Core::Math::Size& GetMaxInfo() const { return maxInfo_; }
            void SetMaxInfo(const Core::Math::Size& maxInfo) { maxInfo_ = maxInfo; }

            void SetSizeBox(const Core::Math::Rect& sizeBox) { sizeBox_ = sizeBox; }
            const Core::Math::Rect& GetSizeBox() const { return sizeBox_; }

            void SetCaptionRect(const Core::Math::Rect& captionRect) { captionRect_ = captionRect; }
            const Core::Math::Rect& GetCaptionRect() const { return captionRect_; }
            

        protected:
            Core::Math::Size size_;
            Core::Math::Size roundCorner_;
            Core::Math::Size maxInfo_;
            Core::Math::Rect sizeBox_;
            Core::Math::Rect captionRect_;

            std::vector<Control*> postPaintControls_;
            Control* focusControl_{ nullptr };
            bool needUpdate_{ false };
            bool noActivate_{ false };

            Control::Ptr control_{ nullptr };
            std::unordered_set<Control::Ptr> controls_;
            DefaultInfo defaultInfo_;
        };
    }
}