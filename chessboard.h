#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QPainter>

class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* p) override;

signals:
};

#endif // CHESSBOARD_H
