#include "agent_coordinator.hpp"

AgentCoordinator::AgentCoordinator(QObject* parent)
    : QObject(parent)
{
}

AgentCoordinator::~AgentCoordinator() = default;

bool AgentCoordinator::submitPlan(const QString& planId, const QVector<TaskPlan>& tasks)
{
    QMutexLocker lock(&m_mutex);

    if (m_plans.contains(planId) || tasks.isEmpty()) {
        return false;
    }

    m_plans.insert(planId, tasks);

    if (detectCycle(planId)) {
        m_plans.remove(planId);
        emit cycleDetected(planId);
        return false;
    }

    return true;
}

bool AgentCoordinator::completeTask(const QString& planId, const QString& taskId, bool success)
{
    QMutexLocker lock(&m_mutex);

    if (!m_plans.contains(planId)) {
        return false;
    }

    auto& tasks = m_plans[planId];
    bool found = false;

    for (auto& task : tasks) {
        if (task.id == taskId) {
            task.status = success ? TaskStatus::Completed : TaskStatus::Failed;
            found = true;
            break;
        }
    }

    if (!found) {
        return false;
    }

    if (!success) {
        emit taskFailed(planId, taskId, QStringLiteral("Task execution failed"));
        return true;
    }

    emit taskCompleted(planId, taskId);

    // Check if all tasks in the plan are completed
    bool allDone = true;
    for (const auto& task : tasks) {
        if (task.status != TaskStatus::Completed && task.status != TaskStatus::Failed
            && task.status != TaskStatus::Cancelled) {
            allDone = false;
            break;
        }
    }

    if (allDone) {
        emit planCompleted(planId);
    }

    return true;
}

AgentCoordinator::TaskStatus AgentCoordinator::getTaskStatus(const QString& planId,
                                                              const QString& taskId) const
{
    QMutexLocker lock(&m_mutex);

    auto it = m_plans.constFind(planId);
    if (it == m_plans.constEnd()) {
        return TaskStatus::Failed;
    }

    for (const auto& task : it.value()) {
        if (task.id == taskId) {
            return task.status;
        }
    }

    return TaskStatus::Failed;
}

QVector<AgentCoordinator::TaskPlan> AgentCoordinator::getPendingTasks(const QString& planId) const
{
    QMutexLocker lock(&m_mutex);

    QVector<TaskPlan> pending;
    auto it = m_plans.constFind(planId);
    if (it == m_plans.constEnd()) {
        return pending;
    }

    for (const auto& task : it.value()) {
        if (task.status == TaskStatus::Pending || task.status == TaskStatus::Running) {
            pending.append(task);
        }
    }

    return pending;
}

bool AgentCoordinator::detectCycle(const QString& planId) const
{
    auto it = m_plans.constFind(planId);
    if (it == m_plans.constEnd()) {
        return false;
    }

    const auto& tasks = it.value();

    // Build adjacency list from task dependencies
    QMap<QString, QStringList> graph;
    for (const auto& task : tasks) {
        if (!graph.contains(task.id)) {
            graph[task.id] = {};
        }
        for (const auto& dep : task.dependencies) {
            graph[dep].append(task.id);
        }
    }

    QSet<QString> visited;
    QSet<QString> recStack;

    for (auto graphIt = graph.constBegin(); graphIt != graph.constEnd(); ++graphIt) {
        if (!visited.contains(graphIt.key())) {
            if (hasCycleDFS(graph, graphIt.key(), visited, recStack)) {
                return true;
            }
        }
    }

    return false;
}

bool AgentCoordinator::hasCycleDFS(const QMap<QString, QStringList>& graph, const QString& node,
                                    QSet<QString>& visited, QSet<QString>& recStack) const
{
    visited.insert(node);
    recStack.insert(node);

    auto it = graph.constFind(node);
    if (it != graph.constEnd()) {
        for (const auto& neighbor : it.value()) {
            if (!visited.contains(neighbor)) {
                if (hasCycleDFS(graph, neighbor, visited, recStack)) {
                    return true;
                }
            } else if (recStack.contains(neighbor)) {
                return true;
            }
        }
    }

    recStack.remove(node);
    return false;
}

int AgentCoordinator::activePlanCount() const
{
    QMutexLocker lock(&m_mutex);
    return m_plans.size();
}

void AgentCoordinator::cancelPlan(const QString& planId)
{
    QMutexLocker lock(&m_mutex);

    auto it = m_plans.find(planId);
    if (it == m_plans.end()) {
        return;
    }

    for (auto& task : it.value()) {
        if (task.status == TaskStatus::Pending || task.status == TaskStatus::Running) {
            task.status = TaskStatus::Cancelled;
        }
    }

    m_plans.remove(planId);
}
