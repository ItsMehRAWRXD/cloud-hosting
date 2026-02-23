#ifndef SEMANTIC_DIFF_ANALYZER_HPP
#define SEMANTIC_DIFF_ANALYZER_HPP

#include <QObject>
#include <QMutex>
#include <QString>
#include <QStringList>
#include <QVector>

class SemanticDiffAnalyzer : public QObject
{
    Q_OBJECT

public:
    struct DiffHunk {
        int startLine;
        int endLine;
        QString content;
        QString type; // "added", "removed", "modified"
    };

    struct SemanticDiff {
        QVector<DiffHunk> hunks;
        double similarity;
        QString summary;
    };

    explicit SemanticDiffAnalyzer(QObject* parent = nullptr);

    SemanticDiff analyzeDiff(const QString& oldContent, const QString& newContent);
    double calculateSimilarity(const QString& a, const QString& b);
    QStringList extractFunctions(const QString& code);
    int analyzedCount() const;

signals:
    void analysisComplete(const QString& summary);
    void errorOccurred(const QString& error);

private:
    mutable QMutex m_mutex;
    int m_analyzedCount = 0;
};

#endif // SEMANTIC_DIFF_ANALYZER_HPP
