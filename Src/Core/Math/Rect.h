#pragma once

#include <glm/glm.hpp>

namespace PPEngine {
    namespace Core {
        namespace Math {
            class Rect {
            public:
                Rect();
                Rect(float x1, float y1, float x2, float y2);
                Rect(const glm::vec2& left, const glm::vec2& right);
                Rect(const Rect& other);
                ~Rect() = default;

                Rect& operator=(const Rect& other) {
                    left_ = other.left_;
                    right_ = other.right_;
                    UpdateSize();
                    return *this;
                }

                bool operator==(const Rect& other) const {
                    return left_ == other.left_ && right_ == other.right_;
                }
                bool operator!=(const Rect& other) const {
                    return left_ != other.left_ || right_ != other.right_;
                }

                Rect operator+(const Rect& other) const {
                    return Rect(left_ + other.left_, right_ + other.right_);
                }

                bool Contains(const glm::vec2& point) const {
                    return point.x >= left_.x && point.x <= right_.x &&
                           point.y >= left_.y && point.y <= right_.y;
                }
                bool Contains(const glm::vec3& point) const {
                    return point.x >= left_.x && point.x <= right_.x &&
                           point.y >= left_.y && point.y <= right_.y;
                }
                bool Contains(const glm::vec4& point) const {
                    return point.x >= left_.x && point.x <= right_.x &&
                           point.y >= left_.y && point.y <= right_.y;
                }

                bool Intersects(const Rect& other) const {
                    return !(left_.x > other.right_.x ||
                             right_.x < other.left_.x ||
                             left_.y > other.right_.y ||
                             right_.y < other.left_.y);
                }

                Rect CalIntersects(const Rect& other) const {
                    return Rect(glm::max(left_, other.left_),
                                glm::min(right_, other.right_));
                }

                Rect CalOuttersects(const Rect& other) const {
                    return Rect(glm::min(left_, other.left_),
                        glm::max(left_ + right_, other.left_ + other.right_));
                }

                bool IsEmpty() const {
                    return right_.x == 0.0f && right_.y == 0.0f;
                }

                void SetLeft(float x) {
                    left_.x = x;
                    UpdateSize();
                }
                float GetLeft() const {
                    return left_.x;
                }

                void SetRight(float x) {
                    right_.x = x;
                    UpdateSize();
                }
                float GetRight() const {
                    return right_.x;
                }

                void SetTop(float y) {
                    left_.y = y;
                    UpdateSize();
                }
                float GetTop() const {
                    return left_.y;
                }

                void SetBottom(float y) {
                    right_.y = y;
                    UpdateSize();
                }
                float GetBottom() const {
                    return right_.y;
                }

                glm::vec2 GetCenter() const {
                    return (left_ + right_) * 0.5f;
                }
                const glm::vec2& GetMin() const {
                    return left_;
                }
                const glm::vec2& GetSize() const {
                    return size_;
                }
                glm::vec2 GetMax() const {
                    return right_;
                }

                float GetWidth() const {
                    return size_.x;
                }
                float GetHeight() const {
                    return size_.y;
                }

                void SetWidth(float width) {
                    right_.x = left_.x + width;
                    UpdateSize();
                }
                void SetHeight(float height) {
                    right_.y = left_.y + height;
                    UpdateSize();
                }

                void SetSize(const glm::vec2& size) {
                    right_ = left_ + size;
                    UpdateSize();
                }

                void SetPosition(const glm::vec2& position) {
                    left_ = position;
                    UpdateSize();
                }

                void Set(const glm::vec2& position, const glm::vec2& size) {
                    left_ = position;
                    right_ = left_ + size;
                    UpdateSize();
                }

                void Join(const Rect& other) {
                    glm::vec2 min = glm::min(left_, other.left_);
                    glm::vec2 max = glm::max(right_, other.right_);
                    left_ = min;
                    right_ = max;
                    UpdateSize();

                }

                void Inset(const glm::vec2& offset) {
                    left_ += offset;
                    right_ -= offset;
                    UpdateSize();
                }

                void Inset(float left, float top, float right, float bottom) {
                    left_.x += left;
                    left_.y += top;
                    right_.x -= left + right;
                    right_.y -= top + bottom;
                    UpdateSize();
                }

                void Inset(const Rect& rect) {
                    left_ += rect.left_;
                    right_ -= rect.right_;
                    UpdateSize();
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
                void UpdateSize() {
                    size_ = right_ - left_;
                }

                glm::vec2 left_;
                glm::vec2 right_;
                glm::vec2 size_;
            };
        }
    }
} // namespace name
