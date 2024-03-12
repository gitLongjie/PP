#pragma once

#include "Window/Container.h"

namespace PPEngine {
    namespace Window {
        class HorizontalLayout : public Container {
        public:
            HorizontalLayout();
            ~HorizontalLayout() override;

            static Control::Ptr Create();

            const char* GetClass() const { return "HorizontalLayout"; }
            void SetAttribute(const char* name, const char* value) override;
            void FixRect(Core::Math::Rect rect) override;

            void SetSepWidth(int32_t width) { width_ = width; }
            int32_t GetSepWidth() const { return width_; }
            void SetSepImmMode(bool immediately);
            bool IsSepImmMode() const { return immediately_; }

        private:
            int32_t width_{ 0 };
            bool immediately_{ false };
            bool captured_{ false };
        };
    }
}