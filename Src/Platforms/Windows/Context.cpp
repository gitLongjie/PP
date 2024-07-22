#include "Platforms/Windows/Context.h"

#include <assert.h>
#include <unordered_map>
#include <Windowsx.h>

#include "Core/Font.h"
#include "Core/EventSystem/Event.h"
#include "Core/EventSystem/EventManager.h"
#include "Window/Constent.h"

#include "Platforms/Windows/WindowRender.h"
#include "Platforms/Windows/ResourceManager.h"


namespace Platforms {
    namespace Windows {
        void Context::Init(HWND hWnd, const char* name /*= nullptr*/) {
            assert(::IsWindow(hWnd));

            if (nullptr != name) { name_ = name; }
            if (hWndPaint_ != hWnd) {
                hWndPaint_ = hWnd;
                hdcPaint_ = ::GetDC(hWnd);
            }

            LOGFONT lf = { 0 };
            ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
            lf.lfCharSet = DEFAULT_CHARSET;
      
            HFONT hDefaultFont = ::CreateFontIndirect(&lf);
            Core::Font* font = AddFont("", -lf.lfHeight, (lf.lfWeight >= FW_BOLD), (lf.lfUnderline == TRUE), (lf.lfItalic == TRUE));
            font->SetFont(hDefaultFont);
            Core::FontManager::Get()->SetDefaultFont(font, false);
        }

        bool Context::Serialize(tinyxml2::XMLElement* xmlElement) {
            if (nullptr == xmlElement) {
                return false;
            }
            return Window::Context::Serialize(xmlElement);
        }

        void Context::DrawLine(const glm::vec2& start, const glm::vec2& end, int32 size, uint32 color, int nStyle) {
            POINT ptStart{ static_cast<long>(start.x), static_cast<long>(start.y) };
            POINT ptEnd{ static_cast<long>(end.x), static_cast<long>(end.y) };
            WindowRender::DrawLine(hdcPaint_, ptStart, ptEnd, color, nStyle);
        }

        void Context::DrawRect(const Core::Math::Rect& rectPaint, int32 size, uint32 color) {
            const glm::vec2& minRect = rectPaint.GetMin();
            const glm::vec2& maxRect = rectPaint.GetMax();
            RECT rc{ static_cast<long>(minRect.x), static_cast<long>(minRect.y),
                static_cast<long>(maxRect.x), static_cast<long>(maxRect.y) };
            WindowRender::DrawRect(hdcPaint_, rc, color, color);
        }

        void Context::DrawRoundRect(const Core::Math::Rect& rectPaint, int32 size, int32 width, int32 height, uint32 color) {
            const glm::vec2& minRect = rectPaint.GetMin();
            const glm::vec2& maxRect = rectPaint.GetMax();
            RECT rc{ static_cast<long>(minRect.x), static_cast<long>(minRect.y),
                static_cast<long>(maxRect.x), static_cast<long>(maxRect.y) };
            WindowRender::DrawRoundRect(hdcPaint_, rc, width, height, size, color);
        }

        void Context::DrawColor(const Core::Math::Rect& rect, uint32 color) {
            const glm::vec2& minRect = rect.GetMin();
            const glm::vec2& maxRect = rect.GetMax();
            RECT rc{ static_cast<long>(minRect.x), static_cast<long>(minRect.y),
                static_cast<long>(maxRect.x), static_cast<long>(maxRect.y) };
            WindowRender::DrawColor(hdcPaint_, rc, color);
        }

        bool Context::DrawImage(Core::ImageDrawUI& imageDrawUI, const Core::Image::Ptr& image) {
            HBITMAP hBitmap = ResourceManager::Get()->GetBitmap(image->GetName());
            if (nullptr == hBitmap) {
                return false;
            }

            const Core::Math::Rect& imageRC = imageDrawUI.rc;
            RECT rc = { static_cast<long>(imageRC.GetMin().x), static_cast<long>(imageRC.GetMin().y),
                static_cast<long>(imageRC.GetMax().x), static_cast<long>(imageRC.GetMax().y) };
            const Core::Math::Rect& imagePaintRC = imageDrawUI.rcPaint;
            RECT rcPaint = { static_cast<long>(imagePaintRC.GetMin().x), static_cast<long>(imagePaintRC.GetMin().y),
                static_cast<long>(imagePaintRC.GetMax().x), static_cast<long>(imagePaintRC.GetMax().y) };
            const Core::Math::Rect& imageBmpRC = imageDrawUI.rcBmpPart;
            RECT rcBmpPart = { static_cast<long>(imageBmpRC.GetMin().x), static_cast<long>(imageBmpRC.GetMin().y),
                static_cast<long>(imageBmpRC.GetMax().x), static_cast<long>(imageBmpRC.GetMax().y) };
            const Core::Math::Rect& imageCornerRC = imageDrawUI.rcCorner;
            RECT rcCornerPart = { static_cast<long>(imageCornerRC.GetMin().x), static_cast<long>(imageCornerRC.GetMin().y),
                static_cast<long>(imageCornerRC.GetMax().x), static_cast<long>(imageCornerRC.GetMax().y) };
            return WindowRender::DrawImage(hdcPaint_, hBitmap, rc, rcPaint, rcBmpPart, rcCornerPart, image->IsAlpha(),
                imageDrawUI.fade, imageDrawUI.hole, imageDrawUI.xtiled, imageDrawUI.ytiled);
        }

        void Context::DrawGradient(const Core::Math::Rect& rect, uint32 color1,
            uint32 color2, bool vertical, int32 steps) {
            const glm::vec2& minRect = rect.GetMin();
            const glm::vec2& maxRect = rect.GetMax();
            RECT rc{ static_cast<long>(minRect.x), static_cast<long>(minRect.y),
                static_cast<long>(maxRect.x), static_cast<long>(maxRect.y) };
            WindowRender::DrawGradient(hdcPaint_, rc, color1, color2, vertical, steps);
        }

        void Context::DrawUIText(const Core::Math::Rect& rect, const std::string& text, uint32 color, int32 font, uint32 style) {
            if (text.empty()) {
                return;
            }

            ::SetBkMode(hdcPaint_, TRANSPARENT);
            ::SetTextColor(hdcPaint_, RGB(GetBValue(color), GetGValue(color), GetRValue(color)));
            Core::Font* fontPtr = Core::FontManager::Get()->GetFont(font);
            HFONT old = static_cast<HFONT>(::SelectObject(hdcPaint_, fontPtr->GetFont()));
            RECT dr{ (long)rect.GetMin().x, (long)rect.GetMin().y, (long)rect.GetMax().x, (long)rect.GetMax().y };
            ::DrawText(hdcPaint_, text.c_str(), -1, &dr, style | DT_NOPREFIX);
            ::SelectObject(hdcPaint_, old);
        }

        void Context::DrawHtmlText(const Core::Math::Rect& rect, const std::string& text, uint32 color, int32 font, uint32 style) {
            RECT rc = { static_cast<long>(rect.GetMin().x), static_cast<long>(rect.GetMin().y),
                        static_cast<long>(rect.GetMax().x), static_cast<long>(rect.GetMax().y) };
            //WindowRender::DrawHtmlText(hdcPaint_, this, rc, text.c_str(), color, nullptr, nullptr, font, style);
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

            case WM_SIZE:
            {
                /* if (m_pFocus != NULL) {
                    TEventUI event = { 0 };
                    event.Type = UIEVENT_WINDOWSIZE;
                    event.pSender = m_pFocus;
                    event.dwTimestamp = ::GetTickCount();
                    m_pFocus->Event(event);
                }*/
                if (control_) control_->NeedUpdate();
            }
            return true;

            case WM_PAINT:{
                if (nullptr == control_) {
                    PAINTSTRUCT ps = { 0 };
                    ::BeginPaint(hWndPaint_, &ps);
                    WindowRender::DrawColor(hdcPaint_, ps.rcPaint, 0xFFFF0000);
                    ::EndPaint(hWndPaint_, &ps);
                    return true;
                }

                    
                RECT clientRect{ 0 };
                ::GetClientRect(hWndPaint_, &clientRect);

                if (control_->IsUpdateNeeded()) {
                    float x = clientRect.left;
                    float y = clientRect.top;
                    float width = clientRect.right - clientRect.left;
                    float height = clientRect.bottom - clientRect.top;
                    const Core::Math::Rect rect(x, y, width, height);
                    control_->FixRect(rect);
                }
                PAINTSTRUCT ps = { 0 };
                ::BeginPaint(hWndPaint_, &ps);
                //WindowRender::DrawColor(hdcPaint_, ps.rcPaint, 0xFFFF0000);
                Core::Math::Rect rect(clientRect.left, clientRect.top,
                    clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
                control_->OnDraw(rect);
                ::EndPaint(hWndPaint_, &ps);
            }
            return true;

            case WM_MOUSEMOVE: {
                lastMousePt_ = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                Window::Control* control = FindControl(lastMousePt_,
                    Window::UIFIND_VISIBLE | Window::UIFIND_HITTEST | Window::UIFIND_TOP_FIRST);
                if (nullptr == control || control->GetContext() != this) {
                    return false;
                }
                //Core::EventSystem::MouseMoveEvent event(lastMousePt_);
//                    Core::EventSystem::EventManager::Get()->Send(event, control);
                }
            break;

            case WM_LBUTTONDOWN: {
                ::SetFocus(hWndPaint_);
                lastMousePt_ = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                Window::Control* control = FindControl(lastMousePt_);
                if (nullptr == control || this != control->GetContext()) {
                    break;
                }
                clickControl_ = control_;
            }

            case WM_LBUTTONUP: {
                lastMousePt_ = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                if (nullptr == clickControl_) {
                    break;
                }

                clickControl_ = nullptr;               
            }
            break;

            case WM_SETCURSOR:
            {
                if (LOWORD(lParam) != HTCLIENT) break;
                if (mouseCapture_) return true;

                POINT pt = { 0 };
                ::GetCursorPos(&pt);
                ::ScreenToClient(hWndPaint_, &pt);
                Core::Math::Point2d mousePt_ = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                const Window::Control* pControl = FindControl(mousePt_,
                    Window::UIFIND_VISIBLE | Window::UIFIND_HITTEST | Window::UIFIND_TOP_FIRST);
            }
            return true;
            }
               

            return false;
        }

        Core::Font* Context::AddFont(const std::string& name, int nSize, bool bBold, bool bUnderline, bool bItalic) {
            LOGFONT lf = { 0 };
            ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
            strcpy(lf.lfFaceName, name.c_str());
            lf.lfCharSet = DEFAULT_CHARSET;
            lf.lfHeight = -nSize;
            if (bBold) lf.lfWeight += FW_BOLD;
            if (bUnderline) lf.lfUnderline = true;
            if (bItalic) lf.lfItalic = true;
            HFONT hFont = ::CreateFontIndirect(&lf);
            if (hFont == NULL) return nullptr;

            Core::Font* pFontInfo = new Core::Font(name, nSize, bBold, bUnderline, bItalic);
            if (nullptr == pFontInfo) {
                return nullptr;
            }
                
            pFontInfo->SetFont(hFont);
            if (hdcPaint_) {
                HFONT hOldFont = (HFONT) ::SelectObject(hdcPaint_, hFont);
                ::GetTextMetrics(hdcPaint_, &pFontInfo->GetTEXTMETRIC());
                ::SelectObject(hdcPaint_, hOldFont);
            }

            return pFontInfo;
        }

        Core::Image::Ptr Context::AddImage(const std::string& name, const std::string& type, uint32 mask) {
            Core::Image::Ptr image = Window::Context::AddImage(name, type, mask);
            if (!image) {
                return nullptr;
            }

            HBITMAP hbitmap = ResourceManager::Get()->GetBitmap(name);
            if (nullptr == hbitmap) {
                ResourceManager::Get()->AddBitmap(image, mask);
            }
            return image;
        }

        HBITMAP Context::GetBitmap(Core::Image::Ptr image, uint32 mask) {
            HBITMAP hbitmap = ResourceManager::Get()->GetBitmap(image->GetName());
            if (nullptr == hbitmap) {
                ResourceManager::Get()->AddBitmap(image, mask);
            }

            return ResourceManager::Get()->GetBitmap(image->GetName());
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

        //void Context::GenerateRoundClip(const Core::Math::Rect& rect, const Core::Math::Rect& rcItem, int width, int height) {
        //    RECT rcClip = { 0 };
        //    ::GetClipBox(hdcPaint_, &rcClip);

        //    const glm::vec2 minPos = rect.GetMin();
        //    const glm::vec2 maxPos = rect.GetMax();
        //    RECT rc = { static_cast<long>(minPos.x), static_cast<long>(minPos.y),
        //                static_cast<long>(maxPos.x), static_cast<long>(maxPos.y)
        //    };
        //    HRGN hOldRgn = ::CreateRectRgnIndirect(&rcClip);
        //    HRGN hRgn = ::CreateRectRgnIndirect(&rc);
        //    HRGN hRgnItem = ::CreateRoundRectRgn(rcItem.GetMin().x, rcItem.GetMin().y, rcItem.GetMax().x + 1, rcItem.GetMax().y + 1, width, height);
        //    ::CombineRgn(hRgn, hRgn, hRgnItem, RGN_AND);
        //    ::ExtSelectClipRgn(hdcPaint_, hRgn, RGN_AND);
        //    ::DeleteObject(hRgnItem);
        //}

    }
}
