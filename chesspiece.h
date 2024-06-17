#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>

class ChessPiece : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
private:
    bool m_color;
    bool m_first_move;
    unsigned short m_piece;
public:
    explicit ChessPiece(bool color, unsigned short piece, const int squareSize);
    bool getColor() const;
    bool getFirst_Move() const;
    unsigned short getPiece() const;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
signals:
    void signal_mousePressEvent(ChessPiece* piece);
    void signal_mouseReleaseEvent(ChessPiece* piece, QPointF position);
};

#endif // CHESSPIECE_H
