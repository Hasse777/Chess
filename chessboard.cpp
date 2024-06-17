#include "chessboard.h"
#include <QDebug>


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
    this->paint_Board();
    this->setScene(this->m_scene);
    this->newGame();
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

void ChessBoard::paint_Board()
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

void ChessBoard::newGame()
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
            connect(m_pieceOnBoard[i][j], &ChessPiece::signal_mousePressEvent, this, &ChessBoard::slot_PiecePressed);
            connect(m_pieceOnBoard[i][j], &ChessPiece::signal_mouseReleaseEvent, this, &ChessBoard::slot_MovePiece);
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
            connect(m_pieceOnBoard[i][j], &ChessPiece::signal_mousePressEvent, this, &ChessBoard::slot_PiecePressed);
            connect(m_pieceOnBoard[i][j], &ChessPiece::signal_mouseReleaseEvent, this, &ChessBoard::slot_MovePiece);
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

void ChessBoard::highlight_Moves(ChessPiece *piece)
{
    get_Valid_Moves(piece);
}

void ChessBoard::clear_highlight()
{
    for (int i = 0; i < m_highlightedCells.size(); i++)
    {
        this->m_scene->removeItem(m_highlightedCells[i]);
        delete m_highlightedCells[i];
    }
    m_highlightedCells.clear();
}

void ChessBoard::get_Valid_Moves(ChessPiece *piece)
{
    unsigned short i = piece->pos().y() / m_square_Size;
    unsigned short j = piece->pos().x() / m_square_Size;
    enum m_piece p = static_cast<m_piece>(piece->getPiece());

    // проверка для пешки
    if(p == pawn)
    {
        int direction = piece->getColor() ? -1 : 1;
        int counter_move = (piece->getFirst_Move() == true) ? 2 : 1;
        unsigned short newRow = i;
        // Проверка ходов вперед
        for (int counter = 1; counter <= counter_move; counter++)
        {
            newRow -= direction;
            if(i < m_pieceOnBoard.size() && i >= 0)
            {
                if(m_pieceOnBoard[newRow][j] != nullptr && m_pieceOnBoard[newRow][j]->getColor() != piece->getColor())
                {
                    QGraphicsRectItem *highlight = m_scene->addRect(j * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, m_square_Size, QPen(Qt::NoPen), QBrush(QColor(0, 255, 0, 100)));
                    m_highlightedCells.push_back(highlight);
                    break;
                }
                else if(m_pieceOnBoard[newRow][j] != nullptr)
                {
                    break;
                }
                else
                {
                    QGraphicsRectItem *highlight = m_scene->addRect(j * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, m_square_Size, QPen(Qt::NoPen), QBrush(QColor(0, 255, 0, 100)));
                    m_highlightedCells.push_back(highlight);
                }
            }
            else
            {
                break;
            }
        }

        // Проверка ходов по диагонали
        newRow = i - direction;
        if (newRow >= 0 && newRow < m_pieceOnBoard.size())
        {
            if (j - 1 >= 0 && m_pieceOnBoard[newRow][j - 1] != nullptr && m_pieceOnBoard[newRow][j - 1]->getColor() != piece->getColor())
            {
                QGraphicsRectItem *highlight = m_scene->addRect((j - 1) * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, m_square_Size, QPen(Qt::NoPen), QBrush(QColor(0, 255, 0, 100)));
                m_highlightedCells.push_back(highlight);
            }
            if (j + 1 < m_pieceOnBoard.size() && m_pieceOnBoard[newRow][j + 1] != nullptr && m_pieceOnBoard[newRow][j + 1]->getColor() != piece->getColor())
            {
                QGraphicsRectItem *highlight = m_scene->addRect((j + 1) * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, m_square_Size, QPen(Qt::NoPen), QBrush(QColor(0, 255, 0, 100)));
                m_highlightedCells.push_back(highlight);
            }
        }
    }
    else return;
}

void ChessBoard::slot_PiecePressed(ChessPiece* piece)
{
    clear_highlight();
    highlight_Moves(piece);
}

void ChessBoard::slot_MovePiece(ChessPiece* piece, QPointF position)
{

}




