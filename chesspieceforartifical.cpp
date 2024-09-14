#include "chesspieceforartifical.h"




ChessPieceForArtifical::ChessPieceForArtifical(bool color, unsigned short piece, std::pair<int, int> posXY) :
    m_color{color}, m_piece{piece}, m_posXY{posXY}
{

}

ChessPieceForArtifical::~ChessPieceForArtifical()
{

}

bool ChessPieceForArtifical::getColor() const
{
    return m_color;
}

bool ChessPieceForArtifical::getFirstMove() const
{
    return true;
}

void ChessPieceForArtifical::setFirstMove()
{

}

unsigned short ChessPieceForArtifical::getPiece() const
{
    return m_piece;
}

void ChessPieceForArtifical::setPos(std::pair<int, int> posXY)
{
    m_posXY.first = posXY.first;
    m_posXY.second = posXY.second;
}

std::pair<int, int> ChessPieceForArtifical::getPos() const
{
    return m_posXY;
}

KingPieceForArtifical::KingPieceForArtifical(bool color, unsigned short piece, std::pair<int, int> posXY, bool firstMove) :
    ChessPieceForArtifical(color, piece, posXY)
{
    m_firstMove = firstMove;
}

KingPieceForArtifical* KingPieceForArtifical::clone()
{
    return new KingPieceForArtifical(*this);
}

bool KingPieceForArtifical::getFirstMove() const
{
    return m_firstMove;
}

bool KingPieceForArtifical::getShah() const
{
    return m_shah;
}

void KingPieceForArtifical::setFirstMove()
{
    m_firstMove = true;
}

void KingPieceForArtifical::setShah(bool shah)
{
    m_shah = shah;
}

KingPieceForArtifical::~KingPieceForArtifical()
{

}

QueenPieceForArtifical::QueenPieceForArtifical(bool color, unsigned short piece, std::pair<int, int> posXY) :
ChessPieceForArtifical(color, piece, posXY)
{

}

QueenPieceForArtifical *QueenPieceForArtifical::clone()
{
    return new QueenPieceForArtifical(*this);
}

QueenPieceForArtifical::~QueenPieceForArtifical()
{

}

RookPieceForArtifical::RookPieceForArtifical(bool color, unsigned short piece, std::pair<int, int> posXY, bool firstMove) :
ChessPieceForArtifical(color, piece, posXY)
{
    m_firstMove = firstMove;
}

bool RookPieceForArtifical::getFirstMove() const
{
    return m_firstMove;
}

RookPieceForArtifical *RookPieceForArtifical::clone()
{
    return new RookPieceForArtifical(*this);
}

void RookPieceForArtifical::setFirstMove()
{
    m_firstMove = true;
}

RookPieceForArtifical::~RookPieceForArtifical()
{

}

ElephantPieceForArtifical::ElephantPieceForArtifical(bool color, unsigned short piece, std::pair<int, int> posXY) :
ChessPieceForArtifical(color, piece, posXY)
{

}

ElephantPieceForArtifical *ElephantPieceForArtifical::clone()
{
    return new ElephantPieceForArtifical(*this);
}

ElephantPieceForArtifical::~ElephantPieceForArtifical()
{

}

HorsePieceForArtifical::HorsePieceForArtifical(bool color, unsigned short piece, std::pair<int, int> posXY) :
ChessPieceForArtifical(color, piece, posXY)
{

}

HorsePieceForArtifical *HorsePieceForArtifical::clone()
{
    return new HorsePieceForArtifical(*this);
}

HorsePieceForArtifical::~HorsePieceForArtifical()
{

}

PawnPieceForArtifical::PawnPieceForArtifical(bool color, unsigned short piece, std::pair<int, int> posXY, bool firstMove) :
ChessPieceForArtifical(color, piece, posXY)
{
    m_firstMove = firstMove;
}

bool PawnPieceForArtifical::getFirstMove() const
{
    return m_firstMove;
}

void PawnPieceForArtifical::setFirstMove()
{
    m_firstMove = true;
}

PawnPieceForArtifical *PawnPieceForArtifical::clone()
{
    return new PawnPieceForArtifical(*this);
}

PawnPieceForArtifical::~PawnPieceForArtifical()
{

}
