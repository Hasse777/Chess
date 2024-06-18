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
    this->m_selectedPiece = nullptr;
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
    QColor backlight_color(0, 255, 0, 100);
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

            // здесь в будущем нужно добавить превращение пешки в другую фигуру
            if(newRow >= 8 || newRow < 0) return;
            //-----------------------------------------------------------------

            if(i < 8 && i >= 0)
            {
                if(m_pieceOnBoard[newRow][j] != nullptr)
                {
                    break;
                }
                else
                {
                    ClickableRect *highlight = new ClickableRect(j * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                    m_scene->addItem(highlight);
                    m_highlightedCells.push_back(highlight);
                    connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
                }
            }
            else
            {
                break;
            }
        }

        // Проверка ходов по диагонали
        newRow = i - direction;
        if (newRow >= 0 && newRow < 8)
        {
            if (j - 1 >= 0 && m_pieceOnBoard[newRow][j - 1] != nullptr && m_pieceOnBoard[newRow][j - 1]->getColor() != piece->getColor())
            {
                ClickableRect *highlight = new ClickableRect((j - 1) * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                m_scene->addItem(highlight);
                m_highlightedCells.push_back(highlight);
                connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
            }
            if (j + 1 < 8 && m_pieceOnBoard[newRow][j + 1] != nullptr && m_pieceOnBoard[newRow][j + 1]->getColor() != piece->getColor())
            {
                ClickableRect *highlight = new ClickableRect((j + 1) * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                m_scene->addItem(highlight);
                m_highlightedCells.push_back(highlight);
                connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
            }
        }
    }

    // Проверка для лошади
    else if(p == horse)
    {
        QVector<std::pair<int, int>> move_horse =
        {
           {i + 2, j - 1},
           {i + 2, j + 1},
           {i - 2, j - 1},
           {i - 2, j + 1},

           {i + 1, j - 2},
           {i + 1, j + 2},
           {i - 1, j - 2},
           {i - 1, j + 2},
        };
        QVector<std::pair<int, int>>::iterator iter = move_horse.begin();
        for (; iter != move_horse.end(); iter++)
        {
            int newRow = iter->first;
            int newCol = iter->second;
            if(newRow < 0 || newRow >=8 || newCol < 0 || newCol >= 8 || (m_pieceOnBoard[newRow][newCol] != nullptr && m_pieceOnBoard[newRow][newCol]->getColor() == piece->getColor())) continue;
            ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
            m_scene->addItem(highlight);
            m_highlightedCells.push_back(highlight);
            connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
        }
    }

    // Проверка для слона
    else if(p == elephant)
    {
        QVector<std::pair<int, int>> move_elephant =
        {
            {1,  1},
            {1, -1},
            {-1, 1},
            {-1, -1}
        };
        QVector<std::pair<int, int>>::Iterator it = move_elephant.begin();
        for (; it != move_elephant.end(); it++)
        {
            int newRow = i + it->first;
            int newCol = j + it->second;
            while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
            {
                if(m_pieceOnBoard[newRow][newCol] == nullptr)
                {
                    ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                    m_scene->addItem(highlight);
                    m_highlightedCells.push_back(highlight);
                    connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
                }
                else
                {
                    if(m_pieceOnBoard[newRow][newCol]->getColor() != piece->getColor())
                    {
                        ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                        m_scene->addItem(highlight);
                        m_highlightedCells.push_back(highlight);
                        connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
                        break;
                    }
                    else break;
                }
                newRow += it->first;
                newCol += it->second;
            }
        }
    }

    // Проверка для ферзя
    else if(p == queen)
    {
        QVector<std::pair<int, int>> move_queen =
            {
                {1,  1},
                {1, -1},
                {-1, 1},
                {-1, -1},
                {0, 1},
                {1, 0},
                {-1, 0},
                {0, -1}
            };
        QVector<std::pair<int, int>>::Iterator it = move_queen.begin();
        for (; it != move_queen.end(); it++)
        {
            int newRow = i + it->first;
            int newCol = j + it->second;
            while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
            {
                if(m_pieceOnBoard[newRow][newCol] == nullptr)
                {
                    ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                    m_scene->addItem(highlight);
                    m_highlightedCells.push_back(highlight);
                    connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
                }
                else
                {
                    if(m_pieceOnBoard[newRow][newCol]->getColor() != piece->getColor())
                    {
                        ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                        m_scene->addItem(highlight);
                        m_highlightedCells.push_back(highlight);
                        connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
                        break;
                    }
                    else break;
                }
                newRow += it->first;
                newCol += it->second;
            }
        }

    }

    // Проверка для ладьи
    else if(p == rook)
    {
        QVector<std::pair<int, int>> move_rook =
            {

                {0, 1},
                {0, -1},
                {1, 0},
                {-1, 0},
            };
        QVector<std::pair<int, int>>::Iterator it = move_rook.begin();
        for (; it != move_rook.end(); it++)
        {
            int newRow = i + it->first;
            int newCol = j + it->second;
            while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
            {
                if(m_pieceOnBoard[newRow][newCol] == nullptr)
                {
                    ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                    m_scene->addItem(highlight);
                    m_highlightedCells.push_back(highlight);
                    connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
                }
                else
                {
                    if(m_pieceOnBoard[newRow][newCol]->getColor() != piece->getColor())
                    {
                        ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                        m_scene->addItem(highlight);
                        m_highlightedCells.push_back(highlight);
                        connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
                        break;
                    }
                    else break;
                }
                newRow += it->first;
                newCol += it->second;
            }
        }
    }

    // Проверка для короля
    else if(p == king)
    {
        QVector<std::pair<int, int>> move_queen =
            {

                {i, j + 1},
                {i, j - 1},
                {i + 1, j},
                {i - 1, j},
                {i + 1, j + 1},
                {i + 1, j - 1},
                {i - 1, j + 1},
                {i - 1, j - 1}
            };
        QVector<std::pair<int, int>>::Iterator it = move_queen.begin();
        for(; it != move_queen.end(); it++)
        {
            int newRow = it->first;
            int newCol = it->second;
            if(newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
            {
                if(m_pieceOnBoard[newRow][newCol] == nullptr)
                {
                    ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                    m_scene->addItem(highlight);
                    m_highlightedCells.push_back(highlight);
                    connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
                }
                else
                {
                    if(m_pieceOnBoard[newRow][newCol]->getColor() != piece->getColor())
                    {
                        ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                        m_scene->addItem(highlight);
                        m_highlightedCells.push_back(highlight);
                        connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
                    }
                }
            }
        }
    }
    else return;
}

void ChessBoard::slot_PiecePressed(ChessPiece* piece)
{
    clear_highlight();
    highlight_Moves(piece);
    m_selectedPiece = piece;
}

void ChessBoard::slot_HighlightedCell_Clicked(QGraphicsRectItem *cell)
{
    if(m_selectedPiece)
    {

        // Получаем текущую позицию выбранной фигуры
        int oldRow = m_selectedPiece->pos().y() / m_square_Size;
        int oldCol = m_selectedPiece->pos().x() / m_square_Size;

        // Получаем новую позицию из выделенной клетки
        int newRow = cell->rect().y() / m_square_Size;
        int newCol = cell->rect().x() / m_square_Size;

        // Проверка на выход за пределы массива
        if(newRow >= 8 || newRow < 0 || newCol >= 8 || newCol < 0)
        {
            throw std::out_of_range("New position is out of bounds");
        }
        if(oldRow >= 8 || oldRow < 0 || oldCol >= 8 || oldCol < 0)
        {
            throw std::out_of_range("Old position is out of bounds");
        }
        //_____________________________________________________________

        if(m_pieceOnBoard[newRow][newCol] != nullptr)
        {
            if(m_pieceOnBoard[newRow][newCol]->getColor() != m_selectedPiece->getColor())
            {
                m_scene->removeItem(m_pieceOnBoard[newRow][newCol]);
                delete m_pieceOnBoard[newRow][newCol];
            }
            else
            {
                // своих нельзя бить
            }
        }
        //Перемещаем фигуру на новую позицию
        m_selectedPiece->setPos(cell->rect().topLeft());
        m_selectedPiece->setFirst_MoveFalse();

        //Обновляем позицию фигуры на доске
        m_pieceOnBoard[oldRow][oldCol] = nullptr;
        m_pieceOnBoard[newRow][newCol] = m_selectedPiece;

        // Очищаем выделение клеток
        clear_highlight();
        m_selectedPiece = nullptr;
    }
    else return;
}




