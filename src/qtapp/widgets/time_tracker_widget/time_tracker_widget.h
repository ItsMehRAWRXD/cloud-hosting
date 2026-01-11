#ifndef TIME_TRACKER_WIDGET_H
#define TIME_TRACKER_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QTime>
#include <QSettings>

class TimeTrackerWidget : public QWidget {
    Q_OBJECT

public:
    explicit TimeTrackerWidget(QWidget *parent = nullptr);
    ~TimeTrackerWidget();

private slots:
    void onStartStop();
    void onPause();
    void onLap();
    void onClear();
    void updateDisplay();

private:
    void setupUI();
    void loadHistory();
    void saveHistory();
    
    QLabel *m_timeDisplay;
    QPushButton *m_startStopButton;
    QPushButton *m_pauseButton;
    QPushButton *m_lapButton;
    QPushButton *m_clearButton;
    QListWidget *m_lapList;
    QTimer *m_timer;
    QTime *m_elapsedTime;
    
    bool m_running;
    bool m_paused;
    int m_lapCounter;
};

#endif // TIME_TRACKER_WIDGET_H
