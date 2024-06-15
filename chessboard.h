#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QVector>

class ChessBoard : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = nullptr);
    ~ChessBoard();
private:
    QGraphicsScene *scene;
    QVector<QGraphicsRectItem*> m_vector_square;
    QVector<QGraphicsTextItem*> m_vector_board_numbering;
    int m_square_Size;
    int m_indentation;
    void Paint_Board();
protected:
     void paintEvent(QPaintEvent *event) override;

signals:
};

#endif // CHESSBOARD_H
