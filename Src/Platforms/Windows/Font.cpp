#include "Platforms/Windows/Font.h"

namespace PPEngine {
    Core::Font::Ptr Core::Font::Create(const std::string& font, int nSize, bool bBold, bool bUnderline, bool bItalic) {
        LOGFONT lf = { 0 };
        ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        strcpy(lf.lfFaceName, font.c_str());
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfHeight = -nSize;
        if (bBold) lf.lfWeight += FW_BOLD;
        if (bUnderline) lf.lfUnderline = TRUE;
        if (bItalic) lf.lfItalic = TRUE;
        HFONT hFont = ::CreateFontIndirect(&lf);
        if (hFont == nullptr) return nullptr;

        Platforms::Windows::Font::Ptr Self = std::make_shared<Platforms::Windows::Font>(font, nSize, bBold, bUnderline, bItalic);
        Self->SetFont(hFont);
        return Self;
    }

    namespace Platforms {
        namespace Windows {

            Font::Font(const std::string& name, int32_t size, bool bold, bool underline, bool italic)
                : Core::Font(name, size, bold, underline, italic) {

            }

            Font::~Font() {
                if (nullptr != font_) {
                    ::DeleteObject(font_);
                }
            }

            void Font::Active() {

            }

        }
    }
}
