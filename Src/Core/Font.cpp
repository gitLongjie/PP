#include "Core/Font.h"

namespace PPEngine {
    namespace Core {
        FontManager* Core::Singleton<FontManager>::instance_ = nullptr;

        Font::Font(const std::string& name) 
            : name_(name) {

        }

        Font::Font(const std::string& name, int32_t size)
            : Font(name) {
            size_ = size;
        }

        Font::Font(const std::string& name, int32_t size, bool bold)
            : Font(name, size) {
            bold_ = bold;
        }

        Font::Font(const std::string& name, int32_t size, bool bold, bool underline)
            : Font(name, size, bold){
            underline_ = underline;
        }

        Font::Font(const std::string& name, int32_t size, bool bold, bool underline, bool italic)
            : Font(name, size, bold, underline) {
            italic_ = italic;
        }

        bool FontManager::Initialize() {
            return true;
        }

        void FontManager::Uninitialize() {

        }

        Font::Ptr FontManager::GetDefaultFontInfo() {
            if (!defaultFont_) {
                return nullptr;
            }

            defaultFont_->Active();
            return defaultFont_;
        }

        Font::Ptr FontManager::GetDefaultFontInfo() const {
            if (defaultFont_) {
                return defaultFont_;
            } else {
                return sharedFont_;
            }
        }

        Font::Ptr FontManager::GetFont(int32_t id) {
            auto itor = fonts_.find(id);
            return fonts_.end() != itor ? itor->second : sharedFonts_[id];
        }

        int32_t FontManager::GetFontCount(bool shared) const {
            return shared ? sharedFonts_.size() : fonts_.size();
        }

        void FontManager::SetDefaultFont(Font::Ptr font, bool shared) {
            if (shared) {
                sharedFont_ = font;
            }
            else {
                defaultFont_ = font;
            }
        }

        bool FontManager::Add(int32_t id, Font::Ptr font, bool shared) {
            if (shared) {
                sharedFonts_.insert(std::make_pair(id, std::move(font)));
            } else {
                fonts_.insert(std::make_pair(id, std::move(font)));
            }
            return true;
        }

        void FontManager::Remove(int32_t id, bool shared) {
            if (shared) {
                sharedFonts_.erase(id);
            } else {
                fonts_.erase(id);
            }
        }

        void FontManager::Clear(bool shared) {
            if (shared) {
                sharedFonts_.clear();
            } else {
                fonts_.clear();
            }
        }

    }
}