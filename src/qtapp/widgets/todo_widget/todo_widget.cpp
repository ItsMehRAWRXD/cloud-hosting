#include "todo_widget.h"
#include <QTime>

TodoWidget::TodoWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadSettings();
}

TodoWidget::~TodoWidget() {
    saveSettings();
}

void TodoWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    m_titleLabel = new QLabel("TODO list with categories and persistence", this);
    m_titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px; "
                               "background-color: #34495e; color: white;");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_titleLabel);
    
    // Content area
    m_contentArea = new QTextEdit(this);
    m_contentArea->setPlaceholderText("TODO list with categories and persistence - Ready for use");
    mainLayout->addWidget(m_contentArea);
    
    // List widget
    m_listWidget = new QListWidget(this);
    m_listWidget->addItem("Sample Item 1");
    m_listWidget->addItem("Sample Item 2");
    m_listWidget->addItem("Sample Item 3");
    mainLayout->addWidget(m_listWidget);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_primaryButton = new QPushButton("Execute", this);
    m_primaryButton->setStyleSheet("background-color: #3498db; color: white; padding: 8px;");
    connect(m_primaryButton, &QPushButton::clicked, this, &TodoWidget::onPrimaryAction);
    buttonLayout->addWidget(m_primaryButton);
    
    m_secondaryButton = new QPushButton("Clear", this);
    m_secondaryButton->setStyleSheet("padding: 8px;");
    connect(m_secondaryButton, &QPushButton::clicked, this, &TodoWidget::onSecondaryAction);
    buttonLayout->addWidget(m_secondaryButton);
    
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
}

void TodoWidget::onPrimaryAction() {
    m_titleLabel->setText("TODO list with categories and persistence - Action Executed");
    m_contentArea->append("Action executed at " + QTime::currentTime().toString());
}

void TodoWidget::onSecondaryAction() {
    m_contentArea->clear();
    m_listWidget->clear();
    m_titleLabel->setText("TODO list with categories and persistence");
}

void TodoWidget::loadSettings() {
    QSettings settings("RawrXD", "TodoWidget");
    // Load settings as needed
}

void TodoWidget::saveSettings() {
    QSettings settings("RawrXD", "TodoWidget");
    // Save settings as needed
}
