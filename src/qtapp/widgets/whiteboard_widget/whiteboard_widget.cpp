#include "whiteboard_widget.h"
#include <QPainter>
#include <QSettings>

// DrawingCanvas implementation
DrawingCanvas::DrawingCanvas(QWidget *parent)
    : QWidget(parent)
    , m_currentColor(Qt::black)
    , m_brushSize(3)
    , m_drawing(false)
{
    setMinimumSize(400, 300);
    m_image = QImage(size(), QImage::Format_RGB32);
    m_image.fill(Qt::white);
}

void DrawingCanvas::setColor(const QColor &color) {
    m_currentColor = color;
}

void DrawingCanvas::setBrushSize(int size) {
    m_brushSize = size;
}

void DrawingCanvas::clear() {
    m_strokes.clear();
    m_undoStack.clear();
    m_image.fill(Qt::white);
    update();
    emit canvasModified();
}

void DrawingCanvas::undo() {
    if (!m_strokes.isEmpty()) {
        m_undoStack.push(m_strokes.takeLast());
        m_image.fill(Qt::white);
        
        // Redraw all remaining strokes
        QPainter painter(&m_image);
        painter.setRenderHint(QPainter::Antialiasing);
        for (const auto &stroke : m_strokes) {
            QPen pen(stroke.color, stroke.brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
            painter.setPen(pen);
            for (int i = 1; i < stroke.points.size(); ++i) {
                painter.drawLine(stroke.points[i-1], stroke.points[i]);
            }
        }
        
        update();
        emit canvasModified();
    }
}

void DrawingCanvas::redo() {
    if (!m_undoStack.isEmpty()) {
        Stroke stroke = m_undoStack.pop();
        m_strokes.append(stroke);
        
        // Draw the restored stroke
        QPainter painter(&m_image);
        painter.setRenderHint(QPainter::Antialiasing);
        QPen pen(stroke.color, stroke.brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);
        for (int i = 1; i < stroke.points.size(); ++i) {
            painter.drawLine(stroke.points[i-1], stroke.points[i]);
        }
        
        update();
        emit canvasModified();
    }
}

bool DrawingCanvas::saveToFile(const QString &filename) {
    return m_image.save(filename);
}

bool DrawingCanvas::loadFromFile(const QString &filename) {
    QImage loaded(filename);
    if (loaded.isNull()) {
        return false;
    }
    
    m_image = loaded.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_strokes.clear();
    m_undoStack.clear();
    update();
    emit canvasModified();
    return true;
}

void DrawingCanvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(0, 0, m_image);
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_drawing = true;
        m_lastPoint = event->pos();
        m_currentStroke.points.clear();
        m_currentStroke.points.append(m_lastPoint);
        m_currentStroke.color = m_currentColor;
        m_currentStroke.brushSize = m_brushSize;
        m_undoStack.clear(); // Clear redo stack on new action
    }
}

void DrawingCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (m_drawing && (event->buttons() & Qt::LeftButton)) {
        QPainter painter(&m_image);
        painter.setRenderHint(QPainter::Antialiasing);
        QPen pen(m_currentColor, m_brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);
        painter.drawLine(m_lastPoint, event->pos());
        
        m_currentStroke.points.append(event->pos());
        m_lastPoint = event->pos();
        update();
    }
}

void DrawingCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && m_drawing) {
        m_drawing = false;
        addStroke();
        emit canvasModified();
    }
}

void DrawingCanvas::addStroke() {
    if (!m_currentStroke.points.isEmpty()) {
        m_strokes.append(m_currentStroke);
    }
}

// WhiteboardWidget implementation
WhiteboardWidget::WhiteboardWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void WhiteboardWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Toolbar
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    
    m_colorButton = new QPushButton("Color", this);
    m_colorButton->setStyleSheet("background-color: black; color: white;");
    connect(m_colorButton, &QPushButton::clicked, this, &WhiteboardWidget::onColorButtonClicked);
    toolbarLayout->addWidget(m_colorButton);
    
    toolbarLayout->addWidget(new QLabel("Brush Size:"));
    m_brushSizeSpinBox = new QSpinBox(this);
    m_brushSizeSpinBox->setRange(1, 50);
    m_brushSizeSpinBox->setValue(3);
    connect(m_brushSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &WhiteboardWidget::onBrushSizeChanged);
    toolbarLayout->addWidget(m_brushSizeSpinBox);
    
    toolbarLayout->addStretch();
    
    m_undoButton = new QPushButton("Undo", this);
    connect(m_undoButton, &QPushButton::clicked, this, &WhiteboardWidget::onUndoClicked);
    toolbarLayout->addWidget(m_undoButton);
    
    m_redoButton = new QPushButton("Redo", this);
    connect(m_redoButton, &QPushButton::clicked, this, &WhiteboardWidget::onRedoClicked);
    toolbarLayout->addWidget(m_redoButton);
    
    m_clearButton = new QPushButton("Clear", this);
    connect(m_clearButton, &QPushButton::clicked, this, &WhiteboardWidget::onClearClicked);
    toolbarLayout->addWidget(m_clearButton);
    
    m_saveButton = new QPushButton("Save", this);
    connect(m_saveButton, &QPushButton::clicked, this, &WhiteboardWidget::onSaveClicked);
    toolbarLayout->addWidget(m_saveButton);
    
    m_loadButton = new QPushButton("Load", this);
    connect(m_loadButton, &QPushButton::clicked, this, &WhiteboardWidget::onLoadClicked);
    toolbarLayout->addWidget(m_loadButton);
    
    mainLayout->addLayout(toolbarLayout);
    
    // Canvas
    m_canvas = new DrawingCanvas(this);
    mainLayout->addWidget(m_canvas);
}

void WhiteboardWidget::onColorButtonClicked() {
    QColor color = QColorDialog::getColor(Qt::black, this, "Select Color");
    if (color.isValid()) {
        m_canvas->setColor(color);
        m_colorButton->setStyleSheet(QString("background-color: %1; color: %2;")
            .arg(color.name())
            .arg(color.lightness() > 128 ? "black" : "white"));
    }
}

void WhiteboardWidget::onBrushSizeChanged(int size) {
    m_canvas->setBrushSize(size);
}

void WhiteboardWidget::onClearClicked() {
    m_canvas->clear();
}

void WhiteboardWidget::onUndoClicked() {
    m_canvas->undo();
}

void WhiteboardWidget::onRedoClicked() {
    m_canvas->redo();
}

void WhiteboardWidget::onSaveClicked() {
    QString filename = QFileDialog::getSaveFileName(this, "Save Whiteboard",
        "", "PNG Image (*.png);;JPEG Image (*.jpg);;All Files (*)");
    if (!filename.isEmpty()) {
        m_canvas->saveToFile(filename);
    }
}

void WhiteboardWidget::onLoadClicked() {
    QString filename = QFileDialog::getOpenFileName(this, "Load Whiteboard",
        "", "Image Files (*.png *.jpg *.jpeg *.bmp);;All Files (*)");
    if (!filename.isEmpty()) {
        m_canvas->loadFromFile(filename);
    }
}
