#include "Platforms/Windows/Context.h"

#include <assert.h>
#include <unordered_map>
#include "Platforms/Windows/Font.h"
#include "Platforms/Windows/WindowRender.h"


namespace PPEngine {
    namespace Platforms {
        namespace Windows {
            HINSTANCE gInstanceHandle_ = nullptr;
            HINSTANCE gResourceInstanceHandle_ = nullptr;


            



            void Context::Init(HWND hWnd, const char* name /*= nullptr*/) {
                assert(::IsWindow(hWnd));

                if (nullptr != name) { name_ = name; }
                if (hWndPaint_ != hWnd) {
                    hWndPaint_ = hWnd;
                    hdcPaint_ = ::GetDC(hWnd);
                }
            }

            Windows::Font* Context::GetDefaultFontInfo() {
                Core::Font::Ptr font = Core::FontManager::Get()->GetDefaultFontInfo();
                if (!font) {
                    return nullptr;
                }

                Font* windowsFont = reinterpret_cast<Font*>(font.get());
                if (0 == windowsFont->GetTEXTMETRIC().tmHeight) {
                    HFONT hOldFont = (HFONT) ::SelectObject(hdcPaint_, windowsFont->GetFont());
                    ::GetTextMetrics(hdcPaint_, &windowsFont->GetTEXTMETRIC());
                    ::SelectObject(hdcPaint_, hOldFont);
                }
                
                return windowsFont;
            }

            void Context::SetDefaultFont(const std::string& font, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared) {
                Font::Ptr ptr = Font::Create(font, nSize, bBold, bUnderline, bItalic);
                if (!ptr) {
                    return;
                }

                Core::FontManager::Get()->SetDefaultFont(std::move(ptr), bShared);
            }

            int32_t Context::GetCustomFontCount(bool bShared) const {
                return Core::FontManager::Get()->GetFontCount(bShared);
            }

            HFONT Context::AddFont(int id, const std::string& font, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared) {
                Font::Ptr ptr = Font::Create(font, nSize, bBold, bUnderline, bItalic);
                if (!ptr) {
                    return nullptr;
                }
                HFONT ret = ptr->GetFont();
                if (hdcPaint_) {
                    HFONT hOldFont = (HFONT) ::SelectObject(hdcPaint_, ptr->GetFont());
                    ::GetTextMetrics(hdcPaint_, &ptr->GetTEXTMETRIC());
                    ::SelectObject(hdcPaint_, hOldFont);
                }

                Core::FontManager::Get()->Add(id, std::move(ptr), bShared);

                return ret;
            }

            HFONT Context::GetFont(int id) {
                if (id < 0) {
                    Font* font = GetDefaultFontInfo();
                    if (nullptr != font) {
                        return font->GetFont();
                    }
                };

                Core::Font::Ptr font = Core::FontManager::Get()->GetFont(id);
                if (!font) {
                    return nullptr;
                }

                Font* windowFont = reinterpret_cast<Font*>(font.get());
                return windowFont->GetFont();
            }

            void Context::RemoveFont(int id, bool bShared) {
                Core::FontManager::Get()->Remove(id, bShared);
            }

            void Context::RemoveAllFonts(bool bShared) {
                Core::FontManager::Get()->Clear(bShared);
            }

            Font* Context::GetFontInfo(int id) {
                Core::Font::Ptr coreFont = Core::FontManager::Get()->GetFont(id);
                if (!coreFont) {
                    return nullptr;
                }

                Font* font = reinterpret_cast<Font*>(coreFont.get());
                return font;
            }

           

            const Core::Image::Ptr Context::AddImage(const std::string& bitmap, const std::string& type, unsigned long mask /*= 0*/, bool hsl /*= false*/, bool shared /*= false*/) {
                return nullptr;
            }

            const Core::Image::Ptr Context::AddImage(const std::string& bitmap, HBITMAP hBitmap, int width, int height, bool alpha, bool shared /*= false*/) {
                return nullptr;
            }

            void Context::SetInstanceHandle(HINSTANCE handle) {
                gInstanceHandle_ = handle;
            }

            HINSTANCE Context::GetInstanceHandle() {
                return gInstanceHandle_;
            }

            void  Context::SetResourceDll(HINSTANCE hInst) {
                gResourceInstanceHandle_ = hInst;
            }
            HINSTANCE Context::GetResourceDll() {
                if (nullptr == gResourceInstanceHandle_ == 0L) return gInstanceHandle_;
                return gResourceInstanceHandle_;
            }

            void Context::Invalidate(Core::Math::Rect& rect) {
                if (nullptr == hWndPaint_) {
                    return;
                }

                const glm::vec2& min = rect.GetMin();
                const glm::vec2& max = rect.GetMax();
                RECT winRect = { static_cast<long>(min.x), static_cast<long>(min.y), static_cast<long>(max.x), static_cast<long>(max.y) };
                ::InvalidateRect(hWndPaint_, &winRect, FALSE);
            }

            void Context::SetInitSize(const Core::Math::Size& size) {
                size_ = size;
                if (nullptr == control_ && hWndPaint_ != nullptr) {
                    ::SetWindowPos(hWndPaint_, nullptr, 0, 0, size_.x, size_.y, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
                }
            }

            bool Context::Attach(Window::Control::Ptr control) {
                if (nullptr == control) {
                    return false;
                }

                control->SetContext(this, nullptr);
                control_ = control;
                return true;
            }

            LRESULT Context::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result) {
                if (nullptr == hWndPaint_) {
                    return false;
                }

                switch (uMsg) {
                    case WM_ERASEBKGND: {
                        // We'll do the painting here...
                        result = 1;
                    }
                    return true;

                    case WM_PAINT:{
                        PAINTSTRUCT ps = { 0 };
                        ::BeginPaint(hWndPaint_, &ps);
                        WindowRender::DrawColor(hdcPaint_, ps.rcPaint, 0xFFFF0000);
                        ::EndPaint(hWndPaint_, &ps);
                        return true;
                    }
                    return true;
                }

                return false;
            }

            void Context::SetFocus(Window::Control* control, bool focusWnd) {
                HWND hFocusWnd = ::GetFocus();
                if (focusWnd && hFocusWnd != hWndPaint_ && control != GetFocus() && !noActivate_) ::SetFocus(hWndPaint_);
                if (control == GetFocus()) return;

                if (GetFocus() != NULL) {
                   /* TEventUI event = { 0 };
                    event.Type = UIEVENT_KILLFOCUS;
                    event.pSender = pControl;
                    event.dwTimestamp = ::GetTickCount();
                    m_pFocus->Event(event);
                    SendNotify(m_pFocus, DUI_MSGTYPE_KILLFOCUS);
                    m_pFocus = NULL;*/
                }
                if (control == NULL) return;

                if (control != NULL
                    && control->GetContext() == this
                    && control->IsVisible()
                    && control->IsEnabled()) {
                    focusControl_ = control;
                   /* TEventUI event = { 0 };
                    event.Type = UIEVENT_SETFOCUS;
                    event.pSender = pControl;
                    event.dwTimestamp = ::GetTickCount();
                    m_pFocus->Event(event);
                    SendNotify(m_pFocus, DUI_MSGTYPE_SETFOCUS);*/
                }
            }

        }
    }
}