#ifndef STANDALONE_WEB_BRIDGE_HPP
#define STANDALONE_WEB_BRIDGE_HPP
// ============================================================================
// Standalone Web Bridge — lightweight HTTP server for the IDE
// ============================================================================
// Provides a dependency-free web interface that replaces the Qt GUI when
// building with CMakeLists-Standalone.txt.  Uses only Win32 / POSIX sockets
// so no external libraries are required.
// ============================================================================

#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <atomic>

#ifdef _WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  pragma comment(lib, "ws2_32.lib")
   using socket_t = SOCKET;
#  define INVALID_SOCK INVALID_SOCKET
#else
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <unistd.h>
   using socket_t = int;
#  define INVALID_SOCK (-1)
#endif

// ---- Request / Response ---------------------------------------------------

struct HttpRequest {
    std::string method;              // GET, POST, …
    std::string path;                // e.g. "/api/health"
    std::string body;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> query;
};

struct HttpResponse {
    int         status = 200;
    std::string contentType = "application/json";
    std::string body;

    static HttpResponse json(int status, const std::string& json);
    static HttpResponse html(int status, const std::string& html);
    static HttpResponse text(int status, const std::string& text);
};

// ---- Handler callback type ------------------------------------------------

using RouteHandler = std::function<HttpResponse(const HttpRequest&)>;

// ---- StandaloneWebBridge --------------------------------------------------

class StandaloneWebBridge {
public:
    explicit StandaloneWebBridge(uint16_t port = 9000);
    ~StandaloneWebBridge();

    // Register route handlers
    void get (const std::string& path, RouteHandler handler);
    void post(const std::string& path, RouteHandler handler);

    // Lifecycle
    bool start();           // bind + listen, returns false on error
    void run();             // blocking accept-loop
    void stop();            // signals the accept-loop to exit

    uint16_t port() const { return m_port; }

private:
    void handleClient(socket_t client);
    HttpRequest  parseRequest(const std::string& raw);
    std::string  serializeResponse(const HttpResponse& resp);
    void registerBuiltinRoutes();

    uint16_t   m_port;
    socket_t   m_listenSock = INVALID_SOCK;
    std::atomic<bool> m_running{false};

    struct Route {
        std::string method;
        std::string path;
        RouteHandler handler;
    };
    std::vector<Route> m_routes;
};

#endif // STANDALONE_WEB_BRIDGE_HPP
