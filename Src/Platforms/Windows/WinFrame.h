#pragma once

#include "Platforms/Windows/WinWindow.h"
#include "Platforms/Windows/Context.h"

namespace Platforms {
    namespace Windows {
        class WinFrame : public CWindowWnd {
        public:
            WinFrame() = default;
            ~WinFrame() override = default;

            LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
            LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
            LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
            LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
            LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
            LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
            LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
            LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
            LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
            LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
            LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

        protected:
            virtual const char* GetSkin() const { return ""; }
            virtual void Init() {}

        public:
            Platforms::Windows::Context context_;
        };
    }
}
