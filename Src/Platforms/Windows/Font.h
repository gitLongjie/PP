#pragma once

#include <Windows.h>

#include "Core/Font.h"

namespace PPEngine {
    namespace Platforms {
        namespace Windows {
            class Font : public Core::Font {
            public:
                using Ptr = std::shared_ptr<Font>;
            public:
                Font(const std::string& name, int32_t size, bool bold, bool underline, bool italic);
                ~Font() override;

                //static Font::Ptr Create(const std::string& font, int nSize, bool bBold, bool bUnderline, bool bItalic);

                void Active() override;

                HFONT GetFont() const { return font_; }
                void SetFont(HFONT font) { font_ = font; }

                const TEXTMETRIC& GetTEXTMETRIC() const { return tm_; }
                TEXTMETRIC& GetTEXTMETRIC() { return tm_; }

            private:
                HFONT font_{nullptr};
                TEXTMETRIC tm_;
            };
        }
    }
}
