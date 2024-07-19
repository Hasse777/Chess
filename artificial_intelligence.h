#ifndef ARTIFICIAL_INTELLIGENCE_H
#define ARTIFICIAL_INTELLIGENCE_H

#include <QVector>
class ChessPiece;
class ChessBoard;

class Artificial_Intelligence
{
public:
    Artificial_Intelligence(const ChessBoard* const, bool color);
private:
    const ChessBoard* const m_ChessBoard;
    QVector<QVector<ChessPiece*>> *m_pieceOnBoard;
    void getPossiblelMove();
    bool m_color;
};

#endif // ARTIFICIAL_INTELLIGENCE_H
