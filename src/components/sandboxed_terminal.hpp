#ifndef SANDBOXED_TERMINAL_HPP
#define SANDBOXED_TERMINAL_HPP

#include <QObject>
#include <QMutex>
#include <QString>
#include <QStringList>

class SandboxedTerminal : public QObject {
    Q_OBJECT

public:
    explicit SandboxedTerminal(QObject* parent = nullptr);

    bool executeCommand(const QString& command);
    bool isCommandAllowed(const QString& command) const;
    void addToAllowlist(const QString& pattern);
    void addToBlocklist(const QString& pattern);
    QStringList allowlist() const;
    QStringList blocklist() const;
    QString lastOutput() const;
    int executionCount() const;
    void setWorkingDirectory(const QString& dir);
    QString workingDirectory() const;
    void setTimeoutMs(int ms);
    int timeoutMs() const;

signals:
    void commandStarted(const QString& command);
    void commandFinished(const QString& command, int exitCode, const QString& output);
    void commandBlocked(const QString& command, const QString& reason);
    void errorOccurred(const QString& error);

private:
    mutable QMutex m_mutex;
    QStringList m_allowlist;
    QStringList m_blocklist;
    QString m_lastOutput;
    int m_executionCount = 0;
    QString m_workingDirectory;
    int m_timeoutMs = 30000;
};

#endif // SANDBOXED_TERMINAL_HPP
