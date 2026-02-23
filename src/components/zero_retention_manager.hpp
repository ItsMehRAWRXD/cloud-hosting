#ifndef ZERO_RETENTION_MANAGER_HPP
#define ZERO_RETENTION_MANAGER_HPP

#include <QObject>
#include <QMutex>
#include <QByteArray>
#include <QString>
#include <QStringList>

class ZeroRetentionManager : public QObject
{
    Q_OBJECT

public:
    explicit ZeroRetentionManager(QObject* parent = nullptr);

    bool secureDelete(const QString& filePath);
    bool secureDeleteData(QByteArray& data);
    bool isRetentionCompliant() const;
    void setMaxRetentionSeconds(int seconds);
    int maxRetentionSeconds() const;
    int deletedItemCount() const;
    void purgeExpiredData();
    QStringList auditLog() const;

signals:
    void dataDeleted(const QString& identifier);
    void complianceViolation(const QString& detail);
    void auditEvent(const QString& event);

private:
    mutable QMutex m_mutex;
    int m_maxRetentionSeconds = 0;
    int m_deletedItemCount = 0;
    QStringList m_auditLog;
};

#endif // ZERO_RETENTION_MANAGER_HPP
