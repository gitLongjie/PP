#pragma once

#include <memory>

namespace Core {
    namespace FileSystem {
        class IFileImpl {
        public:
            static IFileImpl* CreateRead(const char* file, bool allowWrite);

        public:
            virtual ~IFileImpl() = default;
            virtual bool IsValide() const = 0;
            virtual int64_t Size() const = 0;
            virtual int64_t Tell() const = 0;
            virtual bool Seek(int64_t pos) = 0;
            virtual int64_t Read(char* data, int64_t bytes) = 0;
            virtual int64_t Write(const char* data, int64_t bytes) = 0;
            virtual bool Flush() = 0;

        };
    }
}
