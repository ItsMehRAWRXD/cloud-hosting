#include "MiniMap.hpp"
#include <QPainter>

namespace RawrXD {

MiniMap::MiniMap(QWidget* parent)
    : QWidget(parent)
{
    setFixedWidth(150);
    setMinimumHeight(200);
}

MiniMap::~MiniMap() {
}

void MiniMap::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.fillRect(rect(), QColor(240, 240, 240));
    
    // Draw placeholder text
    painter.drawText(rect(), Qt::AlignCenter, "Minimap");
}

} // namespace RawrXD
