#pragma once

#if WIN32
#include <Windows.h>
#endif

#include <string>
#include <memory>
#include <unordered_map>

#include "Core/Object/Object.h"
#include "Core/Object/SharedPtr.h"
#include "Core/Singleton.h"

namespace PPEngine {
    namespace Core {
        class Font : public Object {
        public:

        public:
            explicit Font(const std::string& name);
            explicit Font(const std::string& name, int32 size);
            explicit Font(const std::string& name, int32 size, bool bold);
            explicit Font(const std::string& name, int32 size, bool bold, bool underline);
            explicit Font(const std::string& name, int32 size, bool bold, bool underline, bool italic);

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

        protected:
            ~Font() override = default;

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

            IMPLEMENT_OBJECT_REFCOUN(Font)
        };

        class FontManager : public Singleton<FontManager> {
        public:
            FontManager() = default;
            ~FontManager() override = default;

        public:
            bool Initialize() override;
            void Uninitialize() override;

            Font* GetDefaultFontInfo();

            void SetDefaultFont(Font* font, bool shared);
            bool Add(int32 id, Font* font, bool shared);
            bool Add(Font* font, bool shared);
            void Remove(int32 id, bool shared);
            void Clear(bool shared);

            Font* GetDefaultFontInfo() const;
            Font* GetFont(int32 id);
            Font* GetFont(const std::string& name, int32 nSize, bool bBold, bool bUnderline, bool bItalic);

            int32 GetFontCount(bool shared) const;

        private:
            Font* defaultFont_;
            Font* sharedFont_;
            std::unordered_map<int32, Core::SharedPtr<Font>> sharedFonts_;
            std::unordered_map<int32, Core::SharedPtr<Font>> fonts_;
        };
    }
}