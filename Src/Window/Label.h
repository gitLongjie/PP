#pragma once

#include "Window/Control.h"

namespace PPEngine {
    namespace Window {
        class Label : public Control {
        public:
            Label();
            ~Label() override;

            void SetAttribute(const char* name, const char* value) override;
            void SetRect(const Core::Math::Rect& rect) override;
            void OnDraw(const Core::Math::Rect& rect) override;
        };
    }
}
