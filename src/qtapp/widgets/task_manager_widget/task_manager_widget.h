#ifndef TASK_MANAGER_WIDGET_H
#define TASK_MANAGER_WIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>

class TaskManagerWidget : public QWidget {
    Q_OBJECT

public:
    explicit TaskManagerWidget(QWidget *parent = nullptr);

private slots:
    void onAddTask();
    void onEditTask();
    void onDeleteTask();
    void onFilterChanged(int index);

private:
    void setupUI();
    void addTaskToTree(const QString &name, const QString &priority, 
                      const QString &status, const QString &dueDate);
    
    QTreeWidget *m_taskTree;
    QPushButton *m_addButton;
    QPushButton *m_editButton;
    QPushButton *m_deleteButton;
    QComboBox *m_filterCombo;
};

#endif // TASK_MANAGER_WIDGET_H
