#include "Core/FileSystem/Windows/FileWinodwsImpl.h"

#include "Core/Logger.h"

#include <sys/stat.h>

namespace PPEngine {
    namespace Core {
        namespace FileSystem {

            IFileImpl* IFileImpl::CreateRead(const char* file, bool allowWrite) {
                uint32_t  acess = GENERIC_READ;
                uint32_t  share = FILE_SHARE_READ | (allowWrite ? FILE_SHARE_WRITE : 0);
                uint32_t  createion = OPEN_EXISTING;

                HANDLE handle = CreateFile(file, acess, share, nullptr, createion, FILE_ATTRIBUTE_NORMAL, nullptr);
                if (INVALID_HANDLE_VALUE == handle) {
                    DWORD error = GetLastError();
                    WARNLOG("create file faile, file:{}, error={}", file, error);
                    return nullptr;
                }

                return new FileWindowsImpl(file, handle);
            }

            FileWindowsImpl::FileWindowsImpl(const char* file, HANDLE fileHanle)
                : FileStdImpl(file)
                , fileHanle_(fileHanle) {
                if (IsValide()) {
                    UpdateSize();
                }
            }

            FileWindowsImpl::~FileWindowsImpl() {
            }

            bool FileWindowsImpl::IsValide() const {
                return fileHanle_ != NULL && fileHanle_ != INVALID_HANDLE_VALUE && FileStdImpl::IsValide();
            }

            bool FileWindowsImpl::Seek(int64_t pos) {
                assert(IsValide());
                assert(pos >= 0);

                filePos_ = pos;
                return false;
            }

            int64_t FileWindowsImpl::Read(char* data, int64_t bytes) const
            {
                return 0;
            }

            int64_t FileWindowsImpl::Write(const char* data, int64_t bytes)
            {
                return 0;
            }

            bool FileWindowsImpl::Flush()
            {
                return false;
            }

            void FileWindowsImpl::UpdateSize() {
                LARGE_INTEGER fileSize;
                if (!GetFileSizeEx(fileHanle_, &fileSize)) {
                    DWORD error = GetLastError();
                    WARNLOG("create file faile, file:{}, error={}", fileName_.c_str(), error);
                    fileSize_ = -1;
                }

                fileSize_ = fileSize.QuadPart;
            }
        }
    }
}
