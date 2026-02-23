#include "sandboxed_terminal.hpp"
#include <QMutexLocker>

SandboxedTerminal::SandboxedTerminal(QObject* parent)
    : QObject(parent)
{
    m_blocklist = {
        QStringLiteral("rm -rf /"),
        QStringLiteral("format"),
        QStringLiteral("del /s /q"),
        QStringLiteral("mkfs"),
        QStringLiteral("dd if=/dev/zero"),
        QStringLiteral(":(){:|:&};:")
    };
}

bool SandboxedTerminal::isCommandAllowed(const QString& command) const
{
    QMutexLocker lock(&m_mutex);

    for (const auto& blocked : m_blocklist) {
        if (command.contains(blocked, Qt::CaseInsensitive)) {
            return false;
        }
    }

    if (!m_allowlist.isEmpty()) {
        for (const auto& allowed : m_allowlist) {
            if (command.contains(allowed, Qt::CaseInsensitive)) {
                return true;
            }
        }
        return false;
    }

    return true;
}

bool SandboxedTerminal::executeCommand(const QString& command)
{
    if (!isCommandAllowed(command)) {
        QString reason = QStringLiteral("Command matched blocklist or not in allowlist");
        emit commandBlocked(command, reason);
        return false;
    }

    emit commandStarted(command);

    // Simulate execution — do not run arbitrary commands
    QString simulatedOutput = QStringLiteral("Simulated output for: ") + command;
    int simulatedExitCode = 0;

    {
        QMutexLocker lock(&m_mutex);
        m_lastOutput = simulatedOutput;
        ++m_executionCount;
    }

    emit commandFinished(command, simulatedExitCode, simulatedOutput);
    return true;
}

void SandboxedTerminal::addToAllowlist(const QString& pattern)
{
    QMutexLocker lock(&m_mutex);
    if (!m_allowlist.contains(pattern)) {
        m_allowlist.append(pattern);
    }
}

void SandboxedTerminal::addToBlocklist(const QString& pattern)
{
    QMutexLocker lock(&m_mutex);
    if (!m_blocklist.contains(pattern)) {
        m_blocklist.append(pattern);
    }
}

QStringList SandboxedTerminal::allowlist() const
{
    QMutexLocker lock(&m_mutex);
    return m_allowlist;
}

QStringList SandboxedTerminal::blocklist() const
{
    QMutexLocker lock(&m_mutex);
    return m_blocklist;
}

QString SandboxedTerminal::lastOutput() const
{
    QMutexLocker lock(&m_mutex);
    return m_lastOutput;
}

int SandboxedTerminal::executionCount() const
{
    QMutexLocker lock(&m_mutex);
    return m_executionCount;
}

void SandboxedTerminal::setWorkingDirectory(const QString& dir)
{
    QMutexLocker lock(&m_mutex);
    m_workingDirectory = dir;
}

QString SandboxedTerminal::workingDirectory() const
{
    QMutexLocker lock(&m_mutex);
    return m_workingDirectory;
}

void SandboxedTerminal::setTimeoutMs(int ms)
{
    QMutexLocker lock(&m_mutex);
    m_timeoutMs = ms;
}

int SandboxedTerminal::timeoutMs() const
{
    QMutexLocker lock(&m_mutex);
    return m_timeoutMs;
}
