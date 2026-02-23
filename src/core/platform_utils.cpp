#include "platform_utils.hpp"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/mman.h>
    #include <unistd.h>
#endif

namespace RawrXD {
namespace Platform {

bool MemoryOps::protectMemory(void* address, size_t size, MemoryProtection protection) {
#ifdef _WIN32
    DWORD oldProtect;
    DWORD newProtect;
    
    switch (protection) {
        case MemoryProtection::ReadOnly:
            newProtect = PAGE_READONLY;
            break;
        case MemoryProtection::ReadWrite:
            newProtect = PAGE_READWRITE;
            break;
        case MemoryProtection::ReadExecute:
            newProtect = PAGE_EXECUTE_READ;
            break;
        case MemoryProtection::ReadWriteExecute:
            newProtect = PAGE_EXECUTE_READWRITE;
            break;
        default:
            return false;
    }
    
    return VirtualProtect(address, size, newProtect, &oldProtect) != 0;
#else
    int prot = 0;
    
    switch (protection) {
        case MemoryProtection::ReadOnly:
            prot = PROT_READ;
            break;
        case MemoryProtection::ReadWrite:
            prot = PROT_READ | PROT_WRITE;
            break;
        case MemoryProtection::ReadExecute:
            prot = PROT_READ | PROT_EXEC;
            break;
        case MemoryProtection::ReadWriteExecute:
            prot = PROT_READ | PROT_WRITE | PROT_EXEC;
            break;
        default:
            return false;
    }
    
    return mprotect(address, size, prot) == 0;
#endif
}

void* MemoryOps::allocateExecutable(size_t size) {
#ifdef _WIN32
    return VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
#else
    return mmap(nullptr, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
}

void MemoryOps::freeExecutable(void* address, size_t size) {
#ifdef _WIN32
    VirtualFree(address, 0, MEM_RELEASE);
#else
    munmap(address, size);
#endif
}

size_t MemoryOps::getPageSize() {
#ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return si.dwPageSize;
#else
    return sysconf(_SC_PAGESIZE);
#endif
}

bool PlatformInfo::isWindows() {
#ifdef _WIN32
    return true;
#else
    return false;
#endif
}

bool PlatformInfo::isLinux() {
#ifdef __linux__
    return true;
#else
    return false;
#endif
}

bool PlatformInfo::isMacOS() {
#ifdef __APPLE__
    return true;
#else
    return false;
#endif
}

std::string PlatformInfo::getOSName() {
#ifdef _WIN32
    return "Windows";
#elif __linux__
    return "Linux";
#elif __APPLE__
    return "macOS";
#else
    return "Unknown";
#endif
}

std::string PlatformInfo::getArchitecture() {
#if defined(__x86_64__) || defined(_M_X64)
    return "x64";
#elif defined(__i386__) || defined(_M_IX86)
    return "x86";
#elif defined(__aarch64__) || defined(_M_ARM64)
    return "ARM64";
#elif defined(__arm__) || defined(_M_ARM)
    return "ARM";
#else
    return "Unknown";
#endif
}

} // namespace Platform
} // namespace RawrXD
