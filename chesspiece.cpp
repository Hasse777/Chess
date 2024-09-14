#include "chesspiece.h"
#include <QMessageBox>



ChessPiece::ChessPiece(bool color, unsigned short piece, std::pair<int, int> posXY, const int squareSize)
{

    m_color = color;
    m_piece = piece;
    m_posXY = posXY;


    QString colorStr = color ? "Black" : "White";
    QString pieceName;
    switch(m_piece)
    {
    case 1 : pieceName = "king"; break;
    case 2 : pieceName = "queen"; break;
    case 3 : pieceName = "rook"; break;
    case 4 : pieceName = "elephant"; break;
    case 5 : pieceName = "horse"; break;
    case 6 : pieceName = "pawn"; break;
    default: return;
    }
    QString piecePath = QString(":/figures_img/%1_%2.png").arg(colorStr, pieceName);
    QPixmap pixmap(piecePath);
    if (pixmap.isNull())
    {
        QMessageBox::critical(nullptr, "Error", "Error loading image of figure in class \"ChessPiece\"");
    }
    //QPixmap scaledPixmap = pixmap.scaled(squareSize, squareSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(pixmap);
}

ChessPiece::~ChessPiece()
{

}

bool ChessPiece::getColor() const
{
    return m_color;
}

bool ChessPiece::getFirstMove() const
{
    return true;
}

void ChessPiece::setFirstMove()
{

}

unsigned short ChessPiece::getPiece() const
{
    return m_piece;
}

void ChessPiece::setPos(std::pair<int, int> posXY)
{
    m_posXY = posXY;
}

std::pair<int, int> ChessPiece::getPos() const
{
    return m_posXY;
}

void ChessPiece::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit signal_mousePressEvent(this);
    //QGraphicsPixmapItem::mousePressEvent(event);
}

void ChessPiece::slot_PieceSelection(int pieceType)
{
    QString colorStr = m_color ? "Black" : "White";
    QString pieceName;
    m_piece = pieceType;

    switch(m_piece)
    {
    case 1 : pieceName = "king"; break;
    case 2 : pieceName = "queen"; break;
    case 3 : pieceName = "rook"; break;
    case 4 : pieceName = "elephant"; break;
    case 5 : pieceName = "horse"; break;
    case 6 : pieceName = "pawn"; break;
    default: return;
    }
    QString piecePath = QString(":/figures_img/%1_%2.png").arg(colorStr, pieceName);

    QPixmap pixmap(piecePath);
    if (pixmap.isNull())
    {
        throw std::runtime_error(QString("Failed to load image: %1").arg(piecePath).toStdString());
    }

    //QPixmap scaledPixmap = pixmap.scaled(squareSize, squareSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(pixmap);
}


KingPiece::KingPiece(bool color, unsigned short piece, std::pair<int, int> posXY, const int squareSize) : ChessPiece(color, piece, posXY, squareSize)
{
    m_firstMove = 1;
    m_shah = 0;
}


bool KingPiece::getFirstMove() const
{
    return m_firstMove;
}

bool KingPiece::getShah() const
{
    return m_shah;
}

void KingPiece::setFirstMove()
{
    m_firstMove = 0;
}

void KingPiece::setShah(bool shah)
{
    m_shah = shah;
}

KingPiece::~KingPiece()
{

}

QueenPiece::QueenPiece(bool color, unsigned short piece, std::pair<int, int> posXY, const int squareSize) : ChessPiece(color, piece, posXY, squareSize)
{

}

QueenPiece::~QueenPiece()
{

}

RookPiece::RookPiece(bool color, unsigned short piece, std::pair<int, int> posXY, const int squareSize) : ChessPiece(color, piece, posXY, squareSize)
{
    m_firstMove = 1;
}

bool RookPiece::getFirstMove() const
{
    return m_firstMove;
}

void RookPiece::setFirstMove()
{
    m_firstMove = 0;
}

RookPiece::~RookPiece()
{

}

ElephantPiece::ElephantPiece(bool color, unsigned short piece, std::pair<int, int> posXY, const int squareSize) : ChessPiece(color, piece, posXY, squareSize)
{

}

ElephantPiece::~ElephantPiece()
{

}

HorsePiece::HorsePiece(bool color, unsigned short piece, std::pair<int, int> posXY, const int squareSize) : ChessPiece(color, piece, posXY, squareSize)
{

}

HorsePiece::~HorsePiece()
{

}

PawnPiece::PawnPiece(bool color, unsigned short piece, std::pair<int, int> posXY, const int squareSize) : ChessPiece(color, piece, posXY, squareSize)
{
    m_firstMove = 1;
}

bool PawnPiece::getFirstMove() const
{
    return m_firstMove;
}

void PawnPiece::setFirstMove()
{
    m_firstMove = 0;
}

PawnPiece::~PawnPiece()
{

}
