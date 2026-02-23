// ============================================================================
// Standalone Main — entry point for the Qt-free standalone IDE build
// ============================================================================
// Built via CMakeLists-Standalone.txt.
// Launches the StandaloneWebBridge HTTP server so the IDE UI is available
// in any browser without requiring Qt libraries.
// ============================================================================

#include "standalone_web_bridge.hpp"

#include <csignal>
#include <iostream>
#include <string>

static StandaloneWebBridge* g_bridge = nullptr;

static void signalHandler(int /*sig*/) {
    if (g_bridge) g_bridge->stop();
}

int main(int argc, char* argv[]) {
    uint16_t port = 9000;

    // Simple argument parsing: --port <N>
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if ((arg == "--port" || arg == "-p") && i + 1 < argc) {
            port = static_cast<uint16_t>(std::stoi(argv[++i]));
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: RawrXD-Standalone [--port N]\n"
                      << "  --port, -p   HTTP listen port (default: 9000)\n"
                      << "  --help, -h   Show this message\n";
            return 0;
        }
    }

    std::cout << "=== RawrXD Standalone IDE ===\n"
              << "  Port: " << port << "\n"
              << "  Mode: standalone (no Qt)\n\n";

    StandaloneWebBridge bridge(port);
    g_bridge = &bridge;

    std::signal(SIGINT,  signalHandler);
    std::signal(SIGTERM, signalHandler);

    if (!bridge.start()) {
        std::cerr << "Failed to start web bridge on port " << port << "\n";
        return 1;
    }

    bridge.run();

    std::cout << "\nShutdown complete.\n";
    return 0;
}
