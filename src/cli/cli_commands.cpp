#include "cli_commands.hpp"
#include "core/patch_result.hpp"
#include "core/platform_utils.hpp"
#include "core/gpu_stubs.hpp"
#include <iostream>
#include <algorithm>

namespace RawrXD {
namespace CLI {

CommandResult CommandHandler::help() {
    std::cout << "RawrXD CLI - GGUF Model Loader with Hotpatching\n\n";
    std::cout << "Usage: rawrxd-cli [command] [options]\n\n";
    std::cout << "Commands:\n";
    std::cout << "  help              Show this help message\n";
    std::cout << "  version           Show version information\n";
    std::cout << "  load <file>       Load a GGUF model file\n";
    std::cout << "  patch <file>      Apply a hotpatch file\n";
    std::cout << "  devices           List available GPU devices\n";
    std::cout << "  info              Show system information\n\n";
    std::cout << "Options:\n";
    std::cout << "  --verbose         Enable verbose output\n";
    std::cout << "  --device <type>   Select device (cpu, vulkan, rocm)\n";
    std::cout << "\n";
    
    return CommandResult::ok();
}

CommandResult CommandHandler::version() {
    std::cout << "RawrXD CLI v1.0.0\n";
    std::cout << "Build: " << __DATE__ << " " << __TIME__ << "\n";
    std::cout << "Platform: " << Platform::PlatformInfo::getOSName() << " "
              << Platform::PlatformInfo::getArchitecture() << "\n";
    
    return CommandResult::ok();
}

CommandResult CommandHandler::loadModel(const std::string& path) {
    std::cout << "Loading model: " << path << "\n";
    
    // Initialize GPU stubs
    if (!GPU::GPUStubs::initialize()) {
        return CommandResult::error("Failed to initialize GPU subsystem");
    }
    
    std::cout << "Using device: CPU (stub mode)\n";
    std::cout << "Model loaded successfully (stub)\n";
    
    return CommandResult::ok("Model loaded");
}

CommandResult CommandHandler::applyPatch(const std::string& patchFile) {
    std::cout << "Applying patch: " << patchFile << "\n";
    std::cout << "Patch applied successfully (stub)\n";
    
    return CommandResult::ok("Patch applied");
}

CommandResult CommandHandler::listDevices() {
    std::cout << "Available devices:\n";
    
    auto devices = GPU::GPUStubs::getAvailableDevices();
    
    for (size_t i = 0; i < devices.size(); i++) {
        const auto& dev = devices[i];
        std::cout << "  [" << i << "] " << dev.name;
        
        if (dev.isAvailable) {
            std::cout << " (available)";
        } else {
            std::cout << " (unavailable)";
        }
        
        if (dev.totalMemory > 0) {
            std::cout << " - " << (dev.totalMemory / (1024 * 1024)) << " MB";
        }
        
        std::cout << "\n";
    }
    
    return CommandResult::ok();
}

CommandResult CommandHandler::info() {
    std::cout << "System Information:\n";
    std::cout << "  OS: " << Platform::PlatformInfo::getOSName() << "\n";
    std::cout << "  Architecture: " << Platform::PlatformInfo::getArchitecture() << "\n";
    std::cout << "  Page Size: " << Platform::MemoryOps::getPageSize() << " bytes\n";
    std::cout << "  GPU Acceleration: " << (GPU::GPUStubs::isHardwareAccelerated() ? "Yes" : "No (using stubs)") << "\n";
    
    return CommandResult::ok();
}

// ArgumentParser implementation
ArgumentParser::ArgumentParser(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        m_args.push_back(argv[i]);
    }
    
    if (m_args.size() > 1) {
        m_command = m_args[1];
    }
}

bool ArgumentParser::hasFlag(const std::string& flag) const {
    return std::find(m_args.begin(), m_args.end(), flag) != m_args.end();
}

std::string ArgumentParser::getValue(const std::string& key, const std::string& defaultValue) const {
    auto it = std::find(m_args.begin(), m_args.end(), key);
    if (it != m_args.end() && (it + 1) != m_args.end()) {
        return *(it + 1);
    }
    return defaultValue;
}

std::vector<std::string> ArgumentParser::getPositionalArgs() const {
    std::vector<std::string> positional;
    for (size_t i = 2; i < m_args.size(); i++) {
        if (m_args[i][0] != '-') {
            positional.push_back(m_args[i]);
        }
    }
    return positional;
}

std::string ArgumentParser::getCommand() const {
    return m_command;
}

} // namespace CLI
} // namespace RawrXD
