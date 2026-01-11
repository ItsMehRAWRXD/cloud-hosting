#include "screen_share_widget.h"

ScreenShareWidget::ScreenShareWidget(QWidget *parent)
    : QWidget(parent)
    , m_capturing(false)
    , m_paused(false)
    , m_recording(false)
{
    m_captureTimer = new QTimer(this);
    connect(m_captureTimer, &QTimer::timeout, this, &ScreenShareWidget::captureScreen);
    
    setupUI();
}

void ScreenShareWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Status
    m_statusLabel = new QLabel("Not capturing", this);
    m_statusLabel->setStyleSheet("font-weight: bold; padding: 5px;");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_statusLabel);
    
    // Preview area
    m_previewLabel = new QLabel("Screen preview will appear here", this);
    m_previewLabel->setMinimumSize(400, 300);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setStyleSheet("border: 2px solid gray; background-color: #f0f0f0;");
    m_previewLabel->setScaledContents(true);
    mainLayout->addWidget(m_previewLabel);
    
    // Controls
    QHBoxLayout *controlLayout = new QHBoxLayout();
    
    m_startButton = new QPushButton("Start Capture", this);
    m_startButton->setStyleSheet("background-color: green; color: white; padding: 8px;");
    connect(m_startButton, &QPushButton::clicked, this, &ScreenShareWidget::onStartCapture);
    controlLayout->addWidget(m_startButton);
    
    m_pauseButton = new QPushButton("Pause", this);
    m_pauseButton->setEnabled(false);
    connect(m_pauseButton, &QPushButton::clicked, this, &ScreenShareWidget::onPauseResume);
    controlLayout->addWidget(m_pauseButton);
    
    m_stopButton = new QPushButton("Stop Capture", this);
    m_stopButton->setStyleSheet("background-color: red; color: white; padding: 8px;");
    m_stopButton->setEnabled(false);
    connect(m_stopButton, &QPushButton::clicked, this, &ScreenShareWidget::onStopCapture);
    controlLayout->addWidget(m_stopButton);
    
    m_regionButton = new QPushButton("Select Region", this);
    connect(m_regionButton, &QPushButton::clicked, this, &ScreenShareWidget::onSelectRegion);
    controlLayout->addWidget(m_regionButton);
    
    mainLayout->addLayout(controlLayout);
}

void ScreenShareWidget::onStartCapture() {
    m_capturing = true;
    m_paused = false;
    m_recording = true;
    m_statusLabel->setText("🔴 Capturing...");
    m_statusLabel->setStyleSheet("font-weight: bold; padding: 5px; background-color: red; color: white;");
    m_startButton->setEnabled(false);
    m_stopButton->setEnabled(true);
    m_pauseButton->setEnabled(true);
    m_captureTimer->start(100); // Capture every 100ms
}

void ScreenShareWidget::onStopCapture() {
    m_capturing = false;
    m_recording = false;
    m_statusLabel->setText("Not capturing");
    m_statusLabel->setStyleSheet("font-weight: bold; padding: 5px;");
    m_startButton->setEnabled(true);
    m_stopButton->setEnabled(false);
    m_pauseButton->setEnabled(false);
    m_captureTimer->stop();
}

void ScreenShareWidget::onPauseResume() {
    m_paused = !m_paused;
    m_pauseButton->setText(m_paused ? "Resume" : "Pause");
    m_statusLabel->setText(m_paused ? "⏸ Paused" : "🔴 Capturing...");
}

void ScreenShareWidget::onSelectRegion() {
    m_statusLabel->setText("Region selection (simulated)");
}

void ScreenShareWidget::captureScreen() {
    if (!m_capturing || m_paused) {
        return;
    }
    
    // Capture primary screen
    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        QPixmap screenshot = screen->grabWindow(0);
        m_previewLabel->setPixmap(screenshot.scaled(m_previewLabel->size(), 
            Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}
