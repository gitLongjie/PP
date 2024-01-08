#include "Platforms/Windows/WinPlatform.h"

namespace PPEngine {

    Core::Platform* Core::Platform::Create() {
        return new Platforms::Windows::WinPlatform();
    }

    namespace Platforms {
        namespace Windows {
            std::string WinPlatform::GetPlatformName() {
                return "Windows";
            }

            Core::Font::Ptr WinPlatform::CreateFont(const std::string& font, int nSize, bool bBold, bool bUnderline, bool bItalic) {
                return Core::Font::Create(font, nSize, bBold, bUnderline, bItalic);
            }
        }
    }
}
