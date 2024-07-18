#pragma once

#include <string>
#include <vector>

#include "Core/Constant.h"

namespace Core {
    bool EndsWith(const char* s, const char* part);

    bool FromUtf8(std::string& inout);
    uint32 StringToColor16(const char* color);
    std::vector<std::string> Split(const std::string& s, const std::string& delimiter);
    bool ToBool(std::string value);
}