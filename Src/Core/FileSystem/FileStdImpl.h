#pragma once

#include <string>

#include "Core/FileSystem/IFileImpl.h"

namespace PPEngine {
    namespace Core {
        namespace FileSystem {
            class FileStdImpl : public IFileImpl {
            public:
                FileStdImpl(const char* file);
                ~FileStdImpl() override;

                bool IsValide() const override;
                int64_t Size() const override;
                int64_t Tell() const override;
                bool Seek(int64_t pos) override;
                int64_t Read(char* data, int64_t bytes) const override;
                int64_t Write(const char* data, int64_t bytes) override;
                bool Flush() override;



            protected:
                std::string fileName_;
                int64_t fileSize_{ 0 };
                int64_t filePos_{ 0 };
            };
        }
    }
}