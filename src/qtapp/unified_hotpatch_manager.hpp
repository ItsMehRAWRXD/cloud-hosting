#ifndef UNIFIED_HOTPATCH_MANAGER_HPP
#define UNIFIED_HOTPATCH_MANAGER_HPP

#include "model_memory_hotpatch.hpp"
#include "byte_level_hotpatcher.hpp"
#include "gguf_server_hotpatch.hpp"

namespace RawrXD {

#ifndef NO_QT_SUPPORT
class UnifiedHotpatchManager : public QObject {
    Q_OBJECT
public:
    UnifiedHotpatchManager() : QObject(nullptr) {}
    ~UnifiedHotpatchManager() {}
};
#else
class UnifiedHotpatchManager {
public:
    UnifiedHotpatchManager() {}
    ~UnifiedHotpatchManager() {}
};
#endif

} // namespace RawrXD

#endif // UNIFIED_HOTPATCH_MANAGER_HPP
