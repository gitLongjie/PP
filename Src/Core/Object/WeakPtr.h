#pragma once

#include "Core/Constant.h"
#include "Core/Object/SharedPtr.h"
#include "Core/Thread/ThreadChecker.h"

namespace Core {
    template <typename T>
    class SupportsWeakPtr;
    template <typename T>
    class WeakPtr;

    class WeakReference {
    public:
        class Flag : public ThreadSafeRefCount<Flag> {
        public:
            Flag();

            void Invalidate(void);
            bool IsValid(void) const;

        private:
            friend class ThreadSafeRefCount<Flag>;

            ~Flag() override;

            ThreadChecker threadChecker_;
            mutable bool isValide_{ false };
        };

    public:
        WeakReference();
        explicit WeakReference(const Flag* flag);
        ~WeakReference();

        bool IsValid(void) const;

    private:
        SharedPtr<const Flag> flag_;
    };

    class WeakReferenceOwner {
    public:
        WeakReferenceOwner();
        ~WeakReferenceOwner();

        WeakReference GetRef() const;

        bool HasRefs() const {
            return flag_.Get() && !flag_->HasOnRef();
        }

        void Invalidate();

    private:
        mutable SharedPtr<WeakReference::Flag> flag_;
    };

    class WeakPtrBase {
    public:
        WeakPtrBase();
        ~WeakPtrBase();

    protected:
        explicit WeakPtrBase(const WeakReference& ref);

        WeakReference ref_;
    };

    template <typename T>
    class WeakPtrFactory;

    template <typename T>
    class WeakPtr : public WeakPtrBase {
    public:
        WeakPtr() : m_pObject(NULL) {}

        template <typename U>
        WeakPtr(const WeakPtr<U>& other) : WeakPtrBase(other), m_pObject(other.ref_) {}

        T* Get() const {
            return ref_.IsValid() ? m_pObject : NULL;
        }

        T& operator*() const {
            assert(ref_.IsValid());
            return *Get();
        }
        T* operator->() const {
            assert(ref_.IsValid());
            return Get();
        }

    private:
        typedef T* WeakPtr::* Testable;

    public:
        operator Testable() const {
            return Get() ? &WeakPtr::ptr_ : NULL;
        }

        void Reset() {
            ref_ = WeakReference();
            ptr_ = nullptr;
        }

    private:
        template <class U>
        bool operator==(WeakPtr<U> const&) const;
        template <class U>
        bool operator!=(WeakPtr<U> const&) const;

        friend class SupportsWeakPtrBase;
        template <typename U>
        friend class WeakPtr;
        friend class SupportsWeakPtr<T>;
        friend class WeakPtrFactory<T>;

        WeakPtr(const WeakReference& ref, T* p)
            : WeakPtrBase(ref), m_pObject(p) {}

        T* ptr_{ nullptr };
    };

    class SupportsWeakPtrBase {
    public:
        template <typename Derived>
        static WeakPtr<Derived> StaticAsWeakPtr(Derived* t) {
            typedef IsConvertible<Derived, SupportsWeakPtrBase&> convertible;
            COMPILE_ASSERT(convertible::value, AsWeakPtr_argument_inherits_from_SupportsWeakPtr);
            return AsWeakPtrImpl<Derived>(t, *t);
        }

    private:
        template <typename Derived, typename Base>
        static WeakPtr<Derived> AsWeakPtrImpl(Derived* t,
            const SupportsWeakPtr<Base>&) {
            WeakPtr<Base> ptr = t->Base::AsWeakPtr();
            return WeakPtr<Derived>(ptr.ref_, static_cast<Derived*>(ptr.ptr_));
        }
    };

    template <class T>
    class WeakPtrFactory {
        NON_COPYABLE(WeakPtrFactory);

    public:
        explicit WeakPtrFactory(T* ptr) : ptr_(ptr) {}

        ~WeakPtrFactory() {
            ptr_ = NULL;
        }

        WeakPtr<T> GetWeakPtr() {
            assert(nullptr != ptr_);
            return WeakPtr<T>(weakReferenceOwner_.GetRef(), ptr_);
        }

        void InvalidateWeakPtrs() {
            assert(nullptr != ptr_);
            weakReferenceOwner_.Invalidate();
        }

        bool HasWeakPtrs() const {
            assert(nullptr != ptr_);
            return weakReferenceOwner_.HasRefs();
        }

    private:
        WeakReferenceOwner weakReferenceOwner_;
        T* ptr_{ nullptr };
    };

    template <class T>
    class SupportsWeakPtr : public SupportsWeakPtrBase {
        NON_COPYABLE(SupportsWeakPtr);
    public:
        SupportsWeakPtr() {}

        WeakPtr<T> AsWeakPtr() {
            return WeakPtr<T>(weakReferenceOwner_.GetRef(), static_cast<T*>(this));
        }

    protected:
        ~SupportsWeakPtr() {}

    private:
        WeakReferenceOwner weakReferenceOwner_;
    };
}
