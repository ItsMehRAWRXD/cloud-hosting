#ifndef MODEL_MEMORY_HOTPATCH_HPP
#define MODEL_MEMORY_HOTPATCH_HPP

#include "core/patch_result.hpp"
#include "core/platform_utils.hpp"
#include <string>
#include <vector>
#include <cstddef>
#include <cstdint>

#ifndef NO_QT_SUPPORT
#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QString>
#endif

namespace RawrXD {

/**
 * @brief Memory patch operation types
 */
enum class PatchOperation {
    Write,      // Direct write to memory
    Swap,       // Swap two memory regions
    XOR,        // XOR operation on memory
    Rotate,     // Rotate bytes
    Reverse     // Reverse byte order
};

/**
 * @brief Memory patch descriptor
 */
struct MemoryPatch {
    void* targetAddress;
    std::vector<uint8_t> patchData;
    PatchOperation operation;
    size_t size;
    std::string description;
};

/**
 * @brief Direct RAM patching using OS memory protection
 * 
 * Operates on loaded model tensors in GPU/CPU memory.
 * Cross-platform abstractions for Windows (VirtualProtect) and POSIX (mprotect).
 */
#ifndef NO_QT_SUPPORT
class ModelMemoryHotpatch : public QObject {
    Q_OBJECT
#else
class ModelMemoryHotpatch {
#endif

public:
    ModelMemoryHotpatch();
    ~ModelMemoryHotpatch();
    
    /**
     * @brief Apply a memory patch
     * @param patch Patch descriptor
     * @return Result with success status and details
     */
    PatchResult applyPatch(const MemoryPatch& patch);
    
    /**
     * @brief Apply multiple patches atomically
     * @param patches Vector of patches
     * @return Result with success status
     */
    PatchResult applyPatches(const std::vector<MemoryPatch>& patches);
    
    /**
     * @brief Find pattern in memory
     * @param baseAddress Starting address
     * @param size Region size
     * @param pattern Pattern to find
     * @return Pointer to found pattern, or nullptr
     */
    void* findPattern(void* baseAddress, size_t size, const std::vector<uint8_t>& pattern);
    
    /**
     * @brief Get statistics
     */
    struct Statistics {
        size_t totalPatches;
        size_t successfulPatches;
        size_t failedPatches;
        size_t bytesModified;
    };
    
    Statistics getStatistics() const;
    
    /**
     * @brief Reset statistics
     */
    void resetStatistics();

#ifndef NO_QT_SUPPORT
signals:
    void patchApplied(const QString& description);
    void patchFailed(const QString& error);
#endif

private:
    bool protectMemoryRegion(void* address, size_t size, bool writable);
    PatchResult applyWriteOperation(const MemoryPatch& patch);
    PatchResult applySwapOperation(const MemoryPatch& patch);
    PatchResult applyXOROperation(const MemoryPatch& patch);
    PatchResult applyRotateOperation(const MemoryPatch& patch);
    PatchResult applyReverseOperation(const MemoryPatch& patch);

#ifndef NO_QT_SUPPORT
    mutable QMutex m_mutex;
#endif
    Statistics m_stats;
};

} // namespace RawrXD

#endif // MODEL_MEMORY_HOTPATCH_HPP
