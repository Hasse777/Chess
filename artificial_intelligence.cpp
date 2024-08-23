#include "artificial_intelligence.h"
#include "chessboard.h"
#include <QMessageBox>
#include <QApplication>
#include <QRandomGenerator>
#include <QDebug>



Artificial_Intelligence::Artificial_Intelligence(const ChessBoard * const chessBoard, bool color) : m_ChessBoard{chessBoard}, m_color{color}
{
    //bot_color = color;
    m_color = color;
}

Artificial_Intelligence::~Artificial_Intelligence()
{
    delete m_MinMax;
}

void Artificial_Intelligence::slot_MoveBot()
{
    m_MinMax = new MinMax(m_ChessBoard->m_square_Size, m_color, 6 ,m_ChessBoard->m_pieceOnBoard);


    delete m_MinMax;
    m_MinMax = nullptr;
}


MinMax::MinMax(int squareSize, bool whoMove, int depth, QVector<QVector<ChessPiece *> > pieceOnBoard) :m_depth{depth}, m_squareSize{squareSize},
    m_whoMove{whoMove}
{
    // int row = pieceOnBoard.size();
    // int col = pieceOnBoard[0].size();
    // if(row != 8 || col != 8)
    // {
    //     QMessageBox::critical(nullptr, "Error", "Incorrect array passed to AI");
    //     QApplication::quit();
    // }

    // // Полностью копируем исходную доску
    // for(int i = 0; i < row; i++)
    // {
    //     for(int j = 0; j < col; j++)
    //     {
    //         if(pieceOnBoard[i][j] != nullptr)
    //         {
    //             m_pieceOnBoard[i][j] = new ChessPiece(*pieceOnBoard[i][j]);
    //         }
    //         else
    //         {
    //             m_pieceOnBoard[i][j] = nullptr;
    //         }
    //     }
    // }
}

MinMax::~MinMax()
{
    for(int i = 0; i < m_pieceOnBoard.size(); i++)
    {
        for(int j = 0; j < m_pieceOnBoard[i].size(); j++)
        {
            if(m_pieceOnBoard[i][j] != nullptr)
            {
                delete m_pieceOnBoard[i][j];
            }
        }
    }
}


void MinMax::countingPossibleMovesPawn(ChessPiece *piece)
{

}



// Artificial_Intelligence::Artificial_Intelligence(const ChessBoard * const board, bool color) : m_ChessBoard{board}, m_color{color}
// {

// }

// Artificial_Intelligence::~Artificial_Intelligence()
// {

// }

// void Artificial_Intelligence::setPossiblelMoveInPair()
// {
//     // Ищем все возможные фигуры, которыми бот может походить, для дальнейшего поиска лучшего хода
//     if(m_ChessBoard->m_pieceOnBoard.empty() || m_ChessBoard->m_pieceOnBoard[0].empty())
//     {
//         QMessageBox::critical(nullptr, "Error", "Null array passed to AI");
//         return;
//     }
//     int row = m_ChessBoard->m_pieceOnBoard.size();
//     int col = m_ChessBoard->m_pieceOnBoard[0].size();
//     for(int i = 0; i < row; i++)
//     {
//         for(int j = 0; j < col; j++)
//         {
//             ChessPiece* prt = m_ChessBoard->m_pieceOnBoard[i][j];
//             if(prt != nullptr && prt->getColor() == m_color)
//             {
//                 // Вызываем функцию, которая определит что это за фигура
//                 distributionByChessPiece(prt);
//             }
//         }
//     }
// }



// void Artificial_Intelligence::distributionByChessPiece(ChessPiece * prt)
// {

//     enum m_pieceType PieceType = static_cast<m_pieceType>(prt->getPiece());
//     switch (PieceType)
//     {
//     case pawn:      countingPossibleMovesPawn(prt);     break;
//     case horse:     countingPossibleMovesHorse(prt);    break;
//     case elephant:  countingPossibleMovesElephant(prt); break;
//     case rook:      countingPossibleMovesRook(prt);     break;
//     case queen:     countingPossibleMovesQueen(prt);    break;
//     case king:      countingPossibleMovesKing(prt);     break;
//     default:
//         break;
//     }
// }

// void Artificial_Intelligence::countingPossibleMovesPawn(ChessPiece *prt)
// {
//     // Добавление ходов в массив всех возможных движений для пешки

//     // Проверка на возможность походить прямо
//     int direction = 1; // Направление движение всегда вниз, т.k бот всегда находится сверху
//     int counter_moves = (prt->getFirst_Move() == 0 ? 1 : 2); // Если это первый ход пешкой, то можно пройти две клетки прямо
//     int row = prt->pos().y() / m_ChessBoard->m_square_Size;
//     int col = prt->pos().x() / m_ChessBoard->m_square_Size;
//     QVector<std::pair<int, int>> possible_move;

//     // Проверка выходов за пределы шахматной доски
//     if(row >= 8 || row < 0 || col >= 8 || col < 0)
//     {
//         QMessageBox::critical(nullptr, "Error", "Row or Col >= 8 or < 0 in Artifical_Intelligence");
//         QApplication::quit();
//     }
//     // Проверка ходов вперед
//     int newRow = row;
//     int newCol = col;
//     for(int i = 0; i < counter_moves; i++)
//     {
//         newRow += direction;
//         if(newRow > 7 || m_ChessBoard->m_pieceOnBoard[newRow][newCol] != nullptr) break;
//         possible_move.push_back({newRow, newCol});
//     }

//     //Проверка ходов по диагонали
//     newRow = row + direction;


//     // Переменные для проверки можно ли взять пешку на проходе
//     ChessPiece* pawnPiece = m_ChessBoard->m_pawnPiece;
//     int pawnRow;
//     int pawnCol;
//     if(pawnPiece != nullptr)
//     {
//         pawnRow = m_ChessBoard->m_pawnPos.first;
//         pawnCol = m_ChessBoard->m_pawnPos.second;
//     }
//     //-------------------------------------------------------------------------------

//     // Проверяем можно ли пойти направо
//     newCol = col + 1;
//     if(newRow < 8 && newRow >= 0 && newCol < 8 && newCol >= 0)
//     {
//         if((m_ChessBoard->m_pieceOnBoard[newRow][newCol] != nullptr && m_ChessBoard->m_pieceOnBoard[newRow][newCol]->getColor() != m_color) || (pawnPiece != nullptr && newRow == pawnRow && newCol == pawnCol))
//         {
//             possible_move.push_back({newRow, newCol});
//         }
//     }

//     //Проверяем можно ли пойти налево
//     newCol = col - 1;
//     if(newRow < 8 && newRow >= 0 && newCol < 8 && newCol >= 0)
//     {
//         if((m_ChessBoard->m_pieceOnBoard[newRow][newCol] != nullptr && m_ChessBoard->m_pieceOnBoard[newRow][newCol]->getColor() != m_color) || (pawnPiece != nullptr && newRow == pawnRow && newCol == pawnCol))
//         {
//             possible_move.push_back({newRow, newCol});
//         }
//     }
//     // Добавляем результат для данной фигуры в массив
//     if(!possible_move.empty())
//     {
//         m_possibleMove.push_back({prt, possible_move});
//     }
// }

// void Artificial_Intelligence::countingPossibleMovesHorse(ChessPiece *prt)
// {
//     int row = prt->pos().y() / m_ChessBoard->m_square_Size;
//     int col = prt->pos().x() / m_ChessBoard->m_square_Size;
//     // possible_move нужна для того чтобы добавить в конце координаты возможных ходов для фигуры
//     // если possible_move == empty, то мы не будем добавлять фигуру в массив всех фигур, которые могут походить
//     QVector<std::pair<int, int>> possible_move;

//     // Направление куда может пойти рыцарь
//     QVector<std::pair<int, int>> direction
//     {
//         {row + 2, col + 1}, {row + 2, col - 1},
//         {row - 2, col + 1}, {row - 2, col - 1},
//         {row + 1, col + 2}, {row + 1, col - 2},
//         {row - 1, col + 2}, {row - 1, col - 2}
//     };
//     QVector<std::pair<int, int>>::Iterator it = direction.begin();
//     while (it != direction.end())
//     {
//         int newRow = it->first;
//         int newCol = it->second;
//         if(newRow < 8 && newRow >= 0 && newCol < 8 && newCol >= 0)
//         {
//             if(m_ChessBoard->m_pieceOnBoard[newRow][newCol] == nullptr || m_ChessBoard->m_pieceOnBoard[newRow][newCol]->getColor() != m_color)
//             {
//                 possible_move.push_back({newRow, newCol});
//             }
//         }
//         it++;
//     }
//     if(!possible_move.empty())
//     {
//         m_possibleMove.push_back({prt, possible_move});
//     }
// }

// void Artificial_Intelligence::countingPossibleMovesElephant(ChessPiece *prt)
// {
//     // Аналогично предыдущей функции только для Слона
//     int row = prt->pos().y() / m_ChessBoard->m_square_Size;
//     int col = prt->pos().x() / m_ChessBoard->m_square_Size;
//     QVector<std::pair<int, int>> possible_move;
//     QVector<std::pair<int, int>> direction
//     {
//         {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
//     };
//     QVector<std::pair<int, int>>::Iterator it = direction.begin();
//     for(; it != direction.end(); it++)
//     {
//         int newRow = row + it->first;
//         int newCol = col + it->second;
//         while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
//         {
//             if(m_ChessBoard->m_pieceOnBoard[newRow][newCol] == nullptr)
//             {
//                 possible_move.push_back({newRow, newCol});
//             }
//             else if(m_ChessBoard->m_pieceOnBoard[newRow][newCol]->getColor() != m_color)
//             {
//                 possible_move.push_back({newRow, newCol});
//                 break;
//             }
//             else
//             {
//                 break;
//             }
//             newRow += it->first;
//             newCol += it->second;
//         }
//     }
//     if(!possible_move.empty())
//     {
//         m_possibleMove.push_back({prt, possible_move});
//     }
// }

// void Artificial_Intelligence::countingPossibleMovesRook(ChessPiece *prt)
// {
//     // Аналогично предыдущей функции только для ладья
//     int row = prt->pos().y() / m_ChessBoard->m_square_Size;
//     int col = prt->pos().x() / m_ChessBoard->m_square_Size;
//     QVector<std::pair<int, int>> possible_move;
//     QVector<std::pair<int, int>> direction
//     {
//         {0, 1}, {0, -1}, {1, 0}, {-1, 0}
//     };
//     QVector<std::pair<int, int>>::Iterator it = direction.begin();
//     for(; it != direction.end(); it++)
//     {
//         int newRow = row + it->first;
//         int newCol = col + it->second;
//         while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
//         {
//             if(m_ChessBoard->m_pieceOnBoard[newRow][newCol] == nullptr)
//             {
//                 possible_move.push_back({newRow, newCol});
//             }
//             else if(m_ChessBoard->m_pieceOnBoard[newRow][newCol]->getColor() != m_color)
//             {
//                 possible_move.push_back({newRow, newCol});
//                 break;
//             }
//             else
//             {
//                 break;
//             }
//             newRow += it->first;
//             newCol += it->second;
//         }
//     }
//     if(!possible_move.empty())
//     {
//         m_possibleMove.push_back({prt, possible_move});
//     }
// }

// void Artificial_Intelligence::countingPossibleMovesQueen(ChessPiece *prt)
// {
//     // Аналогично предыдущей функции только для королевы
//     int row = prt->pos().y() / m_ChessBoard->m_square_Size;
//     int col = prt->pos().x() / m_ChessBoard->m_square_Size;
//     QVector<std::pair<int, int>> possible_move;
//     QVector<std::pair<int, int>> direction
//         {
//             {0, 1}, {0, -1}, {1, 0}, {-1, 0},
//             {1, 1}, {-1, -1}, {-1, 1}, {1, -1}
//         };
//     QVector<std::pair<int, int>>::Iterator it = direction.begin();
//     for(; it != direction.end(); it++)
//     {
//         int newRow = row + it->first;
//         int newCol = col + it->second;
//         while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
//         {
//             if(m_ChessBoard->m_pieceOnBoard[newRow][newCol] == nullptr)
//             {
//                 possible_move.push_back({newRow, newCol});
//             }
//             else if(m_ChessBoard->m_pieceOnBoard[newRow][newCol]->getColor() != m_color)
//             {
//                 possible_move.push_back({newRow, newCol});
//                 break;
//             }
//             else
//             {
//                 break;
//             }
//             newRow += it->first;
//             newCol += it->second;
//         }
//     }
//     if(!possible_move.empty())
//     {
//         m_possibleMove.push_back({prt, possible_move});
//     }

// }

// void Artificial_Intelligence::countingPossibleMovesKing(ChessPiece *prt)
// {
//     // Аналогично предыдущей функции только для короля
//     int row = prt->pos().y() / m_ChessBoard->m_square_Size;
//     int col = prt->pos().x() / m_ChessBoard->m_square_Size;
//     QVector<std::pair<int, int>> possible_move;
//     QVector<std::pair<int, int>> direction =
//         {

//             {row, col + 1},
//             {row, col - 1},
//             {row + 1, col},
//             {row - 1, col},
//             {row + 1, col + 1},
//             {row + 1, col - 1},
//             {row - 1, col + 1},
//             {row - 1, col - 1}
//         };
//     QVector<std::pair<int, int>>::Iterator it = direction.begin();
//     for (; it != direction.end(); it++)
//     {
//         int newRow = it->first;
//         int newCol = it->second;
//         if(newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
//         {
//             if(m_ChessBoard->m_pieceOnBoard[newRow][newCol] == nullptr)
//             {
//                 possible_move.push_back({newRow, newCol});
//             }
//             else if(m_ChessBoard->m_pieceOnBoard[newRow][newCol]->getColor() != m_color)
//             {
//                 possible_move.push_back({newRow, newCol});
//             }
//         }
//     }
//     if(!possible_move.empty())
//     {
//         m_possibleMove.push_back({prt, possible_move});
//     }
// }


// void Artificial_Intelligence::slot_MoveBot()
// {
//     // Запускаем функцию, которая найдет все фигуры, за которые можно походить, и добавит их в массив
//     // Для дальнейшего анализа
//     setPossiblelMoveInPair();
//     if(!m_possibleMove.empty())
//     {
//         std::pair<ChessPiece*, std::pair<int, int>> move = choiceMoveTest();
//         m_possibleMove.clear();
//         emit signal_BotChoseMove(move.first, move.second);
//     }
//     else
//     {
//         QMessageBox::critical(nullptr, "Error", "m_possibleMove.empty() == true");
//         QApplication::quit();
//     }
// }


// std::pair<ChessPiece*, std::pair<int, int>> Artificial_Intelligence::choiceMove()
// {
//     int size_possibleMove = m_possibleMove.size();
//     int randomValue = QRandomGenerator::global()->bounded(0, size_possibleMove);
//     return std::pair<ChessPiece*, std::pair<int, int>> {m_possibleMove[randomValue].first, m_possibleMove[randomValue].second[0]};
// }



// void Artificial_Intelligence::test_m_possibleMove()
// {
//     for (int i = 0; i < m_possibleMove.size(); i++)
//     {
//         qDebug() << "----------------------------------";
//         qDebug() << (int)m_possibleMove[i].first->pos().y() / m_ChessBoard->m_square_Size <<(int) m_possibleMove[i].first->pos().x() / m_ChessBoard->m_square_Size;
//         qDebug() << "--";
//         for (int j = 0; j < m_possibleMove[i].second.size(); j++)
//         {
//             qDebug() << m_possibleMove[i].second[j].first << m_possibleMove[i].second[j].second;
//         }
//         qDebug() << "----------------------------------";
//     }
// }

// std::pair<ChessPiece*, std::pair<int, int>> Artificial_Intelligence::choiceMoveTest()
// {
//     int bestValue = INT_MIN;
//     std::pair<ChessPiece*, std::pair<int, int>> best_move;

//     for(int i = 0; i < m_possibleMove.size(); i++)
//     {
//         int tempValue = INT_MIN;
//         for(int j = 0; j < m_possibleMove[i].second.size(); j++)
//         {
//             ChessPiece* tempPiece = m_possibleMove[i].first;
//             int pieceType = tempPiece->getPiece();
//             switch (pieceType)
//             {
//             //QVector<std::pair<ChessPiece*, QVector<std::pair<int, int>>>> m_possibleMove; // Вектор всех фигур и их возможных ходов
//             //enum pieceValue{none = 0, king = 900, queen = 90, rook = 50, elephant = 30, horse = 30, pawn = 10}; // Перечисление очков фигур для оценивания хода
//             case 1: tempValue = evaluateMoveKing(tempPiece, m_possibleMove[i].second[j]); break;
//             case 2: tempValue = evaluateMoveQueen(tempPiece, m_possibleMove[i].second[j]); break;;break;
//             case 3: tempValue = evaluateMoveRook(tempPiece, m_possibleMove[i].second[j]); break;;break;
//             case 4: tempValue = evaluateMoveElephant(tempPiece, m_possibleMove[i].second[j]); break;;break;
//             case 5: tempValue = evaluateMoveHorse(tempPiece, m_possibleMove[i].second[j]); break;;break;
//             case 6: tempValue = evaluateMovePawn(tempPiece, m_possibleMove[i].second[j]); break;;break;

//             default:
//                 break;
//             }
//             if(tempValue > bestValue)
//             {
//                 bestValue = tempValue;
//                 best_move = {tempPiece, m_possibleMove[i].second[j]};
//             }
//         }
//     }

//     return best_move;
// }


// int Artificial_Intelligence::evaluateMovePawn(ChessPiece* piece, std::pair<int, int> best_move)
// {
//     // Оценка хода для пешки
//     int grade = 0;
//     int row = best_move.first;
//     int col = best_move.second;
//     //bool color = piece->getColor();

//     if(m_ChessBoard->m_pieceOnBoard[row][col] != nullptr)
//     {
//         ChessPiece* enemyPiece = m_ChessBoard->m_pieceOnBoard[row][col];
//         grade = evaluatePiece(enemyPiece);
//     }
//     return grade;
// }

// int Artificial_Intelligence::evaluateMoveHorse(ChessPiece* piece, std::pair<int, int> best_move)
// {
//     // Оценка хода для Коня
//     int grade = 0;
//     int row = best_move.first;
//     int col = best_move.second;
//     //bool color = piece->getColor();

//     if(m_ChessBoard->m_pieceOnBoard[row][col] != nullptr)
//     {
//         ChessPiece* enemyPiece = m_ChessBoard->m_pieceOnBoard[row][col];
//         grade = evaluatePiece(enemyPiece);
//     }
//     return grade;
// }

// int Artificial_Intelligence::evaluateMoveElephant(ChessPiece* piece, std::pair<int, int> best_move)
// {
//     // Оценка хода для Слона
//     int grade = 0;
//     int row = best_move.first;
//     int col = best_move.second;
//     //bool color = piece->getColor();

//     if(m_ChessBoard->m_pieceOnBoard[row][col] != nullptr)
//     {
//         ChessPiece* enemyPiece = m_ChessBoard->m_pieceOnBoard[row][col];
//         grade = evaluatePiece(enemyPiece);
//     }
//     return grade;
// }

// int Artificial_Intelligence::evaluateMoveRook(ChessPiece* piece, std::pair<int, int> best_move)
// {
//     // Оценка хода для ладьи
//     int grade = 0;
//     int row = best_move.first;
//     int col = best_move.second;
//     //bool color = piece->getColor();

//     if(m_ChessBoard->m_pieceOnBoard[row][col] != nullptr)
//     {
//         ChessPiece* enemyPiece = m_ChessBoard->m_pieceOnBoard[row][col];
//         grade = evaluatePiece(enemyPiece);
//     }
//     return grade;
// }

// int Artificial_Intelligence::evaluateMoveQueen(ChessPiece* piece, std::pair<int, int> best_move)
// {
//     // Оценка хода для ферзя
//     int grade = 0;
//     int row = best_move.first;
//     int col = best_move.second;
//     //bool color = piece->getColor();

//     if(m_ChessBoard->m_pieceOnBoard[row][col] != nullptr)
//     {
//         ChessPiece* enemyPiece = m_ChessBoard->m_pieceOnBoard[row][col];
//         grade = evaluatePiece(enemyPiece);
//     }
//     return grade;
// }

// int Artificial_Intelligence::evaluateMoveKing(ChessPiece* piece, std::pair<int, int> best_move)
// {
//     // Оценка хода для короля
//     int grade = 0;
//     int row = best_move.first;
//     int col = best_move.second;
//     //bool color = piece->getColor();

//     if(m_ChessBoard->m_pieceOnBoard[row][col] != nullptr)
//     {
//         ChessPiece* enemyPiece = m_ChessBoard->m_pieceOnBoard[row][col];
//         grade = evaluatePiece(enemyPiece);

//     }
//     return grade;
// }

// //enum pieceValue{none = 0, king = 900, queen = 90, rook = 50, elephant = 30, horse = 30, pawn = 10}; // Перечисление очков фигур для оценивания хода

// int Artificial_Intelligence::evaluatePiece(ChessPiece* piece)
// {
//     int evaluate = 0;
//     switch (piece->getPiece())
//     {
//     case 1: evaluate = 900;break;
//     case 2: evaluate = 90;break;
//     case 3: evaluate = 50;break;
//     case 4: evaluate = 30;break;
//     case 5: evaluate = 30;break;
//     case 6: evaluate = 10;break;

//     default:
//         break;
//     }
//     return evaluate;
// }



// MinMax::MinMax(bool whoMove, int depth, QVector<QVector<ChessPiece *> > pieceOnBoard)
// {
//     if(m_pieceOnBoard.size() != pieceOnBoard.size() || m_pieceOnBoard[0].size() != pieceOnBoard[0].size())
//     {
//         QMessageBox::critical(nullptr, "Error", "m_pieceOnBoard.size() != pieceOnBoard.size() in MinMax");
//         QApplication::quit();
//     }
//     for(int i = 0; i < pieceOnBoard.size(); i++)
//     {
//         for(int j = 0; j < pieceOnBoard[i].size(); j++)
//         {
//             if( pieceOnBoard[i][j] != nullptr)
//             {
//                 m_pieceOnBoard[i][j] = new ChessPiece(*pieceOnBoard[i][j]);
//                 m_pieceOnBoard[i][j] = pieceOnBoard[i][j];
//             }
//             else m_pieceOnBoard[i][j] = nullptr;
//         }
//     }
// }

// MinMax::~MinMax()
// {
//     for(int i = 0; i < m_pieceOnBoard.size(); i++)
//     {
//         for(int j = 0; j < m_pieceOnBoard[i].size(); j++)
//         {
//             if(m_pieceOnBoard[i][j] != nullptr)
//             {
//                 delete m_pieceOnBoard[i][j];
//             }
//         }
//     }
// }

