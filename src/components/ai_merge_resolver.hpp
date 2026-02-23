#ifndef AI_MERGE_RESOLVER_HPP
#define AI_MERGE_RESOLVER_HPP

#include <QObject>
#include <QMutex>
#include <QString>
#include <QVector>

class AIMergeResolver : public QObject
{
    Q_OBJECT

public:
    struct MergeConflict {
        QString filePath;
        QString oursContent;
        QString theirsContent;
        QString baseContent;
    };

    struct MergeResolution {
        QString resolvedContent;
        double confidence;
        QString strategy;
    };

    explicit AIMergeResolver(QObject* parent = nullptr);

    MergeResolution resolveConflict(const MergeConflict& conflict);
    QVector<MergeResolution> resolveAll(const QVector<MergeConflict>& conflicts);
    void setConfidenceThreshold(double threshold);
    double confidenceThreshold() const;
    int resolvedCount() const;

signals:
    void conflictResolved(const QString& filePath, double confidence);
    void resolutionFailed(const QString& filePath, const QString& reason);

private:
    mutable QMutex m_mutex;
    double m_confidenceThreshold = 0.7;
    int m_resolvedCount = 0;
};

#endif // AI_MERGE_RESOLVER_HPP
