#pragma once

#include <glm/glm.hpp>

namespace PPEngine {
    namespace Core {
        namespace Math {
            class Rect {
            public:
                Rect() = default;
                Rect(float x, float y, float width, float height);
                Rect(const glm::vec2& position, const glm::vec2& size);
                Rect(const Rect& other);
                ~Rect() = default;

                Rect& operator=(const Rect& other) {
                    position_ = other.position_;
                    size_ = other.size_;
                    return *this;
                }

                bool operator==(const Rect& other) const {
                    return position_ == other.position_ && size_ == other.size_;
                }
                bool operator!=(const Rect& other) const {
                    return position_ != other.position_ || size_ != other.size_;
                }

                Rect operator+(const Rect& other) const {
                    return Rect(position_ + other.position_, size_ + other.size_);
                }

                bool Contains(const glm::vec2& point) const {
                    return point.x >= position_.x && point.x <= position_.x + size_.x &&
                           point.y >= position_.y && point.y <= position_.y + size_.y;
                }
                bool Contains(const glm::vec3& point) const {
                    return point.x >= position_.x && point.x <= position_.x + size_.x &&
                           point.y >= position_.y && point.y <= position_.y + size_.y;
                }
                bool Contains(const glm::vec4& point) const {
                    return point.x >= position_.x && point.x <= position_.x + size_.x &&
                           point.y >= position_.y && point.y <= position_.y + size_.y;
                }

                bool Intersects(const Rect& other) const {
                    return !(position_.x > other.position_.x + other.size_.x ||
                             position_.x + size_.x < other.position_.x ||
                             position_.y > other.position_.y + other.size_.y ||
                             position_.y + size_.y < other.position_.y);
                }
                Rect CalIntersects(const Rect& other) const {
                    return Rect(glm::max(position_, other.position_),
                                glm::min(position_ + size_, other.position_ + other.size_));
                }

                bool IsEmpty() const {
                    return size_.x == 0.0f && size_.y == 0.0f;
                }

                glm::vec2 GetCenter() const {
                    return position_ + size_ * 0.5f;
                }
                const glm::vec2& GetMin() const {
                    return position_;
                }                
                glm::vec2 GetMax() const {
                    return position_ + size_;
                }

                float GetWidth() const {
                    return size_.x;
                }
                float GetHeight() const {
                    return size_.y;
                }

                void SetWidth(float width) {
                    size_.x = width;
                }
                void SetHeight(float height) {
                    size_.y = height;
                }

                void SetSize(const glm::vec2& size) {
                    size_ = size;
                }

                void SetPosition(const glm::vec2& position) {
                    position_ = position;
                }

                void Set(const glm::vec2& position, const glm::vec2& size) {
                    position_ = position;
                    size_ = size;
                }

                static const Rect Zero;
                static const Rect One;

                static Rect FromMinMax(const glm::vec2& min, const glm::vec2& max) {
                    return Rect(min, max - min);
                }

                static Rect FromCenter(const glm::vec2& center, const glm::vec2& size) {
                    return Rect(center - size * 0.5f, size);
                }

                static Rect FromString(const char* s); // "1,1,1,1";

            private:
                glm::vec2 position_;
                glm::vec2 size_;
            };
        }
    }
} // namespace name
