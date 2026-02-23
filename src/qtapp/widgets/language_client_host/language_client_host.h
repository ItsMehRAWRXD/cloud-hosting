#ifndef LANGUAGECLIENTHOST_H
#define LANGUAGECLIENTHOST_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QSettings>

class LanguageClientHost : public QWidget {
    Q_OBJECT

public:
    explicit LanguageClientHost(QWidget *parent = nullptr);
    ~LanguageClientHost();

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

#endif // LANGUAGECLIENTHOST_H
