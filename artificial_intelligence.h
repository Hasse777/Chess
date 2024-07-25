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
    enum m_pieceType {none, king, queen, rook, elephant, horse, pawn};
    std::pair<ChessPiece*, QVector<int>> m_possibleMove;
    QVector<QVector<ChessPiece*>> *m_pieceOnBoard;
    void setPossiblelMoveInPair();
    void distributionByChessPiece(ChessPiece*);
    void countingPossibleMovesPawn(ChessPiece*);
    void countingPossibleMovesHorse(ChessPiece*);
    void countingPossibleMovesElephant(ChessPiece*);
    void countingPossibleMovesRook(ChessPiece*);
    void countingPossibleMovesQueen(ChessPiece*);
    void countingPossibleMovesKing(ChessPiece*);
    bool m_color;
};

#endif // ARTIFICIAL_INTELLIGENCE_H
