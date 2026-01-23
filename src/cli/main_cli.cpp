#include "cli_commands.hpp"
#include "model_loader.hpp"
#include "core/gpu_stubs.hpp"
#include <iostream>

using namespace RawrXD;
using namespace RawrXD::CLI;

int main(int argc, char** argv) {
    // Initialize GPU subsystem
    GPU::GPUStubs::initialize();
    
    ArgumentParser parser(argc, argv);
    
    std::string command = parser.getCommand();
    
    if (command.empty() || command == "help" || command == "--help" || command == "-h") {
        CommandHandler::help();
        return 0;
    }
    
    CommandResult result;
    
    if (command == "version") {
        result = CommandHandler::version();
    }
    else if (command == "load") {
        auto args = parser.getPositionalArgs();
        if (args.empty()) {
            std::cerr << "Error: load command requires a file path\n";
            return 1;
        }
        result = CommandHandler::loadModel(args[0]);
    }
    else if (command == "patch") {
        auto args = parser.getPositionalArgs();
        if (args.empty()) {
            std::cerr << "Error: patch command requires a file path\n";
            return 1;
        }
        result = CommandHandler::applyPatch(args[0]);
    }
    else if (command == "devices") {
        result = CommandHandler::listDevices();
    }
    else if (command == "info") {
        result = CommandHandler::info();
    }
    else {
        std::cerr << "Error: Unknown command '" << command << "'\n";
        std::cerr << "Run 'rawrxd-cli help' for usage information\n";
        return 1;
    }
    
    // Cleanup
    GPU::GPUStubs::shutdown();
    
    return result.success ? 0 : result.exitCode;
}
