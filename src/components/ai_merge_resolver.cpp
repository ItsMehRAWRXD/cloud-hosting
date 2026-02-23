#include "ai_merge_resolver.hpp"
#include <QMutexLocker>

AIMergeResolver::AIMergeResolver(QObject* parent)
    : QObject(parent)
{
}

AIMergeResolver::MergeResolution AIMergeResolver::resolveConflict(const MergeConflict& conflict)
{
    QMutexLocker lock(&m_mutex);

    MergeResolution resolution;

    if (conflict.oursContent == conflict.theirsContent) {
        // Both sides are identical
        resolution.resolvedContent = conflict.oursContent;
        resolution.confidence = 1.0;
        resolution.strategy = "identical";
    } else if (conflict.oursContent == conflict.baseContent) {
        // Ours matches base, take theirs
        resolution.resolvedContent = conflict.theirsContent;
        resolution.confidence = 0.9;
        resolution.strategy = "three-way-merge";
    } else if (conflict.theirsContent == conflict.baseContent) {
        // Theirs matches base, take ours
        resolution.resolvedContent = conflict.oursContent;
        resolution.confidence = 0.9;
        resolution.strategy = "three-way-merge";
    } else {
        // Both sides differ from base — combine content
        resolution.resolvedContent = conflict.oursContent + "\n" + conflict.theirsContent;
        resolution.confidence = 0.6;
        resolution.strategy = "ai-combined";
    }

    if (resolution.confidence >= m_confidenceThreshold) {
        ++m_resolvedCount;
        emit conflictResolved(conflict.filePath, resolution.confidence);
    } else {
        emit resolutionFailed(conflict.filePath, "Confidence below threshold");
    }

    return resolution;
}

QVector<AIMergeResolver::MergeResolution> AIMergeResolver::resolveAll(const QVector<MergeConflict>& conflicts)
{
    QVector<MergeResolution> results;
    results.reserve(conflicts.size());

    for (const auto& conflict : conflicts) {
        results.append(resolveConflict(conflict));
    }

    return results;
}

void AIMergeResolver::setConfidenceThreshold(double threshold)
{
    QMutexLocker lock(&m_mutex);
    m_confidenceThreshold = threshold;
}

double AIMergeResolver::confidenceThreshold() const
{
    QMutexLocker lock(&m_mutex);
    return m_confidenceThreshold;
}

int AIMergeResolver::resolvedCount() const
{
    QMutexLocker lock(&m_mutex);
    return m_resolvedCount;
}
