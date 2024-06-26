#include "Window/Control.h"

#include <unordered_map>

#include "Core/StringUtil.h"
#include "Window/Context.h"
#include "Window/Constent.h"

namespace PPEngine {
    namespace Window {
        static Control::Ptr emptyControl{ nullptr };
        static std::unordered_map<std::string, ControlCreator> controlCreators_;
        void RegisterControlCreator(const char* type, ControlCreator Creator) {
            controlCreators_[type] = std::move(Creator);
        }

        Control::Ptr CreateControl(const char* type) {
            if (controlCreators_.find(type) != controlCreators_.end()) {
                return controlCreators_[type]();
            }
            return Control::Ptr();
        }


        Control::Control()
        {

        }

        Control::~Control()
        {

        }

        Control::Ptr Control::Create() {
            return std::make_shared<Control>();
        }

        Control::Ptr& Control::EmptyControl() {
            return emptyControl;
        }

        bool Control::Serialize(tinyxml2::XMLElement* root) {
            const char* name = root->Name();
            if (0 != strcmp(GetClass(), name)) {
                return false;
            }

            const tinyxml2::XMLAttribute* current = root->FirstAttribute();
            while (nullptr != current) {
                const char* aName = current->Name();
                const char* aValue = current->Value();
                SetAttribute(aName, aValue);
                current = current->Next();
            }

            CreateControl(root);
            return true;
        }

        void Control::InitControl(Control* parent) {
            parent_ = parent;
            if (nullptr != context_) {
                context_->InitControl(shared_from_this(), parent);
            }
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
            if (0 == strcmp("pos", name)) {
                Core::Math::Rect rect = Core::Math::Rect::FromString(value);
                Core::Math::Size xy(rect.GetLeft() >= 0.0 ? rect.GetLeft() : rect.GetRight(),
                    rect.GetTop() >= 0.0f ? rect.GetTop() : rect.GetBottom());
                SetFixedXY(xy);
                SetFixedWidth(rect.GetWidth());
                SetFixedHeight(rect.GetHeight());
            }
            if (0 == strcmp(name, "bkcolor") || 0 == strcmp(name, "bkcolor1")) {
                unsigned long uColor = Core::StringToColor16(value);
                SetBkColor(uColor);
            } else if (0 == strcmp(name, "bkcolor2")) {
                unsigned long uColor = Core::StringToColor16(value);
                SetBkColor2(uColor);
            } else if (0 == strcmp(name, "bkcolor3")) {
                unsigned long uColor = Core::StringToColor16(value);
                SetBkColor3(uColor);
            } else if (0 == strcmp(name, "bordercolor")) {
                unsigned long uColor = Core::StringToColor16(value);
                SetBorderColor(uColor);
            } else if (0 == strcmp(name, "bordersize")) {
                std::string border(value);
                if (std::string::npos == border.find(",")) {
                    SetBorderSize(atoi(value));
                } else {
                    SetBorderSize(Core::Math::Rect::FromString(value));
                }
            } else if (0 == strcmp(name, "borderround")) {
                SetBorderRound(Core::Math::FromString(value));
            } else if (0 == strcmp(name, "bkimage")) {
                SetBkImage(value);
            } else if (strcmp(name, "width") == 0) {
                SetFixedWidth(atoi(value));
            } else if (strcmp(name, "height") == 0) {
                SetFixedHeight(atoi(value));
            } else if (strcmp(name, "minwidth") == 0) {
                SetMinWidth(atoi(value));
            } else if (strcmp(name, "minheight") == 0) {
                SetMinHeight(atoi(value));
            } else if (strcmp(name, "maxwidth") == 0) {
                SetMaxWidth(atoi(value));
            } else if (strcmp(name, "maxheight") == 0) {
                SetMaxHeight(atoi(value));
            } else if (strcmp(name, "name") == 0) {
              //  SetName(value);
            } else if (strcmp(name, "visible") == 0) {
                SetVisible(Core::ToBool(value));
            } else if (strcmp(name, "float") == 0) {
                SetFloat(Core::ToBool(value));
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

        void Control::SetFocuse() {
            if (nullptr == context_) { return; }

            context_->SetFocus(this, true);
        }

        unsigned long Control::GetColor(unsigned long color) {
            if (!isHSL_) { return color; }

            //short H = 0, S = 0, L = 0;
            return color;
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

        void Control::SetBorderColor(unsigned long color) {
            if (borderColor_ == color) { return; }

            borderColor_ = color;
            Invalidate();
        }

        void Control::SetBkImage(const std::string& bkImage) {
            if (bkImage_ == bkImage) {
                return;
            }

            bkImage_ = bkImage;
            Invalidate();
        }

        void Control::SetFocusBorderColor(unsigned long color) {
            if (focusBorderColor_ == color) { return; }

            focusBorderColor_ = color;
            Invalidate();
        }

        void Control::SetPadding(const Core::Math::Rect& padding) {
            rcPadding_ = padding;
            NeedParentUpdate();
        }

        void Control::SetFixedXY(const Core::Math::Size& cxy) {
            cXY_ = cxy;

            if (!float_) {
                NeedParentUpdate();
            } else {
                NeedUpdate();
            }
        }

        void Control::SetFixed(const Core::Math::Size& size) {
            if (cxyFixed_ == size) { return; }

            cxyFixed_ = size;
            if (!float_) {
                NeedParentUpdate();
            } else {
                NeedUpdate();
            }
        }

        void Control::SetFixedWidth(float width) {
            if (cxyFixed_.x == width) { return; }

            cxyFixed_.x = width;
            if (!float_) {
                NeedParentUpdate();
            } else {
                NeedUpdate();
            }
        }

        void Control::SetFixedHeight(float height) {
            if (cxyFixed_.y == height) { return; }

            cxyFixed_.y = height;
            if (!float_) {
                NeedParentUpdate();
            } else {
                NeedUpdate();
            }
        }

        void Control::SetMinWidth(float width) {
            if (cxyMin_.x == width) { return; }

            if (width < 0.0f) return;

            cxyMin_.x = width;
            if (!float_) {
                NeedParentUpdate();
            } else {
                NeedUpdate();
            }
        }

        void Control::SetMinHeight(float height) {
            if (cxyMin_.y == height) { return; }

            if (height < 0.0f) return;

            cxyMin_.y = height;
            if (!float_) {
                NeedParentUpdate();
            } else {
                NeedUpdate();
            }
        }

        void Control::SetMaxWidth(float width) {
            if (cxyMax_.x == width) { return; }

            if (width < 0.0f) return;

            cxyMax_.x = width;
            if (!float_) {
                NeedParentUpdate();
            } else {
                NeedUpdate();
            }
        }

        void Control::SetMaxHeight(float height) {
            if (cxyMax_.y == height) { return; }

            if (height < 0.0f) return;

            cxyMax_.y = height;
            if (!float_) {
                NeedParentUpdate();
            } else {
                NeedUpdate();
            }
        }

        bool Control::OnHandlerEvent(const Core::EventSystem::MouseMoveEvent& mouseMoveEvent) {
            return true;
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
            if (0 != borderColor_ || 0 != focusBorderColor_) {
                if (borderSize_ > 0 && (borderRound_.x > 0 || borderRound_.y > 0)) {
                    if (IsFocused() && 0 != focusBorderColor_) {
                        context_->DrawRoundRect(rectPaint_, borderSize_, borderRound_.x, borderRound_.y, focusBorderColor_);
                    } else {
                        context_->DrawRoundRect(rectPaint_, borderSize_, borderRound_.x, borderRound_.y, borderColor_);
                    }
                } else if (borderSize_ > 0) {
                    context_->DrawRect(rectPaint_, borderSize_, borderColor_);
                } else {
                    if (IsFocused() && 0 != focusBorderColor_) {
                        context_->DrawRect(rectPaint_, borderSize_, focusBorderColor_);
                    } else  if (borderRect_.GetMin().x > 0) {
                        const glm::vec2& start = rectPaint_.GetMin();
                        glm::vec2 end(start);
                        end.y = rectPaint_.GetMax().y;
                        context_->DrawLine(start, end, borderSize_, focusBorderColor_);
                    } else  if (borderRect_.GetMin().y > 0) {
                        const glm::vec2& start = rectPaint_.GetMin();
                        glm::vec2 end(start);
                        end.x = rectPaint_.GetMax().x;
                        context_->DrawLine(start, end, borderSize_, focusBorderColor_);
                    } else  if (borderRect_.GetMax().x > 0) {
                        const glm::vec2& start = rectPaint_.GetMax();
                        glm::vec2 end(start);
                        end.y = rectPaint_.GetMin().y;
                        context_->DrawLine(start, end, borderSize_, focusBorderColor_);
                    } else  if (borderRect_.GetMax().y > 0) {
                        const glm::vec2& start = rectPaint_.GetMax();
                        glm::vec2 end(start);
                        end.x = rectPaint_.GetMin().x;
                        context_->DrawLine(start, end, borderSize_, focusBorderColor_);
                    }
                }
            }
        }

        void Control::FixRect(Core::Math::Rect rect) {
            if (nullptr == context_) return;

            Core::Math::Rect invalidateRc = rect_;
            if (invalidateRc.IsEmpty()) {
                invalidateRc = rect;
            }

            rect_ = rect;

            if (float_) {
                Control* parent = GetParent();
                if (nullptr != parent) {
                    Core::Math::Rect rectParent = parent->GetRect();
                 //   rect_ = rectParent + rect;

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
            invalidateRc.Join(rect_);

            Control* parent = this;
            Core::Math::Rect rcParent;
            while ((parent = parent->GetParent()) != nullptr ) {
                rcParent = parent->GetRect();
                if (!invalidateRc.Intersects(rcParent)) {
                    return;
                }
            }
            context_->Invalidate(invalidateRc);
        }

        void Control::SetInternVisible(bool visible) {
            internVisible_ = visible;
            if (!visible && context_ && context_->GetFocus() == this) {
                context_->SetFocus(nullptr, true);
            }
        }

        void Control::OnDraw(const Core::Math::Rect& rect) {
            if (!rect_.Intersects(rect)) {
                return;
            }

            rectPaint_ = rect.CalIntersects(rect_);

            /*if (borderRound_.x > 0 || borderRound_.y > 0) {
                context_->GenerateRoundClip(rectPaint_, rect_, borderRound_.x, borderRound_.y);
            }*/

            OnDrawBkColor();
            OnDrawBkImage();
            OnDrawStatusImage();
            OnDrawText();
            OnDrawBorder();
        }

        Control* Control::FindControl(const Core::Math::Point2d& pt, uint32 flag) {
            if ((flag & UIFIND_VISIBLE) != 0 && !IsVisible()) return nullptr;
            if ((flag & UIFIND_ENABLED) != 0 && !IsEnabled()) return nullptr;
            if ((flag & UIFIND_HITTEST) != 0 && (!mouseEnabled_ || !rect_.Contains(pt))) return nullptr;
            if (!rect_.Contains(pt)) return nullptr;

            return this;
        }

        const Control* Control::FindControl(const Core::Math::Point2d& pt, uint32 flag) const {
            if ((flag & UIFIND_VISIBLE) != 0 && !IsVisible()) return nullptr;
            if ((flag & UIFIND_ENABLED) != 0 && !IsEnabled()) return nullptr;
            if ((flag & UIFIND_HITTEST) != 0 && (!mouseEnabled_ || !rect_.Contains(pt))) return nullptr;
            if (!rect_.Contains(pt)) return nullptr;

            return this;
        }

        void Control::CreateControl(tinyxml2::XMLElement* root) {
            tinyxml2::XMLElement* xmlElement = root->FirstChildElement();
            while (xmlElement) {
                const char* name = xmlElement->Name();
                Control::Ptr control = Window::CreateControl(name);
                if (nullptr == control) {
                    break;
                }

                control->InitControl(this);
                control->Serialize(xmlElement);

                xmlElement = xmlElement->NextSiblingElement();
            }
        }
    }
}
