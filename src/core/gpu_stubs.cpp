#include "gpu_stubs.hpp"
#include <cstring>
#include <cstdlib>

namespace RawrXD {
namespace GPU {

static DeviceType g_currentDevice = DeviceType::CPU;
static bool g_initialized = false;

bool GPUStubs::initialize(DeviceType preferredType) {
    if (g_initialized) {
        return true;
    }
    
    // Always fall back to CPU since we're using stubs
    g_currentDevice = DeviceType::CPU;
    g_initialized = true;
    
#ifdef ENABLE_VULKAN_STUBS
    Vulkan::initializeVulkan();
#endif

#ifdef ENABLE_ROCM_STUBS
    ROCm::initializeROCm();
#endif
    
    return true;
}

void GPUStubs::shutdown() {
    if (!g_initialized) {
        return;
    }
    
#ifdef ENABLE_VULKAN_STUBS
    Vulkan::shutdownVulkan();
#endif

#ifdef ENABLE_ROCM_STUBS
    ROCm::shutdownROCm();
#endif
    
    g_initialized = false;
}

std::vector<DeviceInfo> GPUStubs::getAvailableDevices() {
    std::vector<DeviceInfo> devices;
    
    // Always provide CPU device
    DeviceInfo cpuDevice;
    cpuDevice.name = "CPU Fallback";
    cpuDevice.type = DeviceType::CPU;
    cpuDevice.totalMemory = 8ULL * 1024 * 1024 * 1024; // 8 GB (fake)
    cpuDevice.availableMemory = 4ULL * 1024 * 1024 * 1024; // 4 GB (fake)
    cpuDevice.computeUnits = 8;
    cpuDevice.isAvailable = true;
    devices.push_back(cpuDevice);
    
#ifdef ENABLE_VULKAN_STUBS
    DeviceInfo vulkanDevice;
    vulkanDevice.name = "Vulkan Stub (No-Op)";
    vulkanDevice.type = DeviceType::VulkanStub;
    vulkanDevice.totalMemory = 0;
    vulkanDevice.availableMemory = 0;
    vulkanDevice.computeUnits = 0;
    vulkanDevice.isAvailable = false;
    devices.push_back(vulkanDevice);
#endif

#ifdef ENABLE_ROCM_STUBS
    DeviceInfo rocmDevice;
    rocmDevice.name = "ROCm Stub (No-Op)";
    rocmDevice.type = DeviceType::ROCmStub;
    rocmDevice.totalMemory = 0;
    rocmDevice.availableMemory = 0;
    rocmDevice.computeUnits = 0;
    rocmDevice.isAvailable = false;
    devices.push_back(rocmDevice);
#endif
    
    return devices;
}

MemoryHandle GPUStubs::allocate(size_t size, DeviceType device) {
    MemoryHandle handle;
    
    // Always allocate on CPU using standard malloc
    handle.ptr = std::malloc(size);
    handle.size = size;
    handle.device = DeviceType::CPU;
    handle.isValid = (handle.ptr != nullptr);
    
    return handle;
}

void GPUStubs::free(MemoryHandle& handle) {
    if (handle.isValid && handle.ptr) {
        std::free(handle.ptr);
        handle.ptr = nullptr;
        handle.size = 0;
        handle.isValid = false;
    }
}

bool GPUStubs::copyToDevice(MemoryHandle& handle, const void* data, size_t size) {
    if (!handle.isValid || !data || size > handle.size) {
        return false;
    }
    
    std::memcpy(handle.ptr, data, size);
    return true;
}

bool GPUStubs::copyFromDevice(const MemoryHandle& handle, void* data, size_t size) {
    if (!handle.isValid || !data || size > handle.size) {
        return false;
    }
    
    std::memcpy(data, handle.ptr, size);
    return true;
}

bool GPUStubs::executeKernel(MemoryHandle& handle, const std::string& kernelType, void* params) {
    // No-op: All operations are CPU-based
    // In a real implementation, this would dispatch to appropriate compute kernel
    return true;
}

void GPUStubs::synchronize() {
    // No-op: CPU operations are synchronous
}

bool GPUStubs::isHardwareAccelerated() {
    // Always false since we're using stubs
    return false;
}

DeviceType GPUStubs::getCurrentDevice() {
    return g_currentDevice;
}

#ifdef ENABLE_VULKAN_STUBS
namespace Vulkan {
    bool initializeVulkan() {
        // No-op stub
        return true;
    }
    
    void shutdownVulkan() {
        // No-op stub
    }
    
    std::string getVulkanVersion() {
        return "Vulkan Stub 1.0 (No Hardware)";
    }
}
#endif

#ifdef ENABLE_ROCM_STUBS
namespace ROCm {
    bool initializeROCm() {
        // No-op stub
        return true;
    }
    
    void shutdownROCm() {
        // No-op stub
    }
    
    std::string getROCmVersion() {
        return "ROCm Stub 1.0 (No Hardware)";
    }
}
#endif

} // namespace GPU
} // namespace RawrXD
