#pragma once

#include <atomic>
#include <thread>

namespace Core {
    class SpinLock {
    public:
        explicit SpinLock() noexcept = default;
        ~SpinLock() noexcept = default;

        void lock() {
            while (flag_.test_and_set(std::memory_order_acquire)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(0));
            }
        }

        void unlock() {
            flag_.clear(std::memory_order_release);
        }

    private:
        std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
    };
}
