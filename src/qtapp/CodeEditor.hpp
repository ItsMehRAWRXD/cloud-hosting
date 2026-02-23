#ifndef CODEEDITOR_HPP
#define CODEEDITOR_HPP

#include <QPlainTextEdit>

namespace RawrXD {

/**
 * @brief Code editor widget with syntax highlighting
 */
class CodeEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit CodeEditor(QWidget* parent = nullptr);
    ~CodeEditor();

protected:
    void lineNumberAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth();

private:
    QWidget* m_lineNumberArea;
};

} // namespace RawrXD

#endif // CODEEDITOR_HPP
