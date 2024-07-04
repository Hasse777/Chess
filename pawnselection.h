#ifndef PAWNSELECTION_H
#define PAWNSELECTION_H
#include <QDialog>

class PawnSelection : public QDialog
{
    Q_OBJECT
    bool color;
public:
    PawnSelection(bool color, QWidget* parent = nullptr);

private slots:
    void slot_clickedPawn();
    void slot_clickedQueen();
    void slot_clickedRook();
    void slot_clickedElephant();
    void slot_clickedHorse();

signals:
    void signals_pieceSelected(int pieceNimber);
};

#endif // PAWNSELECTION_H
