#include "MainWindow.hpp"
#include "CodeEditor.hpp"
#include "MiniMap.hpp"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>

namespace RawrXD {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("RawrXD Agentic IDE");
    resize(1200, 800);
    
    setupUI();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    
    createNewTab();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setTabsClosable(true);
    m_tabWidget->setMovable(true);
    
    connect(m_tabWidget, &QTabWidget::tabCloseRequested, [this](int index) {
        QWidget* widget = m_tabWidget->widget(index);
        m_tabWidget->removeTab(index);
        delete widget;
    });
    
    setCentralWidget(m_tabWidget);
}

void MainWindow::setupMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);
    
    // File menu
    QMenu* fileMenu = menuBar->addMenu("&File");
    fileMenu->addAction("&New", this, &MainWindow::onNewFile, QKeySequence::New);
    fileMenu->addAction("&Open", this, &MainWindow::onOpenFile, QKeySequence::Open);
    fileMenu->addAction("&Save", this, &MainWindow::onSaveFile, QKeySequence::Save);
    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", this, &MainWindow::onExit, QKeySequence::Quit);
    
    // Edit menu
    QMenu* editMenu = menuBar->addMenu("&Edit");
    editMenu->addAction("&Undo", QKeySequence::Undo);
    editMenu->addAction("&Redo", QKeySequence::Redo);
    editMenu->addSeparator();
    editMenu->addAction("Cu&t", QKeySequence::Cut);
    editMenu->addAction("&Copy", QKeySequence::Copy);
    editMenu->addAction("&Paste", QKeySequence::Paste);
    
    // View menu
    QMenu* viewMenu = menuBar->addMenu("&View");
    viewMenu->addAction("Toggle Minimap");
    viewMenu->addAction("Toggle Line Numbers");
    
    // Help menu
    QMenu* helpMenu = menuBar->addMenu("&Help");
    helpMenu->addAction("&About", this, &MainWindow::onAbout);
    
    setMenuBar(menuBar);
}

void MainWindow::setupToolBar() {
    m_toolbar = addToolBar("Main Toolbar");
    m_toolbar->addAction("New");
    m_toolbar->addAction("Open");
    m_toolbar->addAction("Save");
    m_toolbar->addSeparator();
    m_toolbar->addAction("Undo");
    m_toolbar->addAction("Redo");
}

void MainWindow::setupStatusBar() {
    m_statusLabel = new QLabel("Ready");
    statusBar()->addWidget(m_statusLabel);
}

void MainWindow::createNewTab(const QString& title) {
    QWidget* tabWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(tabWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    
    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    
    CodeEditor* editor = new CodeEditor();
    MiniMap* minimap = new MiniMap();
    
    splitter->addWidget(editor);
    splitter->addWidget(minimap);
    splitter->setStretchFactor(0, 4);
    splitter->setStretchFactor(1, 1);
    
    layout->addWidget(splitter);
    
    int index = m_tabWidget->addTab(tabWidget, title);
    m_tabWidget->setCurrentIndex(index);
}

void MainWindow::onNewFile() {
    createNewTab("Untitled");
    m_statusLabel->setText("New file created");
}

void MainWindow::onOpenFile() {
    QString filename = QFileDialog::getOpenFileName(this, "Open File", "", "All Files (*)");
    
    if (!filename.isEmpty()) {
        createNewTab(filename);
        m_statusLabel->setText("Opened: " + filename);
    }
}

void MainWindow::onSaveFile() {
    QString filename = QFileDialog::getSaveFileName(this, "Save File", "", "All Files (*)");
    
    if (!filename.isEmpty()) {
        m_statusLabel->setText("Saved: " + filename);
    }
}

void MainWindow::onExit() {
    close();
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "About RawrXD IDE",
        "RawrXD Agentic IDE v1.0.0\n\n"
        "Advanced GGUF Model Loader with Live Hotpatching\n"
        "© 2025 RawrXD Project");
}

} // namespace RawrXD
