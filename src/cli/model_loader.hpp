#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include "core/patch_result.hpp"
#include "core/gpu_stubs.hpp"
#include <string>

namespace RawrXD {
namespace CLI {

/**
 * @brief Simple model loader for CLI
 */
class ModelLoader {
public:
    ModelLoader();
    ~ModelLoader();
    
    PatchResult load(const std::string& filepath);
    PatchResult unload();
    
    bool isLoaded() const { return m_loaded; }
    std::string getLoadedPath() const { return m_loadedPath; }
    size_t getModelSize() const { return m_modelSize; }

private:
    bool m_loaded;
    std::string m_loadedPath;
    size_t m_modelSize;
    GPU::MemoryHandle m_memoryHandle;
};

} // namespace CLI
} // namespace RawrXD

#endif // MODEL_LOADER_HPP
