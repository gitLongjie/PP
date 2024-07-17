#pragma once

#include <assert.h>

#include "Core/Constant.h"

namespace PPEngine {
    namespace Core {
        class RefCountBase {
            NON_COPYABLE(RefCountBase)

        public:
            bool HasOnRef() const { return 1 == refCount_; }
            bool HasMoreThanOnRef() const { return 1 <= refCount_; }

        protected:
            RefCountBase() = default;
            virtual ~RefCountBase() = default;

            void AddRef() const {
                ++refCount_;
            }
            bool Release() const {
                if (--refCount_ == 0) {
                    return true;
                }
                return false;
            }

        private:
            mutable int32 refCount_ { 0 };
        };

        class ThreadSafeRefCountBase {
            NON_COPYABLE(ThreadSafeRefCountBase)

        public:
            bool HasOnRef() const { return 1 == refCount_.load(); }
            bool HasMoreThanOnRef() const { return 1 <= refCount_.load(); }

        protected:
            ThreadSafeRefCountBase() = default;
            virtual ~ThreadSafeRefCountBase() = default;

            void AddRef() const {
                ++refCount_;
            }
            bool Release() const {
                if (--refCount_ == 0) {
                    return true;
                }
                return false;
            }

        private:
            mutable AtomicRefCount refCount_;
        };

        template <typename T>
        class RefCount : public RefCountBase {
            NON_COPYABLE(RefCount)

        public:
            RefCount() : RefCountBase() = default;

            void AddRef() const {
                RefCountBase::AddRef();
            }
            void Release() const {
                if (RefCountBase::Release()) {
                    delete static_cast<const T*>(this);
                }
            }

        protected:
            ~RefCount() override = default;
        };

        template <typename T, typename D>
        class ThreadSafeRefCount;

        template<typename T>
        struct DefaultDeleter {
            static void Delete(const T* ptr) {
                ThreadSafeRefCount<T, DefaultDeleter<T>>::Delete(ptr);
            }
        };

        template <typename T, typename D = DefaultDeleter<T>>
        class ThreadSafeRefCount : public ThreadSafeRefCountBase {
            NON_COPYABLE(ThreadSafeRefCount)

        public:
            ThreadSafeRefCount() = default;

            void AddRef() const { ThreadSafeRefCountBase::AddRef(); }
            void Release() const {
                if (ThreadSafeRefCountBase::Release()) {
                    D::Delete(reinterpret_cast<const T*>(this));
                }
            }

        protected:
            virtual ~ThreadSafeRefCount() = default;

        private:
            friend class DefaultDeleter<T>;
            static void Delete(const T* p) { delete p; }
        };

        template <class T>
        class SharedPtr {
        public:
            static SharedPtr<T> Create(T* p) {
                return SharedPtr<T>(p);
            }

        public:
            SharedPtr() : ptr_(nullptr) {}
            SharedPtr(T* p) : ptr_(p) {
                if (nullptr != ptr_) {
                    ptr_->AddRef();
                }
            }
            SharedPtr(const SharedPtr<T>& other) : ptr_(other.ptr_) {
                if (nullptr != ptr_) {
                    ptr_->AddRef();
                }
            }
            template <class U>
            SharedPtr(SharedPtr<U>& other) : ptr_(other.Get()) {
                if (nullptr != ptr_) {
                    ptr_->AddRef();
                }
            }
            ~SharedPtr() { 
                if (nullptr != ptr_) {
                    ptr_->Release();
                }
            }

            T* Get() const { return ptr_; }
            operator T*() const { return ptr_; }
            T* operator ->() const { assert(nullptr != ptr_); return ptr_; }

            SharedPtr<T>& operator =(T* other) {
                if (nullptr != other) {
                    other->AddRef();
                }

                T* old = ptr_;
                ptr_ = other;
                if (nullptr != old) {
                    old->Release();
                }
                return *this;
            }
            SharedPtr<T>& operator= (const SharedPtr<T>& other) {
                return *this = other.ptr_;
            }
            template <class U>
            SharedPtr<T>& operator= (const SharedPtr<U>& other) {
                return *this = other.ptr_;
            }

            void Swap(T** p) {
                T* t = *p;
                *p = ptr_;
                ptr_ = t;
            }

            bool IsValid() const {
                return ptr_ != nullptr;
            }

        private:
            T* ptr_{ nullptr };
        };
    }
}
