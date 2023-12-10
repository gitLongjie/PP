#include "Core/Math/Size.h"


namespace PPEngine {
    namespace Core {
        namespace Math {
            Size FromString(const char* s) {
                Size size;
                sscanf(s, "%f,%f", &size.x, &size.y);
                return size;
            }
        }
    }
}