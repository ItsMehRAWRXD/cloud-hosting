#ifndef UPDATECHECKERWIDGET_H
#define UPDATECHECKERWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QSettings>

class UpdateCheckerWidget : public QWidget {
    Q_OBJECT

public:
    explicit UpdateCheckerWidget(QWidget *parent = nullptr);
    ~UpdateCheckerWidget();

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

#endif // UPDATECHECKERWIDGET_H
