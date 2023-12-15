#pragma once

#include <vector>
#include "Window/Control.h"
#include "Core/Math/Rect.h"

namespace PPEngine {
    namespace Window {
        class IContainer {
        public:
            virtual ~IContainer() = default;
            virtual bool AddControl(Control::Ptr control) = 0;
            virtual void RemoveControl(Control::Ptr control) = 0;
            virtual int32_t GetCount() const = 0;
            virtual void Clear() = 0;
        };

        class Container : public Control, public IContainer {
        public:
            Container();
            ~Container() override;

            void SetAttribute(const char* name, const char* value) override;
            void OnDraw(const Core::Math::Rect& rect) override;
            void SetInternVisible(bool visible) override;

            bool AddControl(Control::Ptr control) override;
            void RemoveControl(Control::Ptr control) override;
            int32_t GetCount() const override { return controls_.size(); }
            void Clear() override { controls_.clear(); }

            const Core::Math::Rect& GetInset() const { return rectInset_; }
            void SetInset(const Core::Math::Rect& rect); // 设置内边距，相当于设置客户区
            int32_t GetChildPadding() const { return childPadding_; }
            void SetChildPadding(int32_t padding);
            uint32_t GetChildAlign() const { return childAlign_; }
            void SetChildAlign(uint32_t align);
            uint32_t GetChildVAlign() const { return childVAlign_; }
            void SetChildVAlign(uint32_t align);
            bool IsAutoDestroy() const { return autoDestroy_; }
            void SetAutoDestroy(bool autoDestroy) { autoDestroy_ = autoDestroy; }
            bool IsDelayedDestroy() const { return delayedDestroy_; }
            void SetDelayedDestroy(bool delayed) { delayedDestroy_ = delayed; }
            bool IsMouseChildEnabled() const { return mouseChildEnabled_; }
            void SetMouseChildEnabled(bool enable = true) { mouseChildEnabled_ = enable; }

            /*void Update(float dt) override;
            void Draw() override;*/

        protected:
            std::vector<Control::Ptr> controls_;
            Core::Math::Rect rectInset_;

            int32_t childPadding_{ 0 };
            uint32_t childAlign_{ 0 };
            uint32_t childVAlign_{ 0 };
            bool autoDestroy_{ false };
            bool delayedDestroy_{ false };
            bool mouseChildEnabled_{ false };
        };
    }
}
