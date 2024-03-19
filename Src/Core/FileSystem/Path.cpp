#include "Core/FileSystem/Path.h"

#include <assert.h>

constexpr char* kRootPath = "../../";
constexpr char* kAssetPath = "Asset/";
constexpr char* kEditorRootPath = "Editor/";
constexpr char* kEditorSkinPath = "Skin/";
constexpr char* kEditorImagePath = "Images/";

namespace PPEngine {
    namespace Core {
        namespace FileSystem {
            std::string Path::GetRootPath() {
                return std::string(kRootPath);
            }

            std::string Path::GetAssetPath() {
                return GetRootPath() + kAssetPath;
            }

            std::string Path::GetEditorPath() {
                return GetAssetPath() + kEditorRootPath;
            }

            std::string Path::GetEditorSkinPath() {
                return GetEditorPath() + kEditorSkinPath;
            }

            std::string Path::GetEditorImagePath() {
                return GetEditorSkinPath() + kEditorImagePath;
            }
        }
    }
}
