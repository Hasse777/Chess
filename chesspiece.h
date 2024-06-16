#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QGraphicsPixmapItem >

class ChessPiece : public QGraphicsPixmapItem
{
private:
    bool m_color;
    unsigned short m_piece;
public:
    explicit ChessPiece(bool color, unsigned short piece, const int squareSize);
    bool getColor() const;
    unsigned short getPiece() const;
};

#endif // CHESSPIECE_H
