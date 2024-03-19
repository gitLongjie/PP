#include "Core/Image.h"

#include "stb/stb_image.h"

#include "Core/FileSystem/Path.h"

namespace PPEngine {
    namespace Core {
        static bool GetImagePath(const std::string& file, std::string& path) {
            if (FileSystem::Path::FileExist(file)) {
                path = file;
                return true;
            }

            // is editor image file
            std::string editorImagePath = FileSystem::Path::GetEditorImagePath() + file;
            if (FileSystem::Path::FileExist(editorImagePath)) {
                path = editorImagePath;
                return true;
            }
            return false;
        }

        ImageManager* Core::Singleton<ImageManager>::instance_ = nullptr;

        Image::Image(const std::string& name)
            : name_(name) {

        }

        Image::Image(const std::string& name, int32 width, int32 hegiht, int32 strip, bool alpha, uint32 mask /*= 0*/)
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

        Image::Ptr Image::LoadImage(const std::string& name, uint32 mask) {
            int32 width{ 0 }, height{ 0 }, strip{ 0 };

            std::string filePath;
            if (!GetImagePath(name, filePath)) {
                return nullptr;
            }

            uint8* image = stbi_load(filePath.c_str(), &width, &height, &strip, 0);
            if (nullptr == image) {
                return nullptr;
            }

            Ptr self = std::make_shared<Image>(name, width, height, strip, strip == 4, mask);

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

        const Image::Ptr ImageManager::GetImageEx(const std::string& bitmap, const std::string& type /*= NULL*/, uint32 mask /*= 0*/) {
            if (images_.find(bitmap) != images_.end()) {
                return images_[bitmap];
            }
            return nullptr;
        }

        const Image::Ptr ImageManager::AddImage(const std::string& bitmap, const std::string& type, uint32 mask) {
            Image::Ptr image = Image::LoadImage(bitmap, mask);
            images_.insert(std::make_pair(bitmap, image));
            return image;
        }

        void ImageManager::RemoveImage(const std::string& bitmap) {
            images_.erase(bitmap);
        }

        void ImageManager::RemoveAllImages() {
            images_.clear();
        }

        void ImageManager::ReloadImages() {
            for (auto& image : images_) {
                image.second->Reload();
            }
        }

    }
}