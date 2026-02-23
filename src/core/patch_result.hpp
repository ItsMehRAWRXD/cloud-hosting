#ifndef PATCH_RESULT_HPP
#define PATCH_RESULT_HPP

#include <string>

namespace RawrXD {

/**
 * @brief Result structure for all patch operations
 * 
 * Used across memory, byte-level, and server hotpatching layers.
 * Never throws exceptions - always returns structured results.
 */
struct PatchResult {
    bool success;
    std::string detail;
    int errorCode;
    
    // Factory methods for semantic clarity
    static PatchResult ok(const std::string& message = "Success") {
        return PatchResult{true, message, 0};
    }
    
    static PatchResult error(const std::string& message, int code = -1) {
        return PatchResult{false, message, code};
    }
    
    // Implicit bool conversion for easy checking
    operator bool() const { return success; }
};

/**
 * @brief Patch layer types for unified management
 */
enum class PatchLayer {
    Memory,      // Direct RAM patching
    ByteLevel,   // GGUF binary file manipulation
    Server,      // Request/response transformation
    Proxy        // Agentic output correction
};

/**
 * @brief Unified result for multi-layer operations
 */
struct UnifiedResult {
    bool success;
    std::string operation;
    std::string detail;
    PatchLayer layer;
    
    static UnifiedResult ok(const std::string& op, const std::string& msg, PatchLayer l) {
        return UnifiedResult{true, op, msg, l};
    }
    
    static UnifiedResult failureResult(const std::string& op, const std::string& msg, PatchLayer l) {
        return UnifiedResult{false, op, msg, l};
    }
    
    operator bool() const { return success; }
};

} // namespace RawrXD

#endif // PATCH_RESULT_HPP
