#include "Core/Platform.h"

#include "Core/Image.h"

namespace Core {
    Platform* SingletonSub<Platform>::instance_ = nullptr;

    bool Platform::Initialize() {
        if (!ImageManager::Init()) {
            return false;
        }
        return true;
    }

    void Platform::Uninitialize() {
    }
}
