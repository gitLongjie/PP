#include "Core/FileSystem/FileStdImpl.h"

namespace PPEngine {
    namespace Core {
        namespace FileSystem {
            FileStdImpl::FileStdImpl(const char* file)
                : fileName_(file) {
            }

            FileStdImpl::~FileStdImpl()
            {
            }

            bool FileStdImpl::IsValide() const {
                return fileSize_ != -1;
            }

            int64_t FileStdImpl::Size() const {
                return fileSize_;
            }

            int64_t FileStdImpl::Tell() const {
                return filePos_;
            }

            bool FileStdImpl::Seek(int64_t pos) {
                return false;
            }

            int64_t FileStdImpl::Read(char* data, int64_t bytes) const {
                return 0;
            }

            int64_t FileStdImpl::Write(const char* data, int64_t bytes) {
                return 0;
            }

            bool FileStdImpl::Flush() {
                return false;
            }
        }
    }
}
