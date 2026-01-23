#ifndef PLATFORM_UTILS_HPP
#define PLATFORM_UTILS_HPP

#include <string>
#include <cstddef>

namespace RawrXD {
namespace Platform {

/**
 * @brief Platform-specific memory protection modes
 */
enum class MemoryProtection {
    ReadOnly,
    ReadWrite,
    ReadExecute,
    ReadWriteExecute
};

/**
 * @brief Platform-specific memory operations
 */
class MemoryOps {
public:
    /**
     * @brief Change memory protection for a region
     * @param address Starting address
     * @param size Size of region in bytes
     * @param protection New protection mode
     * @return true on success, false on failure
     */
    static bool protectMemory(void* address, size_t size, MemoryProtection protection);
    
    /**
     * @brief Allocate executable memory
     * @param size Size in bytes
     * @return Pointer to allocated memory, or nullptr on failure
     */
    static void* allocateExecutable(size_t size);
    
    /**
     * @brief Free executable memory
     * @param address Memory address to free
     * @param size Size in bytes
     */
    static void freeExecutable(void* address, size_t size);
    
    /**
     * @brief Get system page size
     * @return Page size in bytes
     */
    static size_t getPageSize();
};

/**
 * @brief Platform detection utilities
 */
class PlatformInfo {
public:
    static bool isWindows();
    static bool isLinux();
    static bool isMacOS();
    static std::string getOSName();
    static std::string getArchitecture();
};

} // namespace Platform
} // namespace RawrXD

#endif // PLATFORM_UTILS_HPP
