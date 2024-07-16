#pragma once 

#include <Windows.h>
#include "Window/WindowImpl.h"

namespace PPEngine {
    namespace Platforms {
        namespace Windows {
#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif

            class CWindowWnd : public PPEngine::Window::WindowImpl {
            public:
                CWindowWnd();

                HWND GetHWND() const;
                operator HWND() const;

                bool RegisterWindowClass();
                bool RegisterSuperclass();

                HWND Create(HWND hwndParent, const char* pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
                HWND Create(HWND hwndParent, const char* pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
                HWND CreateDuiWindow(HWND hwndParent, const char* pstrWindowName, DWORD dwStyle = 0, DWORD dwExStyle = 0);
                bool Create(const PPEngine::Window::WindowSettings& settings, PPRHI::DriverSettings& driverSetting) override;
                HWND Subclass(HWND hWnd);
                void Unsubclass();
                void ShowWindow(bool bShow = true, bool bTakeFocus = true);
                UINT ShowModal();
                void Close(UINT nRet = IDOK);
                void CenterWindow();	// 居中，支持扩展屏幕
                void SetIcon(UINT nRes);

                LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
                LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
                void ResizeClient(int cx = -1, int cy = -1);

                bool ShouldClose() const { return false; }
                void MakeCurrentContext() const { }
                void SwapBuffers() const {}

                //Core::WindowHandle GetWindowHandle() const { return nullptr; }


            protected:
                virtual const char* GetWindowClassName() const = 0;
                virtual const char* GetSuperClassName() const;
                virtual UINT GetClassStyle() const;

                virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
                virtual void OnFinalMessage(HWND hWnd);

                static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
                static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

            protected:
                HWND hWnd_{ nullptr };
                WNDPROC oldWndProc_{ nullptr };
                bool subclassed_{ false };

                IMPLEMENT_OBJECT_REFCOUN(CWindowWnd)
            };
        }
    }
}
