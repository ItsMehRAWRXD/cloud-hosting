#ifndef CODEMINIMAP_H
#define CODEMINIMAP_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QListWidget>
#include <QLineEdit>

class CodeMinimap : public QWidget {
    Q_OBJECT

public:
    explicit CodeMinimap(QWidget *parent = nullptr);

private slots:
    void onAction();

private:
    void setupUI();
    
    QLabel *m_statusLabel;
    QPushButton *m_actionButton;
};

#endif // CODEMINIMAP_H
