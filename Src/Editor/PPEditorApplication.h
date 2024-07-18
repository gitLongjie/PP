#pragma once

#include "Core/MainLoop.h"
#include "Application/PPApplication.h"

#include "Editor/Context.h"

namespace Editor {
    class PPEditorApplication : public PPApplication {
    public:
        explicit PPEditorApplication(Core::MainLoop& mainLoop) noexcept;
        ~PPEditorApplication() override;

        bool Initialize() override;
        int RunLoop() override;
        void Uninitialize() override;

    private:
        Context context_;
        Core::MainLoop& mainLoop_;
    };
}
