#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTextEdit>
#include <QMap>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Widget slots (one for each of the 23 widgets)
    void showWhiteboardWidget();
    void showAudioCallWidget();
    void showScreenShareWidget();
    void showCodeStreamWidget();
    void showAIReviewWidget();
    void showInlineChatWidget();
    void showTelemetryWidget();
    void showProgressManager();
    void showCodeMinimap();
    void showBreadcrumbBar();
    void showTimeTrackerWidget();
    void showTaskManagerWidget();
    void showPomodoroWidget();
    void showSearchResultWidget();
    void showBookmarkWidget();
    void showTodoWidget();
    void showMacroRecorderWidget();
    void showAICompletionCache();
    void showLanguageClientHost();
    void showPluginManagerWidget();
    void showAccessibilityWidget();
    void showUpdateCheckerWidget();
    void showWelcomeScreenWidget();

private:
    void createActions();
    void createMenus();
    void createCentralWidget();
    
    template<typename WidgetType>
    void createAndShowDockWidget(const QString &title, const QString &objectName);

    // Central widget
    QTextEdit *m_centralEditor;
    
    // Menus
    QMenu *m_fileMenu;
    QMenu *m_viewMenu;
    QMenu *m_toolsMenu;
    QMenu *m_helpMenu;
    
    // Dock widgets cache
    QMap<QString, QDockWidget*> m_dockWidgets;
};

#endif // MAINWINDOW_H
