#include "time_tracker_widget.h"

TimeTrackerWidget::TimeTrackerWidget(QWidget *parent)
    : QWidget(parent)
    , m_running(false)
    , m_paused(false)
    , m_lapCounter(0)
{
    m_timer = new QTimer(this);
    m_elapsedTime = new QTime(0, 0, 0);
    connect(m_timer, &QTimer::timeout, this, &TimeTrackerWidget::updateDisplay);
    
    setupUI();
    loadHistory();
}

TimeTrackerWidget::~TimeTrackerWidget() {
    saveHistory();
    delete m_elapsedTime;
}

void TimeTrackerWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Time display
    m_timeDisplay = new QLabel("00:00:00.000", this);
    m_timeDisplay->setStyleSheet("font-size: 32px; font-weight: bold; padding: 20px; "
                                 "background-color: #2c3e50; color: #ecf0f1; border-radius: 5px;");
    m_timeDisplay->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_timeDisplay);
    
    // Control buttons
    QHBoxLayout *controlLayout = new QHBoxLayout();
    
    m_startStopButton = new QPushButton("Start", this);
    m_startStopButton->setStyleSheet("background-color: #27ae60; color: white; padding: 10px; font-size: 14px;");
    connect(m_startStopButton, &QPushButton::clicked, this, &TimeTrackerWidget::onStartStop);
    controlLayout->addWidget(m_startStopButton);
    
    m_pauseButton = new QPushButton("Pause", this);
    m_pauseButton->setEnabled(false);
    m_pauseButton->setStyleSheet("padding: 10px; font-size: 14px;");
    connect(m_pauseButton, &QPushButton::clicked, this, &TimeTrackerWidget::onPause);
    controlLayout->addWidget(m_pauseButton);
    
    m_lapButton = new QPushButton("Lap", this);
    m_lapButton->setEnabled(false);
    m_lapButton->setStyleSheet("padding: 10px; font-size: 14px;");
    connect(m_lapButton, &QPushButton::clicked, this, &TimeTrackerWidget::onLap);
    controlLayout->addWidget(m_lapButton);
    
    m_clearButton = new QPushButton("Clear", this);
    m_clearButton->setStyleSheet("background-color: #e74c3c; color: white; padding: 10px; font-size: 14px;");
    connect(m_clearButton, &QPushButton::clicked, this, &TimeTrackerWidget::onClear);
    controlLayout->addWidget(m_clearButton);
    
    mainLayout->addLayout(controlLayout);
    
    // Lap times list
    mainLayout->addWidget(new QLabel("Lap Times:", this));
    m_lapList = new QListWidget(this);
    mainLayout->addWidget(m_lapList);
}

void TimeTrackerWidget::onStartStop() {
    if (!m_running) {
        m_running = true;
        m_paused = false;
        m_timer->start(10); // Update every 10ms
        m_startStopButton->setText("Stop");
        m_startStopButton->setStyleSheet("background-color: #e74c3c; color: white; padding: 10px; font-size: 14px;");
        m_pauseButton->setEnabled(true);
        m_lapButton->setEnabled(true);
    } else {
        m_running = false;
        m_paused = false;
        m_timer->stop();
        m_startStopButton->setText("Start");
        m_startStopButton->setStyleSheet("background-color: #27ae60; color: white; padding: 10px; font-size: 14px;");
        m_pauseButton->setEnabled(false);
        m_pauseButton->setText("Pause");
        m_lapButton->setEnabled(false);
    }
}

void TimeTrackerWidget::onPause() {
    if (!m_paused) {
        m_paused = true;
        m_timer->stop();
        m_pauseButton->setText("Resume");
    } else {
        m_paused = false;
        m_timer->start(10);
        m_pauseButton->setText("Pause");
    }
}

void TimeTrackerWidget::onLap() {
    m_lapCounter++;
    QString lapTime = m_timeDisplay->text();
    m_lapList->addItem(QString("Lap %1: %2").arg(m_lapCounter).arg(lapTime));
}

void TimeTrackerWidget::onClear() {
    m_elapsedTime->setHMS(0, 0, 0, 0);
    m_timeDisplay->setText("00:00:00.000");
    m_lapList->clear();
    m_lapCounter = 0;
    if (m_running) {
        onStartStop();
    }
}

void TimeTrackerWidget::updateDisplay() {
    *m_elapsedTime = m_elapsedTime->addMSecs(10);
    m_timeDisplay->setText(m_elapsedTime->toString("hh:mm:ss.zzz"));
}

void TimeTrackerWidget::loadHistory() {
    QSettings settings("RawrXD", "TimeTracker");
    int count = settings.beginReadArray("laps");
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        m_lapList->addItem(settings.value("time").toString());
    }
    settings.endArray();
}

void TimeTrackerWidget::saveHistory() {
    QSettings settings("RawrXD", "TimeTracker");
    settings.beginWriteArray("laps");
    for (int i = 0; i < m_lapList->count(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("time", m_lapList->item(i)->text());
    }
    settings.endArray();
}
