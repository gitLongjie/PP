#pragma once

#include <Windows.h>
#include <string>
#include <memory>
#include <unordered_map>

#include "Core/Constant.h"
#include "Core/Singleton.h"
#include "Core/Image.h"

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

                HBITMAP GetBitmap(const std::string& name);
                bool AddBitmap(Core::Image::Ptr image, uint32 mask = 0);

            private:
                HINSTANCE instanceHandle_{ nullptr };
                HINSTANCE resourceDll_{ nullptr };

                using Bitmaps = std::unordered_map<std::string, HBITMAP>;
                Bitmaps bitmaps_;
               
            };
        }
    }
}
