// ============================================================================
// Standalone Web Bridge — implementation
// ============================================================================
// Zero-dependency HTTP server for the standalone IDE build.
// Uses only Win32 sockets (winsock2) on Windows, POSIX sockets elsewhere.
// ============================================================================

#include "standalone_web_bridge.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstring>
#include <thread>

// ---------------------------------------------------------------------------
// Platform helpers
// ---------------------------------------------------------------------------
#ifdef _WIN32
static bool wsa_init() {
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa) == 0;
}
static void wsa_cleanup() { WSACleanup(); }
static void close_socket(socket_t s) { closesocket(s); }
#else
static bool wsa_init() { return true; }
static void wsa_cleanup() {}
static void close_socket(socket_t s) { ::close(s); }
#endif

// ---------------------------------------------------------------------------
// HttpResponse factory helpers
// ---------------------------------------------------------------------------
HttpResponse HttpResponse::json(int status, const std::string& j) {
    return {status, "application/json", j};
}
HttpResponse HttpResponse::html(int status, const std::string& h) {
    return {status, "text/html; charset=utf-8", h};
}
HttpResponse HttpResponse::text(int status, const std::string& t) {
    return {status, "text/plain", t};
}

// ---------------------------------------------------------------------------
// StandaloneWebBridge
// ---------------------------------------------------------------------------

StandaloneWebBridge::StandaloneWebBridge(uint16_t port)
    : m_port(port)
{
    registerBuiltinRoutes();
}

StandaloneWebBridge::~StandaloneWebBridge() {
    stop();
}

void StandaloneWebBridge::get(const std::string& path, RouteHandler handler) {
    m_routes.push_back({"GET", path, std::move(handler)});
}

void StandaloneWebBridge::post(const std::string& path, RouteHandler handler) {
    m_routes.push_back({"POST", path, std::move(handler)});
}

// ---- Builtin routes -------------------------------------------------------

void StandaloneWebBridge::registerBuiltinRoutes() {
    get("/api/health", [](const HttpRequest&) {
        return HttpResponse::json(200,
            R"({"status":"ok","engine":"standalone","version":"1.0.0"})");
    });

    get("/api/version", [](const HttpRequest&) {
        return HttpResponse::json(200,
            R"({"name":"RawrXD-AgenticIDE","variant":"standalone","version":"1.0.0"})");
    });

    get("/", [](const HttpRequest&) {
        std::string html = R"(<!DOCTYPE html>
<html><head><title>RawrXD Standalone IDE</title>
<style>
body{font-family:system-ui;background:#1e1e2e;color:#cdd6f4;margin:0;display:flex;
     align-items:center;justify-content:center;height:100vh}
.card{background:#313244;padding:2em 3em;border-radius:12px;text-align:center;
      box-shadow:0 8px 32px rgba(0,0,0,.4)}
h1{color:#89b4fa;margin:0 0 .5em}
p{color:#a6adc8;margin:.3em 0}
code{background:#45475a;padding:2px 8px;border-radius:4px;font-size:13px}
</style></head><body>
<div class="card">
  <h1>RawrXD Standalone IDE</h1>
  <p>Standalone build &mdash; no Qt dependency</p>
  <p>Health: <code>GET /api/health</code></p>
  <p>Version: <code>GET /api/version</code></p>
</div>
</body></html>)";
        return HttpResponse::html(200, html);
    });
}

// ---- Server lifecycle -----------------------------------------------------

bool StandaloneWebBridge::start() {
    if (!wsa_init()) {
        std::cerr << "[web-bridge] WSA init failed\n";
        return false;
    }

    m_listenSock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_listenSock == INVALID_SOCK) {
        std::cerr << "[web-bridge] socket() failed\n";
        return false;
    }

    int opt = 1;
#ifdef _WIN32
    setsockopt(m_listenSock, SOL_SOCKET, SO_REUSEADDR,
               reinterpret_cast<const char*>(&opt), sizeof(opt));
#else
    setsockopt(m_listenSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(m_port);

    if (::bind(m_listenSock, reinterpret_cast<sockaddr*>(&addr),
               sizeof(addr)) != 0) {
        std::cerr << "[web-bridge] bind() failed on port " << m_port << "\n";
        close_socket(m_listenSock);
        m_listenSock = INVALID_SOCK;
        return false;
    }

    if (::listen(m_listenSock, SOMAXCONN) != 0) {
        std::cerr << "[web-bridge] listen() failed\n";
        close_socket(m_listenSock);
        m_listenSock = INVALID_SOCK;
        return false;
    }

    m_running = true;
    return true;
}

void StandaloneWebBridge::run() {
    std::cout << "[web-bridge] Listening on http://localhost:"
              << m_port << "\n";

    while (m_running) {
        sockaddr_in client_addr{};
#ifdef _WIN32
        int len = sizeof(client_addr);
#else
        socklen_t len = sizeof(client_addr);
#endif
        socket_t client = ::accept(m_listenSock,
                                   reinterpret_cast<sockaddr*>(&client_addr),
                                   &len);
        if (client == INVALID_SOCK) {
            if (!m_running) break;      // stop() closed the socket
            continue;
        }
        // Handle each connection (synchronous for simplicity)
        handleClient(client);
        close_socket(client);
    }
}

void StandaloneWebBridge::stop() {
    m_running = false;
    if (m_listenSock != INVALID_SOCK) {
        close_socket(m_listenSock);
        m_listenSock = INVALID_SOCK;
    }
    wsa_cleanup();
}

// ---- Request handling -----------------------------------------------------

void StandaloneWebBridge::handleClient(socket_t client) {
    char buf[8192];
    int  n = ::recv(client, buf, sizeof(buf) - 1, 0);
    if (n <= 0) return;
    buf[n] = '\0';

    HttpRequest  req  = parseRequest(std::string(buf, static_cast<size_t>(n)));
    HttpResponse resp = HttpResponse::json(404,
                            R"({"error":"Not Found"})");

    for (auto& route : m_routes) {
        if (route.method == req.method && route.path == req.path) {
            resp = route.handler(req);
            break;
        }
    }

    std::string raw = serialiseResponse(resp);
    ::send(client, raw.data(), static_cast<int>(raw.size()), 0);
}

HttpRequest StandaloneWebBridge::parseRequest(const std::string& raw) {
    HttpRequest req;
    std::istringstream stream(raw);
    std::string line;

    // Request line
    if (std::getline(stream, line)) {
        std::istringstream rl(line);
        rl >> req.method >> req.path;
    }

    // Split path and query string
    auto qpos = req.path.find('?');
    if (qpos != std::string::npos) {
        std::string qs = req.path.substr(qpos + 1);
        req.path = req.path.substr(0, qpos);
        // Parse simple key=value pairs
        std::istringstream qstream(qs);
        std::string pair;
        while (std::getline(qstream, pair, '&')) {
            auto eq = pair.find('=');
            if (eq != std::string::npos)
                req.query[pair.substr(0, eq)] = pair.substr(eq + 1);
        }
    }

    // Headers
    while (std::getline(stream, line) && line != "\r" && !line.empty()) {
        if (line.back() == '\r') line.pop_back();
        auto colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string val = line.substr(colon + 1);
            while (!val.empty() && val.front() == ' ') val.erase(val.begin());
            req.headers[key] = val;
        }
    }

    // Body (remaining bytes)
    auto hdr_end = raw.find("\r\n\r\n");
    if (hdr_end != std::string::npos && hdr_end + 4 < raw.size()) {
        req.body = raw.substr(hdr_end + 4);
    }

    return req;
}

std::string StandaloneWebBridge::serialiseResponse(const HttpResponse& resp) {
    std::ostringstream out;
    out << "HTTP/1.1 " << resp.status << " ";
    switch (resp.status) {
        case 200: out << "OK";        break;
        case 400: out << "Bad Request"; break;
        case 403: out << "Forbidden"; break;
        case 404: out << "Not Found"; break;
        case 500: out << "Internal Server Error"; break;
        default:  out << "Unknown";   break;
    }
    out << "\r\n";
    out << "Content-Type: "   << resp.contentType          << "\r\n";
    out << "Content-Length: " << resp.body.size()           << "\r\n";
    out << "Connection: close\r\n";
    out << "Access-Control-Allow-Origin: *\r\n";
    out << "\r\n";
    out << resp.body;
    return out.str();
}
