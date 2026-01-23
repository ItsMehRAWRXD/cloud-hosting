#ifndef BYTE_LEVEL_HOTPATCHER_HPP
#define BYTE_LEVEL_HOTPATCHER_HPP

#include "model_memory_hotpatch.hpp"
#include <string>
#include <vector>

#ifndef NO_QT_SUPPORT
#include <QObject>
#include <QByteArray>
#include <QMutex>
#endif

namespace RawrXD {

/**
 * @brief Precision GGUF binary file manipulation
 * 
 * Pattern matching for tensor location discovery.
 * Zero-copy modifications via directWrite(), directRead(), directSearch().
 */
#ifndef NO_QT_SUPPORT
class ByteLevelHotpatcher : public QObject {
    Q_OBJECT
#else
class ByteLevelHotpatcher {
#endif

public:
    ByteLevelHotpatcher();
    ~ByteLevelHotpatcher();
    
    PatchResult loadFile(const std::string& filepath);
    PatchResult saveFile(const std::string& filepath);
    PatchResult directWrite(size_t offset, const std::vector<uint8_t>& data);
    PatchResult directRead(size_t offset, size_t size, std::vector<uint8_t>& out);
    std::vector<size_t> directSearch(const std::vector<uint8_t>& pattern);

#ifndef NO_QT_SUPPORT
signals:
    void fileLoaded(const QString& path);
    void patchApplied(const QString& description);
#endif

private:
#ifndef NO_QT_SUPPORT
    mutable QMutex m_mutex;
    QByteArray m_data;
#else
    std::vector<uint8_t> m_data;
#endif
};

} // namespace RawrXD

#endif // BYTE_LEVEL_HOTPATCHER_HPP
