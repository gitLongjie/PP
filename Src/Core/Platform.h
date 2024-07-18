#pragma once

#include <string>

#include "Core/Singleton.h"
#include "Core/Font.h"

namespace Core {
    class Platform : public SingletonSub<Platform> {
    public:
        bool Initialize() override;
        void Uninitialize() override;

        virtual std::string GetPlatformName() = 0;
        virtual Font* CreatePlatformFont(const std::string& font, int nSize, bool bBold, bool bUnderline, bool bItalic) = 0;

        static Platform* Create();
        static uint64 GetTickCount64();
    };

}
