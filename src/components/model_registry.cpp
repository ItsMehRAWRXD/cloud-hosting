#include "model_registry.hpp"
#include <QMutexLocker>
#include <QFileInfo>

ModelRegistry::ModelRegistry(QObject* parent)
    : QObject(parent)
{
}

bool ModelRegistry::registerModel(const ModelInfo& info)
{
    if (info.id.isEmpty()) {
        emit errorOccurred(QStringLiteral("Cannot register model with empty id"));
        return false;
    }

    {
        QMutexLocker lock(&m_mutex);
        if (m_models.contains(info.id)) {
            emit errorOccurred(QStringLiteral("Model already registered: ") + info.id);
            return false;
        }
        m_models.insert(info.id, info);
    }

    emit modelRegistered(info.id);
    return true;
}

bool ModelRegistry::unregisterModel(const QString& id)
{
    {
        QMutexLocker lock(&m_mutex);
        auto it = m_models.find(id);
        if (it == m_models.end()) {
            emit errorOccurred(QStringLiteral("Model not found: ") + id);
            return false;
        }
        if (it->loaded) {
            emit errorOccurred(QStringLiteral("Cannot unregister loaded model: ") + id);
            return false;
        }
        m_models.erase(it);
    }

    emit modelUnregistered(id);
    return true;
}

ModelRegistry::ModelInfo ModelRegistry::getModel(const QString& id) const
{
    QMutexLocker lock(&m_mutex);
    return m_models.value(id);
}

QVector<ModelRegistry::ModelInfo> ModelRegistry::allModels() const
{
    QMutexLocker lock(&m_mutex);
    QVector<ModelInfo> result;
    result.reserve(m_models.size());
    for (const auto& model : m_models) {
        result.append(model);
    }
    return result;
}

QVector<ModelRegistry::ModelInfo> ModelRegistry::findModels(const QString& query) const
{
    QMutexLocker lock(&m_mutex);
    QVector<ModelInfo> result;
    for (const auto& model : m_models) {
        if (model.id.contains(query, Qt::CaseInsensitive) ||
            model.name.contains(query, Qt::CaseInsensitive) ||
            model.format.contains(query, Qt::CaseInsensitive)) {
            result.append(model);
        }
    }
    return result;
}

bool ModelRegistry::loadModel(const QString& id)
{
    {
        QMutexLocker lock(&m_mutex);
        auto it = m_models.find(id);
        if (it == m_models.end()) {
            emit errorOccurred(QStringLiteral("Model not found: ") + id);
            return false;
        }
        if (it->loaded) {
            return true;
        }
        if (!QFileInfo::exists(it->path)) {
            emit errorOccurred(QStringLiteral("Model file not found: ") + it->path);
            return false;
        }
        it->loaded = true;
    }

    emit modelLoaded(id);
    return true;
}

bool ModelRegistry::unloadModel(const QString& id)
{
    {
        QMutexLocker lock(&m_mutex);
        auto it = m_models.find(id);
        if (it == m_models.end()) {
            emit errorOccurred(QStringLiteral("Model not found: ") + id);
            return false;
        }
        if (!it->loaded) {
            return true;
        }
        it->loaded = false;
    }

    emit modelUnloaded(id);
    return true;
}

bool ModelRegistry::isModelLoaded(const QString& id) const
{
    QMutexLocker lock(&m_mutex);
    auto it = m_models.find(id);
    if (it == m_models.end()) {
        return false;
    }
    return it->loaded;
}

int ModelRegistry::modelCount() const
{
    QMutexLocker lock(&m_mutex);
    return m_models.size();
}

int ModelRegistry::loadedModelCount() const
{
    QMutexLocker lock(&m_mutex);
    int count = 0;
    for (const auto& model : m_models) {
        if (model.loaded) {
            ++count;
        }
    }
    return count;
}

bool ModelRegistry::validateModel(const QString& id) const
{
    QMutexLocker lock(&m_mutex);
    auto it = m_models.find(id);
    if (it == m_models.end()) {
        return false;
    }
    QFileInfo fileInfo(it->path);
    return fileInfo.exists() && fileInfo.size() > 0;
}
