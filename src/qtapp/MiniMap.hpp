#ifndef MINIMAP_HPP
#define MINIMAP_HPP

#include <QWidget>

namespace RawrXD {

/**
 * @brief Minimap widget showing code overview
 */
class MiniMap : public QWidget {
    Q_OBJECT

public:
    explicit MiniMap(QWidget* parent = nullptr);
    ~MiniMap();

protected:
    void paintEvent(QPaintEvent* event) override;
};

} // namespace RawrXD

#endif // MINIMAP_HPP
