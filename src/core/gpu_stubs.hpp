#ifndef GPU_STUBS_HPP
#define GPU_STUBS_HPP

#include <string>
#include <vector>
#include <cstddef>
#include <cstdint>

namespace RawrXD {
namespace GPU {

/**
 * @brief GPU device types (internal stub implementations)
 */
enum class DeviceType {
    CPU,        // Fallback CPU implementation
    VulkanStub, // Vulkan stub (no-op)
    ROCmStub,   // ROCm stub (no-op)
    CUDAStub    // CUDA stub (no-op)
};

/**
 * @brief GPU memory allocation handle
 */
struct MemoryHandle {
    void* ptr;
    size_t size;
    DeviceType device;
    bool isValid;
    
    MemoryHandle() : ptr(nullptr), size(0), device(DeviceType::CPU), isValid(false) {}
};

/**
 * @brief GPU device information
 */
struct DeviceInfo {
    std::string name;
    DeviceType type;
    size_t totalMemory;
    size_t availableMemory;
    int computeUnits;
    bool isAvailable;
};

/**
 * @brief Internal GPU operations stub interface
 * 
 * Replaces Vulkan/ROCm with CPU fallbacks or no-op stubs.
 * All operations return success but may use CPU implementation.
 */
class GPUStubs {
public:
    /**
     * @brief Initialize GPU subsystem
     * @param preferredType Preferred device type
     * @return true on success
     */
    static bool initialize(DeviceType preferredType = DeviceType::CPU);
    
    /**
     * @brief Shutdown GPU subsystem
     */
    static void shutdown();
    
    /**
     * @brief Get available devices
     * @return Vector of available device info
     */
    static std::vector<DeviceInfo> getAvailableDevices();
    
    /**
     * @brief Allocate GPU memory
     * @param size Size in bytes
     * @param device Device to allocate on
     * @return Memory handle
     */
    static MemoryHandle allocate(size_t size, DeviceType device = DeviceType::CPU);
    
    /**
     * @brief Free GPU memory
     * @param handle Memory handle to free
     */
    static void free(MemoryHandle& handle);
    
    /**
     * @brief Copy memory to GPU
     * @param handle Destination handle
     * @param data Source data
     * @param size Size in bytes
     * @return true on success
     */
    static bool copyToDevice(MemoryHandle& handle, const void* data, size_t size);
    
    /**
     * @brief Copy memory from GPU
     * @param handle Source handle
     * @param data Destination buffer
     * @param size Size in bytes
     * @return true on success
     */
    static bool copyFromDevice(const MemoryHandle& handle, void* data, size_t size);
    
    /**
     * @brief Execute compute kernel (stub - uses CPU)
     * @param handle Memory handle
     * @param kernelType Type of operation
     * @param params Additional parameters
     * @return true on success
     */
    static bool executeKernel(MemoryHandle& handle, const std::string& kernelType, void* params);
    
    /**
     * @brief Synchronize device operations
     */
    static void synchronize();
    
    /**
     * @brief Check if GPU acceleration is truly available
     * @return false (always, since we use stubs)
     */
    static bool isHardwareAccelerated();
    
    /**
     * @brief Get current device type
     */
    static DeviceType getCurrentDevice();
};

// Vulkan-specific stubs
#ifdef ENABLE_VULKAN_STUBS
namespace Vulkan {
    bool initializeVulkan();
    void shutdownVulkan();
    std::string getVulkanVersion();
}
#endif

// ROCm-specific stubs
#ifdef ENABLE_ROCM_STUBS
namespace ROCm {
    bool initializeROCm();
    void shutdownROCm();
    std::string getROCmVersion();
}
#endif

} // namespace GPU
} // namespace RawrXD

#endif // GPU_STUBS_HPP
