#include "Core/FileSystem/FileHandle.h"

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
        }
    }
}
