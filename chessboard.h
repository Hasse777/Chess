#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QVector>
#include "chesspiece.h"
#include "clickablerect.h"
//#include "artificial_intelligence.h"

class Artificial_Intelligence;

class ChessBoard : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = nullptr);
    int get_square_Size() const;

    ~ChessBoard();
private:
    Artificial_Intelligence *m_bot;
    friend Artificial_Intelligence;
    QGraphicsScene *m_scene;
    QVector<QGraphicsRectItem*> m_vector_square;
    QVector<QGraphicsRectItem*> m_list_tempSquare;
    QVector<ClickableRect*> m_highlightedCells; // Все подсвеченные клетки на которые возможно походить
    QVector<QVector<ChessPiece*>> m_pieceOnBoard{8, QVector<ChessPiece*>(8, nullptr)}; // Все фигуры на доске
    QVector<ChessPiece*> m_piece_Attacking_king; // Массив указателей для того чтобы знать какие фигуры атакуют короля
    QVector<std::pair<int, int>> m_whiteSquareCover; // Клетки которые ведут к белому королю, если их можно перекрыть, то мат королю не будет поставлен
    QVector<std::pair<int, int>> m_blackSquareCover; // Клетки которые ведут к белому королю, если их можно перекрыть, то мат королю не будет поставлен
    ChessPiece* m_selectedPiece; // Указатель на шахматную фигуру, которая ходит на выделеную клетку
    ChessPiece* m_pawnPiece;     // Указатель на шахматную фигуру, которую можно взять на проходе
    // std::pair<int, int> m_whiteKingPos; // Храним позиции обоих королей
    // std::pair<int, int> m_blackKingPos;
    //-------------------------------------------------------------------
    KingPiece* m_whiteKing; // Храним указатели обоих королей
    KingPiece* m_blackKing;
    //-------------------------------------------------------------------
    std::pair<int, int> m_pawnPos; // Храним координаты клетки, через которую прошла пешка, которую можно взять на проходе
    enum m_piece {none, king, queen, rook, elephant, horse, pawn}; // Перечисление всех фигур
    int m_square_Size; // Размер шахматной клетки
    int m_indentation; // Отсуп чтобы освободить пространство для подписи клеток a-h,1-8
    int m_counterMoves; // переменная, которая показывает какой по счету ход делается в партии
    // bool m_checkShah_White; // шах белым
    // bool m_checkShah_Black; // шах черным
    bool m_whoseMove; // Переменная для того чтобы узнать чей сейчас ход
    bool m_playerColor; // За кого будет играть игрок
    void paint_Board(); // Рисование шахматной доски
    void newGame(); // Новая игра
    void endGame(bool colorWin); // Обработка выйгрыша-пройгрыша. colorWin: 0 - Белые выйграли, 1 - Черные выйграли
    void ClearMemoryforNewGame(); // Очистка памяти для новой игры
    void create_tempSquare(int x, int y);
    void highlight_Moves(ChessPiece* piece);
    void clear_highlight();
    void highlighting_possible_moves(ChessPiece* piece);
    void addMovesForWidget(ChessPiece *piece, int oldRow, int oldCol, int newRow, int newCol);
    bool castling_check(bool short_or_long, ChessPiece *piece); // Проверка возможности рокировки, короткая 0, длинная 1 и указатель на фигурку короля
    bool Check_King_Shah(bool color);
    bool Check_King_Mate(bool color);
    //bool kingOverlap(bool color);
    bool square_under_attack(std::pair<int, int> coordinates, bool color, bool mod = 0); // Функция проверяет находится ли клетка под атакой.
    //Mod нужен для того чтобы знать проверяем ли мы урозу короля, если проверяем, то нужно запоминать угрожающие королю фигуры
    // color цвет атакуемой фигуры
    void arrangement_piece(bool playerColor); // функция расстановки фигур на шахматной доске

    const QVector<QVector<ChessPiece*>>& getPieceOnBoard() const;
protected:
     void paintEvent(QPaintEvent *event) override;

private slots:
     void slot_PiecePressed(ChessPiece* piece);
     void slot_HighlightedCell_Clicked(QGraphicsRectItem* cell);
     void slot_MoveBot(std::pair<int, int> coordinatesPiece, std::pair<int, int> moveWhere);
     void slot_PlayerColor(bool color);
 public slots:
     void slot_newGame();
 signals:
     void signal_Change_picture(bool color);
     void signal_addMove(const QString text);
     void signal_addEatenImages(const QPixmap);
     void signal_newGame();
     void signal_MoveBots();
};

#endif // CHESSBOARD_H
