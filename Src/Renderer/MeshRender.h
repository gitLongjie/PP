#pragma once

#include "Core/Mesh.h"
#include "RHI/RHI.h"

namespace PPEngine {
    namespace Renderer {
        class MeshRender {
        public:
            MeshRender(Core::Mesh* mesh);
            ~MeshRender();

            void Render();

        private:
            Core::Mesh* mesh_{ nullptr };
            PPRHI::IRHIAPI* rhiApi_{ nullptr };

            unsigned int vaoHandle_{ 0 };
            unsigned int vboHandle_{ 0 };
            unsigned int eboHandle_{ 0 };
        };
    }
}