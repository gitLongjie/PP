#pragma once

#include "Window/Label.h"

namespace PPEngine {
    namespace Window {
        class Button : public Label {
        public:
            Button();
            ~Button() override;

            static Control::Ptr Create();

            const char* GetClass() const { return "Button"; }
            void SetAttribute(const char* name, const char* value) override;

            void SetBkColor(uint32 bkColor) { bkColor_ = bkColor; }
            uint32 GetBkColor() const { return bkColor_; }
            void SetHotBkColor(uint32 hotBKColor) { hotBKColor_ = hotBKColor; }
            uint32 GetHotBkColor() const { return hotBKColor_; }
            void SetHotTextColor(uint32 hotTextColor) { hotTextColor_ = hotTextColor; }
            uint32 GetHotTextColor() const { return hotTextColor_; }
            void SetPushedTextColor(uint32 pushedTextColor) { pushedTextColor_ = pushedTextColor; }
            uint32 GetPushedTextColor() const { return pushedTextColor_; }
            void SetFoucedTextColor(uint32 foucedTextColor) { foucedTextColor_ = foucedTextColor; }
            uint32 GetFoucedTextColor() const { return foucedTextColor_; }

            void SetNormalImage(const char* normalImage);
            const std::string& GetNormalImage() const { return normalImage_; }
            void SetHotImage(const char* hotImage);
            const std::string& GetHotImage() const { return hotImage_; }
            void SetHotForeImage(const char* hotForeImage);
            const std::string& GetHotForeImage() const { return hotForeImage_; }
            void SetPushedImage(const char* pushedImage);
            const std::string& GetPushedImage() const { return pushedImage_; }
            void SetPushedForeImage(const char* pushedForeImage);
            const std::string& GetPushedForeImage() const { return pushedForeImage_; }
            void SetFoucesedImage(const char* foucesedImage);
            const std::string& GetFoucesedImage() const { return foucesedImage_; }
            void SetDisableImage(const char* disableImage);
            const std::string& GetDisableImage() const { return disableImage_; }
           

        protected:
            void OnDrawText() override;

        protected:
            uint32 hotBKColor_{ 0 };
            uint32 hotTextColor_{ 0 };
            uint32 pushedTextColor_{ 0 };
            uint32 foucedTextColor_{ 0 };

            std::string normalImage_;
            std::string hotImage_;
            std::string hotForeImage_;
            std::string pushedImage_;
            std::string pushedForeImage_;
            std::string foucesedImage_;
            std::string disableImage_;

            enum State : uint8 {
                FOCUSED = 1 << 0,
                PUSHED = 1 << 1,
                HOT = 1 << 2,
                DISABLED = 1 << 3,
            };
            uint8 state_{ FOCUSED };
        };
    }
}
