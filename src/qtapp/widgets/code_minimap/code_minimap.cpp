#include "code_minimap.h"

CodeMinimap::CodeMinimap(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void CodeMinimap::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    m_statusLabel = new QLabel("Code Minimap Overview", this);
    m_statusLabel->setStyleSheet("font-size: 14px; font-weight: bold; padding: 10px;");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_statusLabel);
    
    QTextEdit *infoText = new QTextEdit(this);
    infoText->setPlaceholderText("Code Minimap Overview - Ready");
    infoText->setReadOnly(true);
    mainLayout->addWidget(infoText);
    
    m_actionButton = new QPushButton("Execute Action", this);
    m_actionButton->setStyleSheet("padding: 8px;");
    connect(m_actionButton, &QPushButton::clicked, this, &CodeMinimap::onAction);
    mainLayout->addWidget(m_actionButton);
}

void CodeMinimap::onAction() {
    m_statusLabel->setText("Action executed!");
}
