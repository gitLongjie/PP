#include "RHI/RHICommondList.h"

#include "Core/Logger.h"
#include "RHI/RHICommond.h"
#include "RHI/RHI.h"

namespace PPRHI {

    RHICommonList::RHICommonList() noexcept
        : queue_(1024) {
           

    }

    RHICommonList::~RHICommonList() {
           
    }

    RHICommonList::Ptr RHICommonList::Create() {
        RHICommonList::Ptr Self(new RHICommonList());
        return Self;
    }

    bool RHICommonList::Start() {
        if (isRunning_) {
            return true;
        }

        std::weak_ptr<RHICommonList> weakPtr = shared_from_this();
        auto taskProc = [this, weakPtr]() {
            auto ptr = weakPtr.lock();
            if (!ptr) {
                LOGE("RHICommonList is nullptr");
                return;
            }

            while (isRunning_) {
                if (ptr->queue_.empty()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    continue;
                }

                auto task = ptr->queue_.front();
                if (nullptr == task) {
                    continue;
                }

                (*task)->Execute();
                ptr->queue_.pop();
            }

            LOGI("render commond list count({%1})", ptr->queue_.size());
        };
        isRunning_ = true;
        thread_ = std::make_unique<std::thread>(taskProc);
        return true;
    }

    void RHICommonList::Stop() {
        isRunning_ = false;

        if (thread_ && thread_->joinable()) {
            thread_->join();
            thread_.reset();
        }
    }

    void RHICommonList::Push(std::shared_ptr<class RHICommond> commond) {
        queue_.emplace(std::move(commond));
    }
}
