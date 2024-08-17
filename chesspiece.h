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
    ChessPiece(const ChessPiece& other); // Конструктор копирования
    ChessPiece& operator=(const ChessPiece& other);
    bool getColor() const;
    bool getFirst_Move() const;
    void setFirst_MoveFalse();
    unsigned short getPiece() const;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
signals:
    void signal_mousePressEvent(ChessPiece* piece);
    void signal_mouseReleaseEvent(ChessPiece* piece, QPointF position);
public slots:
    void slot_PieceSelection(int pieceType);
};




#endif // CHESSPIECE_H
