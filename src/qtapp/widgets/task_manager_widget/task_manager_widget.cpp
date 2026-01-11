#include "task_manager_widget.h"
#include <QInputDialog>
#include <QMessageBox>

TaskManagerWidget::TaskManagerWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    addTaskToTree("Implement feature X", "High", "In Progress", "2026-01-15");
    addTaskToTree("Fix bug in Y", "Critical", "Open", "2026-01-12");
    addTaskToTree("Code review", "Medium", "Completed", "2026-01-10");
}

void TaskManagerWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Toolbar
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    
    m_addButton = new QPushButton("Add Task", this);
    m_addButton->setStyleSheet("background-color: #27ae60; color: white; padding: 5px 15px;");
    connect(m_addButton, &QPushButton::clicked, this, &TaskManagerWidget::onAddTask);
    toolbarLayout->addWidget(m_addButton);
    
    m_editButton = new QPushButton("Edit Task", this);
    connect(m_editButton, &QPushButton::clicked, this, &TaskManagerWidget::onEditTask);
    toolbarLayout->addWidget(m_editButton);
    
    m_deleteButton = new QPushButton("Delete Task", this);
    m_deleteButton->setStyleSheet("background-color: #e74c3c; color: white; padding: 5px 15px;");
    connect(m_deleteButton, &QPushButton::clicked, this, &TaskManagerWidget::onDeleteTask);
    toolbarLayout->addWidget(m_deleteButton);
    
    toolbarLayout->addStretch();
    
    toolbarLayout->addWidget(new QLabel("Filter:", this));
    m_filterCombo = new QComboBox(this);
    m_filterCombo->addItems({"All", "Open", "In Progress", "Completed"});
    connect(m_filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TaskManagerWidget::onFilterChanged);
    toolbarLayout->addWidget(m_filterCombo);
    
    mainLayout->addLayout(toolbarLayout);
    
    // Task tree
    m_taskTree = new QTreeWidget(this);
    m_taskTree->setHeaderLabels({"Task Name", "Priority", "Status", "Due Date"});
    m_taskTree->setColumnWidth(0, 250);
    m_taskTree->setColumnWidth(1, 80);
    m_taskTree->setColumnWidth(2, 100);
    mainLayout->addWidget(m_taskTree);
}

void TaskManagerWidget::addTaskToTree(const QString &name, const QString &priority,
                                     const QString &status, const QString &dueDate) {
    QTreeWidgetItem *item = new QTreeWidgetItem(m_taskTree);
    item->setText(0, name);
    item->setText(1, priority);
    item->setText(2, status);
    item->setText(3, dueDate);
    
    // Color coding by priority
    QColor color;
    if (priority == "Critical") color = QColor(255, 200, 200);
    else if (priority == "High") color = QColor(255, 230, 200);
    else if (priority == "Medium") color = QColor(255, 255, 200);
    else color = QColor(220, 255, 220);
    
    for (int i = 0; i < 4; ++i) {
        item->setBackground(i, color);
    }
}

void TaskManagerWidget::onAddTask() {
    bool ok;
    QString taskName = QInputDialog::getText(this, "Add Task", "Task name:", 
                                            QLineEdit::Normal, "", &ok);
    if (ok && !taskName.isEmpty()) {
        addTaskToTree(taskName, "Medium", "Open", "2026-01-20");
    }
}

void TaskManagerWidget::onEditTask() {
    QTreeWidgetItem *item = m_taskTree->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Edit Task", "Please select a task to edit.");
        return;
    }
    
    bool ok;
    QString newName = QInputDialog::getText(this, "Edit Task", "Task name:", 
                                           QLineEdit::Normal, item->text(0), &ok);
    if (ok && !newName.isEmpty()) {
        item->setText(0, newName);
    }
}

void TaskManagerWidget::onDeleteTask() {
    QTreeWidgetItem *item = m_taskTree->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Delete Task", "Please select a task to delete.");
        return;
    }
    
    delete item;
}

void TaskManagerWidget::onFilterChanged(int index) {
    // Simple filter implementation
    QString filter = m_filterCombo->currentText();
    for (int i = 0; i < m_taskTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = m_taskTree->topLevelItem(i);
        if (filter == "All" || item->text(2) == filter) {
            item->setHidden(false);
        } else {
            item->setHidden(true);
        }
    }
}
