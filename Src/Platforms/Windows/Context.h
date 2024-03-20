#pragma once

#include <Windows.h>
#include <string>

#include "Core/Constent.h"
#include "Core/Math/Size.h"
#include "Core/Math/Rect.h"
#include "Core/Platform.h"
#include "Core/Image.h"

#include "Window/Context.h"
#include "Platforms/Windows/Font.h"

namespace PPEngine {
    namespace Platforms {
        namespace Windows {
            class Context : public Window::Context {
            public:
                NON_COPYABLE(Context)

                Context() = default;
                ~Context() override = default;

            public:
                void Init(HWND hWnd, const char* name = nullptr);

                Font* GetDefaultFontInfo();
                void SetDefaultFont(const std::string& font, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared = false);
                int32 GetCustomFontCount(bool bShared = false) const;
             /*   HFONT AddFont(int id, const std::string& font, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared = false);*/
                HFONT GetFont(int id);
                void RemoveFont(int id, bool bShared = false);
                void RemoveAllFonts(bool bShared = false);
                Font* GetFontInfo(int id);

                bool Serialize(tinyxml2::XMLElement* xmlElement) override;

                void AddFont(int id, Core::Font::Ptr font, bool shared) override;

                void DrawLine(const glm::vec2& start, const glm::vec2& end, int32 size, uint32 color, int nStyle = 0) override;
                void DrawRect(const Core::Math::Rect& rectPaint, int32 size, uint32 color) override;
                void DrawRoundRect(const Core::Math::Rect& rectPaint, int32 size, int32 width, int32 height, uint32 color) override;
                void DrawColor(const Core::Math::Rect& rect, uint32 color) override;
                bool DrawImage(Core::ImageDrawUI& imageDrawUI, Core::Image::Ptr image) override;
                void DrawGradient(const Core::Math::Rect& rect, uint32 color1,
                    uint32 color2, bool vertical, int32 steps) override;
                void DrawUIText(const Core::Math::Rect& rect, const std::string& text, uint32 color, int32 font, uint32 style) override;
                void DrawHtmlText(const Core::Math::Rect& rect, const std::string& text, uint32 color, int32 font, uint32 style) override;

                void Invalidate(Core::Math::Rect& rect) override;

                void SetFocus(Window::Control* control, bool focusWnd) override;
             //   void GenerateRoundClip(const Core::Math::Rect& rect, const Core::Math::Rect& rcItem, int width, int height) override;

                void SetInitSize(const Core::Math::Size& size) override;

                LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);

            protected:
                Core::Image::Ptr AddImage(const std::string& name, const std::string& type, uint32 mask = 0) override;

                HBITMAP GetBitmap(Core::Image::Ptr image, uint32 mask);

            private:
               

            private:
                std::string name_;
                HWND hWndPaint_;
                HDC hdcPaint_;
            };
        }
    }
}
