#ifndef MOVESWIDGET_H
#define MOVESWIDGET_H
#include <QTextEdit>

class MovesWidget : public QTextEdit
{
private:

public:
    MovesWidget(QWidget* parent = nullptr);

public slots:
    void slot_addText(const QString text);
};

#endif // MOVESWIDGET_H
