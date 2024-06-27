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
    m_whoseMove = false; // Сначала ход белых

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

    //Запоминаем начальную позицию белого и черного короля. Нужно для проверки логики "Шах и мат"
    m_whiteKingPos.first = 7;
    m_whiteKingPos.second = 4;
    m_checkShah_White = false;

    m_blackKingPos.first = 0;
    m_blackKingPos.second = 4;
    m_checkShah_Black = false;

    // Добавление белых фигур
    for(int i = 7; i > 5; i--)
    {
        for(int j = 0; j < 8; j++)
        {
            m_pieceOnBoard[i][j] = new ChessPiece(0, piecePositions[i][j], m_square_Size);
            this->m_scene->addItem(m_pieceOnBoard[i][j]);
            m_pieceOnBoard[i][j]->setZValue(1);
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
            m_pieceOnBoard[i][j]->setZValue(1);
            m_pieceOnBoard[i][j]->setPos(j * m_square_Size + m_indentation, i  * m_square_Size + m_indentation);
            connect(m_pieceOnBoard[i][j], &ChessPiece::signal_mousePressEvent, this, &ChessBoard::slot_PiecePressed);
        }
    }
}

void ChessBoard::create_tempSquare(int x, int y)
{
    QGraphicsRectItem *tempSquare = new QGraphicsRectItem(x * m_square_Size + m_indentation, y * m_square_Size + m_indentation, m_square_Size, m_square_Size);
    tempSquare->setBrush(QBrush(QColor(222, 184, 135)));
    m_scene->addItem(tempSquare);
    m_list_tempSquare.append(tempSquare);
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
    highlighting_possible_moves(piece);


}

void ChessBoard::clear_highlight()
{
    for (int i = 0; i < m_highlightedCells.size(); i++)
    {
        this->m_scene->removeItem(m_highlightedCells[i]);
        delete m_highlightedCells[i];
    }
    m_highlightedCells.clear();

    QVector<QGraphicsRectItem*>::Iterator it = m_list_tempSquare.begin();
    for(; it != m_list_tempSquare.end(); it++)
    {
        this->m_scene->removeItem(*it);
        delete *it;
    }
    m_list_tempSquare.clear();
}

void ChessBoard::highlighting_possible_moves(ChessPiece *piece)
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
                    create_tempSquare(j, newRow);
                    ClickableRect *highlight = new ClickableRect(j * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                    highlight->setZValue(1);
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
                create_tempSquare(j - 1, newRow);
                ClickableRect *highlight = new ClickableRect((j - 1) * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                highlight->setZValue(1);
                m_scene->addItem(highlight);
                m_highlightedCells.push_back(highlight);
                connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
            }
            if (j + 1 < 8 && m_pieceOnBoard[newRow][j + 1] != nullptr && m_pieceOnBoard[newRow][j + 1]->getColor() != piece->getColor())
            {
                create_tempSquare(j + 1, newRow);
                ClickableRect *highlight = new ClickableRect((j + 1) * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                highlight->setZValue(1);
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
            create_tempSquare(newCol, newRow);
            ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
            highlight->setZValue(1);
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
                    create_tempSquare(newCol, newRow);
                    ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                    highlight->setZValue(1);
                    m_scene->addItem(highlight);
                    m_highlightedCells.push_back(highlight);
                    connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
                }
                else
                {
                    if(m_pieceOnBoard[newRow][newCol]->getColor() != piece->getColor())
                    {
                        create_tempSquare(newCol, newRow);
                        ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                        highlight->setZValue(1);
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
                    create_tempSquare(newCol, newRow);
                    ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                    highlight->setZValue(1);
                    m_scene->addItem(highlight);
                    m_highlightedCells.push_back(highlight);
                    connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
                }
                else
                {
                    if(m_pieceOnBoard[newRow][newCol]->getColor() != piece->getColor())
                    {
                        create_tempSquare(newCol, newRow);
                        ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                        highlight->setZValue(1);
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
                    create_tempSquare(newCol, newRow);
                    ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                    highlight->setZValue(1);
                    m_scene->addItem(highlight);
                    m_highlightedCells.push_back(highlight);
                    connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
                }
                else
                {
                    if(m_pieceOnBoard[newRow][newCol]->getColor() != piece->getColor())
                    {
                        create_tempSquare(newCol, newRow);
                        ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                        highlight->setZValue(1);
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
                    create_tempSquare(newCol, newRow);
                    ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                    highlight->setZValue(1);
                    m_scene->addItem(highlight);
                    m_highlightedCells.push_back(highlight);
                    connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
                }
                else
                {
                    if(m_pieceOnBoard[newRow][newCol]->getColor() != piece->getColor())
                    {
                        create_tempSquare(newCol, newRow);
                        ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                        highlight->setZValue(1);
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

bool ChessBoard::Check_King_Shah(bool color)
{
    return color == 0 ? square_under_attack(m_whiteKingPos, 0, true) : square_under_attack(m_blackKingPos, 1, true);
}

bool ChessBoard::Check_King_Mate(bool color)
{
    if(m_piece_Attacking_king.size() == 1 && square_under_attack({m_piece_Attacking_king[0]->pos().x() / m_square_Size, m_piece_Attacking_king[0]->pos().y() / m_square_Size}, m_piece_Attacking_king[0]->getColor()))
    {
        return false; // Если фигуру, которая атакует короля, можно уничтожить, то есть выход из под мата
    }

    int kingRow = color == 0 ? m_whiteKingPos.first : m_blackKingPos.first;
    int kingCol = color == 0 ? m_whiteKingPos.second : m_blackKingPos.second;
    QVector<std::pair<int, int>> kingMove =
    {
        {kingRow + 1, kingCol}, {kingRow + 1, kingCol + 1},
        {kingRow + 1, kingCol - 1}, {kingRow, kingCol + 1},
        {kingRow, kingCol - 1}, {kingRow - 1, kingCol},
        {kingRow - 1, kingCol + 1}, {kingRow - 1, kingCol - 1}
    };
    QVector<std::pair<int, int>>::Iterator it = kingMove.begin();

    while (it != kingMove.end())
    {
        int newRow = it->first;
        int newCol = it->second;
        if(newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
        {
            if((m_pieceOnBoard[newRow][newCol] == nullptr || m_pieceOnBoard[newRow][newCol]->getColor() != color) && !square_under_attack({newRow, newCol}, color))
            {
                return false; // Есть клетки куда королю можно уйти от атаки
            }
        }
        it++;
    }

    return true; // Если нет безопасных клеток для короля, то это мат
}

bool ChessBoard::square_under_attack(std::pair<int, int> coordinates, bool color, bool mod)
{
    int kingRow = coordinates.first;
    int kingCol = coordinates.second;
    if(kingRow  < 0 || kingRow  >= 8 || kingCol < 0 || kingCol >= 8) throw "Incorrect coordinates passed to king check function";

    //Проверка угрозы пешкой
    int direction = color ? -1 : 1;
    if(kingRow - direction < 8 && kingRow - direction >= 0)
    {
        QVector<int> pawnMoves = {kingCol + 1, kingCol - 1};
        QVector<int>::Iterator it = pawnMoves.begin();
        while (it != pawnMoves.end())
        {
            int newRow = kingRow - direction;
            int newCol = *it;
            if(newCol < 8 && newCol >= 0)
            {
                if(m_pieceOnBoard[newRow][newCol] != nullptr && m_pieceOnBoard[newRow][newCol]->getColor() != color && m_pieceOnBoard[newRow][newCol]->getPiece() == 6)
                {
                    if(mod == true) m_piece_Attacking_king.push_back(m_pieceOnBoard[newRow][newCol]);
                    return true;
                }
            }
            it++;
        }
    }

    //Проверка угрозы конем
    QVector<std::pair<int, int>> hourseMoves =
        {
            {kingRow + 2, kingCol + 1}, {kingRow + 2, kingCol - 1},
            {kingRow - 2, kingCol + 1}, {kingRow - 2, kingCol - 1},
            {kingRow + 1, kingCol + 2}, {kingRow + 1, kingCol - 2},
            {kingRow - 1, kingCol + 2}, {kingRow - 1, kingCol - 2}
        };
    QVector<std::pair<int, int>>::Iterator it_hourseMoves = hourseMoves.begin();
    while (it_hourseMoves != hourseMoves.end())
    {
        int newRow = it_hourseMoves->first;
        int newCol = it_hourseMoves->second;
        if(newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
        {
            if(m_pieceOnBoard[newRow][newCol] != nullptr && m_pieceOnBoard[newRow][newCol]->getColor() != color && m_pieceOnBoard[newRow][newCol]->getPiece() == 5)
            {
                if(mod == true) m_piece_Attacking_king.push_back(m_pieceOnBoard[newRow][newCol]);
                return true;
            }
        }
        it_hourseMoves++;
    }


    // Проверка угроз по вертикалям, горизонталям и диагоналям
    QVector<std::pair<int, int>> directions_VH =
        {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1},
            {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
        };
    QVector<std::pair<int, int>>::Iterator it_directions = directions_VH.begin();
    int count = 0;
    while (it_directions != directions_VH.end())
    {
        int newRow = kingRow;
        int newCol = kingCol;

        while (true)
        {
            newRow += it_directions->first;
            newCol += it_directions->second;
            if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) break;
            if(m_pieceOnBoard[newRow][newCol] != nullptr)
            {
                ChessPiece* piece = m_pieceOnBoard[newRow][newCol];
                if(piece->getColor() != color && count <= 3 && (piece->getPiece() == 3 || piece->getPiece() == 2))
                {
                    if(mod == true) m_piece_Attacking_king.push_back(m_pieceOnBoard[newRow][newCol]);
                    return true;
                }
                if(piece->getColor() != color && count > 3 && (piece->getPiece() == 2 || piece->getPiece() == 4))
                {
                    if(mod == true) m_piece_Attacking_king.push_back(m_pieceOnBoard[newRow][newCol]);
                    return true;
                }
                break;
            }
        }
        count++;
        it_directions++;
    }

    // Проверка угрозы королём
    QVector<std::pair<int, int>> kingMoves =
        {
            {kingRow + 1, kingCol + 1}, {kingRow + 1, kingCol - 1},
            {kingRow - 1, kingCol + 1}, {kingRow - 1, kingCol - 1},
            {kingRow, kingCol + 1},     {kingRow, kingCol - 1},
            {kingRow + 1, kingCol},     {kingRow - 1, kingCol}
        };
    QVector<std::pair<int, int>>::Iterator it_kingMoves = kingMoves.begin();
    while (it_kingMoves != kingMoves.end())
    {
        int newRow = it_kingMoves->first;
        int newCol = it_kingMoves->second;
        if(newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
        {
            if(m_pieceOnBoard[newRow][newCol] != nullptr && m_pieceOnBoard[newRow][newCol]->getColor() != color && m_pieceOnBoard[newRow][newCol]->getPiece() == 1)
            {
                return true;
            }
        }
        it_kingMoves++;
    }
    return false;
}

void ChessBoard::slot_PiecePressed(ChessPiece* piece)
{
    clear_highlight();
    if(piece->getColor() == m_whoseMove)
    {
        highlight_Moves(piece);
        m_selectedPiece = piece;
    }
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
                return;
            }
        }
        //Перемещаем фигуру на новую позицию
        m_selectedPiece->setPos(cell->rect().topLeft());
        m_selectedPiece->setFirst_MoveFalse();


        //Обновляем позицию фигуры на доске
        m_pieceOnBoard[oldRow][oldCol] = nullptr;
        m_pieceOnBoard[newRow][newCol] = m_selectedPiece;

        //Если походили королем, то обновляем координаты
        if(m_selectedPiece->getPiece() == 1)
        {
            if(m_selectedPiece->getColor() == 0)
            {
                m_whiteKingPos.first = newRow;
                m_whiteKingPos.second = newCol;
            }
            else
            {
                m_blackKingPos.first = newRow;
                m_blackKingPos.second = newCol;
            }
        }

        // Очищаем выделение клеток
        clear_highlight();
        m_selectedPiece = nullptr;

        bool whiteKingUnderattack = Check_King_Shah(false);
        bool blackKingUnderattack = Check_King_Shah(true);
        if((m_whoseMove == 1 && blackKingUnderattack == true) || (m_checkShah_Black == true && blackKingUnderattack == true) || (blackKingUnderattack == true && Check_King_Mate(0))) qDebug() << "White win"; // Если шах поставлен черным и следующий ход белых, то черные проиграли
        if((m_whoseMove == 0 && whiteKingUnderattack == true) || (m_checkShah_White == true && whiteKingUnderattack == true) || (whiteKingUnderattack == true && Check_King_Mate(1))) qDebug() << "Black win"; // Аналогино только черные выйграли



        m_checkShah_White = whiteKingUnderattack;
        m_checkShah_Black = blackKingUnderattack;
        m_piece_Attacking_king.clear();
        m_whoseMove = !m_whoseMove;
    }
    else return;
}




