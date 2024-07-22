#pragma once

#include "Core/Xml/tinyxml2.h"
#include "Window/Context.h"
#include "Window/Control.h"

namespace Window {
    class Builder {
    public:
        Builder() = default;
        virtual ~Builder() = default;

        Control* Create(const char* xml, Context* context = nullptr, Control* parent = nullptr);
        Control* Create(Context* context = nullptr, Control* parent = nullptr);

    private:
        void RegisterDefaultControl();
        virtual void RegisterCustomControl() {}

        tinyxml2::XMLDocument xmlDoc_;
    };

}
