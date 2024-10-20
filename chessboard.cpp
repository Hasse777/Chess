#include "chessboard.h"
#include <QDebug>
#include "pawnselection.h"
#include "dialogendgame.h"
#include "sideselectiondialog.h"
#include "artificial_intelligence.h"
#include "QMessageBox"
#include <QApplication>


//#define testEndGame

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
    m_bot = nullptr;
    //this->newGame();
}

int ChessBoard::get_square_Size() const
{
    return m_square_Size;
}

ChessBoard::~ChessBoard()
{
    ClearMemoryforNewGame();
    clear_highlight();
    delete m_scene;
    delete m_bot;
}

void ChessBoard::ClearMemoryforNewGame()
{
    for(int i = 0; i < this->m_pieceOnBoard.size(); i++)
    {
        for(int j = 0; j < this->m_pieceOnBoard[i].size(); j++)
        {
            if(m_pieceOnBoard[i][j] != nullptr) m_scene->removeItem(m_pieceOnBoard[i][j]);
            delete m_pieceOnBoard[i][j];
            m_pieceOnBoard[i][j] = nullptr;
        }
    }
    if(!m_piece_Attacking_king.empty()) m_piece_Attacking_king.clear();
}

void ChessBoard::clear_highlight()
{
    for (int i = 0; i < m_highlightedCells.size(); i++)
    {
        if(m_highlightedCells[i] != nullptr)
        {
            this->m_scene->removeItem(m_highlightedCells[i]);
            delete m_highlightedCells[i];
            m_highlightedCells[i] = nullptr;
        }
    }

    QVector<QGraphicsRectItem*>::Iterator it = m_list_tempSquare.begin();
    for(; it != m_list_tempSquare.end(); it++)
    {
        if(*it != nullptr)
        {
            this->m_scene->removeItem(*it);
            delete *it;
            *it = nullptr;
        }
    }
}

void ChessBoard::arrangement_piece(bool playerColor)
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


    // Добавление верхних фигур
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0 ; j < 8; j++)
        {
            switch (piecePositions[i][j])
            {
            case 1: m_pieceOnBoard[i][j] = new KingPiece(!m_playerColor, 1, {i, j}, m_square_Size);  break;
            case 2: m_pieceOnBoard[i][j] = new QueenPiece(!m_playerColor, 2, {i, j}, m_square_Size);  break;
            case 3: m_pieceOnBoard[i][j] = new RookPiece(!m_playerColor, 3, {i, j}, m_square_Size);  break;
            case 4: m_pieceOnBoard[i][j] = new ElephantPiece(!m_playerColor, 4, {i, j}, m_square_Size);  break;
            case 5: m_pieceOnBoard[i][j] = new HorsePiece(!m_playerColor, 5, {i, j}, m_square_Size);  break;
            case 6: m_pieceOnBoard[i][j] = new PawnPiece(!m_playerColor, 6, {i, j}, m_square_Size);  break;
            default: continue;
                break;
            }
            this->m_scene->addItem(m_pieceOnBoard[i][j]);
            m_pieceOnBoard[i][j]->setZValue(1);
            m_pieceOnBoard[i][j]->QGraphicsItem::setPos(j * m_square_Size + m_indentation, i  * m_square_Size + m_indentation);
            connect(m_pieceOnBoard[i][j], &ChessPiece::signal_mousePressEvent, this, &ChessBoard::slot_PiecePressed);
        }
    }

    // Добавление нижних
    for(int i = 6; i < 8; i++)
    {
        for(int j = 0 ; j < 8; j++)
        {
            switch (piecePositions[i][j])
            {
            case 1: m_pieceOnBoard[i][j] = new KingPiece(m_playerColor, 1, {i, j}, m_square_Size);  break;
            case 2: m_pieceOnBoard[i][j] = new QueenPiece(m_playerColor, 2, {i, j}, m_square_Size);  break;
            case 3: m_pieceOnBoard[i][j] = new RookPiece(m_playerColor, 3, {i, j}, m_square_Size);  break;
            case 4: m_pieceOnBoard[i][j] = new ElephantPiece(m_playerColor, 4, {i, j}, m_square_Size);  break;
            case 5: m_pieceOnBoard[i][j] = new HorsePiece(m_playerColor, 5, {i, j}, m_square_Size);  break;
            case 6: m_pieceOnBoard[i][j] = new PawnPiece(m_playerColor, 6, {i, j}, m_square_Size);  break;
            default: continue;
                break;
            }
            this->m_scene->addItem(m_pieceOnBoard[i][j]);
            m_pieceOnBoard[i][j]->setZValue(1);
            m_pieceOnBoard[i][j]->QGraphicsItem::setPos(j * m_square_Size + m_indentation, i  * m_square_Size + m_indentation);
            connect(m_pieceOnBoard[i][j], &ChessPiece::signal_mousePressEvent, this, &ChessBoard::slot_PiecePressed);
        }
    }

    // Храним указатели на королей
    m_whiteKing = dynamic_cast<KingPiece*>(m_pieceOnBoard[7][4]->getColor() == 0 ? m_pieceOnBoard[7][4] : m_pieceOnBoard[0][4]);
    m_blackKing = dynamic_cast<KingPiece*>(m_pieceOnBoard[7][4]->getColor() == 1 ? m_pieceOnBoard[7][4] : m_pieceOnBoard[0][4]);
    if(m_whiteKing == nullptr || m_blackKing == nullptr)
    {
        QMessageBox::critical(nullptr, "Error", "m_whiteKing or m_blackKing == nullptr");
        QApplication::quit();
    }
}

const QVector<QVector<ChessPiece *> >&ChessBoard::getPieceOnBoard() const
{
    return m_pieceOnBoard;
}




void ChessBoard::newGame()
{
    clear_highlight();
    ClearMemoryforNewGame();
    emit signal_newGame();
    m_selectedPiece = nullptr; // Обнуляем указатель на фигуру, которая ходит
    m_pawnPiece = nullptr; // Обнуляем указатель на пешку, которую можно взять на проходе
    m_counterMoves = 1;
    m_whoseMove = false; // Сначала ход белых

    emit signal_Change_picture(m_whoseMove);


    SideSelectionDialog* d = new SideSelectionDialog(this);
    connect(d, &SideSelectionDialog::signal_playerColor, this, &ChessBoard::slot_PlayerColor);
    if(d->exec() == QDialog::Rejected)
    {
        delete d;
        return;
    }
    else
    {
        delete d;
        arrangement_piece(m_playerColor);

        if(m_bot != nullptr) delete m_bot;
        m_bot = new Artificial_Intelligence(this, !m_playerColor); // Создаем бота, передаём ему текущий обьект доски для корректной работы
        // и сторону, за которую бот будет играть
        connect(this, &ChessBoard::signal_MoveBots, m_bot, &Artificial_Intelligence::slot_MoveBot, Qt::QueuedConnection);
        connect(m_bot, &Artificial_Intelligence::signal_BotChoseMove, this, &ChessBoard::slot_MoveBot);
        // Если сейчас ход не игрока, то отправляем сигнал боту
        if(m_playerColor != m_whoseMove)
        {
            emit signal_MoveBots();
        }
    }
}

void ChessBoard::endGame(bool colorWin)
{
    if(colorWin == 0)
    {
        #ifdef testEndGame
        qDebug() << "White win";
        #else

        DialogEndGame* d = new DialogEndGame(0, this);
        d->exec();
        delete d;

        #endif
        newGame();
    }
    else
    {
        #ifdef testEndGame
        qDebug() << "Black win";
        #else

        DialogEndGame* d = new DialogEndGame(1, this);
        d->exec();
        delete d;

        #endif
        newGame();
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

void ChessBoard::highlighting_possible_moves(ChessPiece *piece)
{
    unsigned short i = piece->pos().y() / m_square_Size;
    unsigned short j = piece->pos().x() / m_square_Size;
    QColor backlight_color(0, 255, 0, 100);
    enum m_piece p = static_cast<m_piece>(piece->getPiece());

    // проверка для пешки
    if(p == pawn)
    {
        int direction = (m_playerColor == 0 ? (piece->getColor() ? -1 : 1) : (piece->getColor() ? 1 : -1));

        int counter_move = (piece->getFirstMove() == true) ? 2 : 1;
        unsigned short newRow = i;

        // Проверка ходов вперед
        for (int counter = 1; counter <= counter_move; counter++)
        {
            newRow -= direction;

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
        //qDebug() << newRow;
        if (newRow >= 0 && newRow < 8)
        {
            if ((j - 1 >= 0 && m_pieceOnBoard[newRow][j - 1] != nullptr && m_pieceOnBoard[newRow][j - 1]->getColor() != piece->getColor()))
            {
                //qDebug() << "here" << j - 1;
                create_tempSquare(j - 1, newRow);
                ClickableRect *highlight = new ClickableRect((j - 1) * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, backlight_color);
                highlight->setZValue(1);
                m_scene->addItem(highlight);
                m_highlightedCells.push_back(highlight);
                connect(highlight, &ClickableRect::signal_clicked, this, &ChessBoard::slot_HighlightedCell_Clicked);
            }
            if (j + 1 < 8 && m_pieceOnBoard[newRow][j + 1] != nullptr && m_pieceOnBoard[newRow][j + 1]->getColor() != piece->getColor())
            {
                //qDebug() << "here" << j + 1;
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
                if(m_pieceOnBoard[newRow][newCol] == nullptr && square_under_attack({newRow, newCol}, piece->getColor()) == false)
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
                    if(m_pieceOnBoard[newRow][newCol] != nullptr && m_pieceOnBoard[newRow][newCol]->getColor() != piece->getColor() && square_under_attack({newRow, newCol}, piece->getColor()) == false)
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
            //int newRow = piece->getColor() == 0 ? 7 : 0;
            int newRow = m_playerColor == 0 ? (piece->getColor() == 0 ? 7 : 0) : (piece->getColor() == 0 ? 0 : 7);
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
            int newRow = m_playerColor == 0 ? (piece->getColor() == 0 ? 7 : 0) : (piece->getColor() == 0 ? 0 : 7);
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
    if(!piece->getFirstMove() || (piece->getColor() == false && m_whiteKing->getShah() == true) || ((piece->getColor() == true && m_blackKing->getShah() == true)))    return false;
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
            if(m_pieceOnBoard[row][col] == nullptr || m_pieceOnBoard[row][col]->getFirstMove() == false) return false;
            return true;
        }
        if(m_pieceOnBoard[row][col] != nullptr || (count < 2 && square_under_attack({row, col}, piece->getColor()))) return false;
    }
}

bool ChessBoard::Check_King_Shah(bool color)
{
    return color == 0 ? square_under_attack(m_whiteKing->getPos(), 0, true) : square_under_attack(m_blackKing->getPos(), 1, true);
}

bool ChessBoard::Check_King_Mate(bool color)
{
    std::pair<int, int> kingPos = color == 0 ? m_whiteKing->getPos() : m_blackKing->getPos();

    QVector<std::pair<int, int>> kingMove =
    {
        {kingPos.first + 1, kingPos.second}, {kingPos.first + 1, kingPos.second + 1},
        {kingPos.first + 1, kingPos.second - 1}, {kingPos.first, kingPos.second + 1},
        {kingPos.first, kingPos.second - 1}, {kingPos.first - 1, kingPos.second},
        {kingPos.first - 1, kingPos.second + 1}, {kingPos.first - 1, kingPos.second - 1}
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
        if(square_under_attack({attackPieceRow, attackPieceCol}, m_piece_Attacking_king[0]->getColor()))
        {
            return false; // Если фигуру, которая атакует короля, можно уничтожить, то есть выход из под мата
        }
    }
    else if(m_piece_Attacking_king.size() > 1)
    {
        return true; // Если фигур, атакующих короля, больше 2 то это мат
    }

    // Проверка есть ли фигура которая может перекрыть короля
    QVector<std::pair<int, int>> *arr = (color == 0 ? &m_whiteSquareCover : &m_blackSquareCover);
    for(int i = 0; i < arr->size(); i++)
    {
        int row = (*arr)[i].first;
        int col = (*arr)[i].second;
        if(square_under_attack({row, col}, !color))
        {
            return false;
        }
    }

    return true; // Если нет безопасных клеток для короля, то это мат
}



bool ChessBoard::square_under_attack(std::pair<int, int> coordinates, bool color, bool mod)
{
    int kingRow = coordinates.first;
    int kingCol = coordinates.second;
    if(kingRow  < 0 || kingRow  >= 8 || kingCol < 0 || kingCol >= 8) throw "Incorrect coordinates passed to king check function";
    //Проверка угрозы пешкой
    int direction = (m_playerColor == 0 ? (color ? -1 : 1) : (color ? 1 : -1));
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
    QVector<std::pair<int, int>> *arr = (color == 0 ? &m_whiteSquareCover : &m_blackSquareCover);
    int count = 0;
    while (it_directions != directions_VH.end())
    {
        int newRow = kingRow;
        int newCol = kingCol;

        while (true)
        {
            newRow += it_directions->first;
            newCol += it_directions->second;
            // которые ведут к королю
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
                if(mod == true) arr->clear();
                break;
            }
            if(mod == true) arr->push_front({newRow, newCol}); // если происходит проверка угрозы королю, то нужно запомнить клетки
        }
        count++;
        it_directions++;
    }
    if(mod == true) arr->clear();

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
    if(piece->getColor() == m_whoseMove && piece->getColor() == m_playerColor)
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
        emit signal_Change_picture(!m_whoseMove);
        // Получаем текущую позицию выбранной фигуры
        int oldRow = m_selectedPiece->pos().y() / m_square_Size;
        int oldCol = m_selectedPiece->pos().x() / m_square_Size;


        // Получаем новую позицию из выделенной клетки
        int newRow = cell->rect().y() / m_square_Size;
        int newCol = cell->rect().x() / m_square_Size;


        // Проверка на выход за пределы массива
        if(newRow >= 8 || newRow < 0 || newCol >= 8 || newCol < 0)
        {
            QMessageBox::critical(nullptr, "Error", "New position is out of bounds");
            QApplication::quit();
        }
        if(oldRow >= 8 || oldRow < 0 || oldCol >= 8 || oldCol < 0)
        {
            QMessageBox::critical(nullptr, "Error", "Old position is out of bounds");
            QApplication::quit();
        }
        //_____________________________________________________________

        // Проверка рокировки
        // Если король ходит первый раз и для него подсвечены такие столбцы как 0 и 7, то нам не нужны проверки на валидность тк они выполняются в функции подсветки возможных ходов
        if(m_selectedPiece->getPiece() == 1 && m_selectedPiece->getFirstMove() && (newCol == 0 || newCol == 7))
        {
            int oldColRook = newCol == 7 ? 7 : 0;
            int newColRook = newCol == 7 ? 5 : 3;
            int colKing = newCol == 7 ? 6 : 2;
            //Перемещаем короля
            m_selectedPiece->QGraphicsItem::setPos(QRect(colKing * m_square_Size + m_indentation, oldRow * m_square_Size + m_indentation, m_square_Size, m_square_Size).topLeft());
            m_selectedPiece->setFirstMove();
            m_pieceOnBoard[oldRow][oldCol] = nullptr;
            m_pieceOnBoard[oldRow][colKing] = m_selectedPiece;

            //Перемещаем ладью
            m_pieceOnBoard[oldRow][oldColRook]->QGraphicsItem::setPos(QRect(newColRook * m_square_Size + m_indentation, oldRow * m_square_Size + m_indentation, m_square_Size, m_square_Size).topLeft());
            m_pieceOnBoard[oldRow][oldColRook]->setFirstMove();
            m_pieceOnBoard[oldRow][newColRook] = m_pieceOnBoard[oldRow][oldColRook];
            m_pieceOnBoard[oldRow][newColRook]->setPos({oldRow, newColRook}); // обновляем позицию ладьи
            m_pieceOnBoard[oldRow][oldColRook] = nullptr;

            //Вызываем метод, который сформирует текст для виджета, который отображает все ходы
            addMovesForWidget(m_selectedPiece, oldRow, oldCol, oldRow, colKing);
            m_counterMoves--;
            addMovesForWidget(m_pieceOnBoard[oldRow][newColRook], oldRow, oldColRook, oldRow, newColRook);

            //Обновляем позицию короля
            if(m_selectedPiece->getColor() == 0)
            {
                m_whiteKing->setPos({oldRow, colKing});
            }
            else
            {
                m_blackKing->setPos({oldRow, colKing});
            }
        }
        //Если это не рокировка
        else
        {
            if(m_pieceOnBoard[newRow][newCol] != nullptr)
            {
                if(m_pieceOnBoard[newRow][newCol]->getColor() != m_selectedPiece->getColor())
                {

                    if(m_pieceOnBoard[newRow][newCol]->getPiece() == 1)
                    {
                        bool color = !m_pieceOnBoard[newRow][newCol]->getColor();
                        emit signal_addEatenImages(m_pieceOnBoard[newRow][newCol]->pixmap());
                        m_scene->removeItem(m_pieceOnBoard[newRow][newCol]);
                        m_pieceOnBoard[newRow][newCol] = nullptr;
                        m_selectedPiece->QGraphicsItem::setPos(cell->rect().topLeft());
                        clear_highlight();
                        endGame(color);
                        return;
                    }
                    emit signal_addEatenImages(m_pieceOnBoard[newRow][newCol]->pixmap());
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
                // Если пешку берут на проходе
                if(m_selectedPiece->getPiece() == 6 && m_pawnPiece != nullptr && m_pawnPos.first == newRow && m_pawnPos.second == newCol)
                {
                    int pawnRow = m_pawnPiece->pos().y() / m_square_Size;
                    int pawnCol = m_pawnPiece->pos().x() / m_square_Size;
                    emit signal_addEatenImages(m_pieceOnBoard[pawnRow][pawnCol]->pixmap());
                    m_scene->removeItem(m_pawnPiece);
                    delete  m_pawnPiece;
                    m_pawnPiece = nullptr;
                    m_pieceOnBoard[pawnRow][pawnCol] = nullptr;
                }
            }
            //Перемещаем фигуру на новую позицию
            m_selectedPiece->QGraphicsItem::setPos(cell->rect().topLeft());


            //Обновляем позицию фигуры на доске
            m_pieceOnBoard[oldRow][oldCol] = nullptr;
            m_pieceOnBoard[newRow][newCol] = m_selectedPiece;
            m_selectedPiece->setPos({newRow, newCol});

            //Если походили королем, то обновляем координаты
            if(m_selectedPiece->getPiece() == 1)
            {
                if(m_selectedPiece->getColor() == 0)
                {
                    m_whiteKing->setPos({newRow, newCol});
                }
                else
                {
                    m_blackKing->setPos({newRow, newCol});
                }
            }
            addMovesForWidget(m_selectedPiece, oldRow, oldCol, newRow, newCol);
        }
        //------------------------------------------------------------------------------------------------------------------------

         // Очищаем выделение клеток
         clear_highlight();

         bool whiteKingUnderattack = Check_King_Shah(false); // проверяем нахоидтся ли белый король под атакой
         bool blackKingUnderattack = Check_King_Shah(true); // проверяем нахоидтся ли черный король под атакой
          //qDebug() << ((m_whoseMove == 0 && whiteKingUnderattack == true) || (whiteKingUnderattack == true && Check_King_Mate(0)) || (m_whoseMove == 1 && blackKingUnderattack == true) || (blackKingUnderattack == true && Check_King_Mate(1)));
         //qDebug() << (blackKingUnderattack == true);
         if((m_whoseMove == 1 && blackKingUnderattack == true) || (blackKingUnderattack == true && Check_King_Mate(1)))
         {
             //qDebug() << "end";
             endGame(0); // Если шах поставлен черным и следующий ход белых, то черные проиграли
             return;
         }
         else if((m_whoseMove == 0 && whiteKingUnderattack == true) || (whiteKingUnderattack == true && Check_King_Mate(0)))
         {
             //qDebug() << "end";
             endGame(1); // Аналогино только черные выйграли
             return;
         }
         else
         {
             // Обнуляем указатель на пешку после одного хода
             m_pawnPiece = nullptr;
             //-----------------------------------------------------------------------------------------------------
             // Если это пешка, которую можно взять на проходе, то нужно обновить указатель и координаты на один ход
             if(m_selectedPiece->getPiece() == 6 && m_selectedPiece->getFirstMove() && qMax(newRow,oldRow) - qMin(newRow,oldRow) == 2)
             {
                 int direction =  (m_playerColor == 0 ? (m_selectedPiece->getColor() ? -1 : 1) : (m_selectedPiece->getColor() ? 1 : -1));
                 //int direction = (m_playerColor == 0 ? (piece->getColor() ? -1 : 1) : (piece->getColor() ? 1 : -1));
                 m_pawnPiece = m_selectedPiece;
                 m_pawnPos = {oldRow - direction, oldCol};
             }
             //-----------------------------------------------------------------------------------------------------

             m_whiteKing->setShah(whiteKingUnderattack);
             m_blackKing->setShah(blackKingUnderattack);
             m_piece_Attacking_king.clear();
             m_whoseMove = !m_whoseMove;
             // emit signal_Change_picture(m_whoseMove);
             m_selectedPiece->setFirstMove();

             // Если пешка дошла до конца доски, то нужно вызвать диалоговое окно выбора фигуры
             if(m_selectedPiece->getPiece() == 6)
             {
                 if((m_playerColor == 0 && m_selectedPiece->getColor() == false && newRow == 0) || (m_playerColor == 1 && m_selectedPiece->getColor() == false && newRow ==7))
                 {
                     PawnSelection* selectionPawn = new PawnSelection(false, this);
                     connect(selectionPawn, &PawnSelection::signals_pieceSelected, m_selectedPiece, &ChessPiece::slot_PieceSelection);
                     selectionPawn->exec();
                     disconnect(selectionPawn, &PawnSelection::signals_pieceSelected, m_selectedPiece, &ChessPiece::slot_PieceSelection);
                     delete selectionPawn;
                 }
                 else if((m_playerColor == 0 && m_selectedPiece->getColor() == true && newRow == 7) || (m_playerColor == 1  && m_selectedPiece->getColor() == true && newRow == 0))
                 {
                     PawnSelection* selectionPawn = new PawnSelection(true, this);
                     connect(selectionPawn, &PawnSelection::signals_pieceSelected, m_selectedPiece, &ChessPiece::slot_PieceSelection);
                     selectionPawn->exec();
                     disconnect(selectionPawn, &PawnSelection::signals_pieceSelected, m_selectedPiece, &ChessPiece::slot_PieceSelection);
                     delete selectionPawn;
                 }
                 else;
             }
             m_selectedPiece = nullptr;
             m_whiteSquareCover.clear();
             m_blackSquareCover.clear();
             emit signal_MoveBots();
         }
    }
    else return;
}

void ChessBoard::slot_MoveBot(std::pair<int, int> coordinatesPiece, std::pair<int, int> moveWhere)
{
     ChessPiece* piece = m_pieceOnBoard[coordinatesPiece.first][coordinatesPiece.second];
     if(piece != nullptr)
     {
         emit signal_Change_picture(!m_whoseMove);
         int oldRow = piece->pos().y() / m_square_Size;
         int oldCol = piece->pos().x() / m_square_Size;

         int newRow = moveWhere.first;
         int newCol = moveWhere.second;

         // Проверка на выход за пределы массива
         if(newRow >= 8 || newRow < 0 || newCol >= 8 || newCol < 0)
         {
             QMessageBox::critical(nullptr, "Error", "New position is out of bounds");
             QApplication::quit();
         }
         if(oldRow >= 8 || oldRow < 0 || oldCol >= 8 || oldCol < 0)
         {
             QMessageBox::critical(nullptr, "Error", "Old position is out of bounds");
             QApplication::quit();
         }


         if(m_pieceOnBoard[newRow][newCol] != nullptr)
         {
             if(m_pieceOnBoard[newRow][newCol]->getPiece() == 1)
             {
                 bool color = !m_pieceOnBoard[newRow][newCol]->getColor();
                 emit signal_addEatenImages(m_pieceOnBoard[newRow][newCol]->pixmap());
                 m_scene->removeItem(m_pieceOnBoard[newRow][newCol]);
                 m_pieceOnBoard[newRow][newCol] = nullptr;
                 piece->QGraphicsItem::setPos(QRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, m_square_Size).topLeft());
                 endGame(color);
                 return;
             }
             emit signal_addEatenImages(m_pieceOnBoard[newRow][newCol]->pixmap());
             m_scene->removeItem(m_pieceOnBoard[newRow][newCol]);
             delete m_pieceOnBoard[newRow][newCol];
             m_pieceOnBoard[newRow][newCol] = nullptr;
         }
         else
         {
             //Нужно проверить взятие ли это на проходе
             if(piece->getPiece() == 6 && m_pawnPiece != nullptr && m_pawnPos.first == newRow && m_pawnPos.second == newCol)
             {
                 int pawnRow = m_pawnPiece->pos().y() / m_square_Size;
                 int pawnCol = m_pawnPiece->pos().x() / m_square_Size;
                 emit signal_addEatenImages(m_pieceOnBoard[pawnRow][pawnCol]->pixmap());
                 m_scene->removeItem(m_pawnPiece);
                 delete  m_pawnPiece;
                 m_pawnPiece = nullptr;
                 m_pieceOnBoard[pawnRow][pawnCol] = nullptr;
             }

         }
         piece->QGraphicsItem::setPos(QRect(newCol * m_square_Size + m_indentation, newRow * m_square_Size + m_indentation, m_square_Size, m_square_Size).topLeft());

         //Обновляем позицию фигуры на доске
         m_pieceOnBoard[oldRow][oldCol] = nullptr;
         m_pieceOnBoard[newRow][newCol] = piece;

         //Если походили королем, то обновляем координаты
         if(piece->getPiece() == 1)
         {
             if(piece->getColor() == 0)
             {
                 m_whiteKing->setPos({newRow, newCol});
             }
             else
             {
                 m_blackKing->setPos({newRow, newCol});
             }
         }
         addMovesForWidget(piece, oldRow, oldCol, newRow, newCol);

         bool whiteKingUnderattack = Check_King_Shah(false); // проверяем нахоидтся ли белый король под атакой
         bool blackKingUnderattack = Check_King_Shah(true); // проверяем нахоидтся ли черный король под атакой
         if((m_whoseMove == 1 && blackKingUnderattack == true) || (blackKingUnderattack == true && Check_King_Mate(1)))
         {
             endGame(0); // Если шах поставлен черным и следующий ход белых, то черные проиграли
             return;
         }
         else if((m_whoseMove == 0 && whiteKingUnderattack == true) || (whiteKingUnderattack == true && Check_King_Mate(0)))
         {
             endGame(1); // Аналогино только черные выйграли
             return;
         }
         else
         {
             // Обнуляем указатель на пешку после одного хода
             m_pawnPiece = nullptr;
             //-----------------------------------------------------------------------------------------------------
             // Если это пешка, которую можно взять на проходе, то нужно обновить указатель и координаты на один ход
             if(piece->getPiece() == 6 && piece->getFirstMove() && qMax(newRow,oldRow) - qMin(newRow,oldRow) == 2)
             {
                 int direction =  (m_playerColor == 0 ? (piece->getColor() ? -1 : 1) : (piece->getColor() ? 1 : -1));
                 //int direction = (m_playerColor == 0 ? (piece->getColor() ? -1 : 1) : (piece->getColor() ? 1 : -1));
                 m_pawnPiece = piece;
                 m_pawnPos = {oldRow - direction, oldCol};
             }
             //-----------------------------------------------------------------------------------------------------


             m_whiteKing->setShah(whiteKingUnderattack);
             m_blackKing->setShah(blackKingUnderattack);
             m_piece_Attacking_king.clear();
             m_whoseMove = !m_whoseMove;
             // emit signal_Change_picture(m_whoseMove);
             piece->setFirstMove();


             // Если пешка бота дошла до конца, то автоматом делаем её ферзем
             if(piece->getPiece() == 6)
             {
                 if((m_playerColor == 0 && piece->getColor() == false && newRow == 0) || (m_playerColor == 1 && piece->getColor() == false && newRow ==7))
                 {
                     piece->slot_PieceSelection(2);
                 }
                 else if((m_playerColor == 0 && piece->getColor() == true && newRow == 7) || (m_playerColor == 1  && piece->getColor() == true && newRow == 0))
                 {
                     piece->slot_PieceSelection(2);
                 }
                 else;
             }
             piece->setPos({newRow, newCol});
             piece = nullptr;

             m_whiteSquareCover.clear();
             m_blackSquareCover.clear();
         }
     }
     else
     {
         // qDebug() << coordinatesPiece;
         // for(int i = 0; i < m_pieceOnBoard.size(); i++)
         // {
         //     for(int j = 0; j < m_pieceOnBoard[0].size(); j++)
         //     {
         //         if(m_pieceOnBoard[i][j] != nullptr)
         //         {
         //             qDebug() << "i =" << i << "j =" << j << m_pieceOnBoard[i][j]->getPos();
         //         }
         //     }
         // }
         // // Здесь баг
     }
}

void ChessBoard::slot_PlayerColor(bool color)
{
    m_playerColor = color;
}

void ChessBoard::slot_newGame()
{
    newGame();
}



