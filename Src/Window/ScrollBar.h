#pragma once

#include "Window/Control.h"

namespace PPEngine {
    namespace Window {
        class ScrollBar : public Control {
        public:
            ScrollBar();
            ~ScrollBar() override;

            void SetScrollRange(int32_t range);
            int32_t GetScrollRange() const { return range_; }
            void SetScrollPos(int32_t pos);
            int32_t GetScrollPos() const { return scrollPos_; }

        private:
            bool horizontal_ { false };
            int32_t range_{ 0 };
            int32_t scrollPos_{ 0 };
            int32_t lineSize_{ 0 };
        };
    }
}
