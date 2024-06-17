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
    QVector<ClickableRect*> m_highlightedCells;
    QVector<QVector<ChessPiece*>> m_pieceOnBoard{8, QVector<ChessPiece*>(8, nullptr)};
    ChessPiece* m_selectedPiece;
    enum m_piece {none, king, queen, rook, elephant, horse, pawn};
    int m_square_Size;
    int m_indentation;
    void paint_Board();
    void newGame();
    void highlight_Moves(ChessPiece* piece);
    void clear_highlight();
    void get_Valid_Moves(ChessPiece* piece);
protected:
     void paintEvent(QPaintEvent *event) override;

private slots:
     void slot_PiecePressed(ChessPiece* piece);
     void slot_HighlightedCell_Clicked(QGraphicsRectItem* cell);
     //void slot_MovePiece(ChessPiece* piece, QPointF position);
};

#endif // CHESSBOARD_H
