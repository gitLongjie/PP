#include "Window/VerticalLayout.h"

#include "Core/Math/Utils.h"

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

            Core::Math::Size available = rect.GetSize();
            if (hscrollbar_ && hscrollbar_->IsVisible()) {
                available.x += hscrollbar_->GetScrollRange();
            }

            int32_t adjustables = 0;
            int32_t cyFixed = 0;
            int32_t estimateNum = 0;
            for (auto& control : controls_) {
                if (!control->IsVisible()) {
                    continue;
                }

                if (control->IsFloat()) {
                    continue;
                }

                Core::Math::Size sz = control->EstimateSize(available);
                if (0.0f == sz.y) {
                    adjustables++;
                } else {
                    if (sz.y < control->GetMinHeight()) sz.y = control->GetMinHeight();
                    if (sz.y > control->GetMaxHeight()) sz.y = control->GetMaxHeight();
                }
                cyFixed += sz.y + control->GetPadding().GetMin().y + control->GetPadding().GetMax().y;
                ++estimateNum;
            }

            cyFixed += (estimateNum - 1) * childPadding_;

            int32_t cyNeeded = 0;
            int32_t cyExpand = 0;
            if (adjustables > 0) {
                cyExpand = Core::Math::Max(0.0f, (available.y - cyFixed) / adjustables);
            }

            Core::Math::Size remaining = available;
            int32_t iPosY = rect.GetMin().y;
            if (vscrollbar_ && vscrollbar_->IsVisible()) {
                iPosY -= vscrollbar_->GetScrollPos();
            }
            int32_t iPosX = rect.GetMin().x;
            if (hscrollbar_ && hscrollbar_->IsVisible()) {
                iPosX -= hscrollbar_->GetScrollPos();
            }

            int32_t iAdjustable = 0;
            int32_t cyFixedRemaining = cyFixed;
            for (int32_t i = 0; i < controls_.size(); ++i) {
                Control::Ptr control = controls_[i];
                if (!control->IsVisible()) { continue; }
                if (control->IsFloat()) {
                    SetFloatRect(i);
                    continue;
                }

                Core::Math::Rect rcPadding = control->GetPadding();
                remaining.y -= rcPadding.GetTop();
                Core::Math::Size sz = control->EstimateSize(remaining);
                if (sz.y == 0) {
                    iAdjustable++;
                    sz.y = cyExpand;

                    if (iAdjustable == adjustables) {
                        sz.y = Core::Math::Max(0.0f, remaining.y - rcPadding.GetBottom() - cyFixedRemaining);
                    }
                    if (sz.y < control->GetMinHeight()) sz.y = control->GetMinHeight();
                    if (sz.y > control->GetMaxHeight()) sz.y = control->GetMaxHeight();
                } else {
                    if (sz.y < control->GetMinHeight()) sz.y = control->GetMinHeight();
                    if (sz.y > control->GetMaxHeight()) sz.y = control->GetMaxHeight();
                    cyFixedRemaining -= sz.y;
                }

                sz.x = control->GetFixedWidth();
                if (sz.x == 0) sz.x = available.x - rcPadding.GetLeft() - rcPadding.GetRight();
                if (sz.x < 0) sz.x = 0;
                if (sz.x < control->GetMinWidth()) sz.x = control->GetMinWidth();
                if (sz.x > control->GetMaxWidth()) sz.x = control->GetMaxWidth();

                Core::Math::Rect rcCtrl(iPosX + rcPadding.GetLeft(), iPosY + rcPadding.GetTop(), iPosX + rcPadding.GetLeft() + sz.x,
                    iPosY + sz.y + rcPadding.GetTop() + rcPadding.GetBottom());
                control->FixRect(rcCtrl);

                iPosY += sz.y + childPadding_ + rcPadding.GetTop() + rcPadding.GetBottom();
                cyNeeded += sz.y + rcPadding.GetTop() + rcPadding.GetBottom();
                remaining.y -= sz.y + childPadding_ + rcPadding.GetBottom();
            }
            cyNeeded += (estimateNum - 1) * childPadding_;
            ProcessScrollBar(rect, 0, cyNeeded);
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
