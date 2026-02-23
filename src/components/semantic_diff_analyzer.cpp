#include "semantic_diff_analyzer.hpp"
#include <QMutexLocker>
#include <QRegularExpression>
#include <QSet>

SemanticDiffAnalyzer::SemanticDiffAnalyzer(QObject* parent)
    : QObject(parent)
{
}

SemanticDiffAnalyzer::SemanticDiff SemanticDiffAnalyzer::analyzeDiff(const QString& oldContent, const QString& newContent)
{
    QMutexLocker lock(&m_mutex);

    SemanticDiff result;
    const QStringList oldLines = oldContent.split('\n');
    const QStringList newLines = newContent.split('\n');

    int maxLines = qMax(oldLines.size(), newLines.size());

    for (int i = 0; i < maxLines; ++i) {
        const QString oldLine = (i < oldLines.size()) ? oldLines[i] : QString();
        const QString newLine = (i < newLines.size()) ? newLines[i] : QString();

        if (oldLine.isEmpty() && !newLine.isEmpty()) {
            DiffHunk hunk;
            hunk.startLine = i + 1;
            hunk.endLine = i + 1;
            hunk.content = newLine;
            hunk.type = "added";
            result.hunks.append(hunk);
        } else if (!oldLine.isEmpty() && newLine.isEmpty()) {
            DiffHunk hunk;
            hunk.startLine = i + 1;
            hunk.endLine = i + 1;
            hunk.content = oldLine;
            hunk.type = "removed";
            result.hunks.append(hunk);
        } else if (oldLine != newLine) {
            DiffHunk hunk;
            hunk.startLine = i + 1;
            hunk.endLine = i + 1;
            hunk.content = newLine;
            hunk.type = "modified";
            result.hunks.append(hunk);
        }
    }

    // Unlock before calling calculateSimilarity (which acquires lock)
    lock.unlock();
    result.similarity = calculateSimilarity(oldContent, newContent);

    int hunkCount = result.hunks.size();
    result.summary = QString("Found %1 difference(s) with %2% similarity")
                         .arg(hunkCount)
                         .arg(result.similarity * 100.0, 0, 'f', 1);

    lock.relock();
    ++m_analyzedCount;
    lock.unlock();

    emit analysisComplete(result.summary);
    return result;
}

double SemanticDiffAnalyzer::calculateSimilarity(const QString& a, const QString& b)
{
    QMutexLocker lock(&m_mutex);

    if (a.isEmpty() && b.isEmpty())
        return 1.0;
    if (a.isEmpty() || b.isEmpty())
        return 0.0;

    // Jaccard-like character-level similarity
    QSet<QChar> setA;
    for (const auto& ch : a)
        setA.insert(ch);

    QSet<QChar> setB;
    for (const auto& ch : b)
        setB.insert(ch);

    QSet<QChar> intersection = setA;
    intersection.intersect(setB);

    QSet<QChar> unionSet = setA;
    unionSet.unite(setB);

    if (unionSet.isEmpty())
        return 1.0;

    return static_cast<double>(intersection.size()) / static_cast<double>(unionSet.size());
}

QStringList SemanticDiffAnalyzer::extractFunctions(const QString& code)
{
    QMutexLocker lock(&m_mutex);

    QStringList functions;
    // Match common C/C++ function signatures
    static const QRegularExpression funcRegex(
        R"((?:[\w:]+\s+)+(\w+)\s*\([^)]*\)\s*(?:const)?\s*\{)");

    auto it = funcRegex.globalMatch(code);
    while (it.hasNext()) {
        auto match = it.next();
        functions.append(match.captured(1));
    }

    return functions;
}

int SemanticDiffAnalyzer::analyzedCount() const
{
    QMutexLocker lock(&m_mutex);
    return m_analyzedCount;
}
