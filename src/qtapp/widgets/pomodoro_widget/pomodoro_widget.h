#ifndef POMODOROWIDGET_H
#define POMODOROWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QSettings>

class PomodoroWidget : public QWidget {
    Q_OBJECT

public:
    explicit PomodoroWidget(QWidget *parent = nullptr);
    ~PomodoroWidget();

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

#endif // POMODOROWIDGET_H
