#pragma once 

#include "Core/Platform.h"

namespace Platforms {
    namespace Windows {
        class WinPlatform : public Core::Platform {
        public:
            virtual std::string GetPlatformName() override;
            Core::Font* CreatePlatformFont(const std::string& font, int nSize, bool bBold, bool bUnderline, bool bItalic) override;

            bool Initialize() override;
            void Uninitialize() override;
        };
    }
}
