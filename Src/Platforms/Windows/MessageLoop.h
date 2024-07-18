#pragma once

#include "Core/MainLoop.h"

namespace Platforms {
    namespace Windows {
        class WindowsMainLoopImpl : public Core::MainLoopImpl {
        public:
            WindowsMainLoopImpl() = default;
            ~WindowsMainLoopImpl() = default;

            int Run() override;
        };
    }
}