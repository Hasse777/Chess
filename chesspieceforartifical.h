#ifndef CHESSPIECEFORARTIFICAL_H
#define CHESSPIECEFORARTIFICAL_H
#include <QPair>

class ChessPieceForArtifical
{
private:
    bool m_color;
    unsigned short m_piece;
    std::pair<int, int> m_posXY; // позиция фигуры
public:
    explicit ChessPieceForArtifical(bool color, unsigned short piece, std::pair<int, int> posXY);
    virtual ~ChessPieceForArtifical();
    virtual ChessPieceForArtifical* clone() = 0;
    bool getColor() const; // Возвращает цвет фигуры 0 - белый, 1 - черный
    virtual bool getFirstMove() const; // Возвращает 0 - если фигура еще не ходила
    virtual void setFirstMove();
    unsigned short getPiece() const; // Возвращает тип фигуры от 0 до 6
    void setPos(std::pair<int, int> posXY); // Устанавливает позицию фигуры
    std::pair<int, int> getPos() const; // Возвращает позицию фигуры
};

//enum m_piece {none, king, queen, rook, elephant, horse, pawn}; // Перечисление всех фигур
class KingPieceForArtifical : public ChessPieceForArtifical
{
private:
    bool m_shah; // Находится ли король под шахом
    bool m_firstMove; // Ходила ли фигура, 0 - еще нет, 1 - да
public:
    KingPieceForArtifical(bool color, unsigned short piece, std::pair<int, int> posXY, bool firstMove);
    KingPieceForArtifical* clone() override;
    bool getFirstMove() const override;
    bool getShah() const;
    void setFirstMove() override; // Автоматически ставит поле m_firstMove в значение True
    void setShah(bool shah);
    ~KingPieceForArtifical() override;
};



class QueenPieceForArtifical : public ChessPieceForArtifical
{
private:

public:
    QueenPieceForArtifical(bool color, unsigned short piece, std::pair<int, int> posXY);
    QueenPieceForArtifical* clone() override;
    ~QueenPieceForArtifical() override;
};


class RookPieceForArtifical : public ChessPieceForArtifical
{
private:
    bool m_firstMove; // Ходила ли фигура, 0 - еще нет, 1 - да
public:
    RookPieceForArtifical(bool color, unsigned short piece, std::pair<int, int> posXY, bool firstMove);
    bool getFirstMove() const override;
    RookPieceForArtifical* clone() override;
    void setFirstMove() override;
    ~RookPieceForArtifical() override;
};


class ElephantPieceForArtifical : public ChessPieceForArtifical
{
private:

public:
    ElephantPieceForArtifical(bool color, unsigned short piece, std::pair<int, int> posXY);
    ElephantPieceForArtifical* clone() override;
    ~ElephantPieceForArtifical() override;
};


class HorsePieceForArtifical : public ChessPieceForArtifical
{
private:

public:
    HorsePieceForArtifical(bool color, unsigned short piece, std::pair<int, int> posXY);
    HorsePieceForArtifical* clone() override;
    ~HorsePieceForArtifical() override;
};


class PawnPieceForArtifical : public ChessPieceForArtifical
{
private:
    bool m_firstMove;
public:
    PawnPieceForArtifical(bool color, unsigned short piece, std::pair<int, int> posXY, bool firstMove);
    bool getFirstMove() const override;
    void setFirstMove() override;
    PawnPieceForArtifical* clone() override;
    ~PawnPieceForArtifical() override;
};

#endif // CHESSPIECEFORARTIFICAL_H
