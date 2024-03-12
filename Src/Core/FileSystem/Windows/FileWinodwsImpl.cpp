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
                if (IsValide()) {
                    CloseHandle(fileHanle_);
                    fileHanle_ = NULL;
                }
            }

            bool FileWindowsImpl::IsValide() const {
                return fileHanle_ != NULL && fileHanle_ != INVALID_HANDLE_VALUE && FileStdImpl::IsValide();
            }

            bool FileWindowsImpl::Seek(int64_t pos) {
                assert(IsValide());
                assert(pos >= 0);

                filePos_ = pos;
                UpdateOverlappedPos();
                return false;
            }

            int64_t FileWindowsImpl::Read(char* data, int64_t bytes) {
                assert(IsValide());

                int64_t numberOfBytesRead = 0;
                do {
                    uint32_t byteToRead = std::min<int64_t>(bytes, int64_t(UINT32_MAX));
                    DWORD bytesRead = 0;

                    if (!ReadFile(fileHanle_, data, byteToRead, &bytesRead, &overlapped_)) {
                        DWORD error = GetLastError();
                        if (error != ERROR_IO_PENDING) {
                            WARNLOG("read file faile, file:{}, error={}", fileName_.c_str(), error);
                            return 0;
                        }

                        bytesRead = 0;
                        if (!GetOverlappedResult(fileHanle_, &overlapped_, &bytesRead, true)) {
                            WARNLOG("read file faile, file:{}, GetOverlappedResult failed", fileName_.c_str());
                            return 0;
                        }
                    }

                    bytes -= byteToRead;
                    data += byteToRead;
                    numberOfBytesRead += bytesRead;

                    filePos_ += bytesRead;
                    UpdateOverlappedPos();

                    if (byteToRead != bytesRead) {
                        WARNLOG("read file faile, file:{}, byteToRead({}) != bytesRead({})", fileName_.c_str(), byteToRead, bytesRead);
                        return 0;
                    }
                } while (bytes > 0);
                return numberOfBytesRead;
            }

            int64_t FileWindowsImpl::Write(const char* data, int64_t bytes) {
                assert(IsValide());

                int64_t numberOfBytesWrite = 0;
                do {
                    uint32_t byteToWrite = std::min<int64_t>(bytes, int64_t(UINT32_MAX));
                    DWORD bytesWrite = 0;

                    if (!WriteFile(fileHanle_, data, byteToWrite, &bytesWrite, &overlapped_)) {
                        DWORD error = GetLastError();
                        if (error != ERROR_IO_PENDING) {
                            WARNLOG("read file faile, file:{}, error={}", fileName_.c_str(), error);
                            return 0;
                        }

                        bytesWrite = 0;
                        if (!GetOverlappedResult(fileHanle_, &overlapped_, &bytesWrite, true)) {
                            WARNLOG("read file faile, file:{}, GetOverlappedResult failed", fileName_.c_str());
                            return 0;
                        }
                    }

                    bytes -= byteToWrite;
                    data += byteToWrite;
                    numberOfBytesWrite += bytesWrite;

                    filePos_ += bytesWrite;
                    UpdateOverlappedPos();

                    fileSize_ = std::max<int64_t>(fileSize_, filePos_);

                    if (byteToWrite != bytesWrite) {
                        WARNLOG("write file faile, file:{}, byteToRead({}) != bytesRead({})", fileName_.c_str(), byteToWrite, bytesWrite);
                        return 0;
                    }
                } while (bytes > 0);
                return numberOfBytesWrite;
            }

            bool FileWindowsImpl::Flush() {
                assert(IsValide());
                return FlushFileBuffers(fileHanle_) != 0;
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
