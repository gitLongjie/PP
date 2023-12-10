#pragma once

#include <Windows.h>
#include <stdint.h>

namespace PPEngine {
    namespace Platforms {
        namespace Windows {
            class WindowRender {
            public:
                static void DrawColor(HDC hdc, const RECT& rc, unsigned long color);
                static void DrawImage(HDC hdc, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint,
                    const RECT& rcBmpPart, const RECT& rcScale9, bool alpha, uint8_t fade = 255,
                    bool hole = false, bool xtiled = false, bool ytiled = false);
            };
        }
    }
}