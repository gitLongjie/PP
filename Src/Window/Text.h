#pragma once

#include "Window/Label.h"

namespace PPEngine {
    namespace Window {
        class Text : public Label {
        public:
            Text();
            ~Text() override;

            static Control::Ptr Create();

            const char* GetClass() const { return "Text"; }

        protected:
            void OnDrawText() override;
        };
    }
}
