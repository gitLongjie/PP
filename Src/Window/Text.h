#pragma once

#include "Window/Label.h"

namespace PPEngine {
    namespace Window {
        class Text : public Label {
        public:
            Text();
            ~Text() override;

            void SetAttribute(const char* name, const char* value) override;
            void SetRect(const Core::Math::Rect& rect) override;

        protected:
            void OnDrawText() override;
        };
    }
}
