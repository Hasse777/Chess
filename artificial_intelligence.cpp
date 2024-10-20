#include "artificial_intelligence.h"
#include "chessboard.h"
#include <QMessageBox>
#include <QApplication>
#include <QRandomGenerator>
#include <QDebug>

bool m_botPlayForColor;

Artificial_Intelligence::Artificial_Intelligence(const ChessBoard * const chessBoard, bool color) : m_ChessBoard{chessBoard}, m_color{color}
{
    if(m_ChessBoard->m_pieceOnBoard.size() != 8 || m_ChessBoard->m_pieceOnBoard[0].size() != 8)
    {
        QMessageBox::critical(nullptr, "Error", "m_ChessBoard->m_pieceOnBoard is incorrect size");
        QApplication::quit();
    }
    m_color = color;
    m_botPlayForColor = color;
    m_MinMax = nullptr;
}

Artificial_Intelligence::~Artificial_Intelligence()
{
    freeMemory();
}

void Artificial_Intelligence::freeMemory()
{
    if(m_MinMax != nullptr)
    {
        int row = m_pieceOnBoard.size();
        int col = m_pieceOnBoard[0].size();
        for(int i = 0; i < row; i++)
        {
            for(int j = 0; j < col; j++)
            {
                if(m_pieceOnBoard[i][j] != nullptr)
                {
                    delete m_pieceOnBoard[i][j];
                    m_pieceOnBoard[i][j] = nullptr;
                }
            }
        }
        delete m_MinMax;
        m_MinMax = nullptr;
    }
}

void Artificial_Intelligence::slot_MoveBot()
{
    int row = m_ChessBoard->m_pieceOnBoard.size();
    int col = m_ChessBoard->m_pieceOnBoard[0].size();
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(m_ChessBoard->m_pieceOnBoard[i][j] != nullptr)
            {
                std::pair<int, int> pieceCoordinate = m_ChessBoard->m_pieceOnBoard[i][j]->getPos();
                bool color = m_ChessBoard->m_pieceOnBoard[i][j]->getColor();
                switch (m_ChessBoard->m_pieceOnBoard[i][j]->getPiece())
                {
                case 1: m_pieceOnBoard[i][j] = new KingPieceForArtifical(color, 1, pieceCoordinate, m_ChessBoard->m_pieceOnBoard[i][j]->getFirstMove());

                    if(m_pieceOnBoard[i][j]->getColor() == 0) m_whiteKing = m_pieceOnBoard[i][j];
                    else m_blackKing = m_pieceOnBoard[i][j];

                    break;
                case 2: m_pieceOnBoard[i][j] = new QueenPieceForArtifical(color, 2, pieceCoordinate) ;break;
                case 3: m_pieceOnBoard[i][j] = new RookPieceForArtifical(color, 3, pieceCoordinate, m_ChessBoard->m_pieceOnBoard[i][j]->getFirstMove()) ;break;
                case 4: m_pieceOnBoard[i][j] = new ElephantPieceForArtifical(color, 4, pieceCoordinate) ;break;
                case 5: m_pieceOnBoard[i][j] = new HorsePieceForArtifical(color, 5, pieceCoordinate) ;break;
                case 6: m_pieceOnBoard[i][j] = new PawnPieceForArtifical(color, 6, pieceCoordinate, m_ChessBoard->m_pieceOnBoard[i][j]->getFirstMove()) ;break;
                default:
                    break;
                }
            }
        }
    }
    m_MinMax = new MinMax(m_color, m_pieceOnBoard);
    std::pair<int, int> coordinatesPiece = m_MinMax->getBestMove().first;
    std::pair<int, int> moveWhere = m_MinMax->getBestMove().second;
    emit signal_BotChoseMove(coordinatesPiece, moveWhere);
    freeMemory();
}



void MinMax::distributionByChessPiece(const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>>& allPossibleMoves, bool color)
{
    int row = pieceOnBoard.size();
    int col = pieceOnBoard[0].size();
    if(row != 8 || col != 8)
    {
        QMessageBox::critical(nullptr, "Error", "Row and col aren't == 8");
        QApplication::quit();
    }
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            ChessPieceForArtifical* piece = pieceOnBoard[i][j];
            if(piece != nullptr && piece->getColor() == color)
            {
                switch (piece->getPiece())
                {
                case 1: countingPossibleMovesKing(piece, pieceOnBoard, allPossibleMoves); break;
                case 2: countingPossibleMovesQueen(piece, pieceOnBoard, allPossibleMoves); break;
                case 3: countingPossibleMovesRook(piece, pieceOnBoard, allPossibleMoves); break;
                case 4: countingPossibleMovesElephant(piece, pieceOnBoard, allPossibleMoves);break;
                case 5: countingPossibleMovesHorse(piece, pieceOnBoard, allPossibleMoves); break;
                case 6: countingPossibleMovesPawn(piece, pieceOnBoard, allPossibleMoves); break;
                default:
                    break;
                }
            }
        }
    }

}

void MinMax::countingPossibleMovesPawn(ChessPieceForArtifical* piece, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>>& allPossibleMoves)
{
    int row = piece->getPos().first;
    int col = piece->getPos().second;
    bool color = piece->getColor();
    bool direction = (m_botPlayForColor == color ? 1 : -1);
    // Проверка возможности ходов вперед
    {
        int quantitMove = (piece->getFirstMove() == 1 ? 2 : 1);
        int newRow = row;
        while (quantitMove > 0)
        {
            newRow += direction;
            if(newRow >= 8 || newRow < 0)
            {
                break;
            }
            if(pieceOnBoard[newRow][col] == nullptr)
            {
                allPossibleMoves.push_back({piece, {newRow, col}});
            }
            else
            {
                break;
            }
            quantitMove--;
        }
    }
    // Проверка возможности ходов по диагонали
    int newRow = row + direction;
    if(newRow >= 8 || newRow < 0)
    {
        return;
    }
    int newCol = col + 1;
    if(newCol >= 0 && newCol < 8 && pieceOnBoard[newRow][newCol] != nullptr && pieceOnBoard[newRow][newCol]->getColor() != color)
    {
        allPossibleMoves.push_back({piece, {newRow, newCol}});
    }
    newCol = col - 1;
    if(newCol >= 0 && newCol < 8 && pieceOnBoard[newRow][newCol] != nullptr && pieceOnBoard[newRow][newCol]->getColor() != color)
    {
        allPossibleMoves.push_back({piece, {newRow, newCol}});
    }
}

void MinMax::countingPossibleMovesHorse(ChessPieceForArtifical* piece, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>>& allPossibleMoves)
{
    int row = piece->getPos().first;
    int col = piece->getPos().second;
    bool color = piece->getColor();
    QVector<std::pair<int, int>> direction
        {
            {row + 1, col + 2}, {row - 1, col + 2}, {row + 1, col - 2}, {row - 1, col -2},
            {row + 2, col + 1}, {row + 2, col - 1}, {row - 2, col + 1}, {row - 2, col - 1}
        };
    QVector<std::pair<int, int>>::Iterator it = direction.begin();
    while (it != direction.end())
    {
        int i = it->first;
        int j = it->second;
        if(i >= 0 && i < 8 && j >= 0 && j < 8)
        {
            if(pieceOnBoard[i][j] == nullptr || (pieceOnBoard[i][j] != nullptr && pieceOnBoard[i][j]->getColor() != color))
            {
                allPossibleMoves.push_back({piece, {i, j}});
            }
        }
        it++;
    }
}

void MinMax::countingPossibleMovesElephant(ChessPieceForArtifical* piece, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>>& allPossibleMoves)
{
    int row = piece->getPos().first;
    int col = piece->getPos().second;
    bool color = piece->getColor();
    QVector<std::pair<int, int>> direction
        {
            {1, 1}, {1, -1},{-1, 1},{-1, -1}
        };
    QVector<std::pair<int, int>>::Iterator it = direction.begin();
    int newRow = row;
    int newCol = col;
    while (it != direction.end())
    {
        newRow += it->first;
        newCol += it->second;
        if(newRow >= 0 && newRow < 8 && newCol >=0 && newCol < 8)
        {
            if(pieceOnBoard[newRow][newCol] == nullptr)
            {
                allPossibleMoves.push_back({piece, {newRow, newCol}});
                continue;
            }
            else if(pieceOnBoard[newRow][newCol]->getColor() != color)
            {
                allPossibleMoves.push_back({piece, {newRow, newCol}});
            }
            newRow = row;
            newCol = col;
            it++;

        }
        else
        {
            newRow = row;
            newCol = col;
            it++;
        }
    }
}

void MinMax::countingPossibleMovesRook(ChessPieceForArtifical* piece, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>>& allPossibleMoves)
{
    int row = piece->getPos().first;
    int col = piece->getPos().second;
    bool color = piece->getColor();
    QVector<std::pair<int, int>> direction
        {
            {0, 1}, {0, -1},{1, 0},{-1, 0}
        };
    QVector<std::pair<int, int>>::Iterator it = direction.begin();
    int newRow = row;
    int newCol = col;
    while (it != direction.end())
    {
        newRow += it->first;
        newCol += it->second;
        if(newRow >= 0 && newRow < 8 && newCol >=0 && newCol < 8)
        {
            if(pieceOnBoard[newRow][newCol] == nullptr)
            {
                allPossibleMoves.push_back({piece, {newRow, newCol}});
                continue;
            }
            else if(pieceOnBoard[newRow][newCol]->getColor() != color)
            {
                allPossibleMoves.push_back({piece, {newRow, newCol}});
            }
            newRow = row;
            newCol = col;
            it++;
        }
        else
        {
            newRow = row;
            newCol = col;
            it++;
        }
    }
}

void MinMax::countingPossibleMovesQueen(ChessPieceForArtifical* piece, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>>& allPossibleMoves)
{
    int row = piece->getPos().first;
    int col = piece->getPos().second;
    bool color = piece->getColor();
    QVector<std::pair<int, int>> direction
        {
            {0, 1}, {0, -1},{1, 0},{-1, 0},
            {1, 1}, {1, -1},{-1, 1},{-1, -1}
        };
    QVector<std::pair<int, int>>::Iterator it = direction.begin();
    int newRow = row;
    int newCol = col;
    while (it != direction.end())
    {
        newRow += it->first;
        newCol += it->second;
        if(newRow >= 0 && newRow < 8 && newCol >=0 && newCol < 8)
        {
            if(pieceOnBoard[newRow][newCol] == nullptr)
            {
                allPossibleMoves.push_back({piece, {newRow, newCol}});
                continue;
            }
            else if(pieceOnBoard[newRow][newCol]->getColor() != color)
            {
                allPossibleMoves.push_back({piece, {newRow, newCol}});
            }
            newRow = row;
            newCol = col;
            it++;
        }
        else
        {
            newRow = row;
            newCol = col;
            it++;
        }
    }
}

void MinMax::countingPossibleMovesKing(ChessPieceForArtifical* piece, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>>& allPossibleMoves)
{
    int row = piece->getPos().first;
    int col = piece->getPos().second;
    bool color = piece->getColor();
    QVector<std::pair<int, int>> direction
        {
            {0, 1}, {0, -1},{1, 0},{-1, 0},
            {1, 1}, {1, -1},{-1, 1},{-1, -1}
        };
    QVector<std::pair<int, int>>::Iterator it = direction.begin();
    int newRow = row;
    int newCol = col;
    while (it != direction.end())
    {
        newRow += it->first;
        newCol += it->second;
        if(newRow >= 0 && newRow < 8 && newCol >=0 && newCol < 8)
        {
            if(pieceOnBoard[newRow][newCol] == nullptr || pieceOnBoard[newRow][newCol]->getColor() != color)
            {
                allPossibleMoves.push_back({piece, {newRow, newCol}});

            }
        }
        newRow = row;
        newCol = col;
        it++;
    }
}



int MinMax::evaluateBoard(const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard)
{
    // Можно дописать логику, чем больше фигур у стороны тем больше у неё очков
    int scoreBoard = 0;
    for(int i = 0; i < pieceOnBoard.size(); i++)
    {
        for(int j = 0; j < pieceOnBoard[0].size(); j++)
        {
            if(pieceOnBoard[i][j] != nullptr)
            {
                int scorePiece = evaluatePiece(pieceOnBoard[i][j]);
                bool color = pieceOnBoard[i][j]->getColor();
                if(pieceUnderAttack(pieceOnBoard, {i, j}, color))
                {
                    // Если фигура под атакой, то снимаем либо прибавляем очки
                    scoreBoard += (color == 1 ? scorePiece : -scorePiece);
                }
            }
            else
            {
                if(pieceUnderAttack(pieceOnBoard, {i, j}, false))
                {
                    // Чем больше контроля у черных тем меньше очков
                    scoreBoard -= 5;
                }
                if(pieceUnderAttack(pieceOnBoard, {i, j}, true))
                {
                    // Чем больше контроля у белых тем больше очков
                    scoreBoard += 5;
                }
            }
        }
    }
    return scoreBoard;
}


int MinMax::findOptimalMove(const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, int depth, int alpha, int beta, bool maxOrminPlayer)
{
    if(depth <= 0)
    {
        return evaluateBoard(pieceOnBoard);
    }

    // Все возможные ходы
    QVector<std::pair<ChessPieceForArtifical*, std::pair<int, int>>> allPossibleMoves;
    QVector<QVector<ChessPieceForArtifical*>> *tempBoard = this->copyPieceOnBoard_in_tempBoard(pieceOnBoard);
    int bestEval = -1;

    if(maxOrminPlayer == 0) // Максимизирующий игрок
    {
        bestEval = std::numeric_limits<int>::min();
        distributionByChessPiece(*tempBoard, allPossibleMoves, maxOrminPlayer);

        for(int i = 0; i < allPossibleMoves.size(); i++)
        {
            ChessPieceForArtifical* piece = allPossibleMoves[i].first;
            // Переменные для того чтобы откатить сделанный ход без прохода по всему массиву
            std::pair<int, int> moveFrom = piece->getPos(); // откуда был сделан ход
            std::pair<int, int> moveHere = allPossibleMoves[i].second; // куда будет сделан ход

            makeMove(piece, allPossibleMoves[i].second, *tempBoard);
            int eval = findOptimalMove(*(tempBoard), depth - 1, alpha, beta, !maxOrminPlayer);
            undoMove(pieceOnBoard, *tempBoard, moveFrom, moveHere);


            if (eval > bestEval)
            {
                bestEval = eval;
                if (maxOrminPlayer == m_botPlayForColor && depth == m_depth) // Обновляем лучший ход
                {
                    m_bestMove.first = moveFrom;
                    m_bestMove.second = moveHere;
                }
            }

            alpha = std::max(alpha, eval);
            if (beta <= alpha)
            {
                break;
            }
        }
    }
    else
    {
        bestEval = std::numeric_limits<int>::max();
        distributionByChessPiece(*tempBoard, allPossibleMoves, maxOrminPlayer);
        for(int i = 0; i < allPossibleMoves.size(); i++)
        {
            ChessPieceForArtifical* piece = allPossibleMoves[i].first;
            // Переменные для того чтобы откатить сделанный ход без прохода по всему массиву
            std::pair<int, int> moveFrom = piece->getPos(); // откуда был сделан ход
            std::pair<int, int> moveHere = allPossibleMoves[i].second; // куда будет сделан ход

            makeMove(piece, allPossibleMoves[i].second, *tempBoard); // Делаем ход
            int eval = findOptimalMove(*tempBoard, depth - 1, alpha, beta, !maxOrminPlayer);
            undoMove(pieceOnBoard, *tempBoard, moveFrom, moveHere);


            if (eval < bestEval)
            {
                bestEval = eval;
                if (maxOrminPlayer == m_botPlayForColor && depth == m_depth) // Обновляем лучший ход
                {
                    m_bestMove.first = moveFrom;
                    m_bestMove.second = moveHere;
                }
            }

            beta = std::min(beta, eval);
            if (beta <= alpha)
            {
                break;
            }
        }
    }

    clearTempPieceOnBoard(*tempBoard);
    delete tempBoard;
    tempBoard = nullptr;
    allPossibleMoves.clear();
    return bestEval;
}

QVector<QVector<ChessPieceForArtifical*>>* MinMax::copyPieceOnBoard_in_tempBoard(const QVector<QVector<ChessPieceForArtifical *> > &pieceOnBoard)
{
    if(pieceOnBoard.size() != 8 || pieceOnBoard[0].size() != 8)
    {
        QMessageBox::critical(nullptr, "Error", "m_ChessBoard->m_pieceOnBoard is incorrect size");
        QApplication::quit();
    }
    // Копируем состояние шахматной доски для анализа
    QVector<QVector<ChessPieceForArtifical*>> *tempBoard = new QVector<QVector<ChessPieceForArtifical*>>{8, QVector<ChessPieceForArtifical*>(8, nullptr)};
    int row = pieceOnBoard.size();
    int col = pieceOnBoard[0].size();
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(pieceOnBoard[i][j] != nullptr)
            {
                (*tempBoard)[i][j] = pieceOnBoard[i][j]->clone();
            }
            else
            {
                (*tempBoard)[i][j] = nullptr;
            }
        }
    }
    return tempBoard;
}

void MinMax::clearTempPieceOnBoard(QVector<QVector<ChessPieceForArtifical*>> &board)
{
    for (int i = 0; i < board.size(); ++i)
    {
        for (int j = 0; j < board[i].size(); ++j)
        {
            if (board[i][j] != nullptr)
            {
                delete board[i][j];
                board[i][j] = nullptr;
            }
        }
    }
    board.clear();
}


int MinMax::evaluatePiece(ChessPieceForArtifical *piece) const
{
    int price = 0;
    switch (piece->getPiece())
    {
    case 1: price = 900; break;
    case 2: price = 90; break;
    case 3: price = 50; break;
    case 4: price = 30; break;
    case 5: price = 30; break;
    case 6: price = 10; break;
    default:
        break;
    }
    return price;
}

void MinMax::makeMove(ChessPieceForArtifical *piece, std::pair<int, int> coordinates, QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard)
{
    int oldRow = piece->getPos().first;
    int oldCol = piece->getPos().second;

    int newRow = coordinates.first;
    int newCol = coordinates.second;

    if(newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8 || pieceOnBoard.size() != 8 || pieceOnBoard[0].size() != 8)
    {
        QMessageBox::critical(nullptr, "Error", "Parameters aren't correct in makeMove");
        QApplication::quit();
    }


    pieceOnBoard[oldRow][oldCol] = nullptr;
    if(pieceOnBoard[newRow][newCol] != nullptr)
    {
        delete pieceOnBoard[newRow][newCol];
    }
    pieceOnBoard[newRow][newCol] = piece;
    piece->setPos(coordinates);
}

void MinMax::undoMove(const QVector<QVector<ChessPieceForArtifical *> > &initialPieceOnBoard, QVector<QVector<ChessPieceForArtifical *> > &pieceOnBoard, std::pair<int, int> moveFrom, std::pair<int, int> moveHere)
{
    if(moveFrom.first < 0 || moveFrom.second >= 8 || moveHere.first < 0 || moveHere.second >= 8)
    {
        QMessageBox::critical(nullptr, "Error", "Unable to perform undo move");
        QApplication::quit();
    }
    ChessPieceForArtifical* piece = pieceOnBoard[moveHere.first][moveHere.second];
    if(initialPieceOnBoard[moveHere.first][moveHere.second] != nullptr)
    {
        pieceOnBoard[moveHere.first][moveHere.second] = initialPieceOnBoard[moveHere.first][moveHere.second]->clone();
    }
    else
    {
        pieceOnBoard[moveHere.first][moveHere.second] = nullptr;
    }
    pieceOnBoard[moveFrom.first][moveFrom.second] = piece;
    piece->setPos(moveFrom);
}

std::pair<std::pair<int, int>, std::pair<int, int>> MinMax::getBestMove() const
{
    return m_bestMove;
}


bool MinMax::pieceUnderAttack(const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, std::pair<int, int> coordinates, bool color)
{
    // Проверка атаки пешкой
    int direction = (color == m_botPlayForColor ? 1 : -1); //Определяем с какой стороны должна находится вражеская пешка
    int newRow = coordinates.first + direction;
    if(newRow >= 0 && newRow < 8)
    {
        int newCol = coordinates.second + 1; // проверка атаки справа
        if(newCol >= 0 && newCol < 8 && pieceOnBoard[newRow][newCol] != nullptr && pieceOnBoard[newRow][newCol]->getColor() != color && pieceOnBoard[newRow][newCol]->getPiece() != 3 && pieceOnBoard[newRow][newCol]->getPiece() != 5)
        {
            return true;
        }
        newCol = coordinates.second - 1; // проверка слева
        if(newCol >= 0 && newCol < 8 && pieceOnBoard[newRow][newCol] != nullptr && pieceOnBoard[newRow][newCol]->getColor() != color && pieceOnBoard[newRow][newCol]->getPiece() != 3 && pieceOnBoard[newRow][newCol]->getPiece() != 5)
        {
            return true;
        }
    }

    // Проверка атаки ферзём
    {
        QVector<std::pair<int, int>> direction =
        {   {0, 1}, {0, -1}, {1, 0}, {-1, 0},
            {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
        };
        QVector<std::pair<int, int>>::Iterator it = direction.begin();
        while (it != direction.end())
        {
            int newRow = coordinates.first;
            int newCol = coordinates.second;
            while (true)
            {
                newRow += it->first;
                newCol += it->second;
                if(newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
                {
                    if(pieceOnBoard[newRow][newCol] == nullptr)
                    {
                        continue;
                    }
                    else if(pieceOnBoard[newRow][newCol]->getPiece() == 2 && pieceOnBoard[newRow][newCol]->getColor() != color)
                    {
                        return true;
                    }
                    else break;
                }
                else
                {
                    break;
                }
            }
            it++;
        }
    }

    //Проверка атаки ладьей
    {
        QVector<std::pair<int, int>> direction =
            {   {0, 1}, {0, -1}, {1, 0}, {-1, 0}
            };
        QVector<std::pair<int, int>>::Iterator it = direction.begin();
        while (it != direction.end())
        {
            int newRow = coordinates.first;
            int newCol = coordinates.second;
            while (true)
            {
                newRow += it->first;
                newCol += it->second;
                if(newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
                {
                    if(pieceOnBoard[newRow][newCol] == nullptr)
                    {
                        continue;
                    }
                    else if(pieceOnBoard[newRow][newCol]->getPiece() == 3 && pieceOnBoard[newRow][newCol]->getColor() != color)
                    {
                        return true;
                    }
                    else break;
                }
                else break;
            }
            it++;
        }
    }

    // Проверка атаки слоном
    {
        QVector<std::pair<int, int>> direction =
        {
            {1, 1}, {1, -1}, {-1, -1}, {-1, 1}
        };
        QVector<std::pair<int, int>>::Iterator it = direction.begin();
        while (it != direction.end())
        {
            int newRow = coordinates.first;
            int newCol = coordinates.second;
            while (1)
            {
                newRow += it->first;
                newCol += it->second;
                if(newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
                {
                    if(pieceOnBoard[newRow][newCol] == nullptr)
                    {
                        continue;
                    }
                    else if(pieceOnBoard[newRow][newCol]->getPiece() == 4 && pieceOnBoard[newRow][newCol]->getColor() != color)
                    {
                        return true;
                    }
                    else break;
                }
                else break;
            }
            it++;
        }
    }

    //Проверка атаки конём
    {
        QVector<std::pair<int, int>> direction =
            {
                {2, 1}, {2, -1}, {1, 2}, {-1, 2},
                {-2, 1}, {-2, -1}, {1, -2}, {-1, -2}
            };
        QVector<std::pair<int, int>>::Iterator it = direction.begin();
        while (it != direction.end())
        {
            int newRow = coordinates.first + it->first;
            int newCol = coordinates.second + it->second;
            if(newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
            {
                if(pieceOnBoard[newRow][newCol] != nullptr && pieceOnBoard[newRow][newCol]->getColor() != color && pieceOnBoard[newRow][newCol]->getPiece() == 5)
                {
                    return true;
                }
            }
            it++;
        }
    }

    // Проверка атакой вражеским королем


    {
        QVector<std::pair<int, int>> direction =
            {
                {0, 1}, {0, -1}, {1, 1}, {1, -1},
                {1, 0}, {-1, 0}, {-1, -1}, {-1, 1}
            };
        QVector<std::pair<int, int>>::Iterator it = direction.begin();
        while (it != direction.end())
        {
            int newRow = coordinates.first + it->first;
            int newCol = coordinates.second + it->second;
            if(newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
            {
                if(pieceOnBoard[newRow][newCol] != nullptr && pieceOnBoard[newRow][newCol]->getColor() != color && pieceOnBoard[newRow][newCol]->getPiece() == 1)
                {
                    return true;
                }
            }
            it++;
        }
    }

    return false;
}


MinMax::MinMax(bool whoMove, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard)
    : m_whoMove{whoMove}
{
    // Копируем состояние шахматной доски для анализа
    int row = pieceOnBoard.size();
    int col = pieceOnBoard[0].size();
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(pieceOnBoard[i][j] != nullptr)
            {
                m_pieceOnBoard[i][j] = pieceOnBoard[i][j]->clone();
            }
            else
            {
                m_pieceOnBoard[i][j] = nullptr;
            }
        }
    }

    // for(int i = 0; i < row; i++)
    // {
    //     for(int j = 0; j < col; j++)
    //     {
    //         if(pieceOnBoard[i][j] != nullptr)
    //         {
    //             qDebug() << "i =" << i << "j =" << j <<  pieceOnBoard[i][j]->getPos();
    //         }
    //     }
    // }

    findOptimalMove(pieceOnBoard, m_depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), whoMove);


}

MinMax::~MinMax()
{
    int row = m_pieceOnBoard.size();
    int col = m_pieceOnBoard[0].size();
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(m_pieceOnBoard[i][j] != nullptr)
            {
                delete m_pieceOnBoard[i][j];
                m_pieceOnBoard[i][j] = nullptr;
            }
        }
    }
}
