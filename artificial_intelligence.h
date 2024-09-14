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
    int m_depth; // глубина n-дерева
    int m_score; // игровые очки
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
    // Все возможные ходы
    QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>> allPossibleMoves;
    // ---------------------------------------------------------
    // enum m_pieceType{none, king, queen, rook, elephant, horse, pawn}; // Перечисление очков фигур для оценивания хода
    // // std::pair<ChessPiece*, std::pair<int, int>> choiceMove(); // Выбор хода
    // // std::pair<ChessPiece*, std::pair<int, int>> choiceMoveTest();
    void distributionByChessPiece(); // Функции для записи всех возможных ходов для фигур
    void countingPossibleMovesPawn(ChessPieceForArtifical* piece);
    void countingPossibleMovesHorse(ChessPieceForArtifical* piece);
    void countingPossibleMovesElephant(ChessPieceForArtifical* piece);
    void countingPossibleMovesRook(ChessPieceForArtifical* piece);
    void countingPossibleMovesQueen(ChessPieceForArtifical* piece);
    void countingPossibleMovesKing(ChessPieceForArtifical* piece);
    // //-----------------------------------------------------------------
    // // Функции для оценки хода
    // int evaluateMovePawn(ChessPiece*, std::pair<int, int> best_move);
    // int evaluateMoveHorse(ChessPiece*, std::pair<int, int> best_move);
    // int evaluateMoveElephant(ChessPiece*, std::pair<int, int> best_move);
    // int evaluateMoveRook(ChessPiece*, std::pair<int, int> best_move);
    // int evaluateMoveQueen(ChessPiece*, std::pair<int, int> best_move);
    // int evaluateMoveKing(ChessPiece*, std::pair<int, int> best_move);
    // //-----------------------------------------------------------------
    // int evaluatePiece(ChessPiece*); // Функция оценки фигуры



    // void test_m_possibleMove();

public:
    MinMax(int m_score, bool whoMove, int depth, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, ChessPieceForArtifical* whiteKing, ChessPieceForArtifical* blackKing, ChessPieceForArtifical* pawn);
    ~MinMax();
};





#endif // ARTIFICIAL_INTELLIGENCE_H
