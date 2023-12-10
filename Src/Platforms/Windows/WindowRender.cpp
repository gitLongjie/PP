#include "Platforms/Windows/WindowRender.h"


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

            void WindowRender::DrawImage(HDC hdc, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint,
                const RECT& rcBmpPart, const RECT& rcScale9, bool alpha, uint8_t fade, bool hole, bool xtiled, bool ytiled) {
                typedef BOOL(WINAPI* LPALPHABLEND)(HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION);
                static LPALPHABLEND lpAlphaBlend = (LPALPHABLEND) ::GetProcAddress(::GetModuleHandle(TEXT("msimg32.dll")), "AlphaBlend");

                if (lpAlphaBlend == NULL) lpAlphaBlend = AlphaBitBlt;
                if (hBitmap == NULL) return;

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
            }

        }
    }
}