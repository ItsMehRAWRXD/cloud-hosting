#include "progress_manager.h"
#include <QGroupBox>
#include <QTimer>

ProgressManager::ProgressManager(QWidget *parent)
    : QWidget(parent)
    , m_taskCounter(0)
{
    setupUI();
}

void ProgressManager::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Controls
    QHBoxLayout *controlLayout = new QHBoxLayout();
    m_addTaskButton = new QPushButton("Add Test Task", this);
    connect(m_addTaskButton, &QPushButton::clicked, this, &ProgressManager::onAddTestTask);
    controlLayout->addWidget(m_addTaskButton);
    
    m_clearButton = new QPushButton("Clear Completed", this);
    connect(m_clearButton, &QPushButton::clicked, this, &ProgressManager::onClearCompleted);
    controlLayout->addWidget(m_clearButton);
    
    controlLayout->addStretch();
    
    m_summaryLabel = new QLabel("Active tasks: 0", this);
    controlLayout->addWidget(m_summaryLabel);
    
    mainLayout->addLayout(controlLayout);
    
    // Tasks area
    QGroupBox *tasksGroup = new QGroupBox("Active Tasks", this);
    m_tasksLayout = new QVBoxLayout(tasksGroup);
    m_tasksLayout->addStretch();
    
    mainLayout->addWidget(tasksGroup);
}

void ProgressManager::addTask(const QString &taskName, bool cancellable) {
    if (m_tasks.contains(taskName)) return;
    
    TaskProgress *task = new TaskProgress();
    task->name = taskName;
    task->progress = 0;
    task->cancellable = cancellable;
    
    QHBoxLayout *taskLayout = new QHBoxLayout();
    taskLayout->addWidget(new QLabel(taskName, this));
    
    task->progressBar = new QProgressBar(this);
    task->progressBar->setRange(0, 100);
    task->progressBar->setValue(0);
    taskLayout->addWidget(task->progressBar);
    
    if (cancellable) {
        task->cancelButton = new QPushButton("Cancel", this);
        connect(task->cancelButton, &QPushButton::clicked, this, [this, taskName]() {
            removeTask(taskName);
        });
        taskLayout->addWidget(task->cancelButton);
    } else {
        task->cancelButton = nullptr;
    }
    
    m_tasksLayout->insertLayout(m_tasksLayout->count() - 1, taskLayout);
    m_tasks[taskName] = task;
    m_summaryLabel->setText(QString("Active tasks: %1").arg(m_tasks.size()));
}

void ProgressManager::updateTaskProgress(const QString &taskName, int progress) {
    if (!m_tasks.contains(taskName)) return;
    
    TaskProgress *task = m_tasks[taskName];
    task->progress = progress;
    task->progressBar->setValue(progress);
    
    if (progress >= 100) {
        task->progressBar->setStyleSheet("QProgressBar::chunk { background-color: green; }");
    }
}

void ProgressManager::removeTask(const QString &taskName) {
    if (!m_tasks.contains(taskName)) return;
    
    TaskProgress *task = m_tasks[taskName];
    
    // Remove from layout
    for (int i = 0; i < m_tasksLayout->count(); ++i) {
        QLayoutItem *item = m_tasksLayout->itemAt(i);
        if (item && item->layout()) {
            QLayout *layout = item->layout();
            // Check if this layout contains our progress bar
            for (int j = 0; j < layout->count(); ++j) {
                if (layout->itemAt(j)->widget() == task->progressBar) {
                    // Remove all widgets in this layout
                    while (layout->count() > 0) {
                        QLayoutItem *child = layout->takeAt(0);
                        if (child->widget()) {
                            child->widget()->deleteLater();
                        }
                        delete child;
                    }
                    m_tasksLayout->removeItem(item);
                    delete layout;
                    break;
                }
            }
        }
    }
    
    delete task;
    m_tasks.remove(taskName);
    m_summaryLabel->setText(QString("Active tasks: %1").arg(m_tasks.size()));
}

void ProgressManager::onAddTestTask() {
    QString taskName = QString("Task %1").arg(++m_taskCounter);
    addTask(taskName, true);
    
    // Simulate progress
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, taskName, timer]() {
        if (!m_tasks.contains(taskName)) {
            timer->stop();
            timer->deleteLater();
            return;
        }
        
        TaskProgress *task = m_tasks[taskName];
        int newProgress = task->progress + 10;
        updateTaskProgress(taskName, newProgress);
        
        if (newProgress >= 100) {
            timer->stop();
            timer->deleteLater();
        }
    });
    timer->start(500);
}

void ProgressManager::onCancelTask() {
    // Handled by individual cancel buttons
}

void ProgressManager::onClearCompleted() {
    QStringList toRemove;
    for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it) {
        if (it.value()->progress >= 100) {
            toRemove.append(it.key());
        }
    }
    
    for (const QString &taskName : toRemove) {
        removeTask(taskName);
    }
}
