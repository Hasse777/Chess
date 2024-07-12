#ifndef MOVESWIDGET_H
#define MOVESWIDGET_H
#include <QTextEdit>

class MovesWidget : public QTextEdit
{
private:
    void set_Header();

public:
    MovesWidget(QWidget* parent = nullptr);

public slots:
    void slot_addText(const QString text);
    void slot_clearText();
};

#endif // MOVESWIDGET_H
