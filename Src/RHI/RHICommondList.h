#pragma once

#include <thread>
#include <memory>

#include "Core/CASQueue.h"

namespace PPEngine {
    namespace PPRHI {
        class RHICommonList : public std::enable_shared_from_this<RHICommonList> {
        public:
            using Ptr = std::shared_ptr<RHICommonList>;
            static Ptr Create();

        public:
            ~RHICommonList();

            bool Start();
            void Stop();

            void Push(std::shared_ptr<class RHICommond> commond);


        private:
            explicit RHICommonList() noexcept;

        private:
            std::unique_ptr<std::thread> thread_;
            bool isRunning_{ false };

            Core::CASQueue<std::shared_ptr<class RHICommond>> queue_;
        };
    }
}
