#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QLayout>
#include "chessboard.h"


class MainWindow : public QMainWindow
{
private:
    QWidget* m_central_widget;
    QGridLayout * m_gridLayout;
    ChessBoard* m_chessBoard;
public:
    explicit MainWindow(QWidget* parent = nullptr);
};



#endif // MAINWINDOW_H
