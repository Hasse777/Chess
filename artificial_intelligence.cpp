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
}

Artificial_Intelligence::~Artificial_Intelligence()
{
    freeMemory();
}

void Artificial_Intelligence::freeMemory()
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


    m_MinMax = new MinMax(0, m_color, 6, m_pieceOnBoard, m_whiteKing, m_blackKing, nullptr);
    freeMemory();
}



void MinMax::distributionByChessPiece()
{
    int row = m_pieceOnBoard.size();
    int col = m_pieceOnBoard[0].size();
    if(row != 8 || col != 8)
    {
        QMessageBox::critical(nullptr, "Error", "Row and col aren't == 8");
        QApplication::quit();
    }
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            ChessPieceForArtifical* piece = m_pieceOnBoard[i][j];
            if(piece != nullptr && piece->getColor() == m_whoMove)
            {
                switch (piece->getPiece())
                {
                case 1: countingPossibleMovesKing(piece); break;
                case 2: countingPossibleMovesQueen(piece); break;
                case 3: countingPossibleMovesRook(piece); break;
                case 4: countingPossibleMovesElephant(piece);break;
                case 5: countingPossibleMovesHorse(piece); break;
                case 6: countingPossibleMovesPawn(piece); break;
                default:
                    break;
                }
            }
        }
    }
}

void MinMax::countingPossibleMovesPawn(ChessPieceForArtifical *piece)
{
    int row = piece->getPos().first;
    int col = piece->getPos().second;
    bool direction = (m_botPlayForColor == m_whoMove ? 1 : -1);
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
            if(m_pieceOnBoard[newRow][col] == nullptr)
            {
                allPossibleMoves.push_back({piece, {newRow, col}});
            }
            else if(m_pieceOnBoard[newRow][col] != nullptr && m_pieceOnBoard[newRow][col]->getColor() != m_whoMove)
            {
                allPossibleMoves.push_back({piece, {newRow, col}});
            }
            else
            {

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
    if(newCol >= 0 && newCol < 8 && m_pieceOnBoard[newRow][newCol] != nullptr)
    {
        allPossibleMoves.push_back({piece, {newRow, newCol}});
    }
    newCol = col - 1;
    if(newCol >= 0 && newCol < 8 && m_pieceOnBoard[newRow][newCol] != nullptr)
    {
        allPossibleMoves.push_back({piece, {newRow, newCol}});
    }
}

void MinMax::countingPossibleMovesHorse(ChessPieceForArtifical *piece)
{
    int row = piece->getPos().first;
    int col = piece->getPos().second;
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
            if(m_pieceOnBoard[i][j] == nullptr || (m_pieceOnBoard[i][j] != nullptr && m_pieceOnBoard[i][j]->getColor() != m_whoMove))
            {
                allPossibleMoves.push_back({piece, {i, j}});
            }
        }
        it++;
    }
}

void MinMax::countingPossibleMovesElephant(ChessPieceForArtifical *piece)
{
    int row = piece->getPos().first;
    int col = piece->getPos().second;
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
            if(m_pieceOnBoard[newRow][newCol] == nullptr)
            {
                allPossibleMoves.push_back({piece, {newRow, newCol}});
                continue;
            }
            else if(m_pieceOnBoard[newRow][newCol]->getColor() != m_whoMove)
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

void MinMax::countingPossibleMovesRook(ChessPieceForArtifical *piece)
{
    int row = piece->getPos().first;
    int col = piece->getPos().second;
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
            if(m_pieceOnBoard[newRow][newCol] == nullptr)
            {
                allPossibleMoves.push_back({piece, {newRow, newCol}});
                continue;
            }
            else if(m_pieceOnBoard[newRow][newCol]->getColor() != m_whoMove)
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

void MinMax::countingPossibleMovesQueen(ChessPieceForArtifical *piece)
{
    int row = piece->getPos().first;
    int col = piece->getPos().second;
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
            if(m_pieceOnBoard[newRow][newCol] == nullptr)
            {
                allPossibleMoves.push_back({piece, {newRow, newCol}});
                continue;
            }
            else if(m_pieceOnBoard[newRow][newCol]->getColor() != m_whoMove)
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

void MinMax::countingPossibleMovesKing(ChessPieceForArtifical *piece)
{
    int row = piece->getPos().first;
    int col = piece->getPos().second;
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
            if(m_pieceOnBoard[newRow][newCol] == nullptr || m_pieceOnBoard[newRow][newCol]->getColor() != m_whoMove)
            {
                allPossibleMoves.push_back({piece, {newRow, newCol}});

            }
        }
        newRow = row;
        newCol = col;
        it++;
    }
}

MinMax::MinMax(int m_score, bool whoMove, int depth, const QVector<QVector<ChessPieceForArtifical*>>& pieceOnBoard, ChessPieceForArtifical* whiteKingStatus, ChessPieceForArtifical* blackKingStatus,ChessPieceForArtifical* pawnStatus)
: m_depth{depth}, m_whoMove{whoMove}
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
                std::pair<int, int> pieceCoordinate = pieceOnBoard[i][j]->getPos();
                bool color = pieceOnBoard[i][j]->getColor();
                switch (pieceOnBoard[i][j]->getPiece())
                {
                case 1: m_pieceOnBoard[i][j] = new KingPieceForArtifical(color, pieceOnBoard[i][j]->getPiece(), pieceCoordinate, pieceOnBoard[i][j]->getFirstMove()) ;break;
                case 2: m_pieceOnBoard[i][j] = new QueenPieceForArtifical(color, pieceOnBoard[i][j]->getPiece(), pieceCoordinate) ;break;
                case 3: m_pieceOnBoard[i][j] = new RookPieceForArtifical(color, pieceOnBoard[i][j]->getPiece(), pieceCoordinate, pieceOnBoard[i][j]->getFirstMove()) ;break;
                case 4: m_pieceOnBoard[i][j] = new ElephantPieceForArtifical(color, pieceOnBoard[i][j]->getPiece(), pieceCoordinate);break;
                case 5: m_pieceOnBoard[i][j] = new HorsePieceForArtifical(color, pieceOnBoard[i][j]->getPiece(), pieceCoordinate) ;break;
                case 6: m_pieceOnBoard[i][j] = new PawnPieceForArtifical(color, pieceOnBoard[i][j]->getPiece(), pieceCoordinate, pieceOnBoard[i][j]->getFirstMove()) ;break;
                default:
                    break;
                }
            }
        }
    }
    //-----------------------------------------------------------------

    distributionByChessPiece();

    // for(int i = 0 ; i < allPossibleMoves.size(); i++)
    // {
    //     if(allPossibleMoves[i].first->getPiece() == 1)
    //     {
    //         qDebug() << allPossibleMoves[i].first->getPos();
    //         qDebug() << allPossibleMoves[i].first->getColor() << allPossibleMoves[i].second.first << allPossibleMoves[i].second.second;
    //     }
    // }
    // qDebug() << "-------------------------------";

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
            }
        }
    }
}
