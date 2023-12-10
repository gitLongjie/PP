#pragma once

#include <string>

namespace PPEngine {
    namespace Core {
        bool EndsWith(const char* s, const char* part);

        bool FromUtf8(std::string& inout);
    }
}