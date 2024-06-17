#ifndef CLICKABLERECT_H
#define CLICKABLERECT_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QBrush>

class ClickableRect : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
private:

public:
    explicit ClickableRect(int m_x, int m_y, int squareSize, QColor color, QGraphicsRectItem *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void signal_clicked(ClickableRect *rect);
};

#endif // CLICKABLERECT_H
