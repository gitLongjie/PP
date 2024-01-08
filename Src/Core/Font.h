#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "Core/Singleton.h"

namespace PPEngine {
    namespace Core {
        class Font {
        public:
            using Ptr = std::shared_ptr<Font>;

        public:
            explicit Font(const std::string& name);
            explicit Font(const std::string& name, int32_t size);
            explicit Font(const std::string& name, int32_t size, bool bold);
            explicit Font(const std::string& name, int32_t size, bool bold, bool underline);
            explicit Font(const std::string& name, int32_t size, bool bold, bool underline, bool italic);

            virtual ~Font() = default;

            virtual void Active() = 0;

            bool IsNameEmpty() const { return name_.empty(); }

            static Font::Ptr Create(const std::string& font, int nSize, bool bBold, bool bUnderline, bool bItalic);

        private:
            std::string name_;
            int32_t size_{ 12 };
            bool bold_{ false };
            bool underline_{ false };
            bool italic_{false};
        };

        class FontManager : public Singleton<FontManager> {
        public:
            FontManager() = default;
            ~FontManager() override = default;

        public:
            bool Initialize() override;
            void Uninitialize() override;

            Font::Ptr GetDefaultFontInfo();

            void SetDefaultFont(Font::Ptr font, bool shared);
            bool Add(int32_t id, Font::Ptr font, bool shared);
            void Remove(int32_t id, bool shared);
            void Clear(bool shared);

            Font::Ptr GetDefaultFontInfo() const;
            Font::Ptr GetFont(int32_t id);

            int32_t GetFontCount(bool shared) const;

        private:
            Font::Ptr defaultFont_;
            Font::Ptr sharedFont_;
            std::unordered_map<int32_t, Font::Ptr> sharedFonts_;
            std::unordered_map<int32_t, Font::Ptr> fonts_;
        };
    }
}