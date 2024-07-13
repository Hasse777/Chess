#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QLayout>
#include <QKeyEvent>
#include "chessboard.h"
#include "playerwidget.h"
#include "moveswidget.h"
#include "eatenpieces.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QWidget* m_central_widget;
    QGridLayout * m_gridLayout;
    ChessBoard* m_chessBoard;
    PlayerWidget* m_playerWidget;
    MovesWidget* m_moveWidget;
    EatenPieces* m_eatenPieces;

protected:
    void keyPressEvent(QKeyEvent *event) override;

public:
    explicit MainWindow(QWidget* parent = nullptr);
    void newGame();
signals:
    void signal_newGame();
};



#endif // MAINWINDOW_H
