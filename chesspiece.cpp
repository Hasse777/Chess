#include "chesspiece.h"
#include <QMessageBox>




// ChessPiece::ChessPiece(bool color, unsigned short piece, const int squareSize) : m_color{color},
//     m_first_move{true}, m_piece{piece}
// {
//     QString colorStr = color ? "Black" : "White";
//     QString pieceName;

//     switch(m_piece)
//     {
//     case 1 : pieceName = "king"; break;
//     case 2 : pieceName = "queen"; break;
//     case 3 : pieceName = "rook"; break;
//     case 4 : pieceName = "elephant"; break;
//     case 5 : pieceName = "horse"; break;
//     case 6 : pieceName = "pawn"; break;
//     default: return;
//     }
//     QString piecePath = QString(":/figures_img/%1_%2.png").arg(colorStr, pieceName);

//     QPixmap pixmap(piecePath);
//     if (pixmap.isNull())
//     {
//         QMessageBox::critical(nullptr, "Error", "Error loading image of figure in class \"ChessPiece\"");
//     }

//     //QPixmap scaledPixmap = pixmap.scaled(squareSize, squareSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//     setPixmap(pixmap);
// }


// ChessPiece::ChessPiece(const ChessPiece &other)
// {
//     m_color = other.m_color;
//     m_first_move = other.m_first_move;
//     m_piece = other.m_piece;
// }

// ChessPiece &ChessPiece::operator=(const ChessPiece &other)
// {
//     if (this == &other)
//     {
//         return *this;
//     }
//     m_color = other.m_color;
//     m_first_move = other.m_first_move;
//     m_piece = other.m_piece;
//     return *this;
// }

// bool ChessPiece::getColor() const
// {
//     return m_color;
// }

// bool ChessPiece::getFirst_Move() const
// {
//     return m_first_move;
// }

// void ChessPiece::setFirst_MoveFalse()
// {
//     this->m_first_move = false;
// }

// unsigned short ChessPiece::getPiece() const
// {
//     return m_piece;
// }

// void ChessPiece::mousePressEvent(QGraphicsSceneMouseEvent *event)
// {
//     emit signal_mousePressEvent(this);
//     //QGraphicsPixmapItem::mousePressEvent(event);
// }

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
    m_firstMove = 0;
    m_shah = 0;
}

bool KingPiece::getFirstMove() const
{
    return m_firstMove;
}

void KingPiece::setFirstMove()
{
    m_firstMove = 1;
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
    m_firstMove = 0;
}

bool RookPiece::getFirstMove() const
{
    return m_firstMove;
}

void RookPiece::setFirstMove()
{
    m_firstMove = 1;
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
    m_firstMove = 0;
}

bool PawnPiece::getFirstMove() const
{
    return m_firstMove;
}

void PawnPiece::setFirstMove()
{
    m_firstMove = 1;
}

PawnPiece::~PawnPiece()
{

}
