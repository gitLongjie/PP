#pragma once


namespace PPEngine {
    namespace Core {
        namespace Math {
            template <typename T>
            const T& Max(const T& a, const T& b) {
                return a > b ? a : b;
            }

            template <typename T>
            const T& Min(const T& a, const T& b) {
                return a < b ? a : b;
            }

        }
    }
} // namespace name
