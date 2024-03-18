#include "Platforms/Windows/WinPlatform.h"

#include <assert.h>

#include "Platforms/Windows/ResourceManager.h"

namespace PPEngine {

    Core::Platform* Core::Platform::Create() {
        return new Platforms::Windows::WinPlatform();
    }

    namespace Platforms {
        namespace Windows {
            std::string WinPlatform::GetPlatformName() {
                return "Windows";
            }

            Core::Font::Ptr WinPlatform::CreatePlatformFont(const std::string& font, int nSize, bool bBold, bool bUnderline, bool bItalic) {
                return Core::Font::Create(font, nSize, bBold, bUnderline, bItalic);
            }

            bool WinPlatform::Initialize() {
                if (!ResourceManager::Init()) {
                 //   ResourceManager::Get()->SetInstanceHandle(GetModuleHandle(nullptr));
                    return false;
                }

                return Core::Platform::Initialize();
            }

            void WinPlatform::Uninitialize() {
                ResourceManager::Shotdown();
                Core::Platform::Uninitialize();
            }
        }
    }
}
