#pragma once

#include <string>

#include "Core/FileSystem/IFileImpl.h"

namespace PPEngine {
    namespace Core {
        namespace FileSystem {
            class FileHandle {
            public:
                explicit FileHandle(const char* file) noexcept;
                ~FileHandle();

                bool OpenRead(bool allowWrite);
                int64_t Size() const;
                int64_t Read(char* data, int64_t bytes);

            private:
                std::unique_ptr<IFileImpl> fileImpl_;
                std::string name_;
            };
        }
    }
}