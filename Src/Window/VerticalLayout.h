#pragma once

#include "Window/Container.h"


namespace Window {
    class VerticalLayout : public Container {
    public:
        VerticalLayout();
        ~VerticalLayout() override;

        static Control::Ptr Create();
            
        const char* GetClass() const { return "VerticalLayout"; }
        void SetAttribute(const char* name, const char* value) override;
        void FixRect(Core::Math::Rect rect) override;

        void SetSepHeight(int32_t height) { height_ = height;}
        int32_t GetSepHeight() const { return height_; }
        void SetSepImmMode(bool immediately);
        bool IsSepImmMode() const { return immediately_; }

    private:
        int32_t height_{ 0 };
        bool immediately_{ false };
        bool captured_{ false };
            
    };
}
