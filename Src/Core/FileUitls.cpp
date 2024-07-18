#include "Core/FileUitls.h"

#include <sys/stat.h>

namespace Core {
    int64_t FileUtils::GetFileSize(const char* path) {
        if (nullptr == path) {
            return -1;
        }

        struct stat statbuf;
        if (stat(path, &statbuf) == 0) {
            return statbuf.st_size;
        }
        return -1;
    }
}
