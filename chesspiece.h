#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>

// class ChessPiece : public QObject, public QGraphicsPixmapItem
// {
//     Q_OBJECT
// private:
//     bool m_color;
//     bool m_first_move;
//     unsigned short m_piece;
// public:
//     explicit ChessPiece(bool color, unsigned short piece, const int squareSize);
//     ChessPiece(const ChessPiece& other); // Конструктор копирования
//     ChessPiece& operator=(const ChessPiece& other);
//     bool getColor() const;
//     bool getFirst_Move() const;
//     void setFirst_MoveFalse();
//     unsigned short getPiece() const;
// protected:
//     void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
// signals:
//     void signal_mousePressEvent(ChessPiece* piece);
//     void signal_mouseReleaseEvent(ChessPiece* piece, QPointF position);
// public slots:
//     void slot_PieceSelection(int pieceType);
// };


class ChessPiece : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
private:
    bool m_color;
    unsigned short m_piece;
    std::pair<int, int> m_posXY; // позиция фигуры
public:
    explicit ChessPiece(bool color, unsigned short piece, std::pair<int, int> posXY, const int squareSize);
    virtual ~ChessPiece();
    bool getColor() const; // Возвращает цвет фигуры 0 - белый, 1 - черный
    virtual bool getFirstMove() const; // Возвращает 0 - если фигура еще не ходила
    virtual void setFirstMove();
    unsigned short getPiece() const; // Возвращает тип фигуры от 0 до 6
    void setPos(std::pair<int, int> posXY); // Устанавливает позицию фигуры
    std::pair<int, int> getPos() const; // Возвращает позицию фигуры
    ChessPiece& operator=(const ChessPiece& other) = delete;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
signals:
    void signal_mousePressEvent(ChessPiece* piece);
    void signal_mouseReleaseEvent(ChessPiece* piece, QPointF position);
public slots:
    void slot_PieceSelection(int pieceType);
};

//enum m_piece {none, king, queen, rook, elephant, horse, pawn}; // Перечисление всех фигур
class KingPiece : public ChessPiece
{
private:
    bool m_shah; // Находится ли король под шахом
    bool m_firstMove; // Ходила ли фигура, 0 - еще нет, 1 - да
public:
    KingPiece(bool color, unsigned short piece, std::pair<int, int> posXY, const int squareSize);
    bool getFirstMove() const override;
    void setFirstMove() override; // Автоматически ставит поле m_firstMove в значение True
    ~KingPiece();
};



class QueenPiece : public ChessPiece
{
private:

public:
    QueenPiece(bool color, unsigned short piece, std::pair<int, int> posXY, const int squareSize);
    ~QueenPiece();
};


class RookPiece : public ChessPiece
{
private:
    bool m_firstMove; // Ходила ли фигура, 0 - еще нет, 1 - да
public:
    RookPiece(bool color, unsigned short piece, std::pair<int, int> posXY, const int squareSize);
    bool getFirstMove() const override;
    void setFirstMove() override;
    ~RookPiece();
};


class ElephantPiece : public ChessPiece
{
private:

public:
    ElephantPiece(bool color, unsigned short piece, std::pair<int, int> posXY, const int squareSize);
    ~ElephantPiece();
};


class HorsePiece : public ChessPiece
{
private:

public:
    HorsePiece(bool color, unsigned short piece, std::pair<int, int> posXY, const int squareSize);
    ~HorsePiece();
};


class PawnPiece : public ChessPiece
{
private:
    bool m_firstMove;
public:
    PawnPiece(bool color, unsigned short piece, std::pair<int, int> posXY, const int squareSize);
    bool getFirstMove() const override;
    void setFirstMove() override;
    ~PawnPiece();
};






#endif // CHESSPIECE_H
