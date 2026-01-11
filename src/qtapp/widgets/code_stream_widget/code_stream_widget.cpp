#include "code_stream_widget.h"
#include <QSplitter>

SimpleSyntaxHighlighter::SimpleSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    
    // Keywords
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns = {
        "\\bclass\\b", "\\bstruct\\b", "\\bif\\b", "\\belse\\b",
        "\\bfor\\b", "\\bwhile\\b", "\\breturn\\b", "\\bvoid\\b",
        "\\bint\\b", "\\bfloat\\b", "\\bdouble\\b", "\\bbool\\b"
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        m_rules.append(rule);
    }
    
    // Strings
    QTextCharFormat stringFormat;
    stringFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = stringFormat;
    m_rules.append(rule);
    
    // Comments
    QTextCharFormat commentFormat;
    commentFormat.setForeground(Qt::gray);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = commentFormat;
    m_rules.append(rule);
}

void SimpleSyntaxHighlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : m_rules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

CodeStreamWidget::CodeStreamWidget(QWidget *parent)
    : QWidget(parent)
    , m_streaming(false)
    , m_lineCount(1)
{
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &CodeStreamWidget::simulateCodeUpdate);
    
    setupUI();
}

void CodeStreamWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Status and controls
    QHBoxLayout *controlLayout = new QHBoxLayout();
    m_statusLabel = new QLabel("Not streaming", this);
    m_statusLabel->setStyleSheet("font-weight: bold;");
    controlLayout->addWidget(m_statusLabel);
    controlLayout->addStretch();
    
    m_startButton = new QPushButton("Start Stream", this);
    m_startButton->setStyleSheet("background-color: green; color: white;");
    connect(m_startButton, &QPushButton::clicked, this, &CodeStreamWidget::onStartStream);
    controlLayout->addWidget(m_startButton);
    
    m_stopButton = new QPushButton("Stop Stream", this);
    m_stopButton->setStyleSheet("background-color: red; color: white;");
    m_stopButton->setEnabled(false);
    connect(m_stopButton, &QPushButton::clicked, this, &CodeStreamWidget::onStopStream);
    controlLayout->addWidget(m_stopButton);
    
    m_diffButton = new QPushButton("Show Diff", this);
    connect(m_diffButton, &QPushButton::clicked, this, &CodeStreamWidget::onShowDiff);
    controlLayout->addWidget(m_diffButton);
    
    mainLayout->addLayout(controlLayout);
    
    // Code editor and diff view
    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
    
    m_codeEditor = new QTextEdit(this);
    m_codeEditor->setPlaceholderText("Code stream will appear here...");
    m_codeEditor->setFont(QFont("Courier", 10));
    m_highlighter = new SimpleSyntaxHighlighter(m_codeEditor->document());
    splitter->addWidget(m_codeEditor);
    
    m_diffView = new QTextEdit(this);
    m_diffView->setPlaceholderText("Diff view");
    m_diffView->setFont(QFont("Courier", 9));
    m_diffView->setVisible(false);
    splitter->addWidget(m_diffView);
    
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);
    
    mainLayout->addWidget(splitter);
}

void CodeStreamWidget::onStartStream() {
    m_streaming = true;
    m_statusLabel->setText("🟢 Streaming...");
    m_startButton->setEnabled(false);
    m_stopButton->setEnabled(true);
    m_previousCode = m_codeEditor->toPlainText();
    m_updateTimer->start(2000); // Update every 2 seconds
}

void CodeStreamWidget::onStopStream() {
    m_streaming = false;
    m_statusLabel->setText("Not streaming");
    m_startButton->setEnabled(true);
    m_stopButton->setEnabled(false);
    m_updateTimer->stop();
}

void CodeStreamWidget::onShowDiff() {
    m_diffView->setVisible(!m_diffView->isVisible());
    m_diffButton->setText(m_diffView->isVisible() ? "Hide Diff" : "Show Diff");
}

void CodeStreamWidget::simulateCodeUpdate() {
    if (!m_streaming) return;
    
    // Simulate code changes
    QStringList codeLines = {
        "void function() {",
        "    int x = 42;",
        "    // Comment line",
        "    for (int i = 0; i < 10; i++) {",
        "        printf(\"Line %d\\n\", i);",
        "    }",
        "    return;",
        "}"
    };
    
    if (m_lineCount <= codeLines.size()) {
        QString newCode = codeLines.mid(0, m_lineCount).join("\n");
        m_previousCode = m_codeEditor->toPlainText();
        m_codeEditor->setPlainText(newCode);
        
        // Update diff
        m_diffView->setPlainText(QString("+ Line %1 added\n").arg(m_lineCount));
        m_lineCount++;
    } else {
        onStopStream();
    }
}
