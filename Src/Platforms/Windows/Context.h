#pragma once

#include <Windows.h>
#include <string>

#include "Core/Constent.h"
#include "Core/Math/Size.h"
#include "Core/Math/Rect.h"
#include "Core/Platform.h"
#include "Core/Image.h"

#include "Window/Control.h"
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
                HFONT AddFont(int id, const std::string& font, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared = false);
                HFONT GetFont(int id);
                void RemoveFont(int id, bool bShared = false);
                void RemoveAllFonts(bool bShared = false);
                Font* GetFontInfo(int id);

                const Core::Image::Ptr AddImage(const std::string & bitmap, const std::string & type, unsigned long mask = 0, bool hsl = false, bool shared = false);
                const Core::Image::Ptr AddImage(const std::string& bitmap, HBITMAP hBitmap, int width, int height, bool alpha, bool shared = false);


                static void SetInstanceHandle(HINSTANCE handle);
                static HINSTANCE GetInstanceHandle();
                static void SetResourceDll(HINSTANCE hInst);
                static HINSTANCE GetResourceDll();

                void DrawColor(const Core::Math::Rect& rect, unsigned long color) override;
                void DrawGradient(const Core::Math::Rect& rect, unsigned long color1,
                    unsigned long color2, bool vertical, int32_t steps) override;

                void Invalidate(Core::Math::Rect& rect) override;

                const Core::Math::Size& GetSize() const { return size_; }
                void SetInitSize(const Core::Math::Size& size);
                
                const Core::Math::Size& GetRoundCorner() const { return roundCorner_; }
                void SetRoundCorner(const Core::Math::Size& roundCorner) { roundCorner_ = roundCorner;}
                
                const Core::Math::Size& GetMaxInfo() const { return maxInfo_; }
                void SetMaxInfo(const Core::Math::Size& maxInfo) { maxInfo_ = maxInfo;}

                void SetSizeBox(const Core::Math::Rect& sizeBox) { sizeBox_ = sizeBox; }
                const Core::Math::Rect& GetSizeBox() const { return sizeBox_; }

                void SetCaptionRect(const Core::Math::Rect& captionRect) { captionRect_ = captionRect; }
                const Core::Math::Rect& GetCaptionRect() const { return captionRect_; }

                bool Attach(Window::Control::Ptr control);

                LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result);

                void SetFocus(Window::Control* control, bool focusWnd) override;

            private:
               

            private:
                std::string name_;
                HWND hWndPaint_;
                HDC hdcPaint_;
                Core::Math::Size size_;
                Core::Math::Size roundCorner_;
                Core::Math::Size maxInfo_;
                Core::Math::Rect sizeBox_;
                Core::Math::Rect captionRect_;

                Window::Control::Ptr control_ { nullptr };
            };
        }
    }
}
