#pragma once

#include <Windows.h>

#include <string>

#include "Core/FileSystem/FileStdImpl.h"

namespace PPEngine {
    namespace Core {
        namespace FileSystem {
            class FileWindowsImpl : public FileStdImpl {
            public:
                FileWindowsImpl(const char* file, HANDLE fileHanle);
                ~FileWindowsImpl() override;

                bool IsValide() const override;
                bool Seek(int64_t pos) override;
                int64_t Read(char* data, int64_t bytes) override;
                int64_t Write(const char* data, int64_t bytes) override;
                bool Flush() override;

            protected:
                void UpdateSize();
                void UpdateOverlappedPos() {
                    ULARGE_INTEGER li;
                    li.QuadPart = filePos_;
                    overlapped_.Offset = li.LowPart;
                    overlapped_.OffsetHigh = li.HighPart;
                }
                bool UpdateNonOverlappedPos() {
                    LARGE_INTEGER li;
                    li.QuadPart = filePos_;
                    return SetFilePointer(fileHanle_, li.LowPart, &li.HighPart, FILE_BEGIN) != INVALID_SET_FILE_POINTER;
                }

            private:
                HANDLE fileHanle_{nullptr};
                OVERLAPPED overlapped_{0};
            };
        }
    }
}