#include "RHICommond.h"

#include "RHI/RHI.h"


namespace PPRHI {
    void StartRenderCommand::OnExecute() {
        RHI::Get()->InitRenderEnv();
    }

    void UpdateRenderSizeCommand::OnExecute() {
        RHI::Get()->UpdaeRenderSize();
    }
}
