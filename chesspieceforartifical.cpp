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
    KingPieceForArtifical* piece = new KingPieceForArtifical(this->m_color, this->m_piece, this->m_posXY, this->m_firstMove);
    piece->m_shah = this->m_shah;
    return piece;
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
    QueenPieceForArtifical* piece = new QueenPieceForArtifical(this->m_color, this->m_piece, this->getPos());
    return piece;
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
    RookPieceForArtifical* piece = new RookPieceForArtifical(this->m_color, this->m_piece, this->m_posXY, this->m_firstMove);
    return piece;
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
    ElephantPieceForArtifical* piece = new ElephantPieceForArtifical(this->m_color, this->m_piece, this->m_posXY);
    return piece;
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
    HorsePieceForArtifical* piece = new HorsePieceForArtifical(this->m_color, this->m_piece, this->m_posXY);
    return piece;
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
    PawnPieceForArtifical* piece = new PawnPieceForArtifical(this->m_color, this->m_piece, this->m_posXY, this->m_firstMove);
    return piece;
}

PawnPieceForArtifical::~PawnPieceForArtifical()
{

}
