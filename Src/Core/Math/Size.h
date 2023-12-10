#pragma once

#include <glm/glm.hpp>

namespace PPEngine {
    namespace Core {
        namespace Math {
            using Size = glm::vec2;

            Size FromString(const char* s); // "1,1";
        }
    }
} // namespace name
