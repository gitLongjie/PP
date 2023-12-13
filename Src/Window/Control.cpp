#include "Window/Control.h"

#include "Core/StringUtil.h"
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
                unsigned long uColor = Core::StringToColor16(value);
                SetBkColor(uColor);
            } else if (0 == strcmp(name, "bkcolor2")) {
                unsigned long uColor = Core::StringToColor16(value);
                SetBkColor2(uColor);
            } else if (0 == strcmp(name, "bkcolor3")) {
                unsigned long uColor = Core::StringToColor16(value);
                SetBkColor3(uColor);
            } else if (0 == strcmp(name, "bordersize")) {
                std::string border(value);
                if (std::string::npos == border.find(",")) {
                    SetBorderSize(atoi(value)); 
                } else {
                    SetBorderSize(Core::Math::Rect::FromString(value));
                }
            } else if (0 == strcmp(name, "borderround")) {
                SetBorderRound(Core::Math::FromString(value));
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

        void Control::SetBkColor2(unsigned long color) {
            if (bkColor2_ == color) { return; }

            bkColor2_ = color;
            Invalidate(); 
        }
        
        void Control::SetBkColor3(unsigned long color) {
            if (bkColor3_ == color) { return; }

            bkColor3_ = color;
            Invalidate(); 
        }

        void Control::SetBorderSize(const Core::Math::Rect& size) {
            borderRect_ = size;
            Invalidate();
        }

        void Control::SetBorderSize(int32_t size) {
            borderSize_ = size;
            Invalidate();
        }

        void Control::SetBorderRound(const Core::Math::Size& size) {
            if (borderRound_ == size) { return; }

            borderRound_ = size;
            Invalidate();
        }

        void Control::OnDrawBkColor() {
            if (0 != bkColor_) {
                if (0 != bkColor2_) {
                    if (0 != bkColor3_) {
                        Core::Math::Rect rect = rectPaint_;
                        rect.SetHeight(rect_.GetHeight() / 2);
                        context_->DrawGradient(rect, bkColor_, bkColor2_, true, 8);
                        rect.SetPosition(rect_.GetMin() + glm::vec2(0, rect_.GetHeight() / 2));
                        context_->DrawGradient(rect, bkColor2_, bkColor3_, true, 8);

                    } else {
                        context_->DrawGradient(rectPaint_, bkColor_, bkColor2_, true, 16);
                    }
                } else if (bkColor_ >= 0xFF000000) {
                    context_->DrawColor(rectPaint_, bkColor_);
                } else {
                    context_->DrawColor(rectPaint_, bkColor_);
                }
            }
        }

        void Control::OnDrawBkImage() {
            if (bkImage_.empty()) { return; }

            context_->DrawImageString(rectPaint_, rect_, bkImage_);
        }

        void Control::OnDrawStatusImage() {
        }

        void Control::OnDrawText() {
        }

        void Control::OnDrawBorder() {
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

        void Control::OnDraw(const Core::Math::Rect& rect) {
            if (!rect_.Intersects(rect)) {
                return;
            }

            rectPaint_ = rect_.CalIntersects(rect);

            if (borderRound_.x > 0 || borderRound_.y > 0) {
                context_->GenerateRoundClip(rectPaint_, rect_, borderRound_.x, borderRound_.y);
            }

            OnDrawBkColor();
            OnDrawBkImage();
            OnDrawStatusImage();
            OnDrawText();
        }

    }
}
