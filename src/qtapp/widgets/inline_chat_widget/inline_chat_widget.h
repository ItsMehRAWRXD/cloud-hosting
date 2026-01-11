#ifndef INLINE_CHAT_WIDGET_H
#define INLINE_CHAT_WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QTime>

class InlineChatWidget : public QWidget {
    Q_OBJECT

public:
    explicit InlineChatWidget(QWidget *parent = nullptr);

private slots:
    void onSendMessage();
    void onClearChat();

private:
    void setupUI();
    void addMessage(const QString &sender, const QString &message, bool isUser);
    
    QTextEdit *m_chatHistory;
    QLineEdit *m_messageInput;
    QPushButton *m_sendButton;
    QPushButton *m_clearButton;
};

#endif // INLINE_CHAT_WIDGET_H
