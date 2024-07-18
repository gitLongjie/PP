#include "Window/ScrollBar.h"

#include "Core/StringUtil.h"
#include "Window/Context.h"

namespace Window {
    ScrollBar::ScrollBar() {
    }

    ScrollBar::~ScrollBar() {
    }

    void ScrollBar::SetScrollRange(int32_t range) {
        if (range_ == range) {
            return;
        }

        range_ = range;
        if (range_ < 0) {
            range_ = 0;
        }

        if (scrollPos_ > range_) {
            scrollPos_ = range_;
        }
        FixRect(rect_);
    }

    void ScrollBar::SetScrollPos(int32_t pos) {
        if (scrollPos_ == pos) {
            return;
        }

        scrollPos_ = pos;
        if (scrollPos_ < 0) {
            scrollPos_ = 0;
        }
        if (scrollPos_ > range_) {
            scrollPos_ = range_;
        }
        FixRect(rect_);
    }
}
