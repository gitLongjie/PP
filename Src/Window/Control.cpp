#include "Window/Control.h"

#include "Window/Context.h"

namespace PPEngine {
    namespace Window {

        Control::Control()
        {

        }

        Control::~Control()
        {

        }

        void Control::SetVisible(bool visible) {
            if (visible_ == visible) return;

            bool v = IsVisible();
            visible_ = visible;
            if (focused_) focused_ = false;
            if (!visible && context_ && context_->GetFocus() == this) {
                context_->SetFocus(nullptr, true);
            }
            if (IsVisible() != v) {
                NeedParentUpdate();
            }
        }

        void Control::SetAttribute(const char* name, const char* value) {
            if (0 == strcmp(name, "bkcolor") || 0 == strcmp(name, "bkcolor1")) {
                const char* color = value;
                if (*color == '#') {
                    ++color;
                }

                char* endStr = nullptr;
                unsigned long uColor = strtoul(color, &endStr, 16);
                SetBkColor(uColor);
            } else if (0 == strcmp(name, "bkcolor2")) {
                const char* color = value;
                if (*color == '#') {
                    ++color;
                }

                char* endStr = nullptr;
                unsigned long uColor = strtoul(color, &endStr, 16);
                SetBk2Color(uColor);
            }
        }

        void Control::Invalidate() {
            if (!IsVisible()) return;

            Core::Math::Rect invalidateRc = rect_;

            Control* parent = this;
            while (nullptr != (parent = parent->GetParent())) {
                const Core::Math::Rect& rcParent = parent->GetRect();
                if (!invalidateRc.Intersects(rcParent)) {
                    return;
                }

                invalidateRc = invalidateRc.CalIntersects(rcParent);
            }

            if (context_ != NULL) context_->Invalidate(invalidateRc);
        }

        bool Control::IsUpdateNeeded() const {
            return updateNeeded_;
        }

        void Control::NeedUpdate() {
            if (!IsVisible()) return;
            updateNeeded_ = true;
            Invalidate();

            if (nullptr != context_) context_->NeedUpdate();
        }

        void Control::NeedParentUpdate() {
            if (GetParent()) {
                GetParent()->NeedUpdate();
                GetParent()->Invalidate();
            } else {
                NeedUpdate();
            }

            if (nullptr != context_) context_->NeedUpdate();
        }

        void Control::SetEnabled(bool enable /*= true*/) {
            if (enable == enabled_) {
                return;
            }

            enabled_ = enable;
            Invalidate();
        }

        void Control::SetBkColor(unsigned long color) {
            if (bkColor_ == color) { return; }

            bkColor_ = color;
            Invalidate();
        }

        void Control::SetBk2Color(unsigned long color) {
            if (bk2Color_ == color) { return; }

            bk2Color_ = color;
            Invalidate(); 
        }

        void Control::OnDrawBkColor() {
            if (0 != bkColor_) {
                if (0 != bk2Color_) {
                    if (0 != bk3Color_) {
                        Core::Math::Rect rect = rect_;
                        rect.SetHeight(rect_.GetHeight() / 2);
                        context_->DrawGradient(rect, bkColor_, bk2Color_, true, 8);
                        rect.SetPosition(rect_.GetMin() + glm::vec2(0, rect_.GetHeight() / 2));
                        context_->DrawGradient(rect, bk2Color_, bk3Color_, true, 8);

                    } else {
                        context_->DrawGradient(rect_, bkColor_, bk2Color_, true, 16);
                    }
                } else if (bkColor_ >= 0xFF000000) {
                    context_->DrawColor(rect_, bkColor_);
                } else {
                    context_->DrawColor(rect_, bkColor_);
                }
            }
        }

        void Control::SetRect(const Core::Math::Rect& rect) {
            rect_ = rect;

            if (nullptr == context_) return;

            if (float_) {
                Control* parent = GetParent();
                if (nullptr != parent) {
                    Core::Math::Rect rectParent = parent->GetRect();
                    rect_ = rectParent + rect;

                    float width = rectParent.GetWidth();
                    float height = rectParent.GetHeight();
                    if (cXY_.x >= 0) cXY_.x = rect_.GetMin().x - rectParent.GetMin().x;
                    else cXY_.x = rect_.GetMax().x - rectParent.GetMax().x;
                    if (cXY_.y >= 0) cXY_.y = rect_.GetMin().y - rectParent.GetMin().y;
                    else cXY_.y = rect_.GetMax().y - rectParent.GetMax().y;
                    cxyFixed_.x = rect_.GetWidth();
                    cxyFixed_.y = rect_.GetHeight();
                }
            }

            updateNeeded_ = false;
            //invalidateRc.Join(m_rcItem);

            /*CControlUI* pParent = this;
            RECT rcTemp;
            RECT rcParent;
            while (pParent = pParent->GetParent()) {
                rcTemp = invalidateRc;
                rcParent = pParent->GetPos();
                if (!::IntersectRect(&invalidateRc, &rcTemp, &rcParent)) {
                    return;
                }
            }*/
            if (nullptr != context_) {
                context_->Invalidate(rect_);
            }
            
        }

        void Control::OnDraw() {
            OnDrawBkColor();
        }

    }
}
