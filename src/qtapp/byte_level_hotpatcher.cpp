#include "byte_level_hotpatcher.hpp"
#include <fstream>

namespace RawrXD {

ByteLevelHotpatcher::ByteLevelHotpatcher()
#ifndef NO_QT_SUPPORT
    : QObject(nullptr)
#endif
{
}

ByteLevelHotpatcher::~ByteLevelHotpatcher() {
}

PatchResult ByteLevelHotpatcher::loadFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        return PatchResult::error("Failed to open file: " + filepath);
    }
    
#ifndef NO_QT_SUPPORT
    QMutexLocker lock(&m_mutex);
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    m_data.resize(size);
    file.read(m_data.data(), size);
    
    emit fileLoaded(QString::fromStdString(filepath));
#else
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    m_data.resize(size);
    file.read(reinterpret_cast<char*>(m_data.data()), size);
#endif
    
    return PatchResult::ok("Loaded file: " + filepath);
}

PatchResult ByteLevelHotpatcher::saveFile(const std::string& filepath) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file) {
        return PatchResult::error("Failed to save file: " + filepath);
    }
    
#ifndef NO_QT_SUPPORT
    QMutexLocker lock(&m_mutex);
    file.write(m_data.data(), m_data.size());
#else
    file.write(reinterpret_cast<const char*>(m_data.data()), m_data.size());
#endif
    
    return PatchResult::ok("Saved file: " + filepath);
}

PatchResult ByteLevelHotpatcher::directWrite(size_t offset, const std::vector<uint8_t>& data) {
#ifndef NO_QT_SUPPORT
    QMutexLocker lock(&m_mutex);
    if (offset + data.size() > static_cast<size_t>(m_data.size())) {
        return PatchResult::error("Write would exceed file bounds");
    }
    std::memcpy(m_data.data() + offset, data.data(), data.size());
    emit patchApplied(QString("Write at offset %1").arg(offset));
#else
    if (offset + data.size() > m_data.size()) {
        return PatchResult::error("Write would exceed file bounds");
    }
    std::memcpy(m_data.data() + offset, data.data(), data.size());
#endif
    
    return PatchResult::ok("Write successful");
}

PatchResult ByteLevelHotpatcher::directRead(size_t offset, size_t size, std::vector<uint8_t>& out) {
#ifndef NO_QT_SUPPORT
    QMutexLocker lock(&m_mutex);
    if (offset + size > static_cast<size_t>(m_data.size())) {
        return PatchResult::error("Read would exceed file bounds");
    }
    out.resize(size);
    std::memcpy(out.data(), m_data.data() + offset, size);
#else
    if (offset + size > m_data.size()) {
        return PatchResult::error("Read would exceed file bounds");
    }
    out.resize(size);
    std::memcpy(out.data(), m_data.data() + offset, size);
#endif
    
    return PatchResult::ok("Read successful");
}

std::vector<size_t> ByteLevelHotpatcher::directSearch(const std::vector<uint8_t>& pattern) {
    std::vector<size_t> results;
    
#ifndef NO_QT_SUPPORT
    QMutexLocker lock(&m_mutex);
    for (int i = 0; i <= m_data.size() - static_cast<int>(pattern.size()); i++) {
        if (std::memcmp(m_data.data() + i, pattern.data(), pattern.size()) == 0) {
            results.push_back(i);
        }
    }
#else
    for (size_t i = 0; i <= m_data.size() - pattern.size(); i++) {
        if (std::memcmp(m_data.data() + i, pattern.data(), pattern.size()) == 0) {
            results.push_back(i);
        }
    }
#endif
    
    return results;
}

} // namespace RawrXD
