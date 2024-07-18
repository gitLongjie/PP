#pragma once

#include <mutex>
#include <thread>

#include "Core/Constant.h"

namespace Core {
    class ThreadChecker {
        NON_COPYABLE(ThreadChecker)

    public:
        ThreadChecker();
        ~ThreadChecker();

        bool CalledOnValideThread(void) const;
        void DetachFromThread(void);

    private:
        mutable std::mutex mutex_;
        mutable std::thread::id threadId_;
    };
}

