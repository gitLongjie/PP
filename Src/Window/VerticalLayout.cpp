#include "Window/VerticalLayout.h"

#include "Window/Container.h"
#include "Window/Context.h"
#include "Window/ScrollBar.h"


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

            if (nullptr != GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible()) {
                rect.SetWidth(rect.GetWidth() - GetVerticalScrollBar()->GetRect().GetWidth());
            } if (nullptr != GetHorizontalScrollBar() && GetHorizontalScrollBar()->IsVisible()) {
                rect.SetHeight(rect.GetHeight() - GetVerticalScrollBar()->GetRect().GetHeight());
            }

            if (controls_.empty()) {
                ProcessScrollBar(rect, 0.0f, 0.0f);
                return;
            }

            for (auto& control : controls_) {
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
