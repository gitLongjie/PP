#include "Platforms/Windows/MessageLoop.h"

#include "Core/Logger.h"

namespace Platforms {
    namespace Windows {
        int WindowsMainLoopImpl::Run() {
            MSG msg = { 0 };
            while (WM_QUIT != msg.message) {
                if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                    ::TranslateMessage(&msg);
                    ::DispatchMessage(&msg);
                }
            }
            return 0;
        }
    }
}
