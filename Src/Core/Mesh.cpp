#include "Core/Mesh.h"

namespace Core {
    Mesh::Mesh() {}

    Mesh::~Mesh() {}

    void Mesh::SetVertices(Vertex* vertices, unsigned int vertexCount, bool copied) {
        isVerticesCopied_ = copied;
        vertexCount_ = vertexCount;

        if (isIndicesCopied_) {

        } else {
            vertices_ = vertices;
        }
    }

    void Mesh::SetIndices(unsigned int* indices, unsigned int indexCount, bool copied) {
        isIndicesCopied_ = copied;
        indexCount_ = indexCount;

        if (isVerticesCopied_) {

        } else {
            indices_ = indices;
        }
    }
}
