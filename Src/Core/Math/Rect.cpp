#include "Core/Math/Rect.h"

namespace Core {
    namespace Math {
        const Rect Rect::Zero = Rect(0.0f, 0.0f, 0.0f, 0.0f);
        const Rect Rect::One = Rect(0.0f, 0.0f, 1.0f, 1.0f);
        Rect::Rect()
            : left_(0.0f, 0.0f)
            , right_(0.0f, 0.0f)
            , size_(0.0f, 0.0f) {

        }
        Rect::Rect(float x1, float y1, float x2, float y2)
            : left_(x1, y1)
            , right_(x2, y2) {
            UpdateSize();
        }

        Rect::Rect(const glm::vec2& left, const glm::vec2& right)
            : left_(left)
            , right_(right) {
            UpdateSize();
        }

        Rect::Rect(const Rect& other)
            : left_(other.left_)
            , right_(other.right_) {
            UpdateSize();
        }

        Rect Rect::FromString(const char* s) {
            float x = 0.0f, y = 0.0f, width = 1.0f, height = 1.0f;
            sscanf(s, "%f,%f,%f,%f", &x, &y, &width, &height);
            return Rect(x, y, x + width, y + height);
        }
    }
}
