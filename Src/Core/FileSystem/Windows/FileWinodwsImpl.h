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
                int64_t Read(char* data, int64_t bytes) const override;
                int64_t Write(const char* data, int64_t bytes) override;
                bool Flush() override;

            protected:
                void UpdateSize();

            private:
                HANDLE fileHanle_{nullptr};
            };
        }
    }
}