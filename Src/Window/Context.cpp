#include "Window/Context.h"

#include "Core/Logger.h"
#include "Core/Platform.h"
#include "Core/StringUtil.h"

#include "Window/Control.h"


namespace PPEngine {
    namespace Window {

        static Context::DefaultInfo sharedDefaultInfo_;

        bool Context::Serialize(tinyxml2::XMLElement* root) {
            if (nullptr == root) {
                return false;
            }

            const char* name = root->Name();
            if (strcmp(name, "Window") == 0) {
                ParseAttribute(root);
            }

            tinyxml2::XMLElement* xmlElement = root->FirstChildElement();
            while (nullptr != xmlElement) {
                const char* name = xmlElement->Name();
                if (0 == strcmp(name, "Font")) {
                    ParseFontAttribute(xmlElement);
                } else if (0 == strcmp(name, "Default")) {

                } else {
                    Control::Ptr control = CreateControl(name);
                    if (nullptr == control) {
                        continue;
                    }

                    if (!control_) {
                        Attach(control);
                    }

                    control->Serialize(xmlElement);
                }
                xmlElement = xmlElement->NextSiblingElement();
            }

            return true;
        }

        void Context::AddFont(int id, Core::Font::Ptr font, bool shared) {
            Core::FontManager::Get()->Add(id, font, shared);
        }

        void Context::Invalidate(Core::Math::Rect& rect) {}

        void Context::RemovePostPaint(Control* control) {
            const auto& itor = std::find_if(postPaintControls_.begin(), postPaintControls_.end(), [control](Control* c) {
                return c == control;
            });
            if (itor != postPaintControls_.end()) {
                postPaintControls_.erase(itor);
            }
        }

        void Context::SetFocus(Control* control, bool focusWnd) {

        }

        void Context::DrawLine(const glm::vec2& start, const glm::vec2& end, int32_t size, unsigned long color, int nStyle) {
        }

        void Context::DrawRect(const Core::Math::Rect& rectPaint, int32_t size, unsigned long color) {
        }

        void Context::DrawRoundRect(const Core::Math::Rect& rectPaint, int32_t size, int32_t width, int32_t height, unsigned long color) {
        }

        void Context::DrawImageString(const Core::Math::Rect& rectPaint, const Core::Math::Rect& rect, const std::string& image) {
            // 1¡¢aaa.jpg
            // 2¡¢file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' 
            // mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false'
            std::string file = image;
            std::string imageResType;
            std::string res;
            int32_t restype = 0;
            Core::Math::Rect rcItem;
            Core::Math::Rect rcBmpPart;
            Core::Math::Rect rcCorner;

            uint32 mask = 0;
            uint8 fade = 0xff;

            bool hole = false;
            bool xtiled = false;
            bool ytiled = false;

            std::vector<std::string> ress = Core::Split(image, " ");
            for (const std::string& res : ress) {
                if ("file" == res || "res" == res) {
                    file = res;
                } else if ("restype" == res) {
                    imageResType = res;
                } else if ("dest" == res) {
                    rcItem = Core::Math::Rect::FromString(res.c_str());
                } else if ("source" == res) {
                    rcBmpPart = Core::Math::Rect::FromString(res.c_str());
                } else if ("corner" == res) {
                    rcCorner = Core::Math::Rect::FromString(res.c_str());
                } else if ("mask" == res) {
                    mask = Core::StringToColor16(res.c_str());
                } else if ("fade" == res) {
                    fade = strtoul(res.c_str(), nullptr, 10);
                } else if ("hole" == res) {
                    hole = atoi(res.c_str());
                } else if ("xtiled" == res) {
                    xtiled = atoi(res.c_str());
                } else if ("ytiled" == res) {
                    ytiled = atoi(res.c_str());
                }
            }

            Core::ImageDrawUI imageDrawUI = { rect, rectPaint, file, imageResType,
                rcItem, rcBmpPart, rcCorner, mask, fade, hole, xtiled, ytiled };
            DrawImage(imageDrawUI);
        }

        bool Context::DrawImage(Core::ImageDrawUI& imageDrawUI) {
            if (imageDrawUI.name.empty()) {
                DEBUGLOG("image name is empty");
                return false;
            }
            
            Core::Image::Ptr image = Core::ImageManager::Get()->GetImageEx(imageDrawUI.name, imageDrawUI.type, imageDrawUI.mask);
            if (!image) {
                DEBUGLOG("image in nullptr");
                return false;
            }

            Core::Math::Rect& rcBmpPart = imageDrawUI.rcBmpPart;
            if (rcBmpPart.IsEmpty()) {
                rcBmpPart.SetSize({ image->GetWidth(), image->GetHeight() });
            }
            if (rcBmpPart.GetRight() > image->GetWidth()) rcBmpPart.SetRight(image->GetWidth());
            if (rcBmpPart.GetBottom() > image->GetHeight()) rcBmpPart.SetBottom(image->GetHeight());

            if (!imageDrawUI.rcItem.Intersects(imageDrawUI.rc)) { return true; }
            if (!imageDrawUI.rcItem.Intersects(imageDrawUI.rcPaint)) { return true; }

            return DrawImage(imageDrawUI, image);
        }

        bool Context::DrawImage(Core::ImageDrawUI& imageDrawUI, Core::Image::Ptr image){
            return false;
        }

        void Context::DrawColor(const Core::Math::Rect& rect, unsigned long color1) {

        }

        void Context::DrawGradient(const Core::Math::Rect& rect, unsigned long color1, unsigned long color2, bool vertical, int32_t steps) {

        }

        void Context::DrawUIText(const Core::Math::Rect& rect, const std::string& text, unsigned long color, int32_t font, uint32_t style) {

        }

        void Context::SetDefaultTextColor(unsigned long color, bool shared) {
            if (shared) {
                if (defaultInfo_.fontColor_ == sharedDefaultInfo_.fontColor_) {
                    defaultInfo_.fontColor_ = color;
                }
                sharedDefaultInfo_.fontColor_ = color;
            } else {
                defaultInfo_.fontColor_ = color;
            }
        }

        void Context::SetDisabledColor(unsigned long color, bool shared) {
            if (shared) {
                if (defaultInfo_.disabledColor_ == sharedDefaultInfo_.disabledColor_) {
                    defaultInfo_.disabledColor_ = color;
                }
                sharedDefaultInfo_.disabledColor_ = color;
            } else {
                defaultInfo_.disabledColor_ = color;
            }
        }

        //void Context::GenerateRoundClip(const Core::Math::Rect& rect, const Core::Math::Rect& rcItem, int width, int height) {
        //}

        void Context::InitControl(Control::Ptr control, Control* parent) {
            if (nullptr == control) { return; }

            control->SetContext(this, parent);
            controls_.insert(control);
        }

        void Context::UninitContorl(Control::Ptr control) {
            controls_.erase(control);
        }

        bool Context::Attach(Window::Control::Ptr control) {
            if (nullptr == control) {
                return false;
            }

            control->SetContext(this, nullptr);
            control_ = control;
            return true;
        }

        void Context::ParseAttribute(tinyxml2::XMLElement* root) {
            const char* sizeValue = root->Attribute("size");
            SetInitSize(Core::Math::FromString(sizeValue));

            const char* roundCornerValue = root->Attribute("roundcorner");
            SetRoundCorner(Core::Math::FromString(roundCornerValue));

            const char* maxInfoValue = root->Attribute("mininfo");
            SetMaxInfo(Core::Math::FromString(maxInfoValue));

            const char* captionValue = root->Attribute("caption");
            SetCaptionRect(Core::Math::Rect::FromString(captionValue));

            const char* sizeBoxValue = root->Attribute("sizebox");
            SetSizeBox(Core::Math::Rect::FromString(sizeBoxValue));
        }

        void Context::ParseFontAttribute(tinyxml2::XMLElement* root) {
            const tinyxml2::XMLAttribute* attribte = root->FirstAttribute();
            int32_t id = -1;
            bool shared = false;
            std::string name;
            int32_t size = 0;
            bool bold = false;
            bool underline = false;
            bool italic = false;
            bool defaultfont = false;

            while (nullptr != attribte) {
                const char* aName = root->Name();
                if (0 == strcmp(aName, "id")) {
                    id = attribte->IntValue();
                } else if (0 == strcmp(aName, "shared")) {
                    shared = attribte->BoolValue();
                } else if (0 == strcmp(aName, "name")) {
                    name = attribte->Value();
                } else if (0 == strcmp(aName, "size")) {
                    size = attribte->IntValue();
                } else if (0 == strcmp(aName, "bold")) {
                    bold = attribte->BoolValue();
                } else if (0 == strcmp(aName, "underline")) {
                    underline = attribte->BoolValue();
                } else if (0 == strcmp(aName, "italic")) {
                    italic = attribte->BoolValue();
                } else if (0 == strcmp(aName, "defaultfont")) {
                    defaultfont = attribte->BoolValue();
                }

                attribte = attribte->Next();
            }

            Core::Font::Ptr font = Core::Platform::Get()->CreatePlatformFont(name, size, bold, underline, italic);
            AddFont(id, font, shared);
            //addfont
        }

        void Context::SetInitSize(const Core::Math::Size& size) {
            
        }

    }
}
