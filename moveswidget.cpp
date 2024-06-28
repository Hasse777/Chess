#include "moveswidget.h"

MovesWidget::MovesWidget(QWidget* parrent) : QLabel(parrent)
{
    setFixedSize(350, 595);
    setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    setStyleSheet("border: 2px solid black;");
    QFont font("Arial", 14, QFont::Bold);
    setFont(font);
    setText("All moves:");

}

