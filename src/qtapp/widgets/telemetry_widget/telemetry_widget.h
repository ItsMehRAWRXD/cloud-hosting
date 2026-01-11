#ifndef TELEMETRY_WIDGET_H
#define TELEMETRY_WIDGET_H

#include <QWidget>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

class TelemetryWidget : public QWidget {
    Q_OBJECT

public:
    explicit TelemetryWidget(QWidget *parent = nullptr);

private slots:
    void onMetricChanged(int index);
    void onToggleMonitoring();
    void updateMetrics();

private:
    void setupUI();
    void createChart();
    double getRandomMetricValue();
    
    QChartView *m_chartView;
    QChart *m_chart;
    QLineSeries *m_series;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    QTimer *m_updateTimer;
    QComboBox *m_metricSelector;
    QPushButton *m_toggleButton;
    QLabel *m_currentValueLabel;
    
    int m_dataPoints;
    bool m_monitoring;
    QString m_currentMetric;
};

#endif // TELEMETRY_WIDGET_H
