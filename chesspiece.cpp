#include "chesspiece.h"




ChessPiece::ChessPiece(bool color, unsigned short piece, const int squareSize) : m_color{color},
    m_first_move{true}, m_piece{piece}
{
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
        throw std::runtime_error(QString("Failed to load image: %1").arg(piecePath).toStdString());
    }

    //QPixmap scaledPixmap = pixmap.scaled(squareSize, squareSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(pixmap);
}

bool ChessPiece::getColor() const
{
    return m_color;
}

bool ChessPiece::getFirst_Move() const
{
    return m_first_move;
}

void ChessPiece::setFirst_MoveFalse()
{
    this->m_first_move = false;
}

unsigned short ChessPiece::getPiece() const
{
    return m_piece;
}

void ChessPiece::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit signal_mousePressEvent(this);
    //QGraphicsPixmapItem::mousePressEvent(event);
}

void ChessPiece::slots_PieceSelection(int pieceType)
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

