#include "Core/StringUtil.h"

#include <string>

#include <Windows.h>

namespace PPEngine {
    namespace Core {
        bool EndsWith(const char* s, const char* part) {
            return (strstr(s, part) - s) == (strlen(s) - strlen(part));
        }

        bool FromUtf8(std::string& inout) {
            DWORD nWide = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)inout.c_str(), inout.size(), NULL, 0);

            LPWSTR w_str = new WCHAR[nWide + 1];
            ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)inout.c_str(), inout.size(), w_str, nWide);
            w_str[nWide] = L'\0';

            DWORD wide = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)w_str, nWide, NULL, 0, NULL, NULL);

            char* sz = new char[wide + 1];
            ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)w_str, nWide, sz, wide, NULL, NULL);
            sz[wide] = '\0';
            inout = std::string(sz);
            delete[] w_str;
            delete[] sz;
            
            return true;
        }
        uint32 StringToColor16(const char* color) {
            const char* acolor = color;
            if (*acolor == '#') {
                ++acolor;
            }

            char* endStr = nullptr;
            unsigned long uColor = strtoul(acolor, &endStr, 16);
            return uColor;
        }

        std::vector<std::string> Split(const std::string& s, const std::string& delimiter) {
            std::vector<std::string> tokens;
            size_t prev = 0, pos = 0;
            do {
                pos = s.find(delimiter, prev);
                if (pos == std::string::npos) {
                    pos = s.length();
                }
                std::string token = s.substr(prev, pos - prev);
                if (!token.empty()) {
                    tokens.push_back(token);
                }
                prev = pos + delimiter.length();
            } while (pos < s.length() && prev < s.length());
            return tokens;
        }
    }
}