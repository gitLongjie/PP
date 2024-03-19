#include "Core/FileSystem/Path.h"

#include <Windows.h>

#include <assert.h>

namespace PPEngine {
    namespace Core {
        namespace FileSystem {
            bool Path::FileExist(const std::string& file) {
                return GetFileAttributesA(file.c_str()) != INVALID_FILE_ATTRIBUTES;
            }
        }
    }
}
