#include "chessboard.h"
#include <QDebug>
#include "pawnselection.h"


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
    m_counterMoves = 1;
    m_whoseMove = false; // Сначала ход белых
    m_pawnPiece = nullptr; // Обнуляем указатель на пешку, которую можно взять на проходе

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
            if ((j - 1 >= 0 && m_pieceOnBoard[newRow][j - 1] != nullptr && m_pieceOnBoard[newRow][j - 1]->getColor() != piece->getColor()))
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
            //if(m_pawnPiece != nullptr) qDebug() << m_pawnPos.second << j - 1 << j + 1;

            if(m_pawnPiece != nullptr && m_pawnPiece->getColor() != piece->getColor() && (((j - 1 == m_pawnPos.second) || (j + 1  == m_pawnPos.second)) && newRow == m_pawnPos.first))
            {
                // Добавление прозрачной картинки пешки



                // Добавление подсветки клетки
                create_tempSquare(m_pawnPos.second, newRow);
                ClickableRect *highlight = new ClickableRect((m_pawnPos.second) * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
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
        QVector<std::pair<int, int>> move_king =
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
        QVector<std::pair<int, int>>::Iterator it = move_king.begin();
        for(; it != move_king.end(); it++)
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
        if(castling_check(0, piece))
        {
            int newRow = piece->getColor() == 0 ? 7 : 0;
            int newCol = 0;
            create_tempSquare(newCol, newRow);
            ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
            highlight->setZValue(1);
            m_scene->addItem(highlight);
            m_highlightedCells.push_back(highlight);
            connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
        }
        if(castling_check(1, piece))
        {
            int newRow = piece->getColor() == 0 ? 7 : 0;
            int newCol = 7;
            create_tempSquare(newCol, newRow);
            ClickableRect *highlight = new ClickableRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
            highlight->setZValue(1);
            m_scene->addItem(highlight);
            m_highlightedCells.push_back(highlight);
            connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
        }
    }
    else return;
}

void ChessBoard::addMovesForWidget(ChessPiece *piece, int oldRow, int oldCol, int newRow, int newCol)
{
    QString piece_color = piece->getColor() == 0 ? "White" : "Black";
    QString piece_type;
    QString text;

    switch (piece->getPiece())
    {
    case 1: piece_type = "King"; break;
    case 2: piece_type = "Queen"; break;
    case 3: piece_type = "Rook"; break;
    case 4: piece_type = "Elephant"; break;
    case 5: piece_type = "Horse"; break;
    case 6: piece_type = "Pawn"; break;
    default:
        break;
    }
    text = QString("%1) %2 %3 moves from %4%5 to %6%7")
    .arg(QString::number(m_counterMoves++), piece_color, piece_type, QChar('a' + oldCol), QString::number(8 - oldRow), QChar('a' + newCol), QString::number(8 - newRow));
    emit signal_addMove(text);
}

bool ChessBoard::castling_check(bool short_or_long, ChessPiece *piece)
{
    // Проверяем делал ли король ход и находится ли он под шахом
    if(!piece->getFirst_Move() || (piece->getColor() == false && m_checkShah_White == true) || ((piece->getColor() == true && m_checkShah_Black == true)))    return false;
    int row = piece->pos().y() / m_square_Size;
    int col = piece->pos().x() / m_square_Size;
    int direction = short_or_long == 0 ? -1 : 1; // Направление движения
    int count = -1;
    // Проверяем корректность коордиант
    if(row < 0 || row >= 8 || col < 0 || col >= 8) throw "castling_check() - incorrect coordinates row and col";
    while (1)
    {
        count++;
        col += direction;
        if(col == 0 || col == 7)
        {
            if(m_pieceOnBoard[row][col] == nullptr || m_pieceOnBoard[row][col]->getFirst_Move() == false) return false;
            return true;
        }
        if(m_pieceOnBoard[row][col] != nullptr || (count < 2 && square_under_attack({row, col}, piece->getColor()))) return false;
    }
}

bool ChessBoard::Check_King_Shah(bool color)
{
    return color == 0 ? square_under_attack(m_whiteKingPos, 0, true) : square_under_attack(m_blackKingPos, 1, true);
}

bool ChessBoard::Check_King_Mate(bool color)
{
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

    if(m_piece_Attacking_king.size() == 1)
    {
        int attackPieceRow = m_piece_Attacking_king[0]->pos().y() / m_square_Size;
        int attackPieceCol = m_piece_Attacking_king[0]->pos().x() / m_square_Size;
        return !square_under_attack({attackPieceRow, attackPieceCol}, m_piece_Attacking_king[0]->getColor()); // Если фигуру, которая атакует короля, можно уничтожить, то есть выход из под мата
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
    if(1)
    {
        highlight_Moves(piece);
        m_selectedPiece = piece;
    }
    //piece->getColor() == m_whoseMove
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

        // Проверка рокировки
        // Если король ходит первый раз и для него подсвечены такие столбцы как 0 и 7, то нам не нужны проверки на валидность тк они выполняются в функции подсветки возможных ходов
        if(m_selectedPiece->getPiece() == 1 && m_selectedPiece->getFirst_Move() && (newCol == 0 || newCol == 7))
        {
            int oldColRook = newCol == 7 ? 7 : 0;
            int newColRook = newCol == 7 ? 5 : 3;
            int colKing = newCol == 7 ? 6 : 2;
            //Перемещаем короля
            m_selectedPiece->setPos(QRect(colKing * m_square_Size + m_indentation, oldRow * m_square_Size + m_indentation, m_square_Size, m_square_Size).topLeft());
            m_selectedPiece->setFirst_MoveFalse();
            m_pieceOnBoard[oldRow][oldCol] = nullptr;
            m_pieceOnBoard[oldRow][colKing] = m_selectedPiece;

            //Перемещаем ладью
            m_pieceOnBoard[oldRow][oldColRook]->setPos(QRect(newColRook * m_square_Size + m_indentation, oldRow * m_square_Size + m_indentation, m_square_Size, m_square_Size).topLeft());
            m_pieceOnBoard[oldRow][oldColRook]->setFirst_MoveFalse();
            m_pieceOnBoard[oldRow][newColRook] = m_pieceOnBoard[oldRow][oldColRook];
            m_pieceOnBoard[oldRow][oldColRook] = nullptr;

            //Вызываем метод, который сформирует текст для виджета, который отображает все ходы
            addMovesForWidget(m_selectedPiece, oldRow, oldCol, oldRow, colKing);
            m_counterMoves--;
            addMovesForWidget(m_pieceOnBoard[oldRow][newColRook], oldRow, oldColRook, oldRow, newColRook);

            //Обновляем позицию короля
            if(m_selectedPiece->getColor() == 0)
            {
                m_whiteKingPos.first = oldRow;
                m_whiteKingPos.second = colKing;
            }
            else
            {
                m_blackKingPos.first = oldRow;
                m_blackKingPos.second = colKing;
            }
        }
        //Если это не рокировка
        else
        {
            if(m_pieceOnBoard[newRow][newCol] != nullptr)
            {
                if(m_pieceOnBoard[newRow][newCol]->getColor() != m_selectedPiece->getColor())
                {
                    m_scene->removeItem(m_pieceOnBoard[newRow][newCol]);
                    delete m_pieceOnBoard[newRow][newCol];
                    m_pieceOnBoard[newRow][newCol] = nullptr;
                }
                else
                {
                    // своих нельзя бить
                    return;
                }
            }
            else
            {
                if(m_selectedPiece->getPiece() == 6 && m_pawnPiece != nullptr && m_pawnPos.first == newRow && m_pawnPos.second == newCol)
                {
                    int pawnRow = m_pawnPiece->pos().y() / m_square_Size;
                    int pawnCol = m_pawnPiece->pos().x() / m_square_Size;
                    m_scene->removeItem(m_pawnPiece);
                    delete  m_pawnPiece;
                    m_pawnPiece = nullptr;
                    m_pieceOnBoard[pawnRow][pawnCol] = nullptr;
                }
            }
            //Перемещаем фигуру на новую позицию
            m_selectedPiece->setPos(cell->rect().topLeft());


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
            addMovesForWidget(m_selectedPiece, oldRow, oldCol, newRow, newCol);
        }
        //------------------------------------------------------------------------------------------------------------------------

         // Очищаем выделение клеток
         clear_highlight();

         bool whiteKingUnderattack = Check_King_Shah(false); // проверяем нахоидтся ли белый король под атакой
         bool blackKingUnderattack = Check_King_Shah(true); // проверяем нахоидтся ли черный король под атакой
         if((m_whoseMove == 1 && blackKingUnderattack == true) || (blackKingUnderattack == true && Check_King_Mate(1))) qDebug() << "White win"; // Если шах поставлен черным и следующий ход белых, то черные проиграли
         if((m_whoseMove == 0 && whiteKingUnderattack == true) || (whiteKingUnderattack == true && Check_King_Mate(0))) qDebug() << "Black win"; // Аналогино только черные выйграли

         // Обнуляем указатель на пешку после одного хода
          m_pawnPiece = nullptr;
         //-----------------------------------------------------------------------------------------------------
         // Если это пешка, которую можно взять на проходе, то нужно обновить указатель и координаты на один ход
         if(m_selectedPiece->getPiece() == 6 && m_selectedPiece->getFirst_Move() && qMax(newRow,oldRow) - qMin(newRow,oldRow) == 2)
         {
             int direction =  m_selectedPiece->getColor() == 0 ? -1 : 1;
             m_pawnPiece = m_selectedPiece;
             m_pawnPos = {oldRow + direction, oldCol};
         }
         //-----------------------------------------------------------------------------------------------------


         m_checkShah_White = whiteKingUnderattack;
         m_checkShah_Black = blackKingUnderattack;
         m_piece_Attacking_king.clear();
         m_whoseMove = !m_whoseMove;
         emit signal_Change_picture(m_whoseMove);
         m_selectedPiece->setFirst_MoveFalse();


         // Если пешка дошла до конца доски, то нужно вызвать диалоговое окно выбора фигуры
         if(m_selectedPiece->getPiece() == 6)
         {
             if(m_selectedPiece->getColor() == false && newRow == 0)
             {
                 PawnSelection* selectionPawn = new PawnSelection(false);
                 connect(selectionPawn, &PawnSelection::signals_pieceSelected, m_selectedPiece, &ChessPiece::slots_PieceSelection);
                 selectionPawn->exec();
                 disconnect(selectionPawn, &PawnSelection::signals_pieceSelected, m_selectedPiece, &ChessPiece::slots_PieceSelection);
                 delete selectionPawn;
             }
             else if(m_selectedPiece->getColor() == true && newRow == 7)
             {
                 PawnSelection* selectionPawn = new PawnSelection(true);
                 connect(selectionPawn, &PawnSelection::signals_pieceSelected, m_selectedPiece, &ChessPiece::slots_PieceSelection);
                 selectionPawn->exec();
                 disconnect(selectionPawn, &PawnSelection::signals_pieceSelected, m_selectedPiece, &ChessPiece::slots_PieceSelection);
                 delete selectionPawn;
             }
             else;
         }
         m_selectedPiece = nullptr;
    }
    else return;
}

// void ChessBoard::slot_PawnSelection(int pieceType)
// {
//     switch (pieceType)
//     {
//     case 2:

//         break;
//     default:
//         break;
//     }
// }




