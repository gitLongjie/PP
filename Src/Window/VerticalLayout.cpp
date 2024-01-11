#include "Window/VerticalLayout.h"

#include "Window/Container.h"
#include "Window/Context.h"


namespace PPEngine {
    namespace Window {
        VerticalLayout::VerticalLayout() : Container() {}

        VerticalLayout::~VerticalLayout() {}

        Control::Ptr VerticalLayout::Create() {
            return std::make_shared<VerticalLayout>();
        }

        void VerticalLayout::SetAttribute(const char* name, const char* value) {
            if (0 == strcmp(name, "sepheight")) {
                SetSepHeight(atoi(value));
            } else if (0 == strcmp(name, "sepimm")) {
                SetSepImmMode(strcmp(value, "true") == 0);
            } else {
                Container::SetAttribute(name, value);
            }
        }

        void VerticalLayout::FixRect(Core::Math::Rect rect) {
            Container::FixRect(rect);

            rect = rect_;
            rect.Inset(rectInset_);

            if (controls_.empty()) {
                return;
            }

            Core::Math::Size available = rect.GetSize();

            for (auto control : controls_) {
                if (!control->IsVisible()) {
                    continue;
                }

                if (control->IsFloat()) {
                    continue;
                }


            }


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
