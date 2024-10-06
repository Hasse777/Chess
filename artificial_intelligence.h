#ifndef ARTIFICIAL_INTELLIGENCE_H
#define ARTIFICIAL_INTELLIGENCE_H

#include <QVector>
#include <QObject>
#include "chesspieceforartifical.h"

\
class ChessBoard;
class MinMax;

//bool bot_color;

class Artificial_Intelligence : public QObject
{
    Q_OBJECT
public:
    explicit Artificial_Intelligence(const ChessBoard* const chessBoard, bool color);
    ~Artificial_Intelligence();
private:
    // Указатели на королей
    //----------------------------------------------------------
    ChessPieceForArtifical* m_whiteKing;
    ChessPieceForArtifical* m_blackKing;
    //----------------------------------------------------------
    const ChessBoard* const m_ChessBoard;
    QVector<QVector<ChessPieceForArtifical*>> m_pieceOnBoard{8, QVector<ChessPieceForArtifical*>(8, nullptr)};
    bool m_color;
    MinMax* m_MinMax;
    void freeMemory();
public slots:
    void slot_MoveBot();
signals:
    void signal_BotChoseMove(std::pair<int, int> coordinates);
};




class MinMax
{
private:
    int m_depth = 6; // глубина n-дерева
    bool m_whoMove; // кто ходит
    // Указатели на королей
    //----------------------------------------------------------
    ChessPieceForArtifical* m_whiteKing;
    ChessPieceForArtifical* m_blackKing;
    //----------------------------------------------------------
    // Указатель на пешку, которую можно взять на проходе
    ChessPieceForArtifical* m_pawn;
    //----------------------------------------------------------
    //----------------------------------------------------------
    // Текущее состояние доски
    QVector<QVector<ChessPieceForArtifical*>> m_pieceOnBoard{8, QVector<ChessPieceForArtifical*>(8, nullptr)};
    //----------------------------------------------------------
    // Лучший ход
    std::pair<ChessPieceForArtifical*, std::pair<int, int>> m_bestMove; // лучший ход, ChessPieceForArtifical обьект фигуры, std::pair<int, int> куда она ходит
    //---------------------------------------------------------
    // enum m_pieceType{none, king, queen, rook, elephant, horse, pawn}; // Перечисление очков фигур для оценивания хода
    void distributionByChessPiece(const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, /*- Массив откуда будет запись*/
    QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>>& allPossibleMoves /*- Куда будут записываться ходы*/); // Функции для записи всех возможных ходов для фигур
    bool pieceUnderAttack(const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard /*- Массив где будет проверка*/, std::pair<int, int> coordinates, bool color); // Функция для проверки находится ли клетка под атакой, color - цвет фигуры на клетке
    void countingPossibleMovesPawn(ChessPieceForArtifical* piece, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, /*- Массив откуда будет запись*/ QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>>& allPossibleMoves /*- Куда будут записываться ходы*/);
    void countingPossibleMovesHorse(ChessPieceForArtifical* piece, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, /*- Массив откуда будет запись*/ QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>>& allPossibleMoves /*- Куда будут записываться ходы*/);
    void countingPossibleMovesElephant(ChessPieceForArtifical* piece, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, /*- Массив откуда будет запись*/ QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>>& allPossibleMoves /*- Куда будут записываться ходы*/);
    void countingPossibleMovesRook(ChessPieceForArtifical* piece, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, /*- Массив откуда будет запись*/ QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>>& allPossibleMoves /*- Куда будут записываться ходы*/);
    void countingPossibleMovesQueen(ChessPieceForArtifical* piece, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, /*- Массив откуда будет запись*/ QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>>& allPossibleMoves /*- Куда будут записываться ходы*/);
    void countingPossibleMovesKing(ChessPieceForArtifical* piece, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, /*- Массив откуда будет запись*/ QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>>& allPossibleMoves /*- Куда будут записываться ходы*/);
    // -----------------------------------------------------------------
    int evaluateBoard(const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard);
    int findOptimalMove(const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, int depth, int alpha, int beta, bool maxOrminPlayer); // Выбор хода и вызов рекурсии
    QVector<QVector<ChessPieceForArtifical*>>* copyPieceOnBoard_in_tempBoard(const QVector<QVector<ChessPieceForArtifical *> > &pieceOnBoard);
    void clearTempPieceOnBoard(QVector<QVector<ChessPieceForArtifical*>>& board);
    int evaluatePiece(ChessPieceForArtifical* piece) const; // Функция, которая возвращает стоимость фигуры
    void makeMove(ChessPieceForArtifical* piece, std::pair<int, int> coordinates, QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard /*- Массив где будет делаться ход*/); // Функция, которая делает ход и фиксирует его во временном массиве m_pieceOnBoard_Temp
    // //-----------------------------------------------------------------

public:
    MinMax(bool whoMove, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard);
    ~MinMax();
};





#endif // ARTIFICIAL_INTELLIGENCE_H
