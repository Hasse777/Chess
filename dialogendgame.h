#ifndef DIALOGENDGAME_H
#define DIALOGENDGAME_H

#include <QDialog>
#include "QObject"

class DialogEndGame : public QDialog
{
    Q_OBJECT
private:
    void button_design(QPushButton* button);
public:
    DialogEndGame(bool colorWin, QWidget* parent = nullptr);
private slots:
    void slot_newGame();
    void slot_exit();
};

#endif // DIALOGENDGAME_H
