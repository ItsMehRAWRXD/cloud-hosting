#include "welcome_screen_widget.h"
#include <QTime>

WelcomeScreenWidget::WelcomeScreenWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadSettings();
}

WelcomeScreenWidget::~WelcomeScreenWidget() {
    saveSettings();
}

void WelcomeScreenWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    m_titleLabel = new QLabel("Welcome screen with recent projects", this);
    m_titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px; "
                               "background-color: #34495e; color: white;");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_titleLabel);
    
    // Content area
    m_contentArea = new QTextEdit(this);
    m_contentArea->setPlaceholderText("Welcome screen with recent projects - Ready for use");
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
    connect(m_primaryButton, &QPushButton::clicked, this, &WelcomeScreenWidget::onPrimaryAction);
    buttonLayout->addWidget(m_primaryButton);
    
    m_secondaryButton = new QPushButton("Clear", this);
    m_secondaryButton->setStyleSheet("padding: 8px;");
    connect(m_secondaryButton, &QPushButton::clicked, this, &WelcomeScreenWidget::onSecondaryAction);
    buttonLayout->addWidget(m_secondaryButton);
    
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
}

void WelcomeScreenWidget::onPrimaryAction() {
    m_titleLabel->setText("Welcome screen with recent projects - Action Executed");
    m_contentArea->append("Action executed at " + QTime::currentTime().toString());
}

void WelcomeScreenWidget::onSecondaryAction() {
    m_contentArea->clear();
    m_listWidget->clear();
    m_titleLabel->setText("Welcome screen with recent projects");
}

void WelcomeScreenWidget::loadSettings() {
    QSettings settings("RawrXD", "WelcomeScreenWidget");
    // Load settings as needed
}

void WelcomeScreenWidget::saveSettings() {
    QSettings settings("RawrXD", "WelcomeScreenWidget");
    // Save settings as needed
}
