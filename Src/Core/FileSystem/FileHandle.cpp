#include "Core/FileSystem/FileHandle.h"

#include <assert.h>

namespace PPEngine {
    namespace Core {
        namespace FileSystem {
            FileHandle::FileHandle(const char* file) noexcept
                : name_(file) {
            }

            FileHandle::~FileHandle() {
            }

            bool FileHandle::OpenRead(bool allowWrite) {
                if (fileImpl_) {
                    return true;
                }

                fileImpl_.reset(IFileImpl::CreateRead(name_.c_str(), allowWrite));
                return !!fileImpl_;
            }

            int64_t FileHandle::Size() const {
                assert(!!fileImpl_);
                return fileImpl_->Size();
            }

            int64_t FileHandle::Read(char* data, int64_t bytes) {
                assert(!!fileImpl_);
                return fileImpl_->Read(data, bytes);
            }
        }
    }
}
