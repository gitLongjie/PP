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
//    PPEditor::PPEditorApplication app;// (/*1720, 600, "ProjectorPlayer"*/);
//    if (!app.Initialize()) {
//        app.Uninitialize();
//        return -1;
//    }
//
//    int ret = app.RunLoop();
//    app.Uninitialize();
//    return ret;
//}

#include "Platforms/Windows/WinFrame.h"
#include "Platforms/Windows/MessageLoop.h"
#include "Platforms/Windows/Context.h"
#include "Platforms/Windows/ResourceManager.h"

#include "Window/Builder.h"

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

#define UI_CLASSSTYLE_DIALOG     (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)

class CFrameWnd : public Platforms::Windows::WinFrame {
public:
    CFrameWnd() {};
    const char* GetWindowClassName() const { return "UIFrame"; };
    UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
    void OnFinalMessage(HWND /*hWnd*/) {
        delete this;
    };

protected:
    const char* GetSkin() const override {
        return "skin.xml";
    }

    void Init() override {
    }
};


#define WNDSTYLE_FRAME      (WS_VISIBLE | WS_OVERLAPPEDWINDOW)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Platforms::Windows::WindowsMainLoopImpl impl;
  //  Platforms::Windows::Context::SetInstanceHandle(hInstance);
    Core::MainLoop mainLoop(&impl);
    Editor::PPEditorApplication app(mainLoop);
    if (!app.Initialize()) {
        return -1;
    }
    Platforms::Windows::ResourceManager::Get()->SetInstanceHandle(hInstance);

    Window::WindowSettings settings;
    settings.title = "PPEditor";
    PPRHI::DriverSettings driverSettings;
    Window::Window window(driverSettings, settings, std::make_unique<CFrameWnd>());
    //CFrameWnd* window = new CFrameWnd();
    //window->Create(NULL, "ProjectorPlayer", WNDSTYLE_FRAME, 0L, 0L, 0, 640, 480);
    //window->CenterWindow();
    //window->ShowWindow(true);
    //godot_hinstance = hInstance;

    int ret = app.RunLoop();
    app.Uninitialize();
    return ret;
}
