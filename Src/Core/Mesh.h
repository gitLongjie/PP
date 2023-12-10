#pragma once

#include <glm/glm.hpp>

namespace PPEngine {
    namespace Core {
        class Mesh {
        public:
            struct Vertex {
                glm::vec3 position_;
                glm::vec4 color_;
                glm::vec2 uv_;
                glm::vec3 normal_;
            };
        public:
            Mesh();
            ~Mesh();

            void SetVertices(Vertex* vertices, unsigned int vertexCount, bool copied);
            const Vertex* GetVertices() const { return vertices_; }
            unsigned int GetVertexCount() const { return vertexCount_; }

            void SetIndices(unsigned int* indices, unsigned int indexCount, bool copied);
            const unsigned int* GetIndices() const { return indices_; }
            unsigned int GetIndexCount() const { return indexCount_; }


        private:
            Vertex* vertices_{ nullptr };
            bool isVerticesCopied_{ false };
            unsigned int vertexCount_{ 0 };
            unsigned int* indices_{ nullptr };
            bool isIndicesCopied_{ false };
            unsigned int indexCount_{ 0 };
        };
    }
}
