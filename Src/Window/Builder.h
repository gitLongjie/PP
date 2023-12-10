#pragma once

#include "Window/Context.h"
#include "Window/tinyxml2.h"
#include "Window/Control.h"

namespace PPEngine {
    namespace Window {

        class IBuilderCallback {
        public:
            virtual Control* CreateControl(const char* className) = 0;
        };


        class Builder {
        public:
            Builder();
            Control::Ptr Create(const char* xml, IBuilderCallback* callback = NULL,
                Context* context = NULL, Control::Ptr parent = NULL);
            Control::Ptr Create(IBuilderCallback* callback = NULL, Context* context = NULL,
                Control::Ptr parent = NULL);

            tinyxml2::XMLDocument* GetMarkup();

        private:
            Control::Ptr _Parse(tinyxml2::XMLNode* root, Control::Ptr parent = NULL, Context* context = NULL);

            tinyxml2::XMLDocument xmlDoc_;
            IBuilderCallback* callback_;
            //LPCTSTR m_pstrtype;
        };

    }
}
