#ifndef AI_REVIEW_WIDGET_H
#define AI_REVIEW_WIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QComboBox>
#include <QLabel>

class AIReviewWidget : public QWidget {
    Q_OBJECT

public:
    explicit AIReviewWidget(QWidget *parent = nullptr);
    
    enum Severity {
        Critical,
        High,
        Medium,
        Low,
        Info
    };

private slots:
    void onReviewItemSelected(QTreeWidgetItem *item, int column);
    void onSeverityFilterChanged(int index);
    void onRefreshReview();

private:
    void setupUI();
    void addReviewIssue(const QString &file, int line, Severity severity, 
                       const QString &message, const QString &details);
    void populateSampleData();
    QString severityToString(Severity severity);
    QColor severityToColor(Severity severity);
    
    QTreeWidget *m_issueTree;
    QTextEdit *m_detailsView;
    QComboBox *m_severityFilter;
    QPushButton *m_refreshButton;
    QLabel *m_summaryLabel;
};

#endif // AI_REVIEW_WIDGET_H
