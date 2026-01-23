#include "CodeEditor.hpp"
#include <QPainter>
#include <QTextBlock>

namespace RawrXD {

CodeEditor::CodeEditor(QWidget* parent)
    : QPlainTextEdit(parent)
{
    setFont(QFont("Courier", 10));
    setTabStopDistance(40);
    
    // Set some default text for demonstration
    setPlaceholderText("Start typing code here...");
}

CodeEditor::~CodeEditor() {
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent* event) {
    // Stub implementation
}

int CodeEditor::lineNumberAreaWidth() {
    return 50; // Fixed width for now
}

} // namespace RawrXD
