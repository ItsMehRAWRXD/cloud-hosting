#ifndef AGENT_COORDINATOR_HPP
#define AGENT_COORDINATOR_HPP

#include <QObject>
#include <QMutex>
#include <QMap>
#include <QVector>
#include <QStringList>
#include <QPair>
#include <QSet>

class AgentCoordinator : public QObject
{
    Q_OBJECT

public:
    enum class TaskStatus { Pending, Running, Completed, Failed, Cancelled };

    struct TaskPlan {
        QString id;
        QString description;
        QStringList dependencies;
        TaskStatus status = TaskStatus::Pending;
    };

    explicit AgentCoordinator(QObject* parent = nullptr);
    ~AgentCoordinator() override;

    bool submitPlan(const QString& planId, const QVector<TaskPlan>& tasks);
    bool completeTask(const QString& planId, const QString& taskId, bool success);
    TaskStatus getTaskStatus(const QString& planId, const QString& taskId) const;
    QVector<TaskPlan> getPendingTasks(const QString& planId) const;
    bool detectCycle(const QString& planId) const;
    int activePlanCount() const;
    void cancelPlan(const QString& planId);

signals:
    void taskCompleted(const QString& planId, const QString& taskId);
    void taskFailed(const QString& planId, const QString& taskId, const QString& reason);
    void planCompleted(const QString& planId);
    void cycleDetected(const QString& planId);

private:
    bool hasCycleDFS(const QMap<QString, QStringList>& graph, const QString& node,
                     QSet<QString>& visited, QSet<QString>& recStack) const;

    mutable QMutex m_mutex;
    QMap<QString, QVector<TaskPlan>> m_plans;
};

#endif // AGENT_COORDINATOR_HPP
