#ifndef SCREEN_SHARE_WIDGET_H
#define SCREEN_SHARE_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QPixmap>
#include <QScreen>
#include <QApplication>

class ScreenShareWidget : public QWidget {
    Q_OBJECT

public:
    explicit ScreenShareWidget(QWidget *parent = nullptr);

private slots:
    void onStartCapture();
    void onStopCapture();
    void onPauseResume();
    void onSelectRegion();
    void captureScreen();

private:
    void setupUI();
    
    QLabel *m_previewLabel;
    QLabel *m_statusLabel;
    QPushButton *m_startButton;
    QPushButton *m_stopButton;
    QPushButton *m_pauseButton;
    QPushButton *m_regionButton;
    QTimer *m_captureTimer;
    
    bool m_capturing;
    bool m_paused;
    bool m_recording;
};

#endif // SCREEN_SHARE_WIDGET_H
