#ifndef MAINMENUDIALOG_H
#define MAINMENUDIALOG_H

#include "QDialog"
#include <QPushButton>


class MainMenuDialog : public QDialog
{
    Q_OBJECT
private:
    void button_design(QPushButton* button);
public:
    MainMenuDialog(QWidget *parent = nullptr);
signals:
    void signal_newGame();
private slots:
    void slot_newGame();
    void slot_exit();
};

#endif // MAINMENUDIALOG_H
