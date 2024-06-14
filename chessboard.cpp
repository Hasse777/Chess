#include "chessboard.h"

ChessBoard::ChessBoard(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(760, 760);
}

void ChessBoard::paintEvent(QPaintEvent *p)
{
    QPainter painter;
    int squareSize = width() / 8;
    painter.begin(this);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if((i + j) % 2 == 0)
            {
                painter.setBrush(Qt::white);
            }
            else painter.setBrush(Qt::black);
            painter.drawRect(j * squareSize, i * squareSize, squareSize, squareSize);
        }
    }
    painter.end();
}
