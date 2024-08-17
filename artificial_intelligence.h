#ifndef ARTIFICIAL_INTELLIGENCE_H
#define ARTIFICIAL_INTELLIGENCE_H

#include <QVector>
#include <QObject>

class ChessPiece;
class ChessBoard;
class MinMax;

class Artificial_Intelligence : public QObject
{
    Q_OBJECT
public:
    explicit Artificial_Intelligence(const ChessBoard* const, bool color);
    ~Artificial_Intelligence();
private:
    const ChessBoard* const m_ChessBoard;
    QVector<std::pair<ChessPiece*, QVector<std::pair<int, int>>>> m_possibleMove; // Вектор всех фигур и их возможных ходов
    bool m_color;
    MinMax* m_MinMax;
public slots:
    void slot_MoveBot();
signals:
    void signal_BotChoseMove(ChessPiece *piece, std::pair<int, int> coordinates);
};




class MinMax
{
private:
    int m_depth; // глубина n-дерева
    const int m_squareSize;
    bool m_whoMove; // кто ходит
    QVector<QVector<ChessPiece*>> m_pieceOnBoard{8, QVector<ChessPiece*>(8, nullptr)}; // Текущее состояние доски
    enum m_pieceType{none, king, queen, rook, elephant, horse, pawn}; // Перечисление очков фигур для оценивания хода
    void setPossiblelMoveInPair();
    std::pair<ChessPiece*, std::pair<int, int>> choiceMove(); // Выбор хода
    std::pair<ChessPiece*, std::pair<int, int>> choiceMoveTest();
    void distributionByChessPiece(ChessPiece*);
    // Функции для записи всех возможных ходов для фигур
    void countingPossibleMovesPawn(ChessPiece*);
    void countingPossibleMovesHorse(ChessPiece*);
    void countingPossibleMovesElephant(ChessPiece*);
    void countingPossibleMovesRook(ChessPiece*);
    void countingPossibleMovesQueen(ChessPiece*);
    void countingPossibleMovesKing(ChessPiece*);
    //-----------------------------------------------------------------
    // Функции для оценки хода
    int evaluateMovePawn(ChessPiece*, std::pair<int, int> best_move);
    int evaluateMoveHorse(ChessPiece*, std::pair<int, int> best_move);
    int evaluateMoveElephant(ChessPiece*, std::pair<int, int> best_move);
    int evaluateMoveRook(ChessPiece*, std::pair<int, int> best_move);
    int evaluateMoveQueen(ChessPiece*, std::pair<int, int> best_move);
    int evaluateMoveKing(ChessPiece*, std::pair<int, int> best_move);
    //-----------------------------------------------------------------
    int evaluatePiece(ChessPiece*); // Функция оценки фигуры



    void test_m_possibleMove();

public:
    MinMax(int squareSize, bool whoMove, int depth, QVector<QVector<ChessPiece*>> pieceOnBoard);
    ~MinMax();
};

#endif // ARTIFICIAL_INTELLIGENCE_H
