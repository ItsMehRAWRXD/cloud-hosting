#include "MainWindow.h"
#include "Subsystems.h"
#include <QVBoxLayout>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralEditor(nullptr)
{
    setWindowTitle("RawrXD IDE");
    resize(1200, 800);
    
    createCentralWidget();
    createActions();
    createMenus();
    
    statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow() {
    // QObject parent-child relationship handles cleanup
}

void MainWindow::createCentralWidget() {
    m_centralEditor = new QTextEdit(this);
    m_centralEditor->setPlaceholderText("Code editor area...");
    setCentralWidget(m_centralEditor);
}

void MainWindow::createActions() {
    // Actions are created inline in createMenus() for simplicity
}

void MainWindow::createMenus() {
    // File menu
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *exitAction = m_fileMenu->addAction(tr("E&xit"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    // View menu
    m_viewMenu = menuBar()->addMenu(tr("&View"));
    
    // Tools menu
    m_toolsMenu = menuBar()->addMenu(tr("&Tools"));
    
    // Add widget actions to Tools menu
    QAction *whiteboardAction = m_toolsMenu->addAction(tr("Whiteboard"));
    connect(whiteboardAction, &QAction::triggered, this, &MainWindow::showWhiteboardWidget);
    
    QAction *audioCallAction = m_toolsMenu->addAction(tr("Audio Call"));
    connect(audioCallAction, &QAction::triggered, this, &MainWindow::showAudioCallWidget);
    
    QAction *screenShareAction = m_toolsMenu->addAction(tr("Screen Share"));
    connect(screenShareAction, &QAction::triggered, this, &MainWindow::showScreenShareWidget);
    
    QAction *codeStreamAction = m_toolsMenu->addAction(tr("Code Stream"));
    connect(codeStreamAction, &QAction::triggered, this, &MainWindow::showCodeStreamWidget);
    
    QAction *aiReviewAction = m_toolsMenu->addAction(tr("AI Review"));
    connect(aiReviewAction, &QAction::triggered, this, &MainWindow::showAIReviewWidget);
    
    m_toolsMenu->addSeparator();
    
    QAction *inlineChatAction = m_toolsMenu->addAction(tr("Inline Chat"));
    connect(inlineChatAction, &QAction::triggered, this, &MainWindow::showInlineChatWidget);
    
    QAction *telemetryAction = m_toolsMenu->addAction(tr("Telemetry"));
    connect(telemetryAction, &QAction::triggered, this, &MainWindow::showTelemetryWidget);
    
    QAction *progressAction = m_toolsMenu->addAction(tr("Progress Manager"));
    connect(progressAction, &QAction::triggered, this, &MainWindow::showProgressManager);
    
    QAction *minimapAction = m_toolsMenu->addAction(tr("Code Minimap"));
    connect(minimapAction, &QAction::triggered, this, &MainWindow::showCodeMinimap);
    
    QAction *breadcrumbAction = m_toolsMenu->addAction(tr("Breadcrumb Bar"));
    connect(breadcrumbAction, &QAction::triggered, this, &MainWindow::showBreadcrumbBar);
    
    m_toolsMenu->addSeparator();
    
    QAction *timeTrackerAction = m_toolsMenu->addAction(tr("Time Tracker"));
    connect(timeTrackerAction, &QAction::triggered, this, &MainWindow::showTimeTrackerWidget);
    
    QAction *taskManagerAction = m_toolsMenu->addAction(tr("Task Manager"));
    connect(taskManagerAction, &QAction::triggered, this, &MainWindow::showTaskManagerWidget);
    
    QAction *pomodoroAction = m_toolsMenu->addAction(tr("Pomodoro Timer"));
    connect(pomodoroAction, &QAction::triggered, this, &MainWindow::showPomodoroWidget);
    
    QAction *searchAction = m_toolsMenu->addAction(tr("Search Results"));
    connect(searchAction, &QAction::triggered, this, &MainWindow::showSearchResultWidget);
    
    QAction *bookmarkAction = m_toolsMenu->addAction(tr("Bookmarks"));
    connect(bookmarkAction, &QAction::triggered, this, &MainWindow::showBookmarkWidget);
    
    m_toolsMenu->addSeparator();
    
    QAction *todoAction = m_toolsMenu->addAction(tr("TODO List"));
    connect(todoAction, &QAction::triggered, this, &MainWindow::showTodoWidget);
    
    QAction *macroAction = m_toolsMenu->addAction(tr("Macro Recorder"));
    connect(macroAction, &QAction::triggered, this, &MainWindow::showMacroRecorderWidget);
    
    QAction *cacheAction = m_toolsMenu->addAction(tr("AI Completion Cache"));
    connect(cacheAction, &QAction::triggered, this, &MainWindow::showAICompletionCache);
    
    QAction *lspAction = m_toolsMenu->addAction(tr("Language Server"));
    connect(lspAction, &QAction::triggered, this, &MainWindow::showLanguageClientHost);
    
    QAction *pluginAction = m_toolsMenu->addAction(tr("Plugin Manager"));
    connect(pluginAction, &QAction::triggered, this, &MainWindow::showPluginManagerWidget);
    
    QAction *accessibilityAction = m_toolsMenu->addAction(tr("Accessibility"));
    connect(accessibilityAction, &QAction::triggered, this, &MainWindow::showAccessibilityWidget);
    
    QAction *updateAction = m_toolsMenu->addAction(tr("Update Checker"));
    connect(updateAction, &QAction::triggered, this, &MainWindow::showUpdateCheckerWidget);
    
    // Help menu
    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *welcomeAction = m_helpMenu->addAction(tr("Welcome Screen"));
    connect(welcomeAction, &QAction::triggered, this, &MainWindow::showWelcomeScreenWidget);
}

template<typename WidgetType>
void MainWindow::createAndShowDockWidget(const QString &title, const QString &objectName) {
    // Check if dock widget already exists
    if (m_dockWidgets.contains(objectName)) {
        QDockWidget *dock = m_dockWidgets[objectName];
        dock->show();
        dock->raise();
        return;
    }
    
    // Create new dock widget
    QDockWidget *dockWidget = new QDockWidget(title, this);
    dockWidget->setObjectName(objectName);
    
    WidgetType *widget = new WidgetType(dockWidget);
    dockWidget->setWidget(widget);
    
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    m_dockWidgets[objectName] = dockWidget;
    
    dockWidget->show();
}

// Widget slot implementations
void MainWindow::showWhiteboardWidget() {
    createAndShowDockWidget<WhiteboardWidget>("Whiteboard", "whiteboardDock");
}

void MainWindow::showAudioCallWidget() {
    createAndShowDockWidget<AudioCallWidget>("Audio Call", "audioCallDock");
}

void MainWindow::showScreenShareWidget() {
    createAndShowDockWidget<ScreenShareWidget>("Screen Share", "screenShareDock");
}

void MainWindow::showCodeStreamWidget() {
    createAndShowDockWidget<CodeStreamWidget>("Code Stream", "codeStreamDock");
}

void MainWindow::showAIReviewWidget() {
    createAndShowDockWidget<AIReviewWidget>("AI Review", "aiReviewDock");
}

void MainWindow::showInlineChatWidget() {
    createAndShowDockWidget<InlineChatWidget>("Inline Chat", "inlineChatDock");
}

void MainWindow::showTelemetryWidget() {
    createAndShowDockWidget<TelemetryWidget>("Telemetry", "telemetryDock");
}

void MainWindow::showProgressManager() {
    createAndShowDockWidget<ProgressManager>("Progress Manager", "progressManagerDock");
}

void MainWindow::showCodeMinimap() {
    createAndShowDockWidget<CodeMinimap>("Code Minimap", "codeMinimapDock");
}

void MainWindow::showBreadcrumbBar() {
    createAndShowDockWidget<BreadcrumbBar>("Breadcrumb Bar", "breadcrumbBarDock");
}

void MainWindow::showTimeTrackerWidget() {
    createAndShowDockWidget<TimeTrackerWidget>("Time Tracker", "timeTrackerDock");
}

void MainWindow::showTaskManagerWidget() {
    createAndShowDockWidget<TaskManagerWidget>("Task Manager", "taskManagerDock");
}

void MainWindow::showPomodoroWidget() {
    createAndShowDockWidget<PomodoroWidget>("Pomodoro Timer", "pomodoroDock");
}

void MainWindow::showSearchResultWidget() {
    createAndShowDockWidget<SearchResultWidget>("Search Results", "searchResultDock");
}

void MainWindow::showBookmarkWidget() {
    createAndShowDockWidget<BookmarkWidget>("Bookmarks", "bookmarkDock");
}

void MainWindow::showTodoWidget() {
    createAndShowDockWidget<TodoWidget>("TODO List", "todoDock");
}

void MainWindow::showMacroRecorderWidget() {
    createAndShowDockWidget<MacroRecorderWidget>("Macro Recorder", "macroRecorderDock");
}

void MainWindow::showAICompletionCache() {
    createAndShowDockWidget<AICompletionCache>("AI Completion Cache", "aiCompletionCacheDock");
}

void MainWindow::showLanguageClientHost() {
    createAndShowDockWidget<LanguageClientHost>("Language Server", "languageClientHostDock");
}

void MainWindow::showPluginManagerWidget() {
    createAndShowDockWidget<PluginManagerWidget>("Plugin Manager", "pluginManagerDock");
}

void MainWindow::showAccessibilityWidget() {
    createAndShowDockWidget<AccessibilityWidget>("Accessibility", "accessibilityDock");
}

void MainWindow::showUpdateCheckerWidget() {
    createAndShowDockWidget<UpdateCheckerWidget>("Update Checker", "updateCheckerDock");
}

void MainWindow::showWelcomeScreenWidget() {
    createAndShowDockWidget<WelcomeScreenWidget>("Welcome Screen", "welcomeScreenDock");
}
