#include "Platforms/Windows/ResourceManager.h"

#include <assert.h>

#include "Core/Logger.h"


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

        HBITMAP ResourceManager::GetBitmap(const std::string& name) {
            if (bitmaps_.find(name) != bitmaps_.end()) {
                return bitmaps_[name];
            }
            return nullptr;
        }

        bool ResourceManager::AddBitmap(Core::Image::Ptr image, uint32 mask) {
            if (!image) {
                return false;
            }

            const uint64 bits = image->GetWidth() * image->GetHeight();
            BITMAPINFO bmi;
            ::ZeroMemory(&bmi, sizeof(BITMAPINFO));
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = image->GetWidth();
            bmi.bmiHeader.biHeight = -image->GetHeight();
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;
            bmi.bmiHeader.biSizeImage = bits * 4;

            bool bAlphaChannel = false;
            unsigned char* pDest = NULL;
            HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
            if (!hBitmap) {
                //::MessageBox(0, _T("CreateDIBSectionÊ§°Ü"), _T("×¥BUG"), MB_OK);
                LOGT("CreateDIBSection failed");
                return false;
            }

            uint8 *data = image->GetData();
            for (uint32 i = 0; i < bits; i++) {
                pDest[i * 4 + 3] = data[i * 4 + 3];
                if (pDest[i * 4 + 3] < 255) {
                    pDest[i * 4] = (BYTE)(DWORD(data[i * 4 + 2]) * data[i * 4 + 3] / 255);
                    pDest[i * 4 + 1] = (BYTE)(DWORD(data[i * 4 + 1]) * data[i * 4 + 3] / 255);
                    pDest[i * 4 + 2] = (BYTE)(DWORD(data[i * 4]) * data[i * 4 + 3] / 255);
                    bAlphaChannel = true;
                } else {
                    pDest[i * 4] = data[i * 4 + 2];
                    pDest[i * 4 + 1] = data[i * 4 + 1];
                    pDest[i * 4 + 2] = data[i * 4];
                }

                if (*(DWORD*)(&pDest[i * 4]) == mask) {
                    pDest[i * 4] = (BYTE)0;
                    pDest[i * 4 + 1] = (BYTE)0;
                    pDest[i * 4 + 2] = (BYTE)0;
                    pDest[i * 4 + 3] = (BYTE)0;
                    bAlphaChannel = true;
                }
            }

            bitmaps_[image->GetName()] = hBitmap;
            return true;
        }
    }
}
