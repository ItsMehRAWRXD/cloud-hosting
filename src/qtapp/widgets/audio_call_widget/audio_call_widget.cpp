#include "audio_call_widget.h"

AudioCallWidget::AudioCallWidget(QWidget *parent)
    : QWidget(parent)
    , m_callState(Idle)
    , m_microphoneMuted(false)
    , m_speakerVolume(75)
{
    m_callTimer = new QTimer(this);
    connect(m_callTimer, &QTimer::timeout, this, &AudioCallWidget::updateCallTimer);
    
    setupUI();
}

void AudioCallWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Status display
    QGroupBox *statusGroup = new QGroupBox("Call Status", this);
    QVBoxLayout *statusLayout = new QVBoxLayout(statusGroup);
    
    m_statusLabel = new QLabel("Idle", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    statusLayout->addWidget(m_statusLabel);
    
    m_callTimerLabel = new QLabel("00:00:00", this);
    m_callTimerLabel->setAlignment(Qt::AlignCenter);
    m_callTimerLabel->setStyleSheet("font-size: 24px; color: blue;");
    statusLayout->addWidget(m_callTimerLabel);
    
    mainLayout->addWidget(statusGroup);
    
    // Audio controls
    QGroupBox *controlsGroup = new QGroupBox("Audio Controls", this);
    QVBoxLayout *controlsLayout = new QVBoxLayout(controlsGroup);
    
    // Microphone control
    QHBoxLayout *micLayout = new QHBoxLayout();
    micLayout->addWidget(new QLabel("Microphone:", this));
    m_microphoneButton = new QPushButton("Unmuted", this);
    m_microphoneButton->setCheckable(true);
    m_microphoneButton->setStyleSheet("QPushButton:checked { background-color: red; color: white; }");
    connect(m_microphoneButton, &QPushButton::clicked, this, &AudioCallWidget::onMicrophoneToggled);
    micLayout->addWidget(m_microphoneButton);
    micLayout->addStretch();
    controlsLayout->addLayout(micLayout);
    
    // Speaker volume
    QHBoxLayout *volumeLayout = new QHBoxLayout();
    volumeLayout->addWidget(new QLabel("Speaker Volume:", this));
    m_speakerVolumeSlider = new QSlider(Qt::Horizontal, this);
    m_speakerVolumeSlider->setRange(0, 100);
    m_speakerVolumeSlider->setValue(m_speakerVolume);
    connect(m_speakerVolumeSlider, &QSlider::valueChanged,
            this, &AudioCallWidget::onSpeakerVolumeChanged);
    volumeLayout->addWidget(m_speakerVolumeSlider);
    m_volumeLabel = new QLabel(QString::number(m_speakerVolume) + "%", this);
    m_volumeLabel->setMinimumWidth(40);
    volumeLayout->addWidget(m_volumeLabel);
    controlsLayout->addLayout(volumeLayout);
    
    mainLayout->addWidget(controlsGroup);
    
    // Call buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_callButton = new QPushButton("Start Call", this);
    m_callButton->setStyleSheet("background-color: green; color: white; font-size: 14px; padding: 10px;");
    connect(m_callButton, &QPushButton::clicked, this, &AudioCallWidget::onCallButtonClicked);
    buttonLayout->addWidget(m_callButton);
    
    m_hangupButton = new QPushButton("Hang Up", this);
    m_hangupButton->setStyleSheet("background-color: red; color: white; font-size: 14px; padding: 10px;");
    m_hangupButton->setEnabled(false);
    connect(m_hangupButton, &QPushButton::clicked, this, &AudioCallWidget::onHangupButtonClicked);
    buttonLayout->addWidget(m_hangupButton);
    
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
}

void AudioCallWidget::onMicrophoneToggled() {
    m_microphoneMuted = m_microphoneButton->isChecked();
    m_microphoneButton->setText(m_microphoneMuted ? "Muted" : "Unmuted");
}

void AudioCallWidget::onSpeakerVolumeChanged(int volume) {
    m_speakerVolume = volume;
    m_volumeLabel->setText(QString::number(volume) + "%");
}

void AudioCallWidget::onCallButtonClicked() {
    startCall();
}

void AudioCallWidget::onHangupButtonClicked() {
    endCall();
}

void AudioCallWidget::startCall() {
    m_callState = Calling;
    m_statusLabel->setText("Calling...");
    m_callButton->setEnabled(false);
    m_hangupButton->setEnabled(true);
    
    // Simulate connection after 2 seconds
    QTimer::singleShot(2000, this, [this]() {
        m_callState = Connected;
        m_statusLabel->setText("Connected");
        m_callStartTime = QTime::currentTime();
        m_callTimer->start(1000);
    });
}

void AudioCallWidget::endCall() {
    m_callState = Idle;
    m_statusLabel->setText("Idle");
    m_callTimerLabel->setText("00:00:00");
    m_callButton->setEnabled(true);
    m_hangupButton->setEnabled(false);
    m_callTimer->stop();
}

void AudioCallWidget::updateCallTimer() {
    int seconds = m_callStartTime.secsTo(QTime::currentTime());
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    m_callTimerLabel->setText(QString("%1:%2:%3")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(secs, 2, 10, QChar('0')));
}
