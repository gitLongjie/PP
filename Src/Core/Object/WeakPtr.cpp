#include "Core/Object/WeakPtr.h"

#include "Core/Logger.h"

namespace PPEngine {
    namespace Core {
        WeakReference::Flag::Flag() : isValide_(true) {

        }

        void WeakReference::Flag::Invalidate(void) {
            if (threadChecker_.CalledOnValideThread() || HasOnRef()) {
                ERRORLOG("WeakPtrs must be invalidated on the same thread.");
            }
            isValide_ = false;
        }

        bool WeakReference::Flag::IsValid(void) const {
            if (threadChecker_.CalledOnValideThread()) {
                INFOLOG("WeakPtrs must be checked on the same thread.");
            }
            return isValide_;
        }

        WeakReference::Flag::~Flag() {
        }

        WeakReference::WeakReference() {
        }

        WeakReference::WeakReference(const Flag* flag) : flag_(flag) {
        }

        WeakReference::~WeakReference() {
        }

        bool WeakReference::IsValid(void) const {
            return flag_.Get() && flag_->IsValid();
        }

        WeakReferenceOwner::WeakReferenceOwner() {
        }

        WeakReferenceOwner::~WeakReferenceOwner() {
        }

        WeakReference WeakReferenceOwner::GetRef() const {
            if (!HasRefs()) {
                flag_ = new WeakReference::Flag;
            }
            return WeakReference(flag_.Get());
        }

        void WeakReferenceOwner::Invalidate() {
            if (nullptr != flag_) {
                flag_->Invalidate();
                flag_ = nullptr;
            }
        }

        WeakPtrBase::WeakPtrBase() {}

        WeakPtrBase::~WeakPtrBase() {}

        WeakPtrBase::WeakPtrBase(const WeakReference& ref) : ref_(ref) {}
    }
}
