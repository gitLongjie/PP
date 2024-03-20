#include "Platforms/Windows/WindowRender.h"

#include <assert.h>

#include "Platforms/Windows/Context.h"

namespace PPEngine {
    namespace Platforms {
        namespace Windows {

            static COLORREF PixelAlpha(COLORREF clrSrc, double src_darken, COLORREF clrDest, double dest_darken) {
                return RGB(GetRValue(clrSrc) * src_darken + GetRValue(clrDest) * dest_darken,
                    GetGValue(clrSrc) * src_darken + GetGValue(clrDest) * dest_darken,
                    GetBValue(clrSrc) * src_darken + GetBValue(clrDest) * dest_darken);
            }

            static BOOL WINAPI AlphaBitBlt(HDC hdc, int nDestX, int nDestY, int dwWidth, int dwHeight, HDC hSrcDC,
                int nSrcX, int nSrcY, int wSrc, int hSrc, BLENDFUNCTION ftn)
            {
                HDC hTempDC = ::CreateCompatibleDC(hdc);
                if (NULL == hTempDC)
                    return FALSE;

                //Creates Source DIB
                LPBITMAPINFO lpbiSrc = NULL;
                // Fill in the BITMAPINFOHEADER
                lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
                if (lpbiSrc == NULL) {
                    ::DeleteDC(hTempDC);
                    return FALSE;
                }
                lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                lpbiSrc->bmiHeader.biWidth = dwWidth;
                lpbiSrc->bmiHeader.biHeight = dwHeight;
                lpbiSrc->bmiHeader.biPlanes = 1;
                lpbiSrc->bmiHeader.biBitCount = 32;
                lpbiSrc->bmiHeader.biCompression = BI_RGB;
                lpbiSrc->bmiHeader.biSizeImage = dwWidth * dwHeight;
                lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
                lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
                lpbiSrc->bmiHeader.biClrUsed = 0;
                lpbiSrc->bmiHeader.biClrImportant = 0;

                COLORREF* pSrcBits = NULL;
                HBITMAP hSrcDib = CreateDIBSection(
                    hSrcDC, lpbiSrc, DIB_RGB_COLORS, (void**)&pSrcBits,
                    NULL, NULL);

                if ((NULL == hSrcDib) || (NULL == pSrcBits)) {
                    delete[] lpbiSrc;
                    ::DeleteDC(hTempDC);
                    return FALSE;
                }

                HBITMAP hOldTempBmp = (HBITMAP)::SelectObject(hTempDC, hSrcDib);
                ::StretchBlt(hTempDC, 0, 0, dwWidth, dwHeight, hSrcDC, nSrcX, nSrcY, wSrc, hSrc, SRCCOPY);
                ::SelectObject(hTempDC, hOldTempBmp);

                //Creates Destination DIB
                LPBITMAPINFO lpbiDest = NULL;
                // Fill in the BITMAPINFOHEADER
                lpbiDest = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
                if (lpbiDest == NULL) {
                    delete[] lpbiSrc;
                    ::DeleteObject(hSrcDib);
                    ::DeleteDC(hTempDC);
                    return FALSE;
                }

                lpbiDest->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                lpbiDest->bmiHeader.biWidth = dwWidth;
                lpbiDest->bmiHeader.biHeight = dwHeight;
                lpbiDest->bmiHeader.biPlanes = 1;
                lpbiDest->bmiHeader.biBitCount = 32;
                lpbiDest->bmiHeader.biCompression = BI_RGB;
                lpbiDest->bmiHeader.biSizeImage = dwWidth * dwHeight;
                lpbiDest->bmiHeader.biXPelsPerMeter = 0;
                lpbiDest->bmiHeader.biYPelsPerMeter = 0;
                lpbiDest->bmiHeader.biClrUsed = 0;
                lpbiDest->bmiHeader.biClrImportant = 0;

                COLORREF* pDestBits = NULL;
                HBITMAP hDestDib = CreateDIBSection(
                    hdc, lpbiDest, DIB_RGB_COLORS, (void**)&pDestBits,
                    NULL, NULL);

                if ((NULL == hDestDib) || (NULL == pDestBits)) {
                    delete[] lpbiSrc;
                    ::DeleteObject(hSrcDib);
                    ::DeleteDC(hTempDC);
                    return FALSE;
                }

                ::SelectObject(hTempDC, hDestDib);
                ::BitBlt(hTempDC, 0, 0, dwWidth, dwHeight, hdc, nDestX, nDestY, SRCCOPY);
                ::SelectObject(hTempDC, hOldTempBmp);

                double src_darken;
                BYTE nAlpha;

                for (int pixel = 0; pixel < dwWidth * dwHeight; pixel++, pSrcBits++, pDestBits++) {
                    nAlpha = LOBYTE(*pSrcBits >> 24);
                    src_darken = (double)(nAlpha * ftn.SourceConstantAlpha) / 255.0 / 255.0;
                    if (src_darken < 0.0) src_darken = 0.0;
                    *pDestBits = PixelAlpha(*pSrcBits, src_darken, *pDestBits, 1.0 - src_darken);
                } //for

                ::SelectObject(hTempDC, hDestDib);
                ::BitBlt(hdc, nDestX, nDestY, dwWidth, dwHeight, hTempDC, 0, 0, SRCCOPY);
                ::SelectObject(hTempDC, hOldTempBmp);

                delete[] lpbiDest;
                ::DeleteObject(hDestDib);

                delete[] lpbiSrc;
                ::DeleteObject(hSrcDib);

                ::DeleteDC(hTempDC);
                return TRUE;
            }

            void WindowRender::DrawLine(HDC hDC, const POINT& start, const POINT& end, int nSize, DWORD dwPenColor, int nStyle) {
               assert(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);

                LOGPEN lg;
                lg.lopnColor = RGB(GetBValue(dwPenColor), GetGValue(dwPenColor), GetRValue(dwPenColor));
                lg.lopnStyle = nStyle;
                lg.lopnWidth.x = nSize;
                HPEN hPen = CreatePenIndirect(&lg);
                HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
                POINT ptTemp = { 0 };
                ::MoveToEx(hDC, start.x, start.y, &ptTemp);
                ::LineTo(hDC, end.x, end.y);
                ::SelectObject(hDC, hOldPen);
                ::DeleteObject(hPen);
            }

            void WindowRender::DrawRect(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor) {
                assert(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);
                HPEN hPen = ::CreatePen(PS_SOLID | PS_INSIDEFRAME, nSize, RGB(GetBValue(dwPenColor), GetGValue(dwPenColor), GetRValue(dwPenColor)));
                HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
                ::SelectObject(hDC, ::GetStockObject(HOLLOW_BRUSH));
                ::Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
                ::SelectObject(hDC, hOldPen);
                ::DeleteObject(hPen);
            }

            void WindowRender::DrawRoundRect(HDC hDC, const RECT& rc, int width, int height, int nSize, DWORD dwPenColor) {
                assert(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);
                HPEN hPen = ::CreatePen(PS_SOLID | PS_INSIDEFRAME, nSize, RGB(GetBValue(dwPenColor), GetGValue(dwPenColor), GetRValue(dwPenColor)));
                HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
                ::SelectObject(hDC, ::GetStockObject(HOLLOW_BRUSH));
                ::RoundRect(hDC, rc.left, rc.top, rc.right, rc.bottom, width, height);
                ::SelectObject(hDC, hOldPen);
                ::DeleteObject(hPen);
            }

            void WindowRender::DrawColor(HDC hdc, const RECT& rc, unsigned long color) {
                if (color <= 0x00FFFFFF) return;
                if (color >= 0xFF000000) {
                    ::SetBkColor(hdc, RGB(GetBValue(color), GetGValue(color), GetRValue(color)));
                    ::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
                } else {
                    // Create a new 32bpp bitmap with room for an alpha channel
                    BITMAPINFO bmi = { 0 };
                    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                    bmi.bmiHeader.biWidth = 1;
                    bmi.bmiHeader.biHeight = 1;
                    bmi.bmiHeader.biPlanes = 1;
                    bmi.bmiHeader.biBitCount = 32;
                    bmi.bmiHeader.biCompression = BI_RGB;
                    bmi.bmiHeader.biSizeImage = sizeof(unsigned long);
                    LPDWORD pDest = NULL;
                    HBITMAP hBitmap = ::CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (LPVOID*)&pDest, NULL, 0);
                    if (!hBitmap) return;

                    *pDest = color;

                    RECT rcBmpPart = { 0, 0, 1, 1 };
                    RECT rcCorners = { 0 };
                    DrawImage(hdc, hBitmap, rc, rc, rcBmpPart, rcCorners, true, 255);
                    ::DeleteObject(hBitmap);
                }
            }

            void WindowRender::DrawGradient(HDC hDC, const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps) {
                typedef BOOL(WINAPI* LPALPHABLEND)(HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION);
                static LPALPHABLEND lpAlphaBlend = (LPALPHABLEND) ::GetProcAddress(::GetModuleHandle("msimg32.dll"), "AlphaBlend");
                if (lpAlphaBlend == NULL) lpAlphaBlend = AlphaBitBlt;
                typedef BOOL(WINAPI* PGradientFill)(HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG);
                static PGradientFill lpGradientFill = (PGradientFill) ::GetProcAddress(::GetModuleHandle("msimg32.dll"), "GradientFill");

                BYTE bAlpha = (BYTE)(((dwFirst >> 24) + (dwSecond >> 24)) >> 1);
                if (bAlpha == 0) return;
                int cx = rc.right - rc.left;
                int cy = rc.bottom - rc.top;
                RECT rcPaint = rc;
                HDC hPaintDC = hDC;
                HBITMAP hPaintBitmap = NULL;
                HBITMAP hOldPaintBitmap = NULL;
                if (bAlpha < 255) {
                    rcPaint.left = rcPaint.top = 0;
                    rcPaint.right = cx;
                    rcPaint.bottom = cy;
                    hPaintDC = ::CreateCompatibleDC(hDC);
                    hPaintBitmap = ::CreateCompatibleBitmap(hDC, cx, cy);
                    assert(hPaintDC);
                    assert(hPaintBitmap);
                    hOldPaintBitmap = (HBITMAP) ::SelectObject(hPaintDC, hPaintBitmap);
                }
                if (lpGradientFill != NULL) {
                    TRIVERTEX triv[2] =
                    {
                        { rcPaint.left, rcPaint.top, GetBValue(dwFirst) << 8, GetGValue(dwFirst) << 8, GetRValue(dwFirst) << 8, 0xFF00 },
                        { rcPaint.right, rcPaint.bottom, GetBValue(dwSecond) << 8, GetGValue(dwSecond) << 8, GetRValue(dwSecond) << 8, 0xFF00 }
                    };
                    GRADIENT_RECT grc = { 0, 1 };
                    lpGradientFill(hPaintDC, triv, 2, &grc, 1, bVertical ? GRADIENT_FILL_RECT_V : GRADIENT_FILL_RECT_H);
                } else {
                    // Determine how many shades
                    int nShift = 1;
                    if (nSteps >= 64) nShift = 6;
                    else if (nSteps >= 32) nShift = 5;
                    else if (nSteps >= 16) nShift = 4;
                    else if (nSteps >= 8) nShift = 3;
                    else if (nSteps >= 4) nShift = 2;
                    int nLines = 1 << nShift;
                    for (int i = 0; i < nLines; i++) {
                        // Do a little alpha blending
                        BYTE bR = (BYTE)((GetBValue(dwSecond) * (nLines - i) + GetBValue(dwFirst) * i) >> nShift);
                        BYTE bG = (BYTE)((GetGValue(dwSecond) * (nLines - i) + GetGValue(dwFirst) * i) >> nShift);
                        BYTE bB = (BYTE)((GetRValue(dwSecond) * (nLines - i) + GetRValue(dwFirst) * i) >> nShift);
                        // ... then paint with the resulting color
                        HBRUSH hBrush = ::CreateSolidBrush(RGB(bR, bG, bB));
                        RECT r2 = rcPaint;
                        if (bVertical) {
                            r2.bottom = rc.bottom - ((i * (rc.bottom - rc.top)) >> nShift);
                            r2.top = rc.bottom - (((i + 1) * (rc.bottom - rc.top)) >> nShift);
                            if ((r2.bottom - r2.top) > 0) ::FillRect(hDC, &r2, hBrush);
                        } else {
                            r2.left = rc.right - (((i + 1) * (rc.right - rc.left)) >> nShift);
                            r2.right = rc.right - ((i * (rc.right - rc.left)) >> nShift);
                            if ((r2.right - r2.left) > 0) ::FillRect(hPaintDC, &r2, hBrush);
                        }
                        ::DeleteObject(hBrush);
                    }
                }
                if (bAlpha < 255) {
                    BLENDFUNCTION bf = { AC_SRC_OVER, 0, bAlpha, AC_SRC_ALPHA };
                    lpAlphaBlend(hDC, rc.left, rc.top, cx, cy, hPaintDC, 0, 0, cx, cy, bf);
                    ::SelectObject(hPaintDC, hOldPaintBitmap);
                    ::DeleteObject(hPaintBitmap);
                    ::DeleteDC(hPaintDC);
                }
            }

            bool WindowRender::DrawImage(HDC hdc, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint,
                const RECT& rcBmpPart, const RECT& rcScale9, bool alpha, uint8 fade, bool hole, bool xtiled, bool ytiled) {
                typedef BOOL(WINAPI* LPALPHABLEND)(HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION);
                static LPALPHABLEND lpAlphaBlend = (LPALPHABLEND) ::GetProcAddress(::GetModuleHandle(TEXT("msimg32.dll")), "AlphaBlend");

                if (lpAlphaBlend == NULL) lpAlphaBlend = AlphaBitBlt;
                if (hBitmap == NULL) return false;

                HDC hCloneDC = ::CreateCompatibleDC(hdc);
                HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hCloneDC, hBitmap);
                ::SetStretchBltMode(hdc, COLORONCOLOR);

                RECT rcTemp = { 0 };
                RECT rcDest = { 0 };
                if (lpAlphaBlend && (alpha || fade < 255)) {
                    BLENDFUNCTION bf = { AC_SRC_OVER, 0, fade, AC_SRC_ALPHA };
                    // middle
                    if (!hole) {
                        rcDest.left = rc.left + rcScale9.left;
                        rcDest.top = rc.top + rcScale9.top;
                        rcDest.right = rc.right - rc.left - rcScale9.left - rcScale9.right;
                        rcDest.bottom = rc.bottom - rc.top - rcScale9.top - rcScale9.bottom;
                        rcDest.right += rcDest.left;
                        rcDest.bottom += rcDest.top;
                        if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                            if (!xtiled && !ytiled) {
                                rcDest.right -= rcDest.left;
                                rcDest.bottom -= rcDest.top;
                                lpAlphaBlend(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                    rcBmpPart.left + rcScale9.left, rcBmpPart.top + rcScale9.top, \
                                    rcBmpPart.right - rcBmpPart.left - rcScale9.left - rcScale9.right, \
                                    rcBmpPart.bottom - rcBmpPart.top - rcScale9.top - rcScale9.bottom, bf);
                            } else if (xtiled && ytiled) {
                                LONG lWidth = rcBmpPart.right - rcBmpPart.left - rcScale9.left - rcScale9.right;
                                LONG lHeight = rcBmpPart.bottom - rcBmpPart.top - rcScale9.top - rcScale9.bottom;
                                int iTimesX = (rcDest.right - rcDest.left + lWidth - 1) / lWidth;
                                int iTimesY = (rcDest.bottom - rcDest.top + lHeight - 1) / lHeight;
                                for (int j = 0; j < iTimesY; ++j) {
                                    LONG lDestTop = rcDest.top + lHeight * j;
                                    LONG lDestBottom = rcDest.top + lHeight * (j + 1);
                                    LONG lDrawHeight = lHeight;
                                    if (lDestBottom > rcDest.bottom) {
                                        lDrawHeight -= lDestBottom - rcDest.bottom;
                                        lDestBottom = rcDest.bottom;
                                    }
                                    for (int i = 0; i < iTimesX; ++i) {
                                        LONG lDestLeft = rcDest.left + lWidth * i;
                                        LONG lDestRight = rcDest.left + lWidth * (i + 1);
                                        LONG lDrawWidth = lWidth;
                                        if (lDestRight > rcDest.right) {
                                            lDrawWidth -= lDestRight - rcDest.right;
                                            lDestRight = rcDest.right;
                                        }
                                        lpAlphaBlend(hdc, rcDest.left + lWidth * i, rcDest.top + lHeight * j,
                                            lDestRight - lDestLeft, lDestBottom - lDestTop, hCloneDC,
                                            rcBmpPart.left + rcScale9.left, rcBmpPart.top + rcScale9.top, lDrawWidth, lDrawHeight, bf);
                                    }
                                }
                            } else if (xtiled) {
                                LONG lWidth = rcBmpPart.right - rcBmpPart.left - rcScale9.left - rcScale9.right;
                                int iTimes = (rcDest.right - rcDest.left + lWidth - 1) / lWidth;
                                for (int i = 0; i < iTimes; ++i) {
                                    LONG lDestLeft = rcDest.left + lWidth * i;
                                    LONG lDestRight = rcDest.left + lWidth * (i + 1);
                                    LONG lDrawWidth = lWidth;
                                    if (lDestRight > rcDest.right) {
                                        lDrawWidth -= lDestRight - rcDest.right;
                                        lDestRight = rcDest.right;
                                    }
                                    rcDest.bottom -= rcDest.top;
                                    lpAlphaBlend(hdc, lDestLeft, rcDest.top, lDestRight - lDestLeft, rcDest.bottom,
                                        hCloneDC, rcBmpPart.left + rcScale9.left, rcBmpPart.top + rcScale9.top, \
                                        lDrawWidth, rcBmpPart.bottom - rcBmpPart.top - rcScale9.top - rcScale9.bottom, bf);
                                }
                            } else { // ytiled
                                LONG lHeight = rcBmpPart.bottom - rcBmpPart.top - rcScale9.top - rcScale9.bottom;
                                int iTimes = (rcDest.bottom - rcDest.top + lHeight - 1) / lHeight;
                                for (int i = 0; i < iTimes; ++i) {
                                    LONG lDestTop = rcDest.top + lHeight * i;
                                    LONG lDestBottom = rcDest.top + lHeight * (i + 1);
                                    LONG lDrawHeight = lHeight;
                                    if (lDestBottom > rcDest.bottom) {
                                        lDrawHeight -= lDestBottom - rcDest.bottom;
                                        lDestBottom = rcDest.bottom;
                                    }
                                    rcDest.right -= rcDest.left;
                                    lpAlphaBlend(hdc, rcDest.left, rcDest.top + lHeight * i, rcDest.right, lDestBottom - lDestTop,
                                        hCloneDC, rcBmpPart.left + rcScale9.left, rcBmpPart.top + rcScale9.top, \
                                        rcBmpPart.right - rcBmpPart.left - rcScale9.left - rcScale9.right, lDrawHeight, bf);
                                }
                            }
                        }
                    }

                    // left-top
                    if (rcScale9.left > 0 && rcScale9.top > 0) {
                        rcDest.left = rc.left;
                        rcDest.top = rc.top;
                        rcDest.right = rcScale9.left;
                        rcDest.bottom = rcScale9.top;
                        rcDest.right += rcDest.left;
                        rcDest.bottom += rcDest.top;
                        if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                            rcDest.right -= rcDest.left;
                            rcDest.bottom -= rcDest.top;
                            lpAlphaBlend(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                rcBmpPart.left, rcBmpPart.top, rcScale9.left, rcScale9.top, bf);
                        }
                    }
                    // top
                    if (rcScale9.top > 0) {
                        rcDest.left = rc.left + rcScale9.left;
                        rcDest.top = rc.top;
                        rcDest.right = rc.right - rc.left - rcScale9.left - rcScale9.right;
                        rcDest.bottom = rcScale9.top;
                        rcDest.right += rcDest.left;
                        rcDest.bottom += rcDest.top;
                        if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                            rcDest.right -= rcDest.left;
                            rcDest.bottom -= rcDest.top;
                            lpAlphaBlend(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                rcBmpPart.left + rcScale9.left, rcBmpPart.top, rcBmpPart.right - rcBmpPart.left - \
                                rcScale9.left - rcScale9.right, rcScale9.top, bf);
                        }
                    }
                    // right-top
                    if (rcScale9.right > 0 && rcScale9.top > 0) {
                        rcDest.left = rc.right - rcScale9.right;
                        rcDest.top = rc.top;
                        rcDest.right = rcScale9.right;
                        rcDest.bottom = rcScale9.top;
                        rcDest.right += rcDest.left;
                        rcDest.bottom += rcDest.top;
                        if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                            rcDest.right -= rcDest.left;
                            rcDest.bottom -= rcDest.top;
                            lpAlphaBlend(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                rcBmpPart.right - rcScale9.right, rcBmpPart.top, rcScale9.right, rcScale9.top, bf);
                        }
                    }
                    // left
                    if (rcScale9.left > 0) {
                        rcDest.left = rc.left;
                        rcDest.top = rc.top + rcScale9.top;
                        rcDest.right = rcScale9.left;
                        rcDest.bottom = rc.bottom - rc.top - rcScale9.top - rcScale9.bottom;
                        rcDest.right += rcDest.left;
                        rcDest.bottom += rcDest.top;
                        if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                            rcDest.right -= rcDest.left;
                            rcDest.bottom -= rcDest.top;
                            lpAlphaBlend(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                rcBmpPart.left, rcBmpPart.top + rcScale9.top, rcScale9.left, rcBmpPart.bottom - \
                                rcBmpPart.top - rcScale9.top - rcScale9.bottom, bf);
                        }
                    }
                    // right
                    if (rcScale9.right > 0) {
                        rcDest.left = rc.right - rcScale9.right;
                        rcDest.top = rc.top + rcScale9.top;
                        rcDest.right = rcScale9.right;
                        rcDest.bottom = rc.bottom - rc.top - rcScale9.top - rcScale9.bottom;
                        rcDest.right += rcDest.left;
                        rcDest.bottom += rcDest.top;
                        if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                            rcDest.right -= rcDest.left;
                            rcDest.bottom -= rcDest.top;
                            lpAlphaBlend(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                rcBmpPart.right - rcScale9.right, rcBmpPart.top + rcScale9.top, rcScale9.right, \
                                rcBmpPart.bottom - rcBmpPart.top - rcScale9.top - rcScale9.bottom, bf);
                        }
                    }
                    // left-bottom
                    if (rcScale9.left > 0 && rcScale9.bottom > 0) {
                        rcDest.left = rc.left;
                        rcDest.top = rc.bottom - rcScale9.bottom;
                        rcDest.right = rcScale9.left;
                        rcDest.bottom = rcScale9.bottom;
                        rcDest.right += rcDest.left;
                        rcDest.bottom += rcDest.top;
                        if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                            rcDest.right -= rcDest.left;
                            rcDest.bottom -= rcDest.top;
                            lpAlphaBlend(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                rcBmpPart.left, rcBmpPart.bottom - rcScale9.bottom, rcScale9.left, rcScale9.bottom, bf);
                        }
                    }
                    // bottom
                    if (rcScale9.bottom > 0) {
                        rcDest.left = rc.left + rcScale9.left;
                        rcDest.top = rc.bottom - rcScale9.bottom;
                        rcDest.right = rc.right - rc.left - rcScale9.left - rcScale9.right;
                        rcDest.bottom = rcScale9.bottom;
                        rcDest.right += rcDest.left;
                        rcDest.bottom += rcDest.top;
                        if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                            rcDest.right -= rcDest.left;
                            rcDest.bottom -= rcDest.top;
                            lpAlphaBlend(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC,
                                rcBmpPart.left + rcScale9.left, rcBmpPart.bottom - rcScale9.bottom,
                                rcBmpPart.right - rcBmpPart.left - rcScale9.left - rcScale9.right, rcScale9.bottom, bf);
                        }
                    }
                    // right-bottom
                    if (rcScale9.right > 0 && rcScale9.bottom > 0) {
                        rcDest.left = rc.right - rcScale9.right;
                        rcDest.top = rc.bottom - rcScale9.bottom;
                        rcDest.right = rcScale9.right;
                        rcDest.bottom = rcScale9.bottom;
                        rcDest.right += rcDest.left;
                        rcDest.bottom += rcDest.top;
                        if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                            rcDest.right -= rcDest.left;
                            rcDest.bottom -= rcDest.top;
                            lpAlphaBlend(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                rcBmpPart.right - rcScale9.right, rcBmpPart.bottom - rcScale9.bottom, rcScale9.right, \
                                rcScale9.bottom, bf);
                        }
                    }
                } else {
                    if (rc.right - rc.left == rcBmpPart.right - rcBmpPart.left \
                        && rc.bottom - rc.top == rcBmpPart.bottom - rcBmpPart.top \
                        && rcScale9.left == 0 && rcScale9.right == 0 && rcScale9.top == 0 && rcScale9.bottom == 0) {
                        if (::IntersectRect(&rcTemp, &rcPaint, &rc)) {
                            ::BitBlt(hdc, rcTemp.left, rcTemp.top, rcTemp.right - rcTemp.left, rcTemp.bottom - rcTemp.top, \
                                hCloneDC, rcBmpPart.left + rcTemp.left - rc.left, rcBmpPart.top + rcTemp.top - rc.top, SRCCOPY);
                        }
                    } else {
                        // middle
                        if (!hole) {
                            rcDest.left = rc.left + rcScale9.left;
                            rcDest.top = rc.top + rcScale9.top;
                            rcDest.right = rc.right - rc.left - rcScale9.left - rcScale9.right;
                            rcDest.bottom = rc.bottom - rc.top - rcScale9.top - rcScale9.bottom;
                            rcDest.right += rcDest.left;
                            rcDest.bottom += rcDest.top;
                            if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                                if (!xtiled && !ytiled) {
                                    rcDest.right -= rcDest.left;
                                    rcDest.bottom -= rcDest.top;
                                    ::StretchBlt(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                        rcBmpPart.left + rcScale9.left, rcBmpPart.top + rcScale9.top, \
                                        rcBmpPart.right - rcBmpPart.left - rcScale9.left - rcScale9.right, \
                                        rcBmpPart.bottom - rcBmpPart.top - rcScale9.top - rcScale9.bottom, SRCCOPY);
                                } else if (xtiled && ytiled) {
                                    LONG lWidth = rcBmpPart.right - rcBmpPart.left - rcScale9.left - rcScale9.right;
                                    LONG lHeight = rcBmpPart.bottom - rcBmpPart.top - rcScale9.top - rcScale9.bottom;
                                    int iTimesX = (rcDest.right - rcDest.left + lWidth - 1) / lWidth;
                                    int iTimesY = (rcDest.bottom - rcDest.top + lHeight - 1) / lHeight;
                                    for (int j = 0; j < iTimesY; ++j) {
                                        LONG lDestTop = rcDest.top + lHeight * j;
                                        LONG lDestBottom = rcDest.top + lHeight * (j + 1);
                                        LONG lDrawHeight = lHeight;
                                        if (lDestBottom > rcDest.bottom) {
                                            lDrawHeight -= lDestBottom - rcDest.bottom;
                                            lDestBottom = rcDest.bottom;
                                        }
                                        for (int i = 0; i < iTimesX; ++i) {
                                            LONG lDestLeft = rcDest.left + lWidth * i;
                                            LONG lDestRight = rcDest.left + lWidth * (i + 1);
                                            LONG lDrawWidth = lWidth;
                                            if (lDestRight > rcDest.right) {
                                                lDrawWidth -= lDestRight - rcDest.right;
                                                lDestRight = rcDest.right;
                                            }
                                            ::BitBlt(hdc, rcDest.left + lWidth * i, rcDest.top + lHeight * j, \
                                                lDestRight - lDestLeft, lDestBottom - lDestTop, hCloneDC, \
                                                rcBmpPart.left + rcScale9.left, rcBmpPart.top + rcScale9.top, SRCCOPY);
                                        }
                                    }
                                } else if (xtiled) {
                                    LONG lWidth = rcBmpPart.right - rcBmpPart.left - rcScale9.left - rcScale9.right;
                                    int iTimes = (rcDest.right - rcDest.left + lWidth - 1) / lWidth;
                                    for (int i = 0; i < iTimes; ++i) {
                                        LONG lDestLeft = rcDest.left + lWidth * i;
                                        LONG lDestRight = rcDest.left + lWidth * (i + 1);
                                        LONG lDrawWidth = lWidth;
                                        if (lDestRight > rcDest.right) {
                                            lDrawWidth -= lDestRight - rcDest.right;
                                            lDestRight = rcDest.right;
                                        }
                                        rcDest.bottom -= rcDest.top;
                                        ::StretchBlt(hdc, lDestLeft, rcDest.top, lDestRight - lDestLeft, rcDest.bottom,
                                            hCloneDC, rcBmpPart.left + rcScale9.left, rcBmpPart.top + rcScale9.top, \
                                            lDrawWidth, rcBmpPart.bottom - rcBmpPart.top - rcScale9.top - rcScale9.bottom, SRCCOPY);
                                    }
                                } else { // ytiled
                                    LONG lHeight = rcBmpPart.bottom - rcBmpPart.top - rcScale9.top - rcScale9.bottom;
                                    int iTimes = (rcDest.bottom - rcDest.top + lHeight - 1) / lHeight;
                                    for (int i = 0; i < iTimes; ++i) {
                                        LONG lDestTop = rcDest.top + lHeight * i;
                                        LONG lDestBottom = rcDest.top + lHeight * (i + 1);
                                        LONG lDrawHeight = lHeight;
                                        if (lDestBottom > rcDest.bottom) {
                                            lDrawHeight -= lDestBottom - rcDest.bottom;
                                            lDestBottom = rcDest.bottom;
                                        }
                                        rcDest.right -= rcDest.left;
                                        ::StretchBlt(hdc, rcDest.left, rcDest.top + lHeight * i, rcDest.right, lDestBottom - lDestTop,
                                            hCloneDC, rcBmpPart.left + rcScale9.left, rcBmpPart.top + rcScale9.top, \
                                            rcBmpPart.right - rcBmpPart.left - rcScale9.left - rcScale9.right, lDrawHeight, SRCCOPY);
                                    }
                                }
                            }
                        }

                        // left-top
                        if (rcScale9.left > 0 && rcScale9.top > 0) {
                            rcDest.left = rc.left;
                            rcDest.top = rc.top;
                            rcDest.right = rcScale9.left;
                            rcDest.bottom = rcScale9.top;
                            rcDest.right += rcDest.left;
                            rcDest.bottom += rcDest.top;
                            if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                                rcDest.right -= rcDest.left;
                                rcDest.bottom -= rcDest.top;
                                ::StretchBlt(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                    rcBmpPart.left, rcBmpPart.top, rcScale9.left, rcScale9.top, SRCCOPY);
                            }
                        }
                        // top
                        if (rcScale9.top > 0) {
                            rcDest.left = rc.left + rcScale9.left;
                            rcDest.top = rc.top;
                            rcDest.right = rc.right - rc.left - rcScale9.left - rcScale9.right;
                            rcDest.bottom = rcScale9.top;
                            rcDest.right += rcDest.left;
                            rcDest.bottom += rcDest.top;
                            if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                                rcDest.right -= rcDest.left;
                                rcDest.bottom -= rcDest.top;
                                ::StretchBlt(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                    rcBmpPart.left + rcScale9.left, rcBmpPart.top, rcBmpPart.right - rcBmpPart.left - \
                                    rcScale9.left - rcScale9.right, rcScale9.top, SRCCOPY);
                            }
                        }
                        // right-top
                        if (rcScale9.right > 0 && rcScale9.top > 0) {
                            rcDest.left = rc.right - rcScale9.right;
                            rcDest.top = rc.top;
                            rcDest.right = rcScale9.right;
                            rcDest.bottom = rcScale9.top;
                            rcDest.right += rcDest.left;
                            rcDest.bottom += rcDest.top;
                            if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                                rcDest.right -= rcDest.left;
                                rcDest.bottom -= rcDest.top;
                                ::StretchBlt(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                    rcBmpPart.right - rcScale9.right, rcBmpPart.top, rcScale9.right, rcScale9.top, SRCCOPY);
                            }
                        }
                        // left
                        if (rcScale9.left > 0) {
                            rcDest.left = rc.left;
                            rcDest.top = rc.top + rcScale9.top;
                            rcDest.right = rcScale9.left;
                            rcDest.bottom = rc.bottom - rc.top - rcScale9.top - rcScale9.bottom;
                            rcDest.right += rcDest.left;
                            rcDest.bottom += rcDest.top;
                            if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                                rcDest.right -= rcDest.left;
                                rcDest.bottom -= rcDest.top;
                                ::StretchBlt(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                    rcBmpPart.left, rcBmpPart.top + rcScale9.top, rcScale9.left, rcBmpPart.bottom - \
                                    rcBmpPart.top - rcScale9.top - rcScale9.bottom, SRCCOPY);
                            }
                        }
                        // right
                        if (rcScale9.right > 0) {
                            rcDest.left = rc.right - rcScale9.right;
                            rcDest.top = rc.top + rcScale9.top;
                            rcDest.right = rcScale9.right;
                            rcDest.bottom = rc.bottom - rc.top - rcScale9.top - rcScale9.bottom;
                            rcDest.right += rcDest.left;
                            rcDest.bottom += rcDest.top;
                            if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                                rcDest.right -= rcDest.left;
                                rcDest.bottom -= rcDest.top;
                                ::StretchBlt(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                    rcBmpPart.right - rcScale9.right, rcBmpPart.top + rcScale9.top, rcScale9.right, \
                                    rcBmpPart.bottom - rcBmpPart.top - rcScale9.top - rcScale9.bottom, SRCCOPY);
                            }
                        }
                        // left-bottom
                        if (rcScale9.left > 0 && rcScale9.bottom > 0) {
                            rcDest.left = rc.left;
                            rcDest.top = rc.bottom - rcScale9.bottom;
                            rcDest.right = rcScale9.left;
                            rcDest.bottom = rcScale9.bottom;
                            rcDest.right += rcDest.left;
                            rcDest.bottom += rcDest.top;
                            if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                                rcDest.right -= rcDest.left;
                                rcDest.bottom -= rcDest.top;
                                ::StretchBlt(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                    rcBmpPart.left, rcBmpPart.bottom - rcScale9.bottom, rcScale9.left, rcScale9.bottom, SRCCOPY);
                            }
                        }
                        // bottom
                        if (rcScale9.bottom > 0) {
                            rcDest.left = rc.left + rcScale9.left;
                            rcDest.top = rc.bottom - rcScale9.bottom;
                            rcDest.right = rc.right - rc.left - rcScale9.left - rcScale9.right;
                            rcDest.bottom = rcScale9.bottom;
                            rcDest.right += rcDest.left;
                            rcDest.bottom += rcDest.top;
                            if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                                rcDest.right -= rcDest.left;
                                rcDest.bottom -= rcDest.top;
                                ::StretchBlt(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                    rcBmpPart.left + rcScale9.left, rcBmpPart.bottom - rcScale9.bottom, \
                                    rcBmpPart.right - rcBmpPart.left - rcScale9.left - rcScale9.right, rcScale9.bottom, SRCCOPY);
                            }
                        }
                        // right-bottom
                        if (rcScale9.right > 0 && rcScale9.bottom > 0) {
                            rcDest.left = rc.right - rcScale9.right;
                            rcDest.top = rc.bottom - rcScale9.bottom;
                            rcDest.right = rcScale9.right;
                            rcDest.bottom = rcScale9.bottom;
                            rcDest.right += rcDest.left;
                            rcDest.bottom += rcDest.top;
                            if (::IntersectRect(&rcTemp, &rcPaint, &rcDest)) {
                                rcDest.right -= rcDest.left;
                                rcDest.bottom -= rcDest.top;
                                ::StretchBlt(hdc, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
                                    rcBmpPart.right - rcScale9.right, rcBmpPart.bottom - rcScale9.bottom, rcScale9.right, \
                                    rcScale9.bottom, SRCCOPY);
                            }
                        }
                    }
                }

                ::SelectObject(hCloneDC, hOldBitmap);
                ::DeleteDC(hCloneDC);
                return true;
            }

            void WindowRender::DrawHtmlText(HDC hDC, Context* context, RECT& rc, LPCTSTR pstrText, DWORD dwTextColor,
                RECT* pLinks, char* sLinks, int& nLinkRects, UINT uStyle) {
                assert(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);
                if (pstrText == NULL) return;
                if (::IsRectEmpty(&rc)) return;

                bool bDraw = (uStyle & DT_CALCRECT) == 0;

                //CStdPtrArray aFontArray(10);
                //CStdPtrArray aColorArray(10);
                //CStdPtrArray aPIndentArray(10);

                RECT rcClip = { 0 };
                ::GetClipBox(hDC, &rcClip);
                HRGN hOldRgn = ::CreateRectRgnIndirect(&rcClip);
                HRGN hRgn = ::CreateRectRgnIndirect(&rc);
                if (bDraw) ::ExtSelectClipRgn(hDC, hRgn, RGN_AND);

                TEXTMETRIC* pTm = &context->GetDefaultFontInfo()->GetTEXTMETRIC();
                HFONT hOldFont = (HFONT) ::SelectObject(hDC, context->GetDefaultFontInfo()->GetFont());
                ::SetBkMode(hDC, TRANSPARENT);
                ::SetTextColor(hDC, RGB(GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));
                DWORD dwBkColor = context->GetDefaultSelectedBkColor();
                ::SetBkColor(hDC, RGB(GetBValue(dwBkColor), GetGValue(dwBkColor), GetRValue(dwBkColor)));

                if (((uStyle & DT_CENTER) != 0 || (uStyle & DT_RIGHT) != 0 || (uStyle & DT_VCENTER) != 0 || (uStyle & DT_BOTTOM) != 0) && (uStyle & DT_CALCRECT) == 0) {
                    RECT rcText = { 0, 0, 9999, 100 };
                    int nLinks = 0;
                    DrawHtmlText(hDC, context, rcText, pstrText, dwTextColor, NULL, NULL, nLinks, uStyle | DT_CALCRECT);
                    if ((uStyle & DT_SINGLELINE) != 0) {
                        if ((uStyle & DT_CENTER) != 0) {
                            rc.left = rc.left + ((rc.right - rc.left) / 2) - ((rcText.right - rcText.left) / 2);
                            rc.right = rc.left + (rcText.right - rcText.left);
                        }
                        if ((uStyle & DT_RIGHT) != 0) {
                            rc.left = rc.right - (rcText.right - rcText.left);
                        }
                    }
                    if ((uStyle & DT_VCENTER) != 0) {
                        rc.top = rc.top + ((rc.bottom - rc.top) / 2) - ((rcText.bottom - rcText.top) / 2);
                        rc.bottom = rc.top + (rcText.bottom - rcText.top);
                    }
                    if ((uStyle & DT_BOTTOM) != 0) {
                        rc.top = rc.bottom - (rcText.bottom - rcText.top);
                    }
                }

                bool bHoverLink = false;
                //CDuiString sHoverLink;
                //POINT ptMouse = pManager->GetMousePos();
                //for (int i = 0; !bHoverLink && i < nLinkRects; i++) {
                //    if (::PtInRect(prcLinks + i, ptMouse)) {
                //        sHoverLink = *(CDuiString*)(sLinks + i);
                //        bHoverLink = true;
                //    }
                //}

                POINT pt = { rc.left, rc.top };
                int iLinkIndex = 0;
                int cyLine = pTm->tmHeight + pTm->tmExternalLeading + (int)aPIndentArray.GetAt(aPIndentArray.GetSize() - 1);
                int cyMinHeight = 0;
                int cxMaxWidth = 0;
                POINT ptLinkStart = { 0 };
                bool bLineEnd = false;
                bool bInRaw = false;
                bool bInLink = false;
                bool bInSelected = false;
                int iLineLinkIndex = 0;

                // 排版习惯是图文底部对齐，所以每行绘制都要分两步，先计算高度，再绘制
                CStdPtrArray aLineFontArray;
                CStdPtrArray aLineColorArray;
                CStdPtrArray aLinePIndentArray;
                LPCTSTR pstrLineBegin = pstrText;
                bool bLineInRaw = false;
                bool bLineInLink = false;
                bool bLineInSelected = false;
                int cyLineHeight = 0;
                bool bLineDraw = false; // 行的第二阶段：绘制
                while (*pstrText != _T('\0')) {
                    if (pt.x >= rc.right || *pstrText == _T('\n') || bLineEnd) {
                        if (*pstrText == _T('\n')) pstrText++;
                        if (bLineEnd) bLineEnd = false;
                        if (!bLineDraw) {
                            if (bInLink && iLinkIndex < nLinkRects) {
                                ::SetRect(&prcLinks[iLinkIndex++], ptLinkStart.x, ptLinkStart.y, MIN(pt.x, rc.right), pt.y + cyLine);
                                CDuiString* pStr1 = (CDuiString*)(sLinks + iLinkIndex - 1);
                                CDuiString* pStr2 = (CDuiString*)(sLinks + iLinkIndex);
                                *pStr2 = *pStr1;
                            }
                            for (int i = iLineLinkIndex; i < iLinkIndex; i++) {
                                prcLinks[i].bottom = pt.y + cyLine;
                            }
                            if (bDraw) {
                                bInLink = bLineInLink;
                                iLinkIndex = iLineLinkIndex;
                            }
                        } else {
                            if (bInLink && iLinkIndex < nLinkRects) iLinkIndex++;
                            bLineInLink = bInLink;
                            iLineLinkIndex = iLinkIndex;
                        }
                        if ((uStyle & DT_SINGLELINE) != 0 && (!bDraw || bLineDraw)) break;
                        if (bDraw) bLineDraw = !bLineDraw; // !
                        pt.x = rc.left;
                        if (!bLineDraw) pt.y += cyLine;
                        if (pt.y > rc.bottom && bDraw) break;
                        ptLinkStart = pt;
                        cyLine = pTm->tmHeight + pTm->tmExternalLeading + (int)aPIndentArray.GetAt(aPIndentArray.GetSize() - 1);
                        if (pt.x >= rc.right) break;
                    } else if (!bInRaw && (*pstrText == _T('<') || *pstrText == _T('{'))
                        && (pstrText[1] >= _T('a') && pstrText[1] <= _T('z'))
                        && (pstrText[2] == _T(' ') || pstrText[2] == _T('>') || pstrText[2] == _T('}'))) {
                        pstrText++;
                        LPCTSTR pstrNextStart = NULL;
                        switch (*pstrText) {
                        case _T('a'):  // Link
                        {
                            pstrText++;
                            while (*pstrText > _T('\0') && *pstrText <= _T(' ')) pstrText = ::CharNext(pstrText);
                            if (iLinkIndex < nLinkRects && !bLineDraw) {
                                CDuiString* pStr = (CDuiString*)(sLinks + iLinkIndex);
                                pStr->Empty();
                                while (*pstrText != _T('\0') && *pstrText != _T('>') && *pstrText != _T('}')) {
                                    LPCTSTR pstrTemp = ::CharNext(pstrText);
                                    while (pstrText < pstrTemp) {
                                        *pStr += *pstrText++;
                                    }
                                }
                            }

                            DWORD clrColor = pManager->GetDefaultLinkFontColor();
                            if (bHoverLink && iLinkIndex < nLinkRects) {
                                CDuiString* pStr = (CDuiString*)(sLinks + iLinkIndex);
                                if (sHoverLink == *pStr) clrColor = pManager->GetDefaultLinkHoverFontColor();
                            }
                            //else if( prcLinks == NULL ) {
                            //    if( ::PtInRect(&rc, ptMouse) )
                            //        clrColor = pManager->GetDefaultLinkHoverFontColor();
                            //}
                            aColorArray.Add((LPVOID)clrColor);
                            ::SetTextColor(hDC, RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
                            TFontInfo* pFontInfo = pManager->GetDefaultFontInfo();
                            if (aFontArray.GetSize() > 0) pFontInfo = (TFontInfo*)aFontArray.GetAt(aFontArray.GetSize() - 1);
                            if (pFontInfo->bUnderline == false) {
                                HFONT hFont = pManager->GetFont(pFontInfo->sFontName, pFontInfo->iSize, pFontInfo->bBold, true, pFontInfo->bItalic);
                                if (hFont == NULL) hFont = pManager->AddFont(pFontInfo->sFontName, pFontInfo->iSize, pFontInfo->bBold, true, pFontInfo->bItalic);
                                pFontInfo = pManager->GetFontInfo(hFont);
                                aFontArray.Add(pFontInfo);
                                pTm = &pFontInfo->tm;
                                ::SelectObject(hDC, pFontInfo->hFont);
                                cyLine = MAX(cyLine, pTm->tmHeight + pTm->tmExternalLeading + (int)aPIndentArray.GetAt(aPIndentArray.GetSize() - 1));
                            }
                            ptLinkStart = pt;
                            bInLink = true;
                        }
                        break;
                        case _T('b'):  // Bold
                        {
                            pstrText++;
                            TFontInfo* pFontInfo = pManager->GetDefaultFontInfo();
                            if (aFontArray.GetSize() > 0) pFontInfo = (TFontInfo*)aFontArray.GetAt(aFontArray.GetSize() - 1);
                            if (pFontInfo->bBold == false) {
                                HFONT hFont = pManager->GetFont(pFontInfo->sFontName, pFontInfo->iSize, true, pFontInfo->bUnderline, pFontInfo->bItalic);
                                if (hFont == NULL) hFont = pManager->AddFont(pFontInfo->sFontName, pFontInfo->iSize, true, pFontInfo->bUnderline, pFontInfo->bItalic);
                                pFontInfo = pManager->GetFontInfo(hFont);
                                aFontArray.Add(pFontInfo);
                                pTm = &pFontInfo->tm;
                                ::SelectObject(hDC, pFontInfo->hFont);
                                cyLine = MAX(cyLine, pTm->tmHeight + pTm->tmExternalLeading + (int)aPIndentArray.GetAt(aPIndentArray.GetSize() - 1));
                            }
                        }
                        break;
                        case _T('c'):  // Color
                        {
                            pstrText++;
                            while (*pstrText > _T('\0') && *pstrText <= _T(' ')) pstrText = ::CharNext(pstrText);
                            if (*pstrText == _T('#')) pstrText++;
                            DWORD clrColor = _tcstol(pstrText, const_cast<LPTSTR*>(&pstrText), 16);
                            aColorArray.Add((LPVOID)clrColor);
                            ::SetTextColor(hDC, RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
                        }
                        break;
                        case _T('f'):  // Font
                        {
                            pstrText++;
                            while (*pstrText > _T('\0') && *pstrText <= _T(' ')) pstrText = ::CharNext(pstrText);
                            LPCTSTR pstrTemp = pstrText;
                            int iFont = (int)_tcstol(pstrText, const_cast<LPTSTR*>(&pstrText), 10);
                            //if( isdigit(*pstrText) ) { // debug版本会引起异常
                            if (pstrTemp != pstrText) {
                                TFontInfo* pFontInfo = pManager->GetFontInfo(iFont);
                                aFontArray.Add(pFontInfo);
                                pTm = &pFontInfo->tm;
                                ::SelectObject(hDC, pFontInfo->hFont);
                            } else {
                                CDuiString sFontName;
                                int iFontSize = 10;
                                CDuiString sFontAttr;
                                bool bBold = false;
                                bool bUnderline = false;
                                bool bItalic = false;
                                while (*pstrText != _T('\0') && *pstrText != _T('>') && *pstrText != _T('}') && *pstrText != _T(' ')) {
                                    pstrTemp = ::CharNext(pstrText);
                                    while (pstrText < pstrTemp) {
                                        sFontName += *pstrText++;
                                    }
                                }
                                while (*pstrText > _T('\0') && *pstrText <= _T(' ')) pstrText = ::CharNext(pstrText);
                                if (isdigit(*pstrText)) {
                                    iFontSize = (int)_tcstol(pstrText, const_cast<LPTSTR*>(&pstrText), 10);
                                }
                                while (*pstrText > _T('\0') && *pstrText <= _T(' ')) pstrText = ::CharNext(pstrText);
                                while (*pstrText != _T('\0') && *pstrText != _T('>') && *pstrText != _T('}')) {
                                    pstrTemp = ::CharNext(pstrText);
                                    while (pstrText < pstrTemp) {
                                        sFontAttr += *pstrText++;
                                    }
                                }
                                sFontAttr.MakeLower();
                                if (sFontAttr.Find(_T("bold")) >= 0) bBold = true;
                                if (sFontAttr.Find(_T("underline")) >= 0) bUnderline = true;
                                if (sFontAttr.Find(_T("italic")) >= 0) bItalic = true;
                                HFONT hFont = pManager->GetFont(sFontName, iFontSize, bBold, bUnderline, bItalic);
                                if (hFont == NULL) hFont = pManager->AddFont(sFontName, iFontSize, bBold, bUnderline, bItalic);
                                TFontInfo* pFontInfo = pManager->GetFontInfo(hFont);
                                aFontArray.Add(pFontInfo);
                                pTm = &pFontInfo->tm;
                                ::SelectObject(hDC, pFontInfo->hFont);
                            }
                            cyLine = MAX(cyLine, pTm->tmHeight + pTm->tmExternalLeading + (int)aPIndentArray.GetAt(aPIndentArray.GetSize() - 1));
                        }
                        break;
                        case _T('i'):  // Italic or Image
                        {
                            pstrNextStart = pstrText - 1;
                            pstrText++;
                            CDuiString sImageString = pstrText;
                            int iWidth = 0;
                            int iHeight = 0;
                            while (*pstrText > _T('\0') && *pstrText <= _T(' ')) pstrText = ::CharNext(pstrText);
                            const TImageInfo* pImageInfo = NULL;
                            CDuiString sName;
                            while (*pstrText != _T('\0') && *pstrText != _T('>') && *pstrText != _T('}') && *pstrText != _T(' ')) {
                                LPCTSTR pstrTemp = ::CharNext(pstrText);
                                while (pstrText < pstrTemp) {
                                    sName += *pstrText++;
                                }
                            }
                            if (sName.IsEmpty()) { // Italic
                                pstrNextStart = NULL;
                                TFontInfo* pFontInfo = pManager->GetDefaultFontInfo();
                                if (aFontArray.GetSize() > 0) pFontInfo = (TFontInfo*)aFontArray.GetAt(aFontArray.GetSize() - 1);
                                if (pFontInfo->bItalic == false) {
                                    HFONT hFont = pManager->GetFont(pFontInfo->sFontName, pFontInfo->iSize, pFontInfo->bBold, pFontInfo->bUnderline, true);
                                    if (hFont == NULL) hFont = pManager->AddFont(pFontInfo->sFontName, pFontInfo->iSize, pFontInfo->bBold, pFontInfo->bUnderline, true);
                                    pFontInfo = pManager->GetFontInfo(hFont);
                                    aFontArray.Add(pFontInfo);
                                    pTm = &pFontInfo->tm;
                                    ::SelectObject(hDC, pFontInfo->hFont);
                                    cyLine = MAX(cyLine, pTm->tmHeight + pTm->tmExternalLeading + (int)aPIndentArray.GetAt(aPIndentArray.GetSize() - 1));
                                }
                            } else {
                                while (*pstrText > _T('\0') && *pstrText <= _T(' ')) pstrText = ::CharNext(pstrText);
                                int iImageListNum = (int)_tcstol(pstrText, const_cast<LPTSTR*>(&pstrText), 10);
                                if (iImageListNum <= 0) iImageListNum = 1;
                                while (*pstrText > _T('\0') && *pstrText <= _T(' ')) pstrText = ::CharNext(pstrText);
                                int iImageListIndex = (int)_tcstol(pstrText, const_cast<LPTSTR*>(&pstrText), 10);
                                if (iImageListIndex < 0 || iImageListIndex >= iImageListNum) iImageListIndex = 0;

                                if (_tcsstr(sImageString.GetData(), _T("file=\'")) != NULL || _tcsstr(sImageString.GetData(), _T("res=\'")) != NULL) {
                                    CDuiString sImageResType;
                                    CDuiString sImageName;
                                    LPCTSTR pStrImage = sImageString.GetData();
                                    CDuiString sItem;
                                    CDuiString sValue;
                                    while (*pStrImage != _T('\0')) {
                                        sItem.Empty();
                                        sValue.Empty();
                                        while (*pStrImage > _T('\0') && *pStrImage <= _T(' ')) pStrImage = ::CharNext(pStrImage);
                                        while (*pStrImage != _T('\0') && *pStrImage != _T('=') && *pStrImage > _T(' ')) {
                                            LPTSTR pstrTemp = ::CharNext(pStrImage);
                                            while (pStrImage < pstrTemp) {
                                                sItem += *pStrImage++;
                                            }
                                        }
                                        while (*pStrImage > _T('\0') && *pStrImage <= _T(' ')) pStrImage = ::CharNext(pStrImage);
                                        if (*pStrImage++ != _T('=')) break;
                                        while (*pStrImage > _T('\0') && *pStrImage <= _T(' ')) pStrImage = ::CharNext(pStrImage);
                                        if (*pStrImage++ != _T('\'')) break;
                                        while (*pStrImage != _T('\0') && *pStrImage != _T('\'')) {
                                            LPTSTR pstrTemp = ::CharNext(pStrImage);
                                            while (pStrImage < pstrTemp) {
                                                sValue += *pStrImage++;
                                            }
                                        }
                                        if (*pStrImage++ != _T('\'')) break;
                                        if (!sValue.IsEmpty()) {
                                            if (sItem == _T("file") || sItem == _T("res")) {
                                                sImageName = sValue;
                                            } else if (sItem == _T("restype")) {
                                                sImageResType = sValue;
                                            }
                                        }
                                        if (*pStrImage++ != _T(' ')) break;
                                    }

                                    pImageInfo = pManager->GetImageEx((LPCTSTR)sImageName, sImageResType);
                                } else
                                    pImageInfo = pManager->GetImageEx((LPCTSTR)sName);

                                if (pImageInfo) {
                                    iWidth = pImageInfo->nX;
                                    iHeight = pImageInfo->nY;
                                    if (iImageListNum > 1) iWidth /= iImageListNum;

                                    if (pt.x + iWidth > rc.right && pt.x > rc.left && (uStyle & DT_SINGLELINE) == 0) {
                                        bLineEnd = true;
                                    } else {
                                        pstrNextStart = NULL;
                                        if (bDraw && bLineDraw) {
                                            CDuiRect rcImage(pt.x, pt.y + cyLineHeight - iHeight, pt.x + iWidth, pt.y + cyLineHeight);
                                            if (iHeight < cyLineHeight) {
                                                rcImage.bottom -= (cyLineHeight - iHeight) / 2;
                                                rcImage.top = rcImage.bottom - iHeight;
                                            }
                                            CDuiRect rcBmpPart(0, 0, iWidth, iHeight);
                                            rcBmpPart.left = iWidth * iImageListIndex;
                                            rcBmpPart.right = iWidth * (iImageListIndex + 1);
                                            CDuiRect rcCorner(0, 0, 0, 0);
                                            DrawImage(hDC, pImageInfo->hBitmap, rcImage, rcImage, rcBmpPart, rcCorner, \
                                                pImageInfo->alphaChannel, 255);
                                        }

                                        cyLine = MAX(iHeight, cyLine);
                                        pt.x += iWidth;
                                        cyMinHeight = pt.y + iHeight;
                                        cxMaxWidth = MAX(cxMaxWidth, pt.x);
                                    }
                                } else pstrNextStart = NULL;
                            }
                        }
                        break;
                        case _T('n'):  // Newline
                        {
                            pstrText++;
                            if ((uStyle & DT_SINGLELINE) != 0) break;
                            bLineEnd = true;
                        }
                        break;
                        case _T('p'):  // Paragraph
                        {
                            pstrText++;
                            if (pt.x > rc.left) bLineEnd = true;
                            while (*pstrText > _T('\0') && *pstrText <= _T(' ')) pstrText = ::CharNext(pstrText);
                            int cyLineExtra = (int)_tcstol(pstrText, const_cast<LPTSTR*>(&pstrText), 10);
                            aPIndentArray.Add((LPVOID)cyLineExtra);
                            cyLine = MAX(cyLine, pTm->tmHeight + pTm->tmExternalLeading + cyLineExtra);
                        }
                        break;
                        case _T('r'):  // Raw Text
                        {
                            pstrText++;
                            bInRaw = true;
                        }
                        break;
                        case _T('s'):  // Selected text background color
                        {
                            pstrText++;
                            bInSelected = !bInSelected;
                            if (bDraw && bLineDraw) {
                                if (bInSelected) ::SetBkMode(hDC, OPAQUE);
                                else ::SetBkMode(hDC, TRANSPARENT);
                            }
                        }
                        break;
                        case _T('u'):  // Underline text
                        {
                            pstrText++;
                            TFontInfo* pFontInfo = pManager->GetDefaultFontInfo();
                            if (aFontArray.GetSize() > 0) pFontInfo = (TFontInfo*)aFontArray.GetAt(aFontArray.GetSize() - 1);
                            if (pFontInfo->bUnderline == false) {
                                HFONT hFont = pManager->GetFont(pFontInfo->sFontName, pFontInfo->iSize, pFontInfo->bBold, true, pFontInfo->bItalic);
                                if (hFont == NULL) hFont = pManager->AddFont(pFontInfo->sFontName, pFontInfo->iSize, pFontInfo->bBold, true, pFontInfo->bItalic);
                                pFontInfo = pManager->GetFontInfo(hFont);
                                aFontArray.Add(pFontInfo);
                                pTm = &pFontInfo->tm;
                                ::SelectObject(hDC, pFontInfo->hFont);
                                cyLine = MAX(cyLine, pTm->tmHeight + pTm->tmExternalLeading + (int)aPIndentArray.GetAt(aPIndentArray.GetSize() - 1));
                            }
                        }
                        break;
                        case _T('x'):  // X Indent
                        {
                            pstrText++;
                            while (*pstrText > _T('\0') && *pstrText <= _T(' ')) pstrText = ::CharNext(pstrText);
                            int iWidth = (int)_tcstol(pstrText, const_cast<LPTSTR*>(&pstrText), 10);
                            pt.x += iWidth;
                            cxMaxWidth = MAX(cxMaxWidth, pt.x);
                        }
                        break;
                        case _T('y'):  // Y Indent
                        {
                            pstrText++;
                            while (*pstrText > _T('\0') && *pstrText <= _T(' ')) pstrText = ::CharNext(pstrText);
                            cyLine = (int)_tcstol(pstrText, const_cast<LPTSTR*>(&pstrText), 10);
                        }
                        break;
                        }
                        if (pstrNextStart != NULL) pstrText = pstrNextStart;
                        else {
                            while (*pstrText != _T('\0') && *pstrText != _T('>') && *pstrText != _T('}')) pstrText = ::CharNext(pstrText);
                            pstrText = ::CharNext(pstrText);
                        }
                    } else if (!bInRaw && (*pstrText == _T('<') || *pstrText == _T('{')) && pstrText[1] == _T('/')) {
                        pstrText++;
                        pstrText++;
                        switch (*pstrText) {
                        case _T('c'):
                        {
                            pstrText++;
                            aColorArray.Remove(aColorArray.GetSize() - 1);
                            DWORD clrColor = dwTextColor;
                            if (aColorArray.GetSize() > 0) clrColor = (int)aColorArray.GetAt(aColorArray.GetSize() - 1);
                            ::SetTextColor(hDC, RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
                        }
                        break;
                        case _T('p'):
                            pstrText++;
                            if (pt.x > rc.left) bLineEnd = true;
                            aPIndentArray.Remove(aPIndentArray.GetSize() - 1);
                            cyLine = MAX(cyLine, pTm->tmHeight + pTm->tmExternalLeading + (int)aPIndentArray.GetAt(aPIndentArray.GetSize() - 1));
                            break;
                        case _T('s'):
                        {
                            pstrText++;
                            bInSelected = !bInSelected;
                            if (bDraw && bLineDraw) {
                                if (bInSelected) ::SetBkMode(hDC, OPAQUE);
                                else ::SetBkMode(hDC, TRANSPARENT);
                            }
                        }
                        break;
                        case _T('a'):
                        {
                            if (iLinkIndex < nLinkRects) {
                                if (!bLineDraw) ::SetRect(&prcLinks[iLinkIndex], ptLinkStart.x, ptLinkStart.y, MIN(pt.x, rc.right), pt.y + pTm->tmHeight + pTm->tmExternalLeading);
                                iLinkIndex++;
                            }
                            aColorArray.Remove(aColorArray.GetSize() - 1);
                            DWORD clrColor = dwTextColor;
                            if (aColorArray.GetSize() > 0) clrColor = (int)aColorArray.GetAt(aColorArray.GetSize() - 1);
                            ::SetTextColor(hDC, RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
                            bInLink = false;
                        }
                        case _T('b'):
                        case _T('f'):
                        case _T('i'):
                        case _T('u'):
                        {
                            pstrText++;
                            aFontArray.Remove(aFontArray.GetSize() - 1);
                            TFontInfo* pFontInfo = (TFontInfo*)aFontArray.GetAt(aFontArray.GetSize() - 1);
                            if (pFontInfo == NULL) pFontInfo = pManager->GetDefaultFontInfo();
                            if (pTm->tmItalic && pFontInfo->bItalic == false) {
                                ABC abc;
                                ::GetCharABCWidths(hDC, _T(' '), _T(' '), &abc);
                                pt.x += abc.abcC / 2; // 简单修正一下斜体混排的问题, 正确做法应该是http://support.microsoft.com/kb/244798/en-us
                            }
                            pTm = &pFontInfo->tm;
                            ::SelectObject(hDC, pFontInfo->hFont);
                            cyLine = MAX(cyLine, pTm->tmHeight + pTm->tmExternalLeading + (int)aPIndentArray.GetAt(aPIndentArray.GetSize() - 1));
                        }
                        break;
                        }
                        while (*pstrText != _T('\0') && *pstrText != _T('>') && *pstrText != _T('}')) pstrText = ::CharNext(pstrText);
                        pstrText = ::CharNext(pstrText);
                    } else if (!bInRaw && *pstrText == _T('<') && pstrText[2] == _T('>') && (pstrText[1] == _T('{') || pstrText[1] == _T('}'))) {
                        SIZE szSpace = { 0 };
                        ::GetTextExtentPoint32(hDC, &pstrText[1], 1, &szSpace);
                        if (bDraw && bLineDraw) ::TextOut(hDC, pt.x, pt.y + cyLineHeight - pTm->tmHeight - pTm->tmExternalLeading, &pstrText[1], 1);
                        pt.x += szSpace.cx;
                        cxMaxWidth = MAX(cxMaxWidth, pt.x);
                        pstrText++; pstrText++; pstrText++;
                    } else if (!bInRaw && *pstrText == _T('{') && pstrText[2] == _T('}') && (pstrText[1] == _T('<') || pstrText[1] == _T('>'))) {
                        SIZE szSpace = { 0 };
                        ::GetTextExtentPoint32(hDC, &pstrText[1], 1, &szSpace);
                        if (bDraw && bLineDraw) ::TextOut(hDC, pt.x, pt.y + cyLineHeight - pTm->tmHeight - pTm->tmExternalLeading, &pstrText[1], 1);
                        pt.x += szSpace.cx;
                        cxMaxWidth = MAX(cxMaxWidth, pt.x);
                        pstrText++; pstrText++; pstrText++;
                    } else if (!bInRaw && *pstrText == _T(' ')) {
                        SIZE szSpace = { 0 };
                        ::GetTextExtentPoint32(hDC, _T(" "), 1, &szSpace);
                        // Still need to paint the space because the font might have
                        // underline formatting.
                        if (bDraw && bLineDraw) ::TextOut(hDC, pt.x, pt.y + cyLineHeight - pTm->tmHeight - pTm->tmExternalLeading, _T(" "), 1);
                        pt.x += szSpace.cx;
                        cxMaxWidth = MAX(cxMaxWidth, pt.x);
                        pstrText++;
                    } else {
                        POINT ptPos = pt;
                        int cchChars = 0;
                        int cchSize = 0;
                        int cchLastGoodWord = 0;
                        int cchLastGoodSize = 0;
                        LPCTSTR p = pstrText;
                        LPCTSTR pstrNext;
                        SIZE szText = { 0 };
                        if (!bInRaw && *p == _T('<') || *p == _T('{')) p++, cchChars++, cchSize++;
                        while (*p != _T('\0') && *p != _T('\n')) {
                            // This part makes sure that we're word-wrapping if needed or providing support
                            // for DT_END_ELLIPSIS. Unfortunately the GetTextExtentPoint32() call is pretty
                            // slow when repeated so often.
                            // TODO: Rewrite and use GetTextExtentExPoint() instead!
                            if (bInRaw) {
                                if ((*p == _T('<') || *p == _T('{')) && p[1] == _T('/')
                                    && p[2] == _T('r') && (p[3] == _T('>') || p[3] == _T('}'))) {
                                    p += 4;
                                    bInRaw = false;
                                    break;
                                }
                            } else {
                                if (*p == _T('<') || *p == _T('{')) break;
                            }
                            pstrNext = ::CharNext(p);
                            cchChars++;
                            cchSize += (int)(pstrNext - p);
                            szText.cx = cchChars * pTm->tmMaxCharWidth;
                            if (pt.x + szText.cx >= rc.right) {
                                ::GetTextExtentPoint32(hDC, pstrText, cchSize, &szText);
                            }
                            if (pt.x + szText.cx > rc.right) {
                                if (pt.x + szText.cx > rc.right && pt.x != rc.left) {
                                    cchChars--;
                                    cchSize -= (int)(pstrNext - p);
                                }
                                if ((uStyle & DT_WORDBREAK) != 0 && cchLastGoodWord > 0) {
                                    cchChars = cchLastGoodWord;
                                    cchSize = cchLastGoodSize;
                                }
                                if ((uStyle & DT_END_ELLIPSIS) != 0 && cchChars > 0) {
                                    cchChars -= 1;
                                    LPCTSTR pstrPrev = ::CharPrev(pstrText, p);
                                    if (cchChars > 0) {
                                        cchChars -= 1;
                                        pstrPrev = ::CharPrev(pstrText, pstrPrev);
                                        cchSize -= (int)(p - pstrPrev);
                                    } else
                                        cchSize -= (int)(p - pstrPrev);
                                    pt.x = rc.right;
                                }
                                bLineEnd = true;
                                cxMaxWidth = MAX(cxMaxWidth, pt.x);
                                break;
                            }
                            if (!((p[0] >= _T('a') && p[0] <= _T('z')) || (p[0] >= _T('A') && p[0] <= _T('Z')))) {
                                cchLastGoodWord = cchChars;
                                cchLastGoodSize = cchSize;
                            }
                            if (*p == _T(' ')) {
                                cchLastGoodWord = cchChars;
                                cchLastGoodSize = cchSize;
                            }
                            p = ::CharNext(p);
                        }

                        ::GetTextExtentPoint32(hDC, pstrText, cchSize, &szText);
                        if (bDraw && bLineDraw) {
                            if ((uStyle & DT_SINGLELINE) == 0 && (uStyle & DT_CENTER) != 0) {
                                ptPos.x += (rc.right - rc.left - szText.cx) / 2;
                            } else if ((uStyle & DT_SINGLELINE) == 0 && (uStyle & DT_RIGHT) != 0) {
                                ptPos.x += (rc.right - rc.left - szText.cx);
                            }
                            ::TextOut(hDC, ptPos.x, ptPos.y + cyLineHeight - pTm->tmHeight - pTm->tmExternalLeading, pstrText, cchSize);
                            if (pt.x >= rc.right && (uStyle & DT_END_ELLIPSIS) != 0)
                                ::TextOut(hDC, ptPos.x + szText.cx, ptPos.y, _T("..."), 3);
                        }
                        pt.x += szText.cx;
                        cxMaxWidth = MAX(cxMaxWidth, pt.x);
                        pstrText += cchSize;
                    }

                    if (pt.x >= rc.right || *pstrText == _T('\n') || *pstrText == _T('\0')) bLineEnd = true;
                    if (bDraw && bLineEnd) {
                        if (!bLineDraw) {
                            aFontArray.Resize(aLineFontArray.GetSize());
                            ::CopyMemory(aFontArray.GetData(), aLineFontArray.GetData(), aLineFontArray.GetSize() * sizeof(LPVOID));
                            aColorArray.Resize(aLineColorArray.GetSize());
                            ::CopyMemory(aColorArray.GetData(), aLineColorArray.GetData(), aLineColorArray.GetSize() * sizeof(LPVOID));
                            aPIndentArray.Resize(aLinePIndentArray.GetSize());
                            ::CopyMemory(aPIndentArray.GetData(), aLinePIndentArray.GetData(), aLinePIndentArray.GetSize() * sizeof(LPVOID));

                            cyLineHeight = cyLine;
                            pstrText = pstrLineBegin;
                            bInRaw = bLineInRaw;
                            bInSelected = bLineInSelected;

                            DWORD clrColor = dwTextColor;
                            if (aColorArray.GetSize() > 0) clrColor = (int)aColorArray.GetAt(aColorArray.GetSize() - 1);
                            ::SetTextColor(hDC, RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
                            TFontInfo* pFontInfo = (TFontInfo*)aFontArray.GetAt(aFontArray.GetSize() - 1);
                            if (pFontInfo == NULL) pFontInfo = pManager->GetDefaultFontInfo();
                            pTm = &pFontInfo->tm;
                            ::SelectObject(hDC, pFontInfo->hFont);
                            if (bInSelected) ::SetBkMode(hDC, OPAQUE);
                        } else {
                            aLineFontArray.Resize(aFontArray.GetSize());
                            ::CopyMemory(aLineFontArray.GetData(), aFontArray.GetData(), aFontArray.GetSize() * sizeof(LPVOID));
                            aLineColorArray.Resize(aColorArray.GetSize());
                            ::CopyMemory(aLineColorArray.GetData(), aColorArray.GetData(), aColorArray.GetSize() * sizeof(LPVOID));
                            aLinePIndentArray.Resize(aPIndentArray.GetSize());
                            ::CopyMemory(aLinePIndentArray.GetData(), aPIndentArray.GetData(), aPIndentArray.GetSize() * sizeof(LPVOID));
                            pstrLineBegin = pstrText;
                            bLineInSelected = bInSelected;
                            bLineInRaw = bInRaw;
                        }
                    }

                    ASSERT(iLinkIndex <= nLinkRects);
                }

                nLinkRects = iLinkIndex;

                // Return size of text when requested
                if ((uStyle & DT_CALCRECT) != 0) {
                    rc.bottom = MAX(cyMinHeight, pt.y + cyLine);
                    rc.right = MIN(rc.right, cxMaxWidth);
                }

                if (bDraw) ::SelectClipRgn(hDC, hOldRgn);
                ::DeleteObject(hOldRgn);
                ::DeleteObject(hRgn);

                ::SelectObject(hDC, hOldFont);
            }

        }
    }
}