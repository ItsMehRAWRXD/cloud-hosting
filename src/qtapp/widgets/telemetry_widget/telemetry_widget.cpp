#include "telemetry_widget.h"
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QRandomGenerator>

TelemetryWidget::TelemetryWidget(QWidget *parent)
    : QWidget(parent)
    , m_dataPoints(0)
    , m_monitoring(false)
    , m_currentMetric("CPU")
{
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &TelemetryWidget::updateMetrics);
    
    setupUI();
    createChart();
}

void TelemetryWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Controls
    QHBoxLayout *controlLayout = new QHBoxLayout();
    
    controlLayout->addWidget(new QLabel("Metric:", this));
    m_metricSelector = new QComboBox(this);
    m_metricSelector->addItems({"CPU Usage", "Memory Usage", "Network Latency", "Disk I/O"});
    connect(m_metricSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TelemetryWidget::onMetricChanged);
    controlLayout->addWidget(m_metricSelector);
    
    m_currentValueLabel = new QLabel("Current: 0%", this);
    m_currentValueLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    controlLayout->addWidget(m_currentValueLabel);
    
    controlLayout->addStretch();
    
    m_toggleButton = new QPushButton("Start Monitoring", this);
    m_toggleButton->setStyleSheet("background-color: green; color: white; padding: 5px 15px;");
    connect(m_toggleButton, &QPushButton::clicked, this, &TelemetryWidget::onToggleMonitoring);
    controlLayout->addWidget(m_toggleButton);
    
    mainLayout->addLayout(controlLayout);
    
    // Chart will be added here
}

void TelemetryWidget::createChart() {
    m_series = new QLineSeries();
    m_series->setName(m_currentMetric);
    
    m_chart = new QChart();
    m_chart->addSeries(m_series);
    m_chart->setTitle("System Telemetry");
    m_chart->setAnimationOptions(QChart::SeriesAnimations);
    
    m_axisX = new QValueAxis();
    m_axisX->setRange(0, 60);
    m_axisX->setTitleText("Time (seconds)");
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_series->attachAxis(m_axisX);
    
    m_axisY = new QValueAxis();
    m_axisY->setRange(0, 100);
    m_axisY->setTitleText("Usage (%)");
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_series->attachAxis(m_axisY);
    
    m_chartView = new QChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (mainLayout) {
        mainLayout->addWidget(m_chartView);
    }
}

void TelemetryWidget::onMetricChanged(int index) {
    m_currentMetric = m_metricSelector->currentText();
    m_series->setName(m_currentMetric);
    m_series->clear();
    m_dataPoints = 0;
}

void TelemetryWidget::onToggleMonitoring() {
    m_monitoring = !m_monitoring;
    
    if (m_monitoring) {
        m_toggleButton->setText("Stop Monitoring");
        m_toggleButton->setStyleSheet("background-color: red; color: white; padding: 5px 15px;");
        m_updateTimer->start(1000); // Update every second
    } else {
        m_toggleButton->setText("Start Monitoring");
        m_toggleButton->setStyleSheet("background-color: green; color: white; padding: 5px 15px;");
        m_updateTimer->stop();
    }
}

void TelemetryWidget::updateMetrics() {
    if (!m_monitoring) return;
    
    double value = getRandomMetricValue();
    m_series->append(m_dataPoints, value);
    m_currentValueLabel->setText(QString("Current: %1%").arg(value, 0, 'f', 1));
    
    m_dataPoints++;
    
    // Keep only last 60 data points
    if (m_dataPoints > 60) {
        m_series->remove(0);
        // Shift x-axis
        m_axisX->setRange(m_dataPoints - 60, m_dataPoints);
    }
}

double TelemetryWidget::getRandomMetricValue() {
    // Generate realistic metric values based on type
    QString metric = m_metricSelector->currentText();
    
    if (metric == "CPU Usage") {
        return 20.0 + (QRandomGenerator::global()->bounded(60) * 1.0);
    } else if (metric == "Memory Usage") {
        return 40.0 + (QRandomGenerator::global()->bounded(50) * 1.0);
    } else if (metric == "Network Latency") {
        return 10.0 + (QRandomGenerator::global()->bounded(90) * 1.0);
    } else { // Disk I/O
        return 5.0 + (QRandomGenerator::global()->bounded(90) * 1.0);
    }
}
