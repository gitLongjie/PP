#include "Platforms/Windows/ResourceManager.h"

#include <assert.h>


namespace PPEngine {
    Platforms::Windows::ResourceManager* Core::Singleton<Platforms::Windows::ResourceManager>::instance_ = nullptr;

    namespace Platforms {
        namespace Windows {
            ResourceManager* ResourceManager::Create()
            {
                return nullptr;
            }

            bool ResourceManager::Initialize() {
                return true;
            }

            void ResourceManager::Uninitialize() {
            }

            void ResourceManager::SetInstanceHandle(HINSTANCE handle) {
                instanceHandle_ = handle;
            }

            HINSTANCE ResourceManager::GetInstanceHandle() {
                return instanceHandle_;
            }

            void  ResourceManager::SetResourceDll(HINSTANCE hInst) {
                resourceDll_ = hInst;
            }
            HINSTANCE ResourceManager::GetResourceDll() {
                if (nullptr == resourceDll_) return instanceHandle_;
                return resourceDll_;
            }



        }
    }
}