#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTextEdit>
#include <QTabWidget>
#include <QToolBar>
#include <QMenuBar>
#include <QStatusBar>
#include <QLabel>

namespace RawrXD {

class CodeEditor;
class MiniMap;

/**
 * @brief Main window for RawrXD Qt IDE
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onNewFile();
    void onOpenFile();
    void onSaveFile();
    void onExit();
    void onAbout();

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void createNewTab(const QString& title = "Untitled");

    QTabWidget* m_tabWidget;
    QLabel* m_statusLabel;
    QToolBar* m_toolbar;
};

} // namespace RawrXD

#endif // MAINWINDOW_HPP
