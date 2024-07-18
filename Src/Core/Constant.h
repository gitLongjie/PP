#pragma once

#include <atomic>

using int8 = char;
using uint8 = unsigned char;
using int16 = short;
using uint16 = unsigned short;
using int32 = int;
using uint32 = unsigned int;
using ulong32 = unsigned long;
using int64 = long long;
using uint64 = unsigned long long;

using AtomicRefCount = std::atomic<int32>;

#ifndef MAKEWORD
#define MAKEWORD(a, b)      ((uint16)(((uint8)(((uint64)(a)) & 0xff)) | ((uint16)((uint8)(((uint64)(b)) & 0xff))) << 8))
#endif // !MAKEWORD

#ifndef MAKELONG
#define MAKELONG(a, b)      ((long)(((uint16)(((uint64)(a)) & 0xffff)) | ((unsigned long)((uint16)(((uint64)(b)) & 0xffff))) << 16))
#endif // !MAKELONG

#ifndef LOWORD
#define LOWORD(l)           ((uint16)(((uint64)(l)) & 0xffff))
#endif // !LOWORD

#ifndef HIWORD
#define HIWORD(l)           ((uint16)((((uint64)(l)) >> 16) & 0xffff))
#endif // !HIWORD

#ifndef LOBYTE
#define LOBYTE(w)           ((uint8)(((uint64)(w)) & 0xff))
#endif // !LOBYTE

#ifndef HIBYTE
#define HIBYTE(w)           ((uint8)((((uint64)(w)) >> 8) & 0xff))
#endif // !HIBYTE

#define NON_COPYABLE(class_name) \
    class_name(const class_name&) = delete; \
    class_name& operator=(const class_name&) = delete;
