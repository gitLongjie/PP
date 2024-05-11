#include "Platforms/Windows/WinWindow.h"

#include <Windowsx.h>

#ifdef _DEBUG
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif

#include <cassert>

#include "Platforms/Windows/Context.h"
#include "Platforms/Windows/ResourceManager.h"

namespace PPEngine {
    namespace Platforms {
        namespace Windows {
            CWindowWnd::CWindowWnd() : hWnd_(nullptr), oldWndProc_(::DefWindowProc), subclassed_(false) {}

            HWND CWindowWnd::GetHWND() const {
                return hWnd_;
            }

            UINT CWindowWnd::GetClassStyle() const {
                return 0;
            }

            const char* CWindowWnd::GetSuperClassName() const {
                return nullptr;
            }

            CWindowWnd::operator HWND() const {
                return hWnd_;
            }

            HWND CWindowWnd::CreateDuiWindow(HWND hwndParent, const char* pstrWindowName, DWORD dwStyle /*=0*/, DWORD dwExStyle /*=0*/) {
                return Create(hwndParent, pstrWindowName, dwStyle, dwExStyle, 0, 0, 0, 0, nullptr);
            }

            bool CWindowWnd::Create(const PPEngine::Window::WindowSettings& settings, PPRHI::DriverSettings& driverSetting) {
#define WNDSTYLE_FRAME      (WS_VISIBLE | WS_OVERLAPPEDWINDOW)
                Create(nullptr, settings.title.c_str(), WNDSTYLE_FRAME, 0, 0, 0, settings.width, settings.height);
                CenterWindow();
                ShowWindow(true);
                return true;
            }

            HWND CWindowWnd::Create(HWND hwndParent, const char* pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu) {
                return Create(hwndParent, pstrName, dwStyle, dwExStyle, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hMenu);
            }

            HWND CWindowWnd::Create(HWND hwndParent, const char* pstrName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int cx, int cy, HMENU hMenu) {
                if (GetSuperClassName() != nullptr && !RegisterSuperclass()) return nullptr;
                if (GetSuperClassName() == nullptr && !RegisterWindowClass()) return nullptr;
                hWnd_ = ::CreateWindowEx(dwExStyle, GetWindowClassName(), pstrName, dwStyle, x, y, cx, cy, hwndParent,
                    hMenu, ResourceManager::Get()->GetInstanceHandle(), this);
                ASSERT(hWnd_ != nullptr);
                return hWnd_;
            }

            HWND CWindowWnd::Subclass(HWND hWnd) {
                ASSERT(::IsWindow(hWnd));
                ASSERT(hWnd_ == nullptr);
                oldWndProc_ = SubclassWindow(hWnd, __WndProc);
                if (oldWndProc_ == nullptr) return nullptr;
                subclassed_ = true;
                hWnd_ = hWnd;
                ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(this));
                return hWnd_;
            }

            void CWindowWnd::Unsubclass() {
                ASSERT(::IsWindow(hWnd_));
                if (!::IsWindow(hWnd_)) return;
                if (!subclassed_) return;
                SubclassWindow(hWnd_, oldWndProc_);
                oldWndProc_ = ::DefWindowProc;
                subclassed_ = false;
            }

            void CWindowWnd::ShowWindow(bool bShow /*= true*/, bool bTakeFocus /*= false*/) {
                ASSERT(::IsWindow(hWnd_));
                if (!::IsWindow(hWnd_)) return;
                ::ShowWindow(hWnd_, bShow ? (bTakeFocus ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE) : SW_HIDE);
            }

            UINT CWindowWnd::ShowModal() {
                ASSERT(::IsWindow(hWnd_));
                UINT nRet = 0;
                HWND hWndParent = GetWindowOwner(hWnd_);
                ::ShowWindow(hWnd_, SW_SHOWNORMAL);
                ::EnableWindow(hWndParent, FALSE);
                MSG msg = { 0 };
                while (::IsWindow(hWnd_) && ::GetMessage(&msg, nullptr, 0, 0)) {
                    if (msg.message == WM_CLOSE && msg.hwnd == hWnd_) {
                        nRet = msg.wParam;
                        ::EnableWindow(hWndParent, TRUE);
                        ::SetFocus(hWndParent);
                    }
                    /*if (!CPaintManagerUI::TranslateMessage(&msg)) {
                        ::TranslateMessage(&msg);
                        ::DispatchMessage(&msg);
                    }*/
                    if (msg.message == WM_QUIT) break;
                }
                ::EnableWindow(hWndParent, TRUE);
                ::SetFocus(hWndParent);
                if (msg.message == WM_QUIT) ::PostQuitMessage(msg.wParam);
                return nRet;
            }

            void CWindowWnd::Close(UINT nRet) {
                ASSERT(::IsWindow(hWnd_));
                if (!::IsWindow(hWnd_)) return;
                PostMessage(WM_CLOSE, (WPARAM)nRet, 0L);
            }

            void CWindowWnd::CenterWindow() {
                ASSERT(::IsWindow(hWnd_));
                ASSERT((GetWindowStyle(hWnd_) & WS_CHILD) == 0);
                RECT rcDlg = { 0 };
                ::GetWindowRect(hWnd_, &rcDlg);
                RECT rcArea = { 0 };
                RECT rcCenter = { 0 };
                HWND hWnd = *this;
                HWND hWndParent = ::GetParent(hWnd_);
                HWND hWndCenter = ::GetWindowOwner(hWnd_);
                if (hWndCenter != nullptr)
                    hWnd = hWndCenter;

                // 处理多显示器模式下屏幕居中
                MONITORINFO oMonitor = {};
                oMonitor.cbSize = sizeof(oMonitor);
                ::GetMonitorInfo(::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &oMonitor);
                rcArea = oMonitor.rcWork;

                if (hWndCenter == nullptr)
                    rcCenter = rcArea;
                else
                    ::GetWindowRect(hWndCenter, &rcCenter);

                int DlgWidth = rcDlg.right - rcDlg.left;
                int DlgHeight = rcDlg.bottom - rcDlg.top;

                // Find dialog's upper left based on rcCenter
                int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
                int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

                // The dialog is outside the screen, move it inside
                if (xLeft < rcArea.left) xLeft = rcArea.left;
                else if (xLeft + DlgWidth > rcArea.right) xLeft = rcArea.right - DlgWidth;
                if (yTop < rcArea.top) yTop = rcArea.top;
                else if (yTop + DlgHeight > rcArea.bottom) yTop = rcArea.bottom - DlgHeight;
                ::SetWindowPos(hWnd_, nullptr, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
            }

            void CWindowWnd::SetIcon(UINT nRes) {
                HICON hIcon = (HICON)::LoadImage(ResourceManager::Get()->GetInstanceHandle(), MAKEINTRESOURCE(nRes), IMAGE_ICON,
                    (::GetSystemMetrics(SM_CXICON) + 15) & ~15, (::GetSystemMetrics(SM_CYICON) + 15) & ~15,	// 防止高DPI下图标模糊
                    LR_DEFAULTCOLOR);
                ASSERT(hIcon);
                ::SendMessage(hWnd_, WM_SETICON, (WPARAM)TRUE, (LPARAM)hIcon);

                hIcon = (HICON)::LoadImage(ResourceManager::Get()->GetInstanceHandle(), MAKEINTRESOURCE(nRes), IMAGE_ICON,
                    (::GetSystemMetrics(SM_CXICON) + 15) & ~15, (::GetSystemMetrics(SM_CYICON) + 15) & ~15,	// 防止高DPI下图标模糊
                    LR_DEFAULTCOLOR);
                ASSERT(hIcon);
                ::SendMessage(hWnd_, WM_SETICON, (WPARAM)FALSE, (LPARAM)hIcon);
            }

            bool CWindowWnd::RegisterWindowClass() {
                WNDCLASS wc = { 0 };
                wc.style = GetClassStyle();
                wc.cbClsExtra = 0;
                wc.cbWndExtra = 0;
                wc.hIcon = nullptr;
                wc.lpfnWndProc = CWindowWnd::__WndProc;
                wc.hInstance = ResourceManager::Get()->GetInstanceHandle();
                wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
                wc.hbrBackground = nullptr;
                wc.lpszMenuName = nullptr;
                wc.lpszClassName = GetWindowClassName();
                ATOM ret = ::RegisterClass(&wc);
                ASSERT(ret != 0 || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
                return ret != 0 || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
            }

            bool CWindowWnd::RegisterSuperclass() {
                // Get the class information from an existing
                // window so we can subclass it later on...
                WNDCLASSEX wc = { 0 };
                wc.cbSize = sizeof(WNDCLASSEX);
                if (!::GetClassInfoEx(nullptr, GetSuperClassName(), &wc)) {
                    if (!::GetClassInfoEx(ResourceManager::Get()->GetInstanceHandle(), GetSuperClassName(), &wc)) {
                        ASSERT(!"Unable to locate window class");
                        return nullptr;
                    }
                }
                oldWndProc_ = wc.lpfnWndProc;
                wc.lpfnWndProc = CWindowWnd::__ControlProc;
                wc.hInstance = ResourceManager::Get()->GetInstanceHandle();
                wc.lpszClassName = GetWindowClassName();
                ATOM ret = ::RegisterClassEx(&wc);
                ASSERT(ret != 0 || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
                return ret != 0 || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
            }

            LRESULT CALLBACK CWindowWnd::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
                CWindowWnd* pThis = nullptr;
                if (uMsg == WM_NCCREATE) {
                    LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
                    pThis = static_cast<CWindowWnd*>(lpcs->lpCreateParams);
                    pThis->hWnd_ = hWnd;
                    ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
                } else {
                    pThis = reinterpret_cast<CWindowWnd*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
                    if (uMsg == WM_NCDESTROY && pThis != nullptr) {
                        LRESULT lRes = ::CallWindowProc(pThis->oldWndProc_, hWnd, uMsg, wParam, lParam);
                        ::SetWindowLongPtr(pThis->hWnd_, GWLP_USERDATA, 0L);
                        if (pThis->subclassed_) pThis->Unsubclass();
                        pThis->hWnd_ = nullptr;
                        pThis->OnFinalMessage(hWnd);
                        return lRes;
                    }
                }
                if (uMsg == WM_PAINT) {
                    PAINTSTRUCT ps = { 0 };
                    ::BeginPaint(hWnd, &ps);
                    ::EndPaint(hWnd, &ps);
                    //return 0;
                }   
                if (pThis != nullptr) {
                    return pThis->HandleMessage(uMsg, wParam, lParam);
                } else {
                    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
                }
            }

            LRESULT CALLBACK CWindowWnd::__ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
                CWindowWnd* pThis = nullptr;
                if (uMsg == WM_NCCREATE) {
                    LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
                    pThis = static_cast<CWindowWnd*>(lpcs->lpCreateParams);
                    ::SetProp(hWnd, "WndX", (HANDLE)pThis);
                    pThis->hWnd_ = hWnd;
                } else {
                    pThis = reinterpret_cast<CWindowWnd*>(::GetProp(hWnd, "WndX"));
                    if (uMsg == WM_NCDESTROY && pThis != nullptr) {
                        LRESULT lRes = ::CallWindowProc(pThis->oldWndProc_, hWnd, uMsg, wParam, lParam);
                        if (pThis->subclassed_) pThis->Unsubclass();
                        ::SetProp(hWnd, "WndX", nullptr);
                        pThis->hWnd_ = nullptr;
                        pThis->OnFinalMessage(hWnd);
                        return lRes;
                    }
                }
                if (pThis != nullptr) {
                    return pThis->HandleMessage(uMsg, wParam, lParam);
                } else {
                    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
                }
            }

            LRESULT CWindowWnd::SendMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/) {
                ASSERT(::IsWindow(hWnd_));
                return ::SendMessage(hWnd_, uMsg, wParam, lParam);
            }

            LRESULT CWindowWnd::PostMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/) {
                ASSERT(::IsWindow(hWnd_));
                return ::PostMessage(hWnd_, uMsg, wParam, lParam);
            }

            void CWindowWnd::ResizeClient(int cx /*= -1*/, int cy /*= -1*/) {
                ASSERT(::IsWindow(hWnd_));
                RECT rc = { 0 };
                if (!::GetClientRect(hWnd_, &rc)) return;
                if (cx != -1) rc.right = cx;
                if (cy != -1) rc.bottom = cy;
                if (!::AdjustWindowRectEx(&rc, GetWindowStyle(hWnd_), (!(GetWindowStyle(hWnd_) & WS_CHILD) && (::GetMenu(hWnd_) != nullptr)), GetWindowExStyle(hWnd_))) return;
                ::SetWindowPos(hWnd_, nullptr, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
            }

            LRESULT CWindowWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
                return ::CallWindowProc(oldWndProc_, hWnd_, uMsg, wParam, lParam);
            }

            void CWindowWnd::OnFinalMessage(HWND /*hWnd*/) {}

        }
    }
}