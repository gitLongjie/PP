#pragma once

#include <glm/glm.hpp>

namespace Core {
    namespace Math {
        using Size = glm::vec2;
        using Point2d = glm::vec2;

        Size FromString(const char* s); // "1,1";
    }
}
