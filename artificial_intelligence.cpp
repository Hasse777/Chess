#include "artificial_intelligence.h"
#include "chessboard.h"
#include <QMessageBox>




Artificial_Intelligence::Artificial_Intelligence(const ChessBoard * const board, bool color) : m_ChessBoard{board}, m_color{color}
{

}


void Artificial_Intelligence::setPossiblelMoveInPair()
{
    if(m_ChessBoard->m_pieceOnBoard.empty() || m_ChessBoard->m_pieceOnBoard[0].empty())
    {
        QMessageBox::critical(nullptr, "Error", "Null array passed to AI");
        return;
    }
    int row = m_ChessBoard->m_pieceOnBoard.size();
    int col = m_ChessBoard->m_pieceOnBoard[0].size();
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            ChessPiece* prt = m_ChessBoard->m_pieceOnBoard[row][col];
            if(prt != nullptr && prt->getColor() == m_color)
            {
                distributionByChessPiece(prt);
            }
        }
    }
}

void Artificial_Intelligence::distributionByChessPiece(ChessPiece * prt)
{

    enum m_pieceType PieceType = static_cast<m_pieceType>(prt->getPiece());
    switch (PieceType)
    {
    case pawn: countingPossibleMovesPawn(prt); break;

    default:
        break;
    }
}

void Artificial_Intelligence::countingPossibleMovesPawn(ChessPiece *prt)
{

}

void Artificial_Intelligence::countingPossibleMovesHorse(ChessPiece *)
{

}

void Artificial_Intelligence::countingPossibleMovesElephant(ChessPiece *)
{

}

void Artificial_Intelligence::countingPossibleMovesRook(ChessPiece *)
{

}

void Artificial_Intelligence::countingPossibleMovesQueen(ChessPiece *)
{

}

void Artificial_Intelligence::countingPossibleMovesKing(ChessPiece *)
{

}




















