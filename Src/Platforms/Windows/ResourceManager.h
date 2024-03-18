#pragma once

#include <Windows.h>
#include <string>
#include <memory>

#include "Core/Constent.h"
#include "Core/Singleton.h"

namespace PPEngine {
    namespace Platforms {
        namespace Windows {
            class ResourceManager : public Core::Singleton<ResourceManager> {
            public:
                NON_COPYABLE(ResourceManager)

                 ResourceManager() = default;
                ~ResourceManager() = default;

                bool Initialize() override;
                void Uninitialize() override;

                static ResourceManager* Create();

                void SetInstanceHandle(HINSTANCE handle);
                HINSTANCE GetInstanceHandle();
                void SetResourceDll(HINSTANCE hInst);
                HINSTANCE GetResourceDll();

            private:
                HINSTANCE instanceHandle_;
                HINSTANCE resourceDll_;
               
            };
        }
    }
}
