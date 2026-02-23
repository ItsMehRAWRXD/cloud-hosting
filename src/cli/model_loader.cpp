#include "model_loader.hpp"
#include <fstream>

namespace RawrXD {
namespace CLI {

ModelLoader::ModelLoader() 
    : m_loaded(false), m_modelSize(0) {
}

ModelLoader::~ModelLoader() {
    if (m_loaded) {
        unload();
    }
}

PatchResult ModelLoader::load(const std::string& filepath) {
    if (m_loaded) {
        return PatchResult::error("Model already loaded. Unload first.");
    }
    
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file) {
        return PatchResult::error("Failed to open file: " + filepath);
    }
    
    m_modelSize = file.tellg();
    file.close();
    
    // Allocate memory using GPU stubs (will use CPU)
    m_memoryHandle = GPU::GPUStubs::allocate(m_modelSize);
    
    if (!m_memoryHandle.isValid) {
        return PatchResult::error("Failed to allocate memory");
    }
    
    m_loaded = true;
    m_loadedPath = filepath;
    
    return PatchResult::ok("Model loaded: " + filepath);
}

PatchResult ModelLoader::unload() {
    if (!m_loaded) {
        return PatchResult::error("No model loaded");
    }
    
    GPU::GPUStubs::free(m_memoryHandle);
    
    m_loaded = false;
    m_loadedPath.clear();
    m_modelSize = 0;
    
    return PatchResult::ok("Model unloaded");
}

} // namespace CLI
} // namespace RawrXD
