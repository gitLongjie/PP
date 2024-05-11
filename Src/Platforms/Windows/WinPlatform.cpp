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
                Core::Font::Ptr platformFont = std::make_shared<Core::Font>(font, nSize, bBold, bUnderline, bItalic);
                LOGFONT lf = { 0 };
                ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
                strcpy(lf.lfFaceName, font.c_str());
                lf.lfCharSet = DEFAULT_CHARSET;
                lf.lfHeight = -nSize;
                if (bBold) lf.lfWeight += FW_BOLD;
                if (bUnderline) lf.lfUnderline = TRUE;
                if (bItalic) lf.lfItalic = TRUE;
                HFONT hFont = ::CreateFontIndirect(&lf);
                if (hFont == NULL) return nullptr;
                platformFont->SetFont(hFont);
                return platformFont;
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
