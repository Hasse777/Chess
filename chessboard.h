#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QVector>
#include "chesspiece.h"

class ChessBoard : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = nullptr);
    ~ChessBoard();
private:
    QGraphicsScene *m_scene;
    QVector<QGraphicsRectItem*> m_vector_square;
    enum m_piece {none, king, queen, rook, elephant, horse, pawn};
    QVector<QVector<ChessPiece*>> m_pieceOnBoard{8, QVector<ChessPiece*>(8, nullptr)};
    int m_square_Size;
    int m_indentation;
    void Paint_Board();
    void NewGame();
protected:
     void paintEvent(QPaintEvent *event) override;

signals:
};

#endif // CHESSBOARD_H
