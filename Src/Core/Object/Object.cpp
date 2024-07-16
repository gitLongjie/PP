#include "Core/Object/Object.h"

#include <atomic>
#include <cassert>

namespace PPEngine {
    namespace Core {
        class ObjectRefCountImpl {
            public:
            ObjectRefCountImpl() {}

            void AddRef(void) const {
                ++refCount_; 
            }
            bool Release(void) const {
                if (0 == --refCount_) {
                    return true;
                }

                return false;
            }
            bool HasOneRef(void) const { return 1 == refCount_.load(); }
            bool HasAtLeastOneRef(void) const { return refCount_.load() >= 1; }

        private:
            mutable std::atomic<int> refCount_{0};
        };

        ObjectRefCount::ObjectRefCount() : impl_(new ObjectRefCountImpl) {
        }

        ObjectRefCount::~ObjectRefCount() {
            delete impl_;
        }

        void ObjectRefCount::AddRef() const {
            assert(nullptr != impl_);
            impl_->AddRef();
        }

        bool ObjectRefCount::Release() const {
            assert(nullptr != impl_);
            return impl_->Release();
        }

        bool ObjectRefCount::HasOneRef() const {
            assert(nullptr != impl_);
            return impl_->HasOneRef();
        }

        bool ObjectRefCount::HasAtLeastOneRef() const {
            assert(nullptr != impl_);
            return impl_->HasAtLeastOneRef();
        }
    }
}