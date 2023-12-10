#pragma once

#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif // defined(_WIN32) || defined(_WIN64)

namespace PPEngine {
    namespace Core {
        struct FontInfo {
#if defined(_WIN32) || defined(_WIN64)
            HFONT hFont;
            TEXTMETRIC tm;
#else 
            using InstanceHandle = void*;
#endif // defined(_WIN32) || defined(_WIN64)
           
            std::string sFontName;
            int iSize;
            bool bBold;
            bool bUnderline;
            bool bItalic;
        } ;

    } // namespace Core
} // namespace name
