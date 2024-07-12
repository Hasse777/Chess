#include "dialogendgame.h"
#include <QApplication>
#include "QLayout"
#include "QLabel"
#include "QPushButton"


DialogEndGame::DialogEndGame(bool colorWin, QWidget *parent) : QDialog(parent)
{
    setFixedSize(400, 150);
    setStyleSheet("background-color: #F5F5DC; border: 2px solid #383636;");

    // Выключаем стандартную рамку окна и задаём флаг диалогового окна
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    QGridLayout *lay = new QGridLayout(this);
    QLabel* text = new QLabel(this);
    QPushButton* newGame = new QPushButton("New game", this);
    QPushButton* exit = new QPushButton("Exit out game", this);

    // Редактируем дизайн заголовка
    text->setFont(QFont("Arial", 16, QFont::Bold));
    text->setStyleSheet("border: none;");

    // Вызываем функцию дизайна кнопок
    button_design(newGame);
    button_design(exit);
    // newGame->setFont(QFont("Arial", 11));
    // exit->setFont(QFont("Arial", 11));

    if(!colorWin)
    text->setText("White win");
    else
    text->setText("Black win");

    lay->addWidget(text, 0, 0, 1, 2, Qt::AlignCenter);
    lay->addWidget(newGame, 1, 0);
    lay->addWidget(exit, 1, 1);

    //Соединяем кнопу выхода со слотом
    connect(newGame, &QPushButton::clicked, this, &DialogEndGame::slot_newGame);
    connect(exit, &QPushButton::clicked, this, &DialogEndGame::slot_exit);

    this->setLayout(lay);
}

void DialogEndGame::button_design(QPushButton* button)
{
    if(button != nullptr)
    {
        button->setStyleSheet(
            "QPushButton {"
            "background-color: #333333;"  // Темно-серый фон
            "border: 2px solid #555555;"  // Серебристая рамка
            "color: #FFFFFF;"  // Белый цвет текста
            "padding: 10px 20px;"  // Внутренние отступы
            "text-align: center;"  // Выравнивание текста по центру
            "font-size: 18px;"  // Размер шрифта
            "font-weight: bold;"  // Жирный шрифт
            "border-radius: 10px;"  // Скругленные углы
            "}"
            "QPushButton:hover {"
            "background-color: #444444;"  // Светло-серый фон при наведении
            "border: 2px solid #666666;"  // Светло-серая рамка при наведении
            "}"
            "QPushButton:pressed {"
            "background-color: #222222;"  // Очень темный фон при нажатии
            "border: 2px solid #444444;"  // Темно-серая рамка при нажатии
            "}"
            "QPushButton:disabled {"
            "background-color: #555555;"  // Темно-серый фон для отключенной кнопки
            "color: #AAAAAA;"  // Светло-серый цвет текста для отключенной кнопки
            "border: 2px solid #777777;"  // Серебристая рамка для отключенной кнопки
            "}"
            );
    }
}

void DialogEndGame::slot_newGame()
{
    reject();
}

void DialogEndGame::slot_exit()
{
    QApplication::quit();
}
