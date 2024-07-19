#include "mainmenudialog.h"
#include "QLayout"
#include <QApplication>

MainMenuDialog::MainMenuDialog(QWidget *parent) : QDialog{parent}
{
    setFixedSize(400, 200);
    setStyleSheet("background-color: #F5F5DC; border: 2px solid #383636;");

    // Выключаем стандартную рамку окна и задаём флаг диалогового окна
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    // Создаём кнопки
    QPushButton* newGame = new QPushButton("New game", this);
    // QPushButton* settings = new QPushButton("Setting", this);
    // settings->setEnabled(false);
    QPushButton* exit = new QPushButton("Exit out game", this);

    // Вызываем дизайнер кнопок
    button_design(newGame);
    //button_design(settings);
    button_design(exit);

    QVBoxLayout * lay = new QVBoxLayout(this);
    lay->addWidget(newGame);
    //lay->addWidget(settings);
    lay->addWidget(exit);

    // Соединяем кнопки со слотами
    connect(newGame, &QPushButton::clicked, this, &MainMenuDialog::slot_newGame);
    connect(exit, &QPushButton::clicked, this, &MainMenuDialog::slot_exit);


    setLayout(lay);
}

void MainMenuDialog::slot_newGame()
{
    reject();
    emit signal_newGame();
}

void MainMenuDialog::slot_exit()
{
    QApplication::quit();
}


void MainMenuDialog::button_design(QPushButton *button)
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
