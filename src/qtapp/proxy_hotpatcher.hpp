#ifndef PROXY_HOTPATCHER_HPP
#define PROXY_HOTPATCHER_HPP

#include "model_memory_hotpatch.hpp"

namespace RawrXD {

#ifndef NO_QT_SUPPORT
class ProxyHotpatcher : public QObject {
    Q_OBJECT
public:
    ProxyHotpatcher() : QObject(nullptr) {}
    ~ProxyHotpatcher() {}
};
#else
class ProxyHotpatcher {
public:
    ProxyHotpatcher() {}
    ~ProxyHotpatcher() {}
};
#endif

} // namespace RawrXD

#endif // PROXY_HOTPATCHER_HPP
