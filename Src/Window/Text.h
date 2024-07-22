#pragma once

#include "Window/Label.h"

namespace Window {
    class Text : public Label {
    public:
        Text();
        ~Text() override;

        static Control* Create();

        const char* GetClass() const { return "Text"; }

    protected:
        void OnDrawText() override;
    };
}
