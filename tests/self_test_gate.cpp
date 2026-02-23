#include "core/patch_result.hpp"
#include "core/platform_utils.hpp"
#include "core/gpu_stubs.hpp"
#include "qtapp/model_memory_hotpatch.hpp"
#include <iostream>
#include <cassert>

using namespace RawrXD;

void test_patch_result() {
    std::cout << "Testing PatchResult..." << std::endl;
    
    auto okResult = PatchResult::ok("Success");
    assert(okResult.success);
    assert(okResult);
    
    auto errResult = PatchResult::error("Failed", -1);
    assert(!errResult.success);
    assert(!errResult);
    
    std::cout << "  ✓ PatchResult tests passed" << std::endl;
}

void test_platform_utils() {
    std::cout << "Testing Platform utilities..." << std::endl;
    
    std::string os = Platform::PlatformInfo::getOSName();
    std::string arch = Platform::PlatformInfo::getArchitecture();
    
    assert(!os.empty());
    assert(!arch.empty());
    
    size_t pageSize = Platform::MemoryOps::getPageSize();
    assert(pageSize > 0);
    
    std::cout << "  Platform: " << os << " " << arch << std::endl;
    std::cout << "  Page size: " << pageSize << " bytes" << std::endl;
    std::cout << "  ✓ Platform utilities tests passed" << std::endl;
}

void test_gpu_stubs() {
    std::cout << "Testing GPU stubs..." << std::endl;
    
    bool init = GPU::GPUStubs::initialize();
    assert(init);
    
    auto devices = GPU::GPUStubs::getAvailableDevices();
    assert(!devices.empty());
    
    std::cout << "  Found " << devices.size() << " device(s)" << std::endl;
    
    auto handle = GPU::GPUStubs::allocate(1024);
    assert(handle.isValid);
    
    GPU::GPUStubs::free(handle);
    
    GPU::GPUStubs::shutdown();
    
    std::cout << "  ✓ GPU stubs tests passed" << std::endl;
}

void test_memory_hotpatch() {
    std::cout << "Testing Memory Hotpatch..." << std::endl;
    
    ModelMemoryHotpatch hotpatch;
    
    // Allocate test memory
    std::vector<uint8_t> testData(256, 0);
    
    MemoryPatch patch;
    patch.targetAddress = testData.data();
    patch.patchData = {0x01, 0x02, 0x03, 0x04};
    patch.operation = PatchOperation::Write;
    patch.size = 4;
    patch.description = "Test patch";
    
    auto result = hotpatch.applyPatch(patch);
    assert(result.success);
    
    // Verify patch was applied
    assert(testData[0] == 0x01);
    assert(testData[1] == 0x02);
    assert(testData[2] == 0x03);
    assert(testData[3] == 0x04);
    
    auto stats = hotpatch.getStatistics();
    assert(stats.totalPatches == 1);
    assert(stats.successfulPatches == 1);
    
    std::cout << "  ✓ Memory hotpatch tests passed" << std::endl;
}

int main() {
    std::cout << "=== RawrXD Self-Test Gate ===" << std::endl;
    std::cout << std::endl;
    
    try {
        test_patch_result();
        test_platform_utils();
        test_gpu_stubs();
        test_memory_hotpatch();
        
        std::cout << std::endl;
        std::cout << "=== All tests passed ✓ ===" << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
