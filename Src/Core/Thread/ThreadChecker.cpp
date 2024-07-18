#include "Core/Thread/ThreadChecker.h"

namespace Core {
    ThreadChecker::ThreadChecker(void) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (std::thread::id() == threadId_) {
            threadId_ = std::this_thread::get_id();
        }
    }

    ThreadChecker::~ThreadChecker(void) {

    }

    bool ThreadChecker::CalledOnValideThread(void) const {
        std::lock_guard<std::mutex> lock(mutex_);
        return threadId_ == std::this_thread::get_id();
    }

    void ThreadChecker::DetachFromThread(void) {
        std::lock_guard<std::mutex> lock(mutex_);
        threadId_ = std::thread::id();
    }
}
