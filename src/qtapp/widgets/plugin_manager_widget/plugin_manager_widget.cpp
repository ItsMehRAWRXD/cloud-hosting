#include "plugin_manager_widget.h"
#include <QTime>

PluginManagerWidget::PluginManagerWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    loadSettings();
}

PluginManagerWidget::~PluginManagerWidget() {
    saveSettings();
}

void PluginManagerWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    m_titleLabel = new QLabel("Plugin management with enable/disable", this);
    m_titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px; "
                               "background-color: #34495e; color: white;");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_titleLabel);
    
    // Content area
    m_contentArea = new QTextEdit(this);
    m_contentArea->setPlaceholderText("Plugin management with enable/disable - Ready for use");
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
    connect(m_primaryButton, &QPushButton::clicked, this, &PluginManagerWidget::onPrimaryAction);
    buttonLayout->addWidget(m_primaryButton);
    
    m_secondaryButton = new QPushButton("Clear", this);
    m_secondaryButton->setStyleSheet("padding: 8px;");
    connect(m_secondaryButton, &QPushButton::clicked, this, &PluginManagerWidget::onSecondaryAction);
    buttonLayout->addWidget(m_secondaryButton);
    
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
}

void PluginManagerWidget::onPrimaryAction() {
    m_titleLabel->setText("Plugin management with enable/disable - Action Executed");
    m_contentArea->append("Action executed at " + QTime::currentTime().toString());
}

void PluginManagerWidget::onSecondaryAction() {
    m_contentArea->clear();
    m_listWidget->clear();
    m_titleLabel->setText("Plugin management with enable/disable");
}

void PluginManagerWidget::loadSettings() {
    QSettings settings("RawrXD", "PluginManagerWidget");
    // Load settings as needed
}

void PluginManagerWidget::saveSettings() {
    QSettings settings("RawrXD", "PluginManagerWidget");
    // Save settings as needed
}
