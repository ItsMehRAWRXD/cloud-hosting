#ifndef WHITEBOARD_WIDGET_H
#define WHITEBOARD_WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QColor>
#include <QPen>
#include <QImage>
#include <QVector>
#include <QStack>
#include <QPushButton>
#include <QSpinBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QLabel>

class DrawingCanvas : public QWidget {
    Q_OBJECT

public:
    explicit DrawingCanvas(QWidget *parent = nullptr);
    
    void setColor(const QColor &color);
    void setBrushSize(int size);
    void clear();
    void undo();
    void redo();
    bool saveToFile(const QString &filename);
    bool loadFromFile(const QString &filename);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void canvasModified();

private:
    struct Stroke {
        QVector<QPoint> points;
        QColor color;
        int brushSize;
    };
    
    void addStroke();
    
    QImage m_image;
    QVector<Stroke> m_strokes;
    QStack<Stroke> m_undoStack;
    Stroke m_currentStroke;
    QColor m_currentColor;
    int m_brushSize;
    bool m_drawing;
    QPoint m_lastPoint;
};

class WhiteboardWidget : public QWidget {
    Q_OBJECT

public:
    explicit WhiteboardWidget(QWidget *parent = nullptr);

private slots:
    void onColorButtonClicked();
    void onBrushSizeChanged(int size);
    void onClearClicked();
    void onUndoClicked();
    void onRedoClicked();
    void onSaveClicked();
    void onLoadClicked();

private:
    void setupUI();
    
    DrawingCanvas *m_canvas;
    QPushButton *m_colorButton;
    QSpinBox *m_brushSizeSpinBox;
    QPushButton *m_clearButton;
    QPushButton *m_undoButton;
    QPushButton *m_redoButton;
    QPushButton *m_saveButton;
    QPushButton *m_loadButton;
};

#endif // WHITEBOARD_WIDGET_H
