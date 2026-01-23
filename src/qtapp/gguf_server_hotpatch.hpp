#ifndef GGUF_SERVER_HOTPATCH_HPP
#define GGUF_SERVER_HOTPATCH_HPP

#include "model_memory_hotpatch.hpp"

namespace RawrXD {

#ifndef NO_QT_SUPPORT
class GGUFServerHotpatch : public QObject {
    Q_OBJECT
public:
    GGUFServerHotpatch() : QObject(nullptr) {}
    ~GGUFServerHotpatch() {}
};
#else
class GGUFServerHotpatch {
public:
    GGUFServerHotpatch() {}
    ~GGUFServerHotpatch() {}
};
#endif

} // namespace RawrXD

#endif // GGUF_SERVER_HOTPATCH_HPP
