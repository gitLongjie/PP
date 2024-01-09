#pragma once

#include "Core/Xml/tinyxml2.h"
#include "Window/Context.h"
#include "Window/Control.h"

namespace PPEngine {
    namespace Window {
        class Builder {
        public:
            Builder() = default;
            virtual ~Builder() = default;

            Control::Ptr Create(const char* xml, Context* context = NULL, Control::Ptr parent = NULL);
            Control::Ptr Create(Context* context = NULL,Control::Ptr parent = NULL);

        private:
            void RegisterDefaultControl();
            virtual void RegisterCustomControl() {}

            tinyxml2::XMLDocument xmlDoc_;
        };

    }
}
