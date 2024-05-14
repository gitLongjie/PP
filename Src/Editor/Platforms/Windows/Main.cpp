#include <iostream>

#if defined(_WIN32) || defined(_WIN64)

#include <Windows.h>

#endif // defined(_WIN32) || defined(_WIN64)

#include <iostream>
#include <cmath>

#include "Editor/PPEditorApplication.h"
#include "Core/MainLoop.h"
#include "Core/Math/Rect.h"
#include "Window/Constent.h"
#include "Window/Control.h"
#include "RHI/DriverSettings.h"

//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

//int main(int argc, char* argv[]) {
//
//    PPEngine::PPEditor::PPEditorApplication app;// (/*1720, 600, "ProjectorPlayer"*/);
//    if (!app.Initialize()) {
//        app.Uninitialize();
//        return -1;
//    }
//
//    int ret = app.RunLoop();
//    app.Uninitialize();
//    return ret;
//}

#include "Platforms/Windows/WinWindow.h"
#include "Platforms/Windows/MessageLoop.h"
#include "Platforms/Windows/Context.h"
#include "Platforms/Windows/ResourceManager.h"

#include "Window/Builder.h"

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

#define UI_CLASSSTYLE_DIALOG     (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)

class CFrameWnd : public PPEngine::Platforms::Windows::CWindowWnd {
public:
    CFrameWnd() {};
    const char* GetWindowClassName() const { return "UIFrame"; };
    UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
    void OnFinalMessage(HWND /*hWnd*/) {
        delete this;
    };

    void Init() {}

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
        styleValue &= ~WS_CAPTION;
        ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

       // m_pm.Init(m_hWnd);
        context_.Init(hWnd_);
        PPEngine::Window::Builder builder;
        builder.Create("skin.xml", &context_);
        Init();
        return 0;
    }

    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        ::PostQuitMessage(0L);

        bHandled = FALSE;
        return 0;
    }

    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        if (::IsIconic(*this)) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
    }

    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        return 0;
    }

    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        return 0;
    }

    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        POINT pt = { GET_X_LPARAM(lParam) , GET_Y_LPARAM(lParam) };
        ::ScreenToClient(*this, &pt);

        RECT rcClient;
        ::GetClientRect(*this, &rcClient);
        const PPEngine::Core::Math::Rect& caption = context_.GetCaptionRect();
        RECT rcCaption = { static_cast<long>(caption.GetMin().x), static_cast<long>(caption.GetMin().y),
            static_cast<long>(caption.GetMax().x), static_cast<long>(caption.GetMax().y) };
        if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
            && pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
            PPEngine::Core::Math::Point2d point(pt.x, pt.y);
            const PPEngine::Window::Control* control = context_.FindControl(point, PPEngine::Window::UIFIND_VISIBLE |
                PPEngine::Window::UIFIND_HITTEST | PPEngine::Window::UIFIND_TOP_FIRST);
            if (nullptr == control || 0 != strcmp("Button", control->GetClass())) {
                return HTCAPTION;
            }
        }

        return HTCLIENT;
    }

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        const PPEngine::Core::Math::Size& size = context_.GetRoundCorner();
         if (!::IsIconic(*this) ) {
             RECT rcWnd;
             ::GetWindowRect(*this, &rcWnd);
             ::OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);
             rcWnd.right++; rcWnd.bottom++;
             HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, size.x, size.y);
             ::SetWindowRgn(*this, hRgn, TRUE);
             ::DeleteObject(hRgn);
         }

         bHandled = FALSE;
        return 0;
    }

    LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        RECT rcWork = oMonitor.rcWork;
        ::OffsetRect(&rcWork, -oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
        lpMMI->ptMaxPosition.x = rcWork.left;
        lpMMI->ptMaxPosition.y = rcWork.top;
        lpMMI->ptMaxSize.x = rcWork.right;
        lpMMI->ptMaxSize.y = rcWork.bottom;

        bHandled = FALSE;
        return 0;
    }

    LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        // 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
        if (wParam == SC_CLOSE) {
            ::PostQuitMessage(0L);
            bHandled = TRUE;
            return 0;
        }
        BOOL bZoomed = ::IsZoomed(*this);
        LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
        /*if (::IsZoomed(*this) != bZoomed) {
            if (!bZoomed) {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
                if (pControl) pControl->SetVisible(false);
                pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
                if (pControl) pControl->SetVisible(true);
            } else {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
                if (pControl) pControl->SetVisible(true);
                pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
                if (pControl) pControl->SetVisible(false);
            }
        }*/
        return lRes;
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch (uMsg) {
        case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
        case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
        case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
        case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
        case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
        case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
        case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
        case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
        case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
        case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
        default:
            bHandled = FALSE;
        }
        if (bHandled) return lRes;
        if (context_.HandleMessage(uMsg, wParam, lParam, lRes)) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }

public:
    PPEngine::Platforms::Windows::Context context_;
    //  CPaintManagerUI m_pm;
};


#define WNDSTYLE_FRAME      (WS_VISIBLE | WS_OVERLAPPEDWINDOW)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    PPEngine::Platforms::Windows::WindowsMainLoopImpl impl;
  //  PPEngine::Platforms::Windows::Context::SetInstanceHandle(hInstance);
    PPEngine::Core::MainLoop mainLoop(&impl);
    PPEngine::PPEditor::PPEditorApplication app(mainLoop);
    if (!app.Initialize()) {
        return -1;
    }
    PPEngine::Platforms::Windows::ResourceManager::Get()->SetInstanceHandle(hInstance);

    PPEngine::Window::WindowSettings settings;
    settings.title = "PPEditor";
    PPEngine::PPRHI::DriverSettings driverSettings;
    PPEngine::Window::Window window(driverSettings, settings, std::make_unique<CFrameWnd>());
    //CFrameWnd* window = new CFrameWnd();
    //window->Create(NULL, "ProjectorPlayer", WNDSTYLE_FRAME, 0L, 0L, 0, 640, 480);
    //window->CenterWindow();
    //window->ShowWindow(true);
    //godot_hinstance = hInstance;

    int ret = app.RunLoop();
    app.Uninitialize();
    return ret;
}
