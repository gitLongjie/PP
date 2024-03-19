#include "Window/Builder.h"

#include <string>

#include "Core/Logger.h"
#include "Core/StringUtil.h"
#include "Core/Math/Size.h"
#include "Core/FileSystem/FileHandle.h"
#include "Core/FileSystem/Path.h"

#include "Window/Control.h"
#include "Window/VerticalLayout.h"
#include "Window/HorizontalLayout.h"
#include "Window/Container.h"
#include "Window/Label.h"
#include "Window/Text.h"
#include "Window/Button.h"

#include "Platforms/Windows/Context.h"



namespace PPEngine {
    namespace Window {

        struct ImageVisitor : public tinyxml2::XMLVisitor {
            ~ImageVisitor() override = default;
            bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* attribute) override {
                const tinyxml2::XMLAttribute* first = attribute;
                while (nullptr != first) {
                    const char* aName = first->Name();
                    if (0 == strcmp("name", aName)) {
                        name_ = first->Value();
                    } else if (0 == strcmp("restype", aName)) {
                        resType_ = first->Value();
                    } else if (0 == strcmp("mask", aName)) {
                        std::string mask = first->Value();
                        const char* v = mask.c_str();
                        if (!mask.empty() && '#' == mask[0]) {
                            ++v;
                        }

                        char* last = nullptr;
                        mask_ = strtoul(v, &last, 16);
                    } else if (0 == strcmp("shared", aName)) {
                        shared_ = first->BoolValue();
                    }

                    first = first->Next();
                }
                return true;
            }

            std::string name_;
            std::string resType_;
            unsigned long mask_{ 0 };
            bool shared_{ false };

        };

        struct FontVisitor : public tinyxml2::XMLVisitor {
            ~FontVisitor() override = default;
            bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* firstAttribute) override {
                const tinyxml2::XMLAttribute* root = firstAttribute;
                while (nullptr != root) {
                    const char* aName = root->Name();
                    if (0 == strcmp(aName, "id")) {
                        id = root->IntValue();
                    } else if (0 == strcmp(aName, "shared")) {
                        shared = root->BoolValue();
                    } else if (0 == strcmp(aName, "name")) {
                        name = root->Value();
                    } else if (0 == strcmp(aName, "size")) {
                        size = root->IntValue();
                    } else if (0 == strcmp(aName, "bold")) {
                        bold = root->BoolValue();
                    } else if (0 == strcmp(aName, "underline")) {
                        underline = root->BoolValue();
                    } else if (0 == strcmp(aName, "italic")) {
                        italic = root->BoolValue();
                    } else if (0 == strcmp(aName, "defaultfont")) {
                        defaultfont = root->BoolValue();
                    }

                    root = root->Next();
                }
                
                return true;
            }

            int32_t id{ -1 };
            std::string name;
            int32_t size{ 12 };
            bool bold{ false };
            bool underline{ false };
            bool italic{ false };
            bool defaultfont{ false };
            bool shared{ false };
        };

        struct DefaultVisitor : public tinyxml2::XMLVisitor {
            ~DefaultVisitor() override = default;
            bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* attribute) override {
                const tinyxml2::XMLAttribute* first = attribute;
                while (nullptr != first) {
                    const char* aName = first->Name();
                    if (0 == strcmp("name", aName)) {
                        name_ = first->Value();
                    } else if (0 == strcmp("value", aName)) {
                        value_ = first->Value();
                    } else if (0 == strcmp("shared", aName)) {
                        shared_ = first->BoolValue();
                    }
                    first = first->Next();
                }
            }

            std::string name_;
            std::string value_;
            bool shared_{ false };
        };
        
        struct MultiLanguageVisitor : public tinyxml2::XMLVisitor {
            ~MultiLanguageVisitor() override = default;
            bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* attribute) override {
                const tinyxml2::XMLAttribute* first = attribute;
                while (nullptr != first) {
                    const char* aName = first->Name();
                    if (0 == strcmp("id", aName)) {
                        id = first->IntValue();
                    } else if (0 == strcmp("value", aName)) {
                        language_ = first->Value();
                    }
                    first = first->Next();
                }
            }

            int32_t id{ -1 };
            std::string language_;
        };
        
        struct WindowVisitor : public tinyxml2::XMLVisitor {
            WindowVisitor(Platforms::Windows::Context* context) : context_(context) { }
            ~WindowVisitor() override = default;
            bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* attribute) override {
                const tinyxml2::XMLAttribute* first = attribute;
                while (nullptr != first) {
                    const char* aName = first->Name();
                    const char* aValue = first->Value();
                    if (0 == strcmp("size", aName)) {
                        context_->SetInitSize(Core::Math::FromString(aValue));
                    } else if (0 == strcmp("sizebox", aName)) {
                        context_->SetSizeBox(Core::Math::Rect::FromString(aValue));
                    } else if (0 == strcmp("caption", aName)) {
                        context_->SetCaptionRect(Core::Math::Rect::FromString(aValue));
                    } else if (0 == strcmp("roundcorner", aName)) {
                        context_->SetRoundCorner(Core::Math::FromString(aValue));
                    } else if (0 == strcmp("mininfo", aName)) {
                        context_->SetMaxInfo(Core::Math::FromString(aValue));
                    }
                    
                    first = first->Next();
                }
                return false;
            }

            Platforms::Windows::Context* context_;
        };

        struct ControlAttributeVisitor : public tinyxml2::XMLVisitor {

        };

        struct ControlCreatorVisitor : public tinyxml2::XMLVisitor {
            ControlCreatorVisitor(Context* context) : context_(context) {}
            ~ControlCreatorVisitor() override = default;

            bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* attribute) override {
                Window::Control::Ptr control;
                const char* eleName = element.Name();
                if (0 == strcmp("VerticalLayout", eleName)) {
                    control = std::make_shared<VerticalLayout>();
                } else if (0 == strcmp("HorizontalLayout", eleName)) {
                    control = std::make_shared<HorizontalLayout>();
                } else if (0 == strcmp("Container", eleName)) {
                    control = std::make_shared<Container>();
                } else if (0 == strcmp("Text", eleName)) {
                    control = std::make_shared<Text>();
                }
                if (nullptr == control) {
                    return true;
                }
                
                const tinyxml2::XMLAttribute* current = attribute;
                while (nullptr != current) {
                    const char* aName = current->Name();
                    const char* aValue = current->Value();
                    control->SetAttribute(aName, aValue);
                    current = current->Next();
                }

                control->SetContext(context_, parent_);
                if (!root_) {
                    root_ = control;
                }
                parent_ = control.get();

                return true;
            }
            Context* context_;
            Control::Ptr root_;
            Control* parent_{ nullptr };
        };

        Control::Ptr Builder::Create(const char* xml, Context* context, Control::Ptr parent) {
            const std::string xmlPath = Core::FileSystem::Path::GetEditorSkinPath() + xml;
            Core::FileSystem::FileHandle fileHandle(xmlPath.c_str());
            if (!fileHandle.OpenRead(false)) {
                DEBUGLOG("open read file failed: {}", xml);
                return nullptr;
            }

            int64_t size = fileHandle.Size();
            if (size <= 0) {
                DEBUGLOG("file size is 0: {}", xml);
                return nullptr;
            }

            std::string buffer(size, 0);
            if (size != fileHandle.Read(&buffer[0], size)) {
                DEBUGLOG("read file failed: {}", xml);
                return nullptr;
            }
            Core::FromUtf8(buffer);
            xmlDoc_.Parse(buffer.c_str(), size);
            if (xmlDoc_.Error()) {
                return nullptr;
            }

            return Create(context, parent);
        }

        Control::Ptr Builder::Create( Context* context, Control::Ptr parent) {
            static bool isRegisterDefaultControl = false;
            if (!isRegisterDefaultControl) {
                isRegisterDefaultControl = true;
                RegisterDefaultControl();
            }

            RegisterCustomControl();


            tinyxml2::XMLElement* root = xmlDoc_.RootElement();
            if (nullptr == root || nullptr ==context) return nullptr;

            context->Serialize(root);

         /*   Platforms::Windows::Context* winContext = reinterpret_cast<Platforms::Windows::Context*>(context);
            const char* className = root->Name();
            if (0 == strcmp(className, "Window")) {
                WindowVisitor visitor(winContext);
                root->Accept(&visitor);
            }
            
            for (tinyxml2::XMLElement* element = root->FirstChildElement(); nullptr != element; element = element->NextSiblingElement()) {
                className = element->Name();
                if (0 == strcmp(className, "Image")) {
                    ImageVisitor visitor;
                    element->Accept(&visitor);
                    if (!visitor.name_.empty()) {

                    }
                }
            }*/
            return nullptr;
        }

        void Builder::RegisterDefaultControl() {
            RegisterControlCreator("VerticalLayout", VerticalLayout::Create);
            RegisterControlCreator("HorizontalLayout", HorizontalLayout::Create);
            RegisterControlCreator("Container", Container::Create);
            RegisterControlCreator("Label", Label::Create);
            RegisterControlCreator("Text", Text::Create);
            RegisterControlCreator("Button", Button::Create);
        }
    }
}
