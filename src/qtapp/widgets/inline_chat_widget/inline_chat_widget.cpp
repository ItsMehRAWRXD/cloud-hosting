#include "inline_chat_widget.h"
#include <QScrollBar>
#include <QTimer>

InlineChatWidget::InlineChatWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    addMessage("AI", "Hello! How can I help you today?", false);
}

void InlineChatWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Chat history
    m_chatHistory = new QTextEdit(this);
    m_chatHistory->setReadOnly(true);
    m_chatHistory->setPlaceholderText("Chat messages will appear here...");
    mainLayout->addWidget(m_chatHistory);
    
    // Input area
    QHBoxLayout *inputLayout = new QHBoxLayout();
    
    m_messageInput = new QLineEdit(this);
    m_messageInput->setPlaceholderText("Type your message...");
    connect(m_messageInput, &QLineEdit::returnPressed, this, &InlineChatWidget::onSendMessage);
    inputLayout->addWidget(m_messageInput);
    
    m_sendButton = new QPushButton("Send", this);
    m_sendButton->setStyleSheet("background-color: #4CAF50; color: white; padding: 5px 15px;");
    connect(m_sendButton, &QPushButton::clicked, this, &InlineChatWidget::onSendMessage);
    inputLayout->addWidget(m_sendButton);
    
    m_clearButton = new QPushButton("Clear", this);
    connect(m_clearButton, &QPushButton::clicked, this, &InlineChatWidget::onClearChat);
    inputLayout->addWidget(m_clearButton);
    
    mainLayout->addLayout(inputLayout);
}

void InlineChatWidget::onSendMessage() {
    QString message = m_messageInput->text().trimmed();
    if (message.isEmpty()) return;
    
    addMessage("User", message, true);
    m_messageInput->clear();
    
    // Simulate AI response
    QTimer::singleShot(500, this, [this, message]() {
        QString response = QString("I received your message: \"%1\". How can I assist further?").arg(message);
        addMessage("AI", response, false);
    });
}

void InlineChatWidget::onClearChat() {
    m_chatHistory->clear();
    addMessage("AI", "Chat cleared. How can I help you?", false);
}

void InlineChatWidget::addMessage(const QString &sender, const QString &message, bool isUser) {
    QString timestamp = QTime::currentTime().toString("hh:mm:ss");
    QString color = isUser ? "#0084ff" : "#666666";
    QString align = isUser ? "right" : "left";
    
    QString html = QString(
        "<div style='text-align: %1; margin: 5px;'>"
        "<span style='color: %2; font-weight: bold;'>%3</span> "
        "<span style='color: #999; font-size: 10px;'>%4</span><br>"
        "<span style='background-color: %5; color: white; padding: 5px 10px; "
        "border-radius: 10px; display: inline-block;'>%6</span>"
        "</div>"
    ).arg(align, color, sender, timestamp, color, message);
    
    m_chatHistory->append(html);
    m_chatHistory->verticalScrollBar()->setValue(m_chatHistory->verticalScrollBar()->maximum());
}
