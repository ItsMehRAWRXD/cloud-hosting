#ifndef AUDIO_CALL_WIDGET_H
#define AUDIO_CALL_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

class AudioCallWidget : public QWidget {
    Q_OBJECT

public:
    explicit AudioCallWidget(QWidget *parent = nullptr);

private slots:
    void onMicrophoneToggled();
    void onSpeakerVolumeChanged(int volume);
    void onCallButtonClicked();
    void onHangupButtonClicked();
    void updateCallTimer();

private:
    void setupUI();
    void startCall();
    void endCall();
    
    enum CallState {
        Idle,
        Calling,
        Connected
    };
    
    CallState m_callState;
    QTimer *m_callTimer;
    QTime m_callStartTime;
    
    // UI elements
    QLabel *m_statusLabel;
    QLabel *m_callTimerLabel;
    QPushButton *m_microphoneButton;
    QPushButton *m_callButton;
    QPushButton *m_hangupButton;
    QSlider *m_speakerVolumeSlider;
    QLabel *m_volumeLabel;
    
    bool m_microphoneMuted;
    int m_speakerVolume;
};

#endif // AUDIO_CALL_WIDGET_H
