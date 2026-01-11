#ifndef AICOMPLETIONCACHE_H
#define AICOMPLETIONCACHE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QSettings>

class AICompletionCache : public QWidget {
    Q_OBJECT

public:
    explicit AICompletionCache(QWidget *parent = nullptr);
    ~AICompletionCache();

private slots:
    void onPrimaryAction();
    void onSecondaryAction();

private:
    void setupUI();
    void loadSettings();
    void saveSettings();
    
    QLabel *m_titleLabel;
    QTextEdit *m_contentArea;
    QListWidget *m_listWidget;
    QPushButton *m_primaryButton;
    QPushButton *m_secondaryButton;
};

#endif // AICOMPLETIONCACHE_H
