#include "Core/MainLoop.h"

#include <assert.h>
#include "Core/Platform.h"

namespace Core {
    MainLoop::MainLoop(MainLoopImpl* impl)
        : impl_(impl) {
    }


    int MainLoop::Run() {
        assert(nullptr != impl_ && "MainLoopImpl is nullptr");

        return impl_->Run();
    }
}
