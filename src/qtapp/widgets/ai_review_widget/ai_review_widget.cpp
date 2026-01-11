#include "ai_review_widget.h"

AIReviewWidget::AIReviewWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    populateSampleData();
}

void AIReviewWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Top controls
    QHBoxLayout *controlLayout = new QHBoxLayout();
    
    controlLayout->addWidget(new QLabel("Filter by severity:", this));
    m_severityFilter = new QComboBox(this);
    m_severityFilter->addItem("All");
    m_severityFilter->addItem("Critical");
    m_severityFilter->addItem("High");
    m_severityFilter->addItem("Medium");
    m_severityFilter->addItem("Low");
    m_severityFilter->addItem("Info");
    connect(m_severityFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AIReviewWidget::onSeverityFilterChanged);
    controlLayout->addWidget(m_severityFilter);
    
    controlLayout->addStretch();
    
    m_refreshButton = new QPushButton("Refresh Review", this);
    connect(m_refreshButton, &QPushButton::clicked, this, &AIReviewWidget::onRefreshReview);
    controlLayout->addWidget(m_refreshButton);
    
    mainLayout->addLayout(controlLayout);
    
    // Summary
    m_summaryLabel = new QLabel(this);
    m_summaryLabel->setStyleSheet("padding: 5px; background-color: #e0e0e0;");
    mainLayout->addWidget(m_summaryLabel);
    
    // Issue tree and details
    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
    
    m_issueTree = new QTreeWidget(this);
    m_issueTree->setHeaderLabels({"File", "Line", "Severity", "Message"});
    m_issueTree->setColumnWidth(0, 200);
    m_issueTree->setColumnWidth(1, 50);
    m_issueTree->setColumnWidth(2, 80);
    connect(m_issueTree, &QTreeWidget::itemClicked,
            this, &AIReviewWidget::onReviewItemSelected);
    splitter->addWidget(m_issueTree);
    
    m_detailsView = new QTextEdit(this);
    m_detailsView->setReadOnly(true);
    m_detailsView->setPlaceholderText("Select an issue to view details");
    splitter->addWidget(m_detailsView);
    
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);
    
    mainLayout->addWidget(splitter);
}

void AIReviewWidget::addReviewIssue(const QString &file, int line, Severity severity,
                                    const QString &message, const QString &details) {
    QTreeWidgetItem *item = new QTreeWidgetItem(m_issueTree);
    item->setText(0, file);
    item->setText(1, QString::number(line));
    item->setText(2, severityToString(severity));
    item->setText(3, message);
    item->setData(0, Qt::UserRole, details);
    item->setData(1, Qt::UserRole, static_cast<int>(severity));
    
    QColor color = severityToColor(severity);
    for (int i = 0; i < 4; ++i) {
        item->setBackground(i, color);
    }
}

void AIReviewWidget::populateSampleData() {
    addReviewIssue("src/main.cpp", 42, Critical, 
                  "Null pointer dereference",
                  "The pointer 'ptr' may be null at this location. "
                  "This can lead to a crash. Add null check before dereferencing.");
    
    addReviewIssue("src/utils.cpp", 128, High,
                  "Buffer overflow risk",
                  "The array access may exceed bounds. Consider using bounds checking.");
    
    addReviewIssue("src/parser.cpp", 256, Medium,
                  "Inefficient algorithm",
                  "This loop has O(n²) complexity. Consider using a hash map for O(n).");
    
    addReviewIssue("src/config.cpp", 15, Low,
                  "Variable naming convention",
                  "Variable 'x' should have a more descriptive name.");
    
    addReviewIssue("src/logger.cpp", 89, Info,
                  "Consider adding logging",
                  "This error path should log the exception for debugging purposes.");
    
    m_summaryLabel->setText("Total issues: 5 | Critical: 1 | High: 1 | Medium: 1 | Low: 1 | Info: 1");
}

void AIReviewWidget::onReviewItemSelected(QTreeWidgetItem *item, int column) {
    if (!item) return;
    
    QString details = item->data(0, Qt::UserRole).toString();
    QString file = item->text(0);
    QString line = item->text(1);
    QString severity = item->text(2);
    QString message = item->text(3);
    
    QString fullDetails = QString(
        "<h3>%1</h3>"
        "<p><b>File:</b> %2<br>"
        "<b>Line:</b> %3<br>"
        "<b>Severity:</b> %4</p>"
        "<p><b>Details:</b><br>%5</p>"
    ).arg(message, file, line, severity, details);
    
    m_detailsView->setHtml(fullDetails);
}

void AIReviewWidget::onSeverityFilterChanged(int index) {
    // Filter implementation (simplified for now)
    m_summaryLabel->setText(QString("Filter: %1").arg(m_severityFilter->currentText()));
}

void AIReviewWidget::onRefreshReview() {
    m_issueTree->clear();
    populateSampleData();
}

QString AIReviewWidget::severityToString(Severity severity) {
    switch (severity) {
        case Critical: return "Critical";
        case High: return "High";
        case Medium: return "Medium";
        case Low: return "Low";
        case Info: return "Info";
        default: return "Unknown";
    }
}

QColor AIReviewWidget::severityToColor(Severity severity) {
    switch (severity) {
        case Critical: return QColor(255, 200, 200); // Light red
        case High: return QColor(255, 220, 200); // Light orange
        case Medium: return QColor(255, 255, 200); // Light yellow
        case Low: return QColor(220, 255, 220); // Light green
        case Info: return QColor(220, 220, 255); // Light blue
        default: return QColor(255, 255, 255);
    }
}
