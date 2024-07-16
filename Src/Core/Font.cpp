#include "Core/Font.h"

namespace PPEngine {
    namespace Core {
        FontManager* Core::Singleton<FontManager>::instance_ = nullptr;

        Font::Font(const std::string& name)
            : name_(name) {

        }

        Font::Font(const std::string& name, int32 size)
            : Font(name) {
            size_ = size;
        }

        Font::Font(const std::string& name, int32 size, bool bold)
            : Font(name, size) {
            bold_ = bold;
        }

        Font::Font(const std::string& name, int32 size, bool bold, bool underline)
            : Font(name, size, bold) {
            underline_ = underline;
        }

        Font::Font(const std::string& name, int32 size, bool bold, bool underline, bool italic)
            : Font(name, size, bold, underline) {
            italic_ = italic;
        }

        Font::~Font() {
            int n = 0;
            n = 1;
        }

        bool FontManager::Initialize() {
            return true;
        }

        void FontManager::Uninitialize() {
        }

        Font* FontManager::GetDefaultFontInfo() {
            if (!defaultFont_) {
                return nullptr;
            }

            defaultFont_->Active();
            return defaultFont_;
        }

        Font* FontManager::GetDefaultFontInfo() const {
            if (defaultFont_) {
                return defaultFont_;
            } else {
                return sharedFont_;
            }
        }

        Font* FontManager::GetFont(const std::string& name, int32 nSize, bool bBold, bool bUnderline, bool bItalic) {
            for (const auto& item : fonts_) {
                Font* font = item.second;
                if (font->GetName() == name && font->GetSize() == nSize
                    && font->IsBold() == bBold && font->IsUnderLine() == bUnderline
                    && font->IsItalic() == bItalic) {
                    return font;
                }
            }
            return nullptr;
        }

        Font* FontManager::GetFont(int32 id) {
            auto itor = fonts_.find(id);
            return fonts_.end() != itor ? itor->second : sharedFonts_[id];
        }

        int32 FontManager::GetFontCount(bool shared) const {
            return shared ? sharedFonts_.size() : fonts_.size();
        }

        void FontManager::SetDefaultFont(Font* font, bool shared) {
            if (shared) {
                sharedFont_ = font;
            }
            else {
                defaultFont_ = font;
            }
        }

        bool FontManager::Add(int32 id, Font* font, bool shared) {
            if (shared) {
                if (-1 == id) {
                    id = sharedFonts_.size();
                }
                sharedFonts_.insert(std::make_pair(id, std::move(font)));
            } else {
                if (-1 == id) {
                    id = fonts_.size();
                }
                fonts_.insert(std::make_pair(id, std::move(font)));
            }
            return true;
        }

        bool FontManager::Add(Font* font, bool shared) {
            if (shared) {
                sharedFonts_.insert(std::make_pair(sharedFonts_.size(), std::move(font)));
            } else {
                fonts_.insert(std::make_pair(fonts_.size(), std::move(font)));
            }
            return true;
        }

        void FontManager::Remove(int32 id, bool shared) {
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