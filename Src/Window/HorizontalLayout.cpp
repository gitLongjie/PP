#include "Window/HorizontalLayout.h"

#include "Core/Math/Utils.h"

#include "Window/Container.h"
#include "Window/Context.h"
#include "Window/ScrollBar.h"

namespace PPEngine {
    namespace Window {
        HorizontalLayout::HorizontalLayout() : Container() {}

        HorizontalLayout::~HorizontalLayout() {}

        Control::Ptr HorizontalLayout::Create() {
            return std::make_shared<HorizontalLayout>();
        }

        void HorizontalLayout::SetAttribute(const char* name, const char* value) {
            if (0 == strcmp(name, "sepheight")) {
                SetSepWidth(atoi(value));
            } else if (0 == strcmp(name, "sepimm")) {
                SetSepImmMode(strcmp(value, "true") == 0);
            } else {
                Container::SetAttribute(name, value);
            }
        }

        void HorizontalLayout::FixRect(Core::Math::Rect rect) {
            Container::FixRect(rect);

            rect = rect_;
            rect.Inset(rectInset_);

            if (controls_.empty()) {
                ProcessScrollBar(rect, 0.0f, 0.0f);
                return;
            }

            if (vscrollbar_ && vscrollbar_->IsVisible()) rect.SetRight(rect.GetRight() - vscrollbar_->GetFixedWidth());
            if (hscrollbar_ && hscrollbar_->IsVisible()) rect.SetBottom(rect.GetBottom() - hscrollbar_->GetFixedHeight());

			// Determine the width of elements that are sizeable
			Core::Math::Size szAvailable(rect.GetSize());
			if (hscrollbar_ && hscrollbar_->IsVisible())
				szAvailable.x += hscrollbar_->GetScrollRange();

			int32_t nAdjustables = 0;
			int32_t cxFixed = 0;
			int32_t nEstimateNum = 0;
			for (int it1 = 0; it1 < controls_.size(); it1++) {
				Control::Ptr control = controls_[it1];
				if (!control->IsVisible()) continue;
				if (control->IsFloat()) continue;
				Core::Math::Size sz = control->EstimateSize(szAvailable);
				if (sz.x == 0) {
					nAdjustables++;
				}
				else {
					if (sz.x < control->GetMinWidth()) sz.x = control->GetMinWidth();
					if (sz.x > control->GetMaxWidth()) sz.x = control->GetMaxWidth();
				}
				cxFixed += sz.x + control->GetPadding().GetLeft() + control->GetPadding().GetRight();
				nEstimateNum++;
			}
			cxFixed += (nEstimateNum - 1) * childPadding_;

			int cxExpand = 0;
			int cxNeeded = 0;
			if (nAdjustables > 0) cxExpand = Core::Math::Max(0.0f, (szAvailable.x - cxFixed) / nAdjustables);
			// Position the elements
			Core::Math::Size szRemaining = szAvailable;
			int iPosX = rect.GetLeft();
			if (hscrollbar_ && hscrollbar_->IsVisible()) {
				iPosX -= hscrollbar_->GetScrollPos();
			}
			int32_t iAdjustable = 0;
			int32_t cxFixedRemaining = cxFixed;
			for (int32_t it2 = 0; it2 < controls_.size(); it2++) {
				Control::Ptr control = controls_[it2];
				if (!control->IsVisible()) continue;
				if (control->IsFloat()) {
					SetFloatRect(it2);
					continue;
				}
				Core::Math::Rect rcPadding = control->GetPadding();
				szRemaining.x -= rcPadding.GetLeft();
				Core::Math::Size sz = control->EstimateSize(szRemaining);
				if (sz.x == 0) {
					iAdjustable++;
					sz.x = cxExpand;
					// Distribute remaining to last element (usually round-off left-overs)
					if (iAdjustable == nAdjustables) {
						sz.x = Core::Math::Max(0.0f, szRemaining.x - rcPadding.GetRight() - cxFixedRemaining);
					}
					if (sz.x < control->GetMinWidth()) sz.x = control->GetMinWidth();
					if (sz.x > control->GetMaxWidth()) sz.x = control->GetMaxWidth();
				}
				else {
					if (sz.x < control->GetMinWidth()) sz.x = control->GetMinWidth();
					if (sz.x > control->GetMaxWidth()) sz.x = control->GetMaxWidth();

					cxFixedRemaining -= sz.x;
				}

				sz.y = control->GetFixedHeight();
				if (sz.y == 0) sz.y = rect.GetSize().y - rcPadding.GetSize().y;
				if (sz.y < 0) sz.y = 0;
				if (sz.y < control->GetMinHeight()) sz.y = control->GetMinHeight();
				if (sz.y > control->GetMaxHeight()) sz.y = control->GetMaxHeight();

				Core::Math::Rect rcCtrl = { iPosX + rcPadding.GetLeft(), rect.GetTop() + rcPadding.GetTop(),
					iPosX + sz.x + rcPadding.GetLeft() + rcPadding.GetRight(), rect.GetTop() + rcPadding.GetTop() + sz.y};
				control->FixRect(rcCtrl);
				iPosX += sz.x + childPadding_ + rcPadding.GetLeft() + rcPadding.GetRight();
				cxNeeded += sz.x + rcPadding.GetLeft() + rcPadding.GetRight();
				szRemaining.x -= sz.x + childPadding_ + rcPadding.GetRight();
			}
			cxNeeded += (nEstimateNum - 1) * childPadding_;

			// Process the scrollbar
			ProcessScrollBar(rect, cxNeeded, 0);

        }

        void HorizontalLayout::SetSepImmMode(bool immediately) {
            if (immediately_ == immediately) return;
            if (captured_ && !immediately_ && nullptr != GetContext()) {
                GetContext()->RemovePostPaint(this);
            }

            immediately_ = immediately;
        }

    }
}
