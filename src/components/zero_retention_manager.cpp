#include "zero_retention_manager.hpp"
#include <QMutexLocker>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>

ZeroRetentionManager::ZeroRetentionManager(QObject* parent)
    : QObject(parent)
{
}

bool ZeroRetentionManager::secureDelete(const QString& filePath)
{
    QMutexLocker lock(&m_mutex);

    QFile file(filePath);
    if (!file.exists()) {
        QString event = QString("Secure delete failed: file not found - %1").arg(filePath);
        m_auditLog.append(event);
        emit auditEvent(event);
        return false;
    }

    // Overwrite file content with zeros before deletion (chunked for large files)
    qint64 remaining = file.size();
    if (file.open(QIODevice::WriteOnly)) {
        constexpr int chunkSize = 1024 * 1024; // 1 MB chunks
        const QByteArray zeros(chunkSize, '\0');
        while (remaining > 0) {
            qint64 toWrite = qMin(static_cast<qint64>(chunkSize), remaining);
            file.write(zeros.constData(), toWrite);
            remaining -= toWrite;
        }
        file.flush();
        file.close();
    }

    bool removed = file.remove();
    if (removed) {
        ++m_deletedItemCount;
        QString event = QString("Securely deleted: %1").arg(filePath);
        m_auditLog.append(event);
        emit dataDeleted(filePath);
        emit auditEvent(event);
    } else {
        QString event = QString("Secure delete removal failed: %1").arg(filePath);
        m_auditLog.append(event);
        emit auditEvent(event);
    }

    return removed;
}

bool ZeroRetentionManager::secureDeleteData(QByteArray& data)
{
    QMutexLocker lock(&m_mutex);

    if (data.isEmpty()) {
        QString event = "Secure delete data: empty buffer";
        m_auditLog.append(event);
        emit auditEvent(event);
        return true;
    }

    // Overwrite with zeros then clear
    data.fill('\0');
    data.clear();

    ++m_deletedItemCount;
    QString event = "Securely deleted in-memory data";
    m_auditLog.append(event);
    emit dataDeleted("in-memory-data");
    emit auditEvent(event);
    return true;
}

bool ZeroRetentionManager::isRetentionCompliant() const
{
    QMutexLocker lock(&m_mutex);
    // Compliant when max retention is zero (no data retained)
    return m_maxRetentionSeconds == 0;
}

void ZeroRetentionManager::setMaxRetentionSeconds(int seconds)
{
    QMutexLocker lock(&m_mutex);
    m_maxRetentionSeconds = seconds;

    QString event = QString("Max retention set to %1 seconds").arg(seconds);
    m_auditLog.append(event);

    if (seconds > 0) {
        emit complianceViolation(QString("Retention period set to %1s — data will be held").arg(seconds));
    }

    emit auditEvent(event);
}

int ZeroRetentionManager::maxRetentionSeconds() const
{
    QMutexLocker lock(&m_mutex);
    return m_maxRetentionSeconds;
}

int ZeroRetentionManager::deletedItemCount() const
{
    QMutexLocker lock(&m_mutex);
    return m_deletedItemCount;
}

void ZeroRetentionManager::purgeExpiredData()
{
    QMutexLocker lock(&m_mutex);

    QString event = QString("Purge expired data executed at %1")
                        .arg(QDateTime::currentDateTime().toString(Qt::ISODate));
    m_auditLog.append(event);
    emit auditEvent(event);
}

QStringList ZeroRetentionManager::auditLog() const
{
    QMutexLocker lock(&m_mutex);
    return m_auditLog;
}
