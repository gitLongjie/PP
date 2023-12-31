#pragma once

#include <Windows.h>
#include <stdint.h>

namespace PPEngine {
    namespace Platforms {
        namespace Windows {
            class WindowRender {
            public:
                static void DrawLine(HDC hDC, const POINT& start, const POINT& end, int nSize, DWORD dwPenColor, int nStyle = PS_SOLID);
                static void DrawRect(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor);
                static void DrawRoundRect(HDC hDC, const RECT& rc, int width, int height, int nSize, DWORD dwPenColor);
                static void DrawColor(HDC hdc, const RECT& rc, unsigned long color);
                static void DrawGradient(HDC hDC, const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps);
                static void DrawImage(HDC hdc, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint,
                    const RECT& rcBmpPart, const RECT& rcScale9, bool alpha, uint8_t fade = 255,
                    bool hole = false, bool xtiled = false, bool ytiled = false);
            };
        }
    }
}