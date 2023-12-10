#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "Core/Singleton.h"

namespace PPEngine {
    namespace Core {
        class Image {
        public:
            using Ptr = std::shared_ptr<Image>;

        public:
            explicit Image(const std::string& name);
            explicit Image (const std::string& name, int32_t width, int32_t hegiht, int32_t strip, bool alpha, bool hsl, unsigned long mask = 0);

            virtual ~Image();

            static Ptr LoadImage(const std::string& name, bool hsl, unsigned long mask = 0);

            bool IsNameEmpty() const { return name_.empty(); }
            void Reload();

        private:
            std::string name_;
            int32_t width_{ 0 };
            int32_t height_{ 0 };
            int32_t strip_{ 0 };
            bool alpha_{ false };
            bool hsl_{ false };
            unsigned long mask_{ 0 };
            uint8_t* data_{ nullptr };
            uint32_t count_{ 0 };
        };

        class ImageManager : public Singleton<ImageManager> {
        public:
            ImageManager() = default;
            ~ImageManager() override = default;

        public:
            bool Initialize() override;
            void Uninitialize() override;

            Image::Ptr GetImage(const std::string& bitmap) const;
            const Image::Ptr GetImageEx(const std::string&  bitmap, const std::string& type, unsigned long mask = 0, bool bUseHSL = false);
            const Image::Ptr AddImage(const std::string&  bitmap, const std::string&  type, unsigned long mask = 0, bool bUseHSL = false, bool bShared = false);
            void RemoveImage(const std::string&  bitmap, bool bShared = false);
            void RemoveAllImages(bool bShared = false);
            void ReloadSharedImages();
            void ReloadImages();

        private:
            std::unordered_map<std::string, Image::Ptr> images_;
            std::unordered_map<std::string, Image::Ptr> sharedImages_;
        };
    }
}