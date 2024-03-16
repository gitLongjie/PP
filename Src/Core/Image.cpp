#include "Core/Image.h"

#include "stb/stb_image.h"

namespace PPEngine {
    namespace Core {
        ImageManager* Core::Singleton<ImageManager>::instance_ = nullptr;

        Image::Image(const std::string& name)
            : name_(name) {

        }

        Image::Image(const std::string& name, int32 width, int32 hegiht, int32 strip, bool alpha, bool hsl, unsigned long mask /*= 0*/)
            : name_(name)
            , width_(width)
            , height_(hegiht)
            , strip_(strip)
            , alpha_(alpha)
            , mask_(mask) {

        }

        Image::~Image() {
            if (nullptr != data_) {
                delete[] data_;
                data_ = nullptr;
            }
        }

        Image::Ptr Image::LoadImage(const std::string& name, bool hsl, unsigned long mask) {
            int32 width{ 0 }, height{ 0 }, strip{ 0 };
            uint8* image = stbi_load(name.c_str(), &width, &height, &strip, 0);

            Ptr self = std::make_shared<Image>(name, width, height, strip, strip == 4, hsl, mask);

            uint32 count = width * height * strip;
            self->data_ = new uint8[count]{ 0 };
            self->count_ = count;
            memcpy(self->data_, image, count);
            stbi_image_free(image);

            return self;
        }

        void Image::Reload() {

        }

        bool ImageManager::Initialize() {
            return true;
        }

        void ImageManager::Uninitialize()
        {

        }

        Image::Ptr ImageManager::GetImage(const std::string& bitmap) const {
            const auto itor = images_.find(bitmap);
            if (images_.end() == itor) {
                return nullptr;
            }

            return itor->second;
        }

        const Image::Ptr ImageManager::GetImageEx(const std::string& bitmap, const std::string& type /*= NULL*/, uint32 mask /*= 0*/, bool bUseHSL /*= false*/) {
            return nullptr;
        }

        const Image::Ptr ImageManager::AddImage(const std::string& bitmap, const std::string& type /*= NULL*/,
            uint32 mask /*= 0*/, bool bUseHSL /*= false*/, bool bShared /*= false*/) {
            Image::Ptr image = Image::LoadImage(bitmap, bUseHSL, mask);
            if (bShared) {
                sharedImages_.insert(std::make_pair(bitmap, image));
            } else {
                images_.insert(std::make_pair(bitmap, image));
            }
            return image;
        }

        void ImageManager::RemoveImage(const std::string& bitmap, bool bShared /*= false*/) {
            if (bShared) {
                sharedImages_.erase(bitmap);
            } else {
                images_.erase(bitmap);
            }
        }

        void ImageManager::RemoveAllImages(bool bShared /*= false*/) {
            if (bShared) {
                sharedImages_.clear();
            } else {
                images_.clear();
            }
        }

        void ImageManager::ReloadSharedImages() {
            for (auto& image : sharedImages_) {
                image.second->Reload();
            }
        }

        void ImageManager::ReloadImages() {
            for (auto& image : images_) {
                image.second->Reload();
            }
        }

    }
}