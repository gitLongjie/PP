#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "Core/Singleton.h"
#include "Core/Math/Rect.h"
#include "Core/Constent.h"

namespace PPEngine {
    namespace Core {
        struct ImageDrawUI {
            Math::Rect rc;
            Math::Rect rcPaint;
            std::string& name;
            std::string& type;
            Math::Rect rcItem;
            Math::Rect rcBmpPart;
            Math::Rect rcCorner;
            uint32 mask;
            uint8 fade;
            bool hole;
            bool xtiled;
            bool ytiled;
        };

        class Image {
        public:
            using Ptr = std::shared_ptr<Image>;

        public:
            explicit Image(const std::string& name);
            explicit Image (const std::string& name, int32 width, int32 hegiht, int32 strip, bool alpha, bool hsl, unsigned long mask = 0);

            virtual ~Image();

            static Ptr LoadImage(const std::string& name, bool hsl, unsigned long mask = 0);

            bool IsNameEmpty() const { return name_.empty(); }
            void Reload();

            int32 GetWidth() const { return width_; }
            int32 GetHeight() const { return height_; }

            bool IsAlpha() const { return alpha_; }

        private:
            std::string name_;
            int32 width_{ 0 };
            int32 height_{ 0 };
            int32 strip_{ 0 };
            bool alpha_{ false };
            bool hsl_{ false };
            uint32 mask_{ 0 };
            uint8* data_{ nullptr };
            uint32 count_{ 0 };
        };

        class ImageManager : public Singleton<ImageManager> {
        public:
            ImageManager() = default;
            ~ImageManager() override = default;

        public:
            bool Initialize() override;
            void Uninitialize() override;

            Image::Ptr GetImage(const std::string& bitmap) const;
            const Image::Ptr GetImageEx(const std::string&  bitmap, const std::string& type, uint32 mask = 0, bool bUseHSL = false);
            const Image::Ptr AddImage(const std::string&  bitmap, const std::string&  type, uint32 mask = 0, bool bUseHSL = false, bool bShared = false);
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