#pragma once

#include <stdint.h>
#include <string>

namespace Window {
    struct WindowSettings {
        std::string title;
        uint32_t width = 800;
        uint32_t height = 600;
        bool fullscreen;
        bool decorated = true;
        bool resizable = true;
        bool focused = true;
        bool maximized = false;
        bool floating = false;
        bool visible = true;
        bool autoIconify = true;
        uint32_t samples = 4;
    };
}
