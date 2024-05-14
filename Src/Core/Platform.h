#pragma once

#include <string>

#include "Core/Singleton.h"
#include "Core/Font.h"

namespace PPEngine {
    namespace Core {
        class Platform : public SingletonSub<Platform> {
        public:
            bool Initialize() override;
            void Uninitialize() override;

            virtual std::string GetPlatformName() = 0;
            virtual Font::Ptr CreatePlatformFont(const std::string& font, int nSize, bool bBold, bool bUnderline, bool bItalic) = 0;

            static Platform* Create();
            static uint64 GetTickCount64();
        };

    } // namespace Core
} // namespace name
