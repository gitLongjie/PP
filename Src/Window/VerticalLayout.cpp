#include "Window/VerticalLayout.h"

#include "Window/Container.h"
#include "Window/Context.h"


namespace PPEngine {
    namespace Window {
        VerticalLayout::VerticalLayout() : Container() {}

        VerticalLayout::~VerticalLayout() {}

        void VerticalLayout::SetAttribute(const char* name, const char* value) {
            if (0 == strcmp(name, "sepheight")) {
                SetSepHeight(atoi(value));
            } else if (0 == strcmp(name, "sepimm")) {
                SetSepImmMode(strcmp(value, "true") == 0);
            } else {
                Container::SetAttribute(name, value);
            }
        }

        void VerticalLayout::SetRect(const Core::Math::Rect& rect) {
            Container::SetRect(rect);

            Core::Math::Rect rc(rect_);

        }

        void VerticalLayout::SetSepImmMode(bool immediately) {
            if (immediately_ == immediately) return;
            if (captured_ && !immediately_ && nullptr != GetContext()) {
                GetContext()->RemovePostPaint(this);
            }

            immediately_ = immediately;
        }

    }
}
