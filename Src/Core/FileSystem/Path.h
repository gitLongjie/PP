#pragma once

#include <string>

#include "Core/FileSystem/IFileImpl.h"

namespace PPEngine {
    namespace Core {
        namespace FileSystem {
            class Path {
            public:
                static std::string GetRootPath();
                static std::string GetAssetPath();
                static std::string GetEditorPath();
                static std::string GetEditorSkinPath();
                static std::string GetEditorImagePath();
                static bool FileExist(const std::string& file);
            };
        }
    }
}