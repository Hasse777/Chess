#include "moveswidget.h"

MovesWidget::MovesWidget(QWidget* parent) : QTextEdit(parent)
{
    setFixedSize(350, 595);
    setAlignment(Qt::AlignTop);
    setStyleSheet("border: 3px solid black;");

    //Устанавливаем курсор в начало
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::Start);

    //Выравниваем заголовок по центру
    QTextBlockFormat block_Format;
    block_Format.setAlignment(Qt::AlignCenter);
    cursor.setBlockFormat(block_Format);

    //Устанавливаем шрифт заголовка
    QTextCharFormat header_Format;
    QFont header_font("Arial", 14, QFont::Bold);
    header_Format.setFont(header_font);
    cursor.setCharFormat(header_Format);
    cursor.insertText("All moves:\n\n");



}

void MovesWidget::slot_addText(const QString text)
{
    QTextCursor cursor = textCursor();

    //Устанавливаем шрифт сделанных ходов
    QTextCharFormat text_Format;
    QFont text_Font("Courier", 12);
    text_Format.setFont(text_Font);
    cursor.setCharFormat(text_Format);

    //Выравниваем по левому краю
    QTextBlockFormat block_Format;
    block_Format.setAlignment(Qt::AlignLeft);
    cursor.setBlockFormat(block_Format);

    cursor.setCharFormat(text_Format);
    cursor.insertText(text + "\n");

}

