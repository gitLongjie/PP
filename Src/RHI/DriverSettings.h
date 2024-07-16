#pragma once

#include <stdint.h>

#include "Core/Platform.h"
#include "Core/Constant.h"

namespace PPEngine {
    namespace PPRHI {
        struct DriverSettings {
            bool debugProfile = false;
            bool forwardCompatibility = false;
            uint8_t contextMajorVersion = 4;
            uint8_t contextMinorVersion = 1;
            uint8_t samples = 4;
          //  Core::WindowHandle windowHandle = nullptr;
        };
    }
}