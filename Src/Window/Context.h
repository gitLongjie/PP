#pragma once

#include <vector>
#include <string>
#include <unordered_set>

#include "Core/Xml/tinyxml2.h"
#include "Core/Math/Rect.h"
#include "Core/Font.h"
#include "Core/Image.h"
#include "Window/Control.h"


namespace PPEngine {
    namespace Window {

        class Context {
        public:
            struct DefaultInfo {
                uint32 disabledColor_{ 0 };
                uint32 fontColor_{ 0 };
                uint32 selectedBkColor_{ 0 };
                uint32 defaultLinkFontColor_{ 0 };
                uint32 defaultLinkHoverFontColor_{ 0 };
            };

        public:
            virtual ~Context() = default;

            virtual bool Serialize(tinyxml2::XMLElement* root);

            virtual void AddFont(int id, Core::Font::Ptr font, bool shared);

            virtual void Invalidate(Core::Math::Rect& rect);
            virtual void RemovePostPaint(Control* control);
            virtual void SetFocus(Control* control, bool focusWnd);
            virtual Control* GetFocus() const { return focusControl_; }
            virtual void DrawLine(const glm::vec2& start, const glm::vec2& end, int32 size, uint32 color, int nStyle = 0);
            virtual void DrawRect(const Core::Math::Rect& rectPaint, int32 size, uint32 color);
            virtual void DrawRoundRect(const Core::Math::Rect& rectPaint, int32 size, int32 width, int32 height, uint32 color);
            virtual void DrawImageString(const Core::Math::Rect& rectPaint, const Core::Math::Rect& rect, const std::string& image);
            virtual bool DrawImage(Core::ImageDrawUI& imageDrawUI);
            virtual bool DrawImage(Core::ImageDrawUI& imageDrawUI, Core::Image::Ptr image);
            virtual void DrawColor(const Core::Math::Rect& rect, uint32 color);
            virtual void DrawGradient(const Core::Math::Rect& rect, uint32 color1,
                uint32 color2, bool vertical, int32 steps);
            virtual void DrawUIText(const Core::Math::Rect& rect, const std::string& text, uint32 color, int32 font, uint32 style);
            virtual void DrawHtmlText(const Core::Math::Rect& rect, const std::string& text, uint32 color, int32 font, uint32 style);
            //virtual void GenerateRoundClip(const Core::Math::Rect& rect, const Core::Math::Rect& rcItem, int width, int height);

            void SetDefaultTextColor(uint32 color, bool shared);
            uint32 GetDefaultTextColor() const { return defaultInfo_.fontColor_; }
            void SetDisabledColor(uint32 color, bool shared);
            uint32 GetDisabledColor() const { return defaultInfo_.fontColor_; }
            void SetDefaultLinkFontColor(uint32 color, bool shared);
            uint32 GetDefaultLinkFontColor() const { return defaultInfo_.defaultLinkFontColor_; }
            void SetDefaultLinkHoverFontColor(uint32 color, bool shared);
            uint32 GetDefaultLinkHoverFontColor() const { return defaultInfo_.defaultLinkHoverFontColor_; }
            uint32 GetDefaultSelectedBkColor() const { return defaultInfo_.selectedBkColor_; }

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

            const Core::Math::Size& GetLastMoustPoint() const { return lastMousePt_; }

        protected:
            Core::Image::Ptr GetImageEx(const std::string& name, const std::string& type, uint32 mask = 0);
            virtual Core::Image::Ptr AddImage(const std::string& bitmap, const std::string& type, uint32 mask = 0);
            

        protected:
            Core::Math::Size size_{0, 0};
            Core::Math::Size roundCorner_{0, 0};
            Core::Math::Size maxInfo_{0, 0};
            Core::Math::Rect sizeBox_;
            Core::Math::Rect captionRect_;

            std::vector<Control*> postPaintControls_;
            Control* focusControl_{ nullptr };
            bool needUpdate_{ false };
            bool noActivate_{ false };

            Control::Ptr control_{ nullptr };
            std::unordered_set<Control::Ptr> controls_;
            DefaultInfo defaultInfo_;

            Core::Math::Size lastMousePt_{-1, -1};
        };
    }
}