#pragma once

#if WIN32
#include <Windows.h>
#endif

#include <string>
#include <memory>
#include <unordered_map>

#include "Core/Constent.h"
#include "Core/Singleton.h"

namespace PPEngine {
    namespace Core {
        class Font {
        public:
            using Ptr = std::shared_ptr<Font>;

        public:
            explicit Font(const std::string& name);
            explicit Font(const std::string& name, int32 size);
            explicit Font(const std::string& name, int32 size, bool bold);
            explicit Font(const std::string& name, int32 size, bool bold, bool underline);
            explicit Font(const std::string& name, int32 size, bool bold, bool underline, bool italic);

            virtual ~Font() = default;

            virtual void Active() {}

            const std::string& GetName() const { return name_; }
            const int32 GetSize() const { return size_; }
            bool IsBold() const { return bold_; }
            bool IsNameEmpty() const { return name_.empty(); }
            bool IsUnderLine() const { return underline_; }
            bool IsItalic() const { return italic_; }

#if WIN32
            HFONT GetFont() const {
                return font_;
            }
            void SetFont(HFONT font) {
                font_ = font;
            }

            const TEXTMETRIC& GetTEXTMETRIC() const {
                return tm_;
            }
            TEXTMETRIC& GetTEXTMETRIC() {
                return tm_;
            }

#endif // WIN32

        private:
            std::string name_;
            int32 size_{ 12 };
            bool bold_{ false };
            bool underline_{ false };
            bool italic_{false};

#if WIN32
            HFONT font_{ nullptr };
            TEXTMETRIC tm_;
#endif
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
            bool Add(int32 id, Font::Ptr font, bool shared);
            bool Add(Font::Ptr font, bool shared);
            void Remove(int32 id, bool shared);
            void Clear(bool shared);

            Font::Ptr GetDefaultFontInfo() const;
            Font::Ptr GetFont(int32 id);
            Font::Ptr GetFont(const std::string& name, int32 nSize, bool bBold, bool bUnderline, bool bItalic);

            int32 GetFontCount(bool shared) const;

        private:
            Font::Ptr defaultFont_;
            Font::Ptr sharedFont_;
            std::unordered_map<int32, Font::Ptr> sharedFonts_;
            std::unordered_map<int32, Font::Ptr> fonts_;
        };
    }
}