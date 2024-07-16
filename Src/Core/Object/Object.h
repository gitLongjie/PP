#pragma once

#include "Core/Constant.h"

namespace PPEngine {
    namespace Core {
        class Object {
        public:
            virtual void AddRef() const = 0;
            virtual bool Release() const = 0;
            virtual bool HasOneRef() const = 0;
            virtual bool HasAtLeastOneRef() const = 0;

        protected:
            virtual ~Object() = default;
        };

        class ObjectRefCount {
            NON_COPYABLE(ObjectRefCount)

        public:
            ObjectRefCount();
            ~ObjectRefCount();

            void AddRef() const;
            bool Release() const;
            bool HasOneRef() const;
            bool HasAtLeastOneRef() const;

        private:
            friend class ObjectRefCountImpl;
            ObjectRefCountImpl* impl_ { nullptr };
        };
    }
}

#define IMPLEMENT_OBJECT_REFCOUN(CLASSNAME)                                                              \
public:                                                                                                  \
        void AddRef(void) const override { refCount_.AddRef(); }                                         \
        bool Release(void) const override {                                                              \
            if (refCount_.Release()) {                                                                   \
                delete reinterpret_cast<const CLASSNAME*>(this);                                         \
                return true;                                                                             \
            }                                                                                            \
            return false;                                                                                \
        }                                                                                                \
        bool HasOneRef(void) const override { return refCount_.HasOneRef(); }                            \
        bool HasAtLeastOneRef(void) const override { return refCount_.HasAtLeastOneRef(); }              \
                                                                                                         \
private:                                                                                                 \
    PPEngine::Core::ObjectRefCount refCount_;                                                            \
