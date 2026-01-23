#include "model_memory_hotpatch.hpp"
#include <cstring>
#include <algorithm>

namespace RawrXD {

ModelMemoryHotpatch::ModelMemoryHotpatch()
#ifndef NO_QT_SUPPORT
    : QObject(nullptr)
#endif
{
    m_stats = {0, 0, 0, 0};
}

ModelMemoryHotpatch::~ModelMemoryHotpatch() {
}

bool ModelMemoryHotpatch::protectMemoryRegion(void* address, size_t size, bool writable) {
    auto protection = writable ? 
        Platform::MemoryProtection::ReadWrite : 
        Platform::MemoryProtection::ReadOnly;
    
    return Platform::MemoryOps::protectMemory(address, size, protection);
}

PatchResult ModelMemoryHotpatch::applyPatch(const MemoryPatch& patch) {
#ifndef NO_QT_SUPPORT
    QMutexLocker lock(&m_mutex);
#endif
    
    if (!patch.targetAddress) {
        return PatchResult::error("Invalid target address", -1);
    }
    
    if (patch.patchData.empty() && patch.operation != PatchOperation::Reverse) {
        return PatchResult::error("Empty patch data", -2);
    }
    
    // Make memory writable
    if (!protectMemoryRegion(patch.targetAddress, patch.size, true)) {
        return PatchResult::error("Failed to change memory protection", -3);
    }
    
    PatchResult result;
    
    switch (patch.operation) {
        case PatchOperation::Write:
            result = applyWriteOperation(patch);
            break;
        case PatchOperation::Swap:
            result = applySwapOperation(patch);
            break;
        case PatchOperation::XOR:
            result = applyXOROperation(patch);
            break;
        case PatchOperation::Rotate:
            result = applyRotateOperation(patch);
            break;
        case PatchOperation::Reverse:
            result = applyReverseOperation(patch);
            break;
        default:
            result = PatchResult::error("Unknown patch operation", -4);
    }
    
    // Restore memory protection
    protectMemoryRegion(patch.targetAddress, patch.size, false);
    
    // Update statistics
    if (result.success) {
        m_stats.successfulPatches++;
        m_stats.bytesModified += patch.size;
#ifndef NO_QT_SUPPORT
        emit patchApplied(QString::fromStdString(patch.description));
#endif
    } else {
        m_stats.failedPatches++;
#ifndef NO_QT_SUPPORT
        emit patchFailed(QString::fromStdString(result.detail));
#endif
    }
    m_stats.totalPatches++;
    
    return result;
}

PatchResult ModelMemoryHotpatch::applyPatches(const std::vector<MemoryPatch>& patches) {
#ifndef NO_QT_SUPPORT
    QMutexLocker lock(&m_mutex);
#endif
    
    for (const auto& patch : patches) {
        // Unlock temporarily for individual patch (which will relock)
#ifndef NO_QT_SUPPORT
        lock.unlock();
#endif
        PatchResult result = applyPatch(patch);
#ifndef NO_QT_SUPPORT
        lock.relock();
#endif
        
        if (!result.success) {
            return PatchResult::error("Batch patch failed: " + result.detail, result.errorCode);
        }
    }
    
    return PatchResult::ok("Applied " + std::to_string(patches.size()) + " patches");
}

PatchResult ModelMemoryHotpatch::applyWriteOperation(const MemoryPatch& patch) {
    std::memcpy(patch.targetAddress, patch.patchData.data(), patch.size);
    return PatchResult::ok("Write operation successful");
}

PatchResult ModelMemoryHotpatch::applySwapOperation(const MemoryPatch& patch) {
    if (patch.patchData.size() < sizeof(void*)) {
        return PatchResult::error("Swap operation requires second address", -5);
    }
    
    void* secondAddress = *reinterpret_cast<void* const*>(patch.patchData.data());
    
    std::vector<uint8_t> temp(patch.size);
    std::memcpy(temp.data(), patch.targetAddress, patch.size);
    std::memcpy(patch.targetAddress, secondAddress, patch.size);
    std::memcpy(secondAddress, temp.data(), patch.size);
    
    return PatchResult::ok("Swap operation successful");
}

PatchResult ModelMemoryHotpatch::applyXOROperation(const MemoryPatch& patch) {
    uint8_t* target = static_cast<uint8_t*>(patch.targetAddress);
    
    for (size_t i = 0; i < patch.size; i++) {
        target[i] ^= patch.patchData[i % patch.patchData.size()];
    }
    
    return PatchResult::ok("XOR operation successful");
}

PatchResult ModelMemoryHotpatch::applyRotateOperation(const MemoryPatch& patch) {
    if (patch.patchData.empty()) {
        return PatchResult::error("Rotate operation requires rotation amount", -6);
    }
    
    int rotateAmount = static_cast<int>(patch.patchData[0]);
    uint8_t* target = static_cast<uint8_t*>(patch.targetAddress);
    
    std::vector<uint8_t> temp(target, target + patch.size);
    std::rotate(temp.begin(), temp.begin() + rotateAmount, temp.end());
    std::memcpy(target, temp.data(), patch.size);
    
    return PatchResult::ok("Rotate operation successful");
}

PatchResult ModelMemoryHotpatch::applyReverseOperation(const MemoryPatch& patch) {
    uint8_t* target = static_cast<uint8_t*>(patch.targetAddress);
    std::reverse(target, target + patch.size);
    
    return PatchResult::ok("Reverse operation successful");
}

void* ModelMemoryHotpatch::findPattern(void* baseAddress, size_t size, 
                                      const std::vector<uint8_t>& pattern) {
#ifndef NO_QT_SUPPORT
    QMutexLocker lock(&m_mutex);
#endif
    
    if (!baseAddress || pattern.empty() || size < pattern.size()) {
        return nullptr;
    }
    
    const uint8_t* base = static_cast<const uint8_t*>(baseAddress);
    
    // Simple pattern search (Boyer-Moore would be better for production)
    for (size_t i = 0; i <= size - pattern.size(); i++) {
        if (std::memcmp(base + i, pattern.data(), pattern.size()) == 0) {
            return static_cast<void*>(const_cast<uint8_t*>(base + i));
        }
    }
    
    return nullptr;
}

ModelMemoryHotpatch::Statistics ModelMemoryHotpatch::getStatistics() const {
#ifndef NO_QT_SUPPORT
    QMutexLocker lock(&m_mutex);
#endif
    return m_stats;
}

void ModelMemoryHotpatch::resetStatistics() {
#ifndef NO_QT_SUPPORT
    QMutexLocker lock(&m_mutex);
#endif
    m_stats = {0, 0, 0, 0};
}

} // namespace RawrXD
