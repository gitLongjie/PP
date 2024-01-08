#pragma once 

#include "Core/Platform.h"

namespace PPEngine {
    namespace Platforms {
        namespace Windows {
            class WinPlatform : public Core::Platform {
            public:
                virtual std::string GetPlatformName() override;
                Core::Font::Ptr CreateFont(const std::string& font, int nSize, bool bBold, bool bUnderline, bool bItalic) override;
            };
        }
    }
}