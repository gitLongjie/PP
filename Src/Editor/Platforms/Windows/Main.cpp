#include <iostream>

#if defined(_WIN32) || defined(_WIN64)

#include <Windows.h>

#endif // defined(_WIN32) || defined(_WIN64)

#include <iostream>
#include <cmath>

#include "Editor/PPEditorApplication.h"
#include "Core/MainLoop.h"
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

#include "Window/Builder.h"

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

#define UI_CLASSSTYLE_DIALOG     (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)

class CFrameWnd : public PPEngine::Platforms::Windows::CWindowWnd {
public:
    CFrameWnd() {};
    const char* GetWindowClassName() const { return "UIFrame"; };
    UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
    void OnFinalMessage(HWND /*hWnd*/) {  };

    void Init() {}

    //void Notify(TNotifyUI& msg) {
    //    if (msg.sType == _T("click")) {
    //        if (msg.pSender->GetName() == _T("closebtn") || msg.pSender->GetName() == _T("closebtn2")) {
    //            PostQuitMessage(0);
    //            return;
    //        }
    //    } else if (msg.sType == _T("windowinit")) {	// name = flashUI 控件在UI布局中默认注释掉，查看效果需修改xml
    //        CFlashUI* pFlashUI = static_cast<CFlashUI*>(m_pm.FindControl(_T("flashUI")));
    //        if (pFlashUI) {
    //            pFlashUI->m_pFlash->put_WMode(_bstr_t(_T("Transparent")));	// FlashUI没有实现特定接口，需要完善才能支持
    //            pFlashUI->m_pFlash->put_Movie(_bstr_t(CPaintManagerUI::GetInstancePath() + _T("\\skin\\FlashRes\\test.swf")));
    //            pFlashUI->m_pFlash->DisableLocalSecurity();
    //            pFlashUI->m_pFlash->put_AllowScriptAccess(L"always");

    //            BSTR request, response;
    //            request = SysAllocString(L"<invoke name=\"setButtonText\" returntype=\"xml\"><arguments><string>Click me!</string></arguments></invoke>");
    //            response = SysAllocString(L"");
    //            pFlashUI->m_pFlash->CallFunction(request, &response);
    //            SysFreeString(request);
    //            SysFreeString(response);
    //        }
    //    } else if (msg.sType == _T("showactivex")) {
    //        if (msg.pSender->GetName() == _T("flashActiveX")) {
    //            IShockwaveFlash* pFlash = NULL;
    //            CActiveXUI* pActiveX = static_cast<CActiveXUI*>(msg.pSender);
    //            pActiveX->GetControl(IID_IUnknown, (void**)&pFlash);
    //            if (pFlash != NULL) {
    //                pFlash->put_WMode(_bstr_t(_T("Transparent")));
    //                pFlash->put_Movie(_bstr_t(CPaintManagerUI::GetInstancePath() + _T("\\skin\\FlashRes\\test.swf")));
    //                pFlash->DisableLocalSecurity();
    //                pFlash->put_AllowScriptAccess(L"always");

    //                BSTR request, response;
    //                request = SysAllocString(L"<invoke name=\"setButtonText\" returntype=\"xml\"><arguments><string>Click me!</string></arguments></invoke>");
    //                response = SysAllocString(L"");
    //                pFlash->CallFunction(request, &response);
    //                SysFreeString(request);
    //                SysFreeString(response);
    //            }
    //        }
    //    }
    //}

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
        styleValue &= ~WS_CAPTION;
        ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

       // m_pm.Init(m_hWnd);
        context_.Init(hWnd_);
        PPEngine::Window::Builder builder;
        builder.Create("skin.xml", &context_);
        //ASSERT(pRoot && "Failed to parse XML");
     //   context_.Attach(pRoot);
        // m_pm.AddNotifier(this);

        Init();
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

        RECT rcCaption = { 0, 0, 0, 40 }; // m_pm.GetCaptionRect();
        if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
            && pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
            //CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
            //if (pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0)
                return HTCAPTION;
        }

        return HTCLIENT;
    }

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {

         if (!::IsIconic(*this) ) {
             RECT rcWnd;
             ::GetWindowRect(*this, &rcWnd);
             ::OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);
             rcWnd.right++; rcWnd.bottom++;
             HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, 3, 2);
             ::SetWindowRgn(*this, hRgn, TRUE);
             ::DeleteObject(hRgn);
         }

         bHandled = FALSE;
        return 0;
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch (uMsg) {
        case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
        case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
        case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
        case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
        case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
        case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
        case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
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
    PPEngine::Platforms::Windows::Context::SetInstanceHandle(hInstance);
    PPEngine::Core::MainLoop mainLoop(&impl);
    PPEngine::PPEditor::PPEditorApplication app(mainLoop);
    if (!app.Initialize()) {
        return -1;
    }

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
