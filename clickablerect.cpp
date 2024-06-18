#include "clickablerect.h"

ClickableRect::ClickableRect(int m_x, int m_y, int squareSize, QColor color, QGraphicsRectItem *parent)
    : QGraphicsRectItem{parent}
{
    setBrush(QBrush(color));
    setRect(m_x, m_y, squareSize, squareSize);
}

void ClickableRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit ClickableRect::signal_clicked(this);
    //QGraphicsRectItem::mousePressEvent(event);
}
