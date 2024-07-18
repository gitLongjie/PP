#pragma once

#include <string>

namespace Core {
    class FileUtils {
    public:
        static int64_t GetFileSize(const char* path);
    };
}
