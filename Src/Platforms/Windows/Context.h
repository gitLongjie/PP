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
                int32_t GetCustomFontCount(bool bShared = false) const;
             /*   HFONT AddFont(int id, const std::string& font, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared = false);*/
                HFONT GetFont(int id);
                void RemoveFont(int id, bool bShared = false);
                void RemoveAllFonts(bool bShared = false);
                Font* GetFontInfo(int id);

                bool Serialize(tinyxml2::XMLElement* xmlElement) override;

                void AddFont(int id, Core::Font::Ptr font, bool shared) override;

                void DrawLine(const glm::vec2& start, const glm::vec2& end, int32_t size, unsigned long color, int nStyle = 0) override;
                void DrawRect(const Core::Math::Rect& rectPaint, int32_t size, unsigned long color) override;
                void DrawRoundRect(const Core::Math::Rect& rectPaint, int32_t size, int32_t width, int32_t height, unsigned long color) override;
                void DrawColor(const Core::Math::Rect& rect, unsigned long color) override;
                bool DrawImage(Core::ImageDrawUI& imageDrawUI, Core::Image::Ptr image) override;
                void DrawGradient(const Core::Math::Rect& rect, unsigned long color1,
                    unsigned long color2, bool vertical, int32_t steps) override;
                void DrawUIText(const Core::Math::Rect& rect, const std::string& text, unsigned long color, int32_t font, uint32_t style) override;

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
