#pragma once

#include "Core/Constant.h"

namespace PPEngine {
    namespace Window {

        // Flags for FindControl()
        constexpr uint32 UIFIND_ALL = 0x00000000;
        constexpr uint32 UIFIND_VISIBLE = 0x00000001;
        constexpr uint32 UIFIND_ENABLED = 0x00000002;
        constexpr uint32 UIFIND_HITTEST = 0x00000004;
        constexpr uint32 UIFIND_TOP_FIRST = 0x00000008;
        constexpr uint32 UIFIND_ME_FIRST = 0x80000000;
    }
}

