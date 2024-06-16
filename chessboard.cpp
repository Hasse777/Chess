#include "chessboard.h"


ChessBoard::ChessBoard(QWidget *parent)
    : QGraphicsView{parent}, m_pieceOnBoard{8, QVector<ChessPiece*>(8, nullptr)},
    m_indentation{30}
{
    setFixedSize(760, 760);
    setBackgroundBrush(QBrush(QColor(75, 75, 75)));
    this->m_square_Size = (this->width() - 60) / 8;
    this->m_scene = new QGraphicsScene(this);
    this->m_scene->setSceneRect(0, 0, this->width(), this->height());
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->Paint_Board();
    this->setScene(this->m_scene);
    this->NewGame();
}

ChessBoard::~ChessBoard()
{
    for(int i = 0; i < this->m_vector_square.size(); i++)
    {
        delete m_vector_square[i];
    }
\
    for(int i = 0; i < this->m_pieceOnBoard.size(); i++)
    {
        for(int j = 0; j < this->m_pieceOnBoard[i].size(); j++)
        {
            delete this->m_pieceOnBoard[i][j];
        }
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
            QGraphicsRectItem *square = m_scene->addRect(i * m_square_Size + m_indentation, j * m_square_Size + m_indentation, m_square_Size, m_square_Size, QPen(Qt::NoPen), QBrush(color));
            this->m_vector_square.push_back(square);
        }
    }


}

void ChessBoard::NewGame()
{
    // Позиции начальных фигур
    unsigned short piecePositions[8][8] =
    {
        { 3, 5, 4, 2, 1, 4, 5, 3 },
        { 6, 6, 6, 6, 6, 6, 6, 6 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 6, 6, 6, 6, 6, 6, 6, 6 },
        { 3, 5, 4, 2, 1, 4, 5, 3 }
    };

    // Добавление белых фигур
    for(int i = 7; i > 5; i--)
    {
        for(int j = 0; j < 8; j++)
        {
            m_pieceOnBoard[i][j] = new ChessPiece(0, piecePositions[i][j], m_square_Size);
            this->m_scene->addItem(m_pieceOnBoard[i][j]);
            m_pieceOnBoard[i][j]->setPos(j * m_square_Size + m_indentation, i  * m_square_Size + m_indentation);
        }
    }

    // Добавление черных фигур
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            m_pieceOnBoard[i][j] = new ChessPiece(1, piecePositions[i][j], m_square_Size);
            this->m_scene->addItem(m_pieceOnBoard[i][j]);
            m_pieceOnBoard[i][j]->setPos(j * m_square_Size + m_indentation, i  * m_square_Size + m_indentation);
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
    painter.setPen(QColor(250, 250, 250));


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


