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

class ChessBoard : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = nullptr);
    ~ChessBoard();
private:
    QGraphicsScene *m_scene;
    QVector<QGraphicsRectItem*> m_vector_square;
    QVector<QGraphicsRectItem*> m_list_tempSquare;
    QVector<ClickableRect*> m_highlightedCells;
    QVector<QVector<ChessPiece*>> m_pieceOnBoard{8, QVector<ChessPiece*>(8, nullptr)};
    QVector<ChessPiece*> m_piece_Attacking_king;
    ChessPiece* m_selectedPiece;
    std::pair<int, int> m_whiteKingPos;
    std::pair<int, int> m_blackKingPos;
    enum m_piece {none, king, queen, rook, elephant, horse, pawn};
    int m_square_Size;
    int m_indentation;
    bool m_checkShah_White;
    bool m_checkShah_Black;
    bool m_whoseMove;
    void paint_Board();
    void newGame();
    void create_tempSquare(int x, int y);
    void highlight_Moves(ChessPiece* piece);
    void clear_highlight();
    void highlighting_possible_moves(ChessPiece* piece);
    bool Check_King_Shah(bool color);
    bool Check_King_Mate(bool color);
    bool square_under_attack(std::pair<int, int> coordinates, bool color, bool mod = 0);
protected:
     void paintEvent(QPaintEvent *event) override;

private slots:
     void slot_PiecePressed(ChessPiece* piece);
     void slot_HighlightedCell_Clicked(QGraphicsRectItem* cell);
     //void slot_MovePiece(ChessPiece* piece, QPointF position);
 signals:
     void signal_Change_picture(bool color);
};

#endif // CHESSBOARD_H
