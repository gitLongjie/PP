#include "Core/Math/Rect.h"

namespace PPEngine {
    namespace Core {
        namespace Math {
            const Rect Rect::Zero = Rect(0.0f, 0.0f, 0.0f, 0.0f);
            const Rect Rect::One = Rect(0.0f, 0.0f, 1.0f, 1.0f);
            Rect::Rect()
                : position_(0.0f, 0.0f)
                , size_(0.0f, 0.0f) {
            }
            Rect::Rect(float x, float y, float width, float height)
                : position_(x, y)
                , size_(width, height) {
            }

            Rect::Rect(const glm::vec2& position, const glm::vec2& size)
                : position_(position)
                , size_(size) {
            }

            Rect::Rect(const Rect& other)
                : position_(other.position_)
                , size_(other.size_) {}

            Rect Rect::FromString(const char* s) {
                float x = 0.0f, y = 0.0f, width = 1.0f, height = 1.0f;
                sscanf(s, "%f,%f,%f,%f", &x, &y, &width, &height);
                return Rect(x, y, width, height);
            }
        }
    }
}
