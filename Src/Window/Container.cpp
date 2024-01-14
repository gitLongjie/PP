#include "Window/Container.h"

#include "Window/Context.h"

namespace PPEngine {
    namespace Window {
        Container::Container() {
           
        }

        Container::~Container() {
        }

		Control::Ptr Container::Create() {
			return std::make_shared<Container>();
		}

		void Container::SetAttribute(const char* name, const char* value) {
			if (strcmp(name, "inset") == 0) {
				Core::Math::Rect rcInset = Core::Math::Rect::FromString(value);
				SetInset(rcInset);
			} else if (strcmp(name, "mousechild") == 0) {
				SetMouseChildEnabled(strcmp(value, "true") == 0);
			} else if (strcmp(name, "vscrollbar") == 0) {
				//EnableScrollBar(strcmp(value, "true") == 0, GetHorizontalScrollBar() != NULL);
			} else if (strcmp(name, "vscrollbarstyle") == 0) {
				/*EnableScrollBar(true, GetHorizontalScrollBar() != NULL);
				if (GetVerticalScrollBar()) GetVerticalScrollBar()->ApplyAttributeList(value);*/
			} else if (strcmp(name, "hscrollbar") == 0) {
				//EnableScrollBar(GetVerticalScrollBar() != NULL, strcmp(value, "true") == 0);
			} else if (strcmp(name, "hscrollbarstyle") == 0) {
				/*EnableScrollBar(GetVerticalScrollBar() != NULL, true);
				if (GetHorizontalScrollBar()) GetHorizontalScrollBar()->ApplyAttributeList(value);*/
			} else if (strcmp(name, "childpadding") == 0) {
				SetChildPadding(atoi(value));
			} else {
				Control::SetAttribute(name, value);
			}
		}

		void Container::FixRect(Core::Math::Rect rect) {
			Control::FixRect(rect);
		}

		void Container::OnDraw(const Core::Math::Rect& rect) {
			Control::OnDraw(rect);

			for (auto& control : controls_) {
				if (control->IsVisible()) {
                    control->OnDraw(rect);
                }
            }
		}

		void Container::SetInternVisible(bool visible) {
			Control::SetInternVisible(visible);
			for (auto& control : controls_) {
				control->SetInternVisible(IsVisible());
			}
		}

        bool Container::AddControl(Control::Ptr control) {
			if (!control) return false;

			if (nullptr != context_) {
				context_->InitControl(control, this);
			}
			if (IsVisible()) {
				NeedUpdate();
			} else {
				control->SetInternVisible(false);
			}

			controls_.emplace_back(std::move(control));
			return true;
		}

        void Container::RemoveControl(Control::Ptr control) {
            if (!control) return;

			const auto itor = std::find(controls_.begin(), controls_.end(), control);
			if (itor != controls_.end()) {
				context_->UninitContorl(control);
				controls_.erase(itor);
			}
		}

		void Container::SetInset(const Core::Math::Rect& rect) {
			rectInset_ = rect;
			NeedUpdate();
		}

        void Container::SetChildPadding(int32_t padding) {
			childPadding_ = padding;
			if (childPadding_ < 0) {
				childPadding_ = 0;
			}

			NeedUpdate();
        }

        void Container::SetChildAlign(uint32_t align) {
			childAlign_ = align;
			NeedUpdate();
        }

        void Container::SetChildVAlign(uint32_t align) {
			childVAlign_ = align;
			NeedUpdate();
        }

		void Container::CreateControl(tinyxml2::XMLElement* root) {
			tinyxml2::XMLElement* xmlElement = root->FirstChildElement();
			while (xmlElement) {
				const char* name = xmlElement->Name();
				Control::Ptr control = Window::CreateControl(name);
				if (nullptr == control) {
					break;
				}

				AddControl(control);
				control->Serialize(xmlElement);

				xmlElement = xmlElement->NextSiblingElement();
			}
		}

        void Container::ProcessScrollBar(Core::Math::Rect rect, float cx, float cy) {
			if (nullptr != hscrollbar_) {
				if (nullptr == vscrollbar_) {

				}
			}
        }

    }
}

