#ifndef BREADCRUMBBAR_H
#define BREADCRUMBBAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QListWidget>
#include <QLineEdit>

class BreadcrumbBar : public QWidget {
    Q_OBJECT

public:
    explicit BreadcrumbBar(QWidget *parent = nullptr);

private slots:
    void onAction();

private:
    void setupUI();
    
    QLabel *m_statusLabel;
    QPushButton *m_actionButton;
};

#endif // BREADCRUMBBAR_H
