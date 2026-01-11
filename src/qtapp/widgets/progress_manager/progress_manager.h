#ifndef PROGRESS_MANAGER_H
#define PROGRESS_MANAGER_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMap>

struct TaskProgress {
    QString name;
    int progress;
    bool cancellable;
    QProgressBar *progressBar;
    QPushButton *cancelButton;
};

class ProgressManager : public QWidget {
    Q_OBJECT

public:
    explicit ProgressManager(QWidget *parent = nullptr);
    void addTask(const QString &taskName, bool cancellable = true);
    void updateTaskProgress(const QString &taskName, int progress);
    void removeTask(const QString &taskName);

private slots:
    void onAddTestTask();
    void onCancelTask();
    void onClearCompleted();

private:
    void setupUI();
    
    QVBoxLayout *m_tasksLayout;
    QPushButton *m_addTaskButton;
    QPushButton *m_clearButton;
    QLabel *m_summaryLabel;
    QMap<QString, TaskProgress*> m_tasks;
    int m_taskCounter;
};

#endif // PROGRESS_MANAGER_H
