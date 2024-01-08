#include "Core/Platform.h"

namespace PPEngine {
    namespace Core {
        Platform* SingletonSub<Platform>::instance_ = nullptr;

        bool Platform::Initialize() {
            return true;
        }

        void Platform::Uninitialize() {
        }


    }
}