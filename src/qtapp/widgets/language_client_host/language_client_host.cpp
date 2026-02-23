#include "language_client_host.h"
#include <QTime>

LanguageClientHost::LanguageClientHost(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadSettings();
}

LanguageClientHost::~LanguageClientHost() {
    saveSettings();
}

void LanguageClientHost::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    m_titleLabel = new QLabel("Language Server Protocol status", this);
    m_titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px; "
                               "background-color: #34495e; color: white;");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_titleLabel);
    
    // Content area
    m_contentArea = new QTextEdit(this);
    m_contentArea->setPlaceholderText("Language Server Protocol status - Ready for use");
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
    connect(m_primaryButton, &QPushButton::clicked, this, &LanguageClientHost::onPrimaryAction);
    buttonLayout->addWidget(m_primaryButton);
    
    m_secondaryButton = new QPushButton("Clear", this);
    m_secondaryButton->setStyleSheet("padding: 8px;");
    connect(m_secondaryButton, &QPushButton::clicked, this, &LanguageClientHost::onSecondaryAction);
    buttonLayout->addWidget(m_secondaryButton);
    
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
}

void LanguageClientHost::onPrimaryAction() {
    m_titleLabel->setText("Language Server Protocol status - Action Executed");
    m_contentArea->append("Action executed at " + QTime::currentTime().toString());
}

void LanguageClientHost::onSecondaryAction() {
    m_contentArea->clear();
    m_listWidget->clear();
    m_titleLabel->setText("Language Server Protocol status");
}

void LanguageClientHost::loadSettings() {
    QSettings settings("RawrXD", "LanguageClientHost");
    // Load settings as needed
}

void LanguageClientHost::saveSettings() {
    QSettings settings("RawrXD", "LanguageClientHost");
    // Save settings as needed
}
