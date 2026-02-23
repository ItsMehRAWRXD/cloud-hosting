#ifndef CODE_STREAM_WIDGET_H
#define CODE_STREAM_WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

class SimpleSyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit SimpleSyntaxHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> m_rules;
};

class CodeStreamWidget : public QWidget {
    Q_OBJECT

public:
    explicit CodeStreamWidget(QWidget *parent = nullptr);

private slots:
    void onStartStream();
    void onStopStream();
    void onShowDiff();
    void simulateCodeUpdate();

private:
    void setupUI();
    
    QTextEdit *m_codeEditor;
    QTextEdit *m_diffView;
    QLabel *m_statusLabel;
    QPushButton *m_startButton;
    QPushButton *m_stopButton;
    QPushButton *m_diffButton;
    QTimer *m_updateTimer;
    SimpleSyntaxHighlighter *m_highlighter;
    
    bool m_streaming;
    int m_lineCount;
    QString m_previousCode;
};

#endif // CODE_STREAM_WIDGET_H
