#include "chessboard.h"


ChessBoard::ChessBoard(QWidget *parent)
    : QGraphicsView{parent}
{
    setFixedSize(760, 760);
    this->m_square_Size = (this->width() - 60) / 8;
    this->m_indentation = 30;
    this->scene = new QGraphicsScene(this);
    this->scene->setSceneRect(0, 0, this->width(), this->height());
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->Paint_Board();
    this->setScene(this->scene);
}

ChessBoard::~ChessBoard()
{
    for(int i = 0; i < this->m_vector_square.size(); i++)
    {
        delete m_vector_square[i];
    }

    for(int i = 0; i < this->m_vector_board_numbering.size(); i++)
    {
        delete m_vector_board_numbering[i];
    }
}

void ChessBoard::Paint_Board()
{
    // Рисование шахматной доски

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            QColor color = (i + j) % 2 == 0 ? QColor(222, 184, 135) : QColor(139, 69, 19);
            QGraphicsRectItem *square = scene->addRect(i * m_square_Size + m_indentation, j * m_square_Size + m_indentation, m_square_Size, m_square_Size, QPen(Qt::NoPen), QBrush(color));
            this->m_vector_square.push_back(square);
        }
    }


}

void ChessBoard::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);

    QPainter painter(viewport());

    QFont font;
    font.setPointSize(14);
    painter.setFont(font);


    // Добавление букв (a-h) внизу и вверху доски
    for(int i = 0; i < 8; i++)
    {
        QString book = QChar('a' + i);
        painter.drawText(i * m_square_Size + m_indentation * 2 + 8, this->height() - m_indentation + 20, book);
        painter.drawText(i * m_square_Size + m_indentation * 2 + 8, m_indentation - 10, book);
    }

    // Добавление цифр (1-8) слева и справа от доски
    for(int i = 0; i < 8; i++)
    {
        QString number = QString::number(8 - i);
        painter.drawText(m_indentation - 20, i * m_square_Size + m_indentation * 2 + 18, number);
        painter.drawText(this->width() - m_indentation + 5, i * m_square_Size + m_indentation * 2 + 18, number);
    }
}


