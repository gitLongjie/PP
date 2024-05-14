#include "Window/Text.h"

namespace PPEngine {
    namespace Window {
        Text::Text() : Label() {
        }

        Text::~Text() {
        }

        Control::Ptr Text::Create() {
            return std::make_shared<Text>();
        }

        void Text::OnDrawText() {
            Label::OnDrawText();
        }

    }
}
