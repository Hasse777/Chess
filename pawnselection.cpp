#include "pawnselection.h"
#include <QLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>

PawnSelection::PawnSelection(bool color, QWidget* parent) : QDialog{parent}, color{color}
{
    setModal(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog); // Отключение крестика
    setWindowIcon(QIcon());  // Удаление иконки окна

    QVBoxLayout* l_V = new QVBoxLayout();
    QHBoxLayout* l_H = new QHBoxLayout();

    // Создаем кнопки выбора
    QPushButton* pawn = new QPushButton(this);
    QPushButton* queen = new QPushButton(this);
    QPushButton* rook = new QPushButton(this);
    QPushButton* elephant = new QPushButton(this);
    QPushButton* horse = new QPushButton(this);
    QFont font("Arial", 14, QFont::Bold);
    QLabel *textLabel = new QLabel("Select a figurine:", this);
    textLabel->setFont(font);


    // Создаем иконки для кнопок
    QIcon pawnIcon;
    QIcon queenIcon;
    QIcon rookIcon;
    QIcon elephantIcon;
    QIcon horseIcon;

    // В зависимости от цвета пешки выбираем подходящие картинки
    if(color == 0)
    {
        pawnIcon.addPixmap(QPixmap(":/figures_img/White_pawn.png"));
        queenIcon.addPixmap(QPixmap(":/figures_img/White_queen.png"));
        rookIcon.addPixmap(QPixmap(":/figures_img/White_rook.png"));
        elephantIcon.addPixmap(QPixmap(":/figures_img/White_elephant.png"));
        horseIcon.addPixmap(QPixmap(":/figures_img/White_horse.png"));
    }
    else
    {
        pawnIcon.addPixmap(QPixmap(":/figures_img/Black_pawn.png"));
        queenIcon.addPixmap(QPixmap(":/figures_img/Black_queen.png"));
        rookIcon.addPixmap(QPixmap(":/figures_img/Black_rook.png"));
        elephantIcon.addPixmap(QPixmap(":/figures_img/Black_elephant.png"));
        horseIcon.addPixmap(QPixmap(":/figures_img/Black_horse.png"));
    }

    if(queenIcon.isNull() || rookIcon.isNull() || elephantIcon.isNull() || horseIcon.isNull())
    {
        QMessageBox::critical(this, "Error", "Error inserting a picture in pawn selection");
        reject();
    }

    // Устанавливаем иконки и размер иконок для кнопок
    QSize iconSize(64, 64);
    pawn->setIcon(pawnIcon);
    pawn->setIconSize(iconSize);

    queen->setIcon(queenIcon);
    queen->setIconSize(iconSize);

    rook->setIcon(rookIcon);
    rook->setIconSize(iconSize);

    elephant->setIcon(elephantIcon);
    elephant->setIconSize(iconSize);

    horse->setIcon(horseIcon);
    horse->setIconSize(iconSize);

    l_H->addWidget(pawn);
    l_H->addWidget(queen);
    l_H->addWidget(rook);
    l_H->addWidget(elephant);
    l_H->addWidget(horse);


    //Устанавливаем основной макет
    l_V->addWidget(textLabel, 0, Qt::AlignCenter);
    l_V->addLayout(l_H, 1);
    setLayout(l_V);

    // Подключаем кнопки к слотам
    connect(pawn, &QPushButton::clicked, this, &PawnSelection::slot_clickedPawn);
    connect(queen, &QPushButton::clicked, this, &PawnSelection::slot_clickedQueen);
    connect(rook, &QPushButton::clicked, this, &PawnSelection::slot_clickedRook);
    connect(elephant, &QPushButton::clicked, this, &PawnSelection::slot_clickedElephant);
    connect(horse, &QPushButton::clicked, this, &PawnSelection::slot_clickedHorse);
}

void PawnSelection::slot_clickedPawn()
{
    reject();
}

void PawnSelection::slot_clickedQueen()
{
    emit signals_pieceSelected(2);
    accept();
}

void PawnSelection::slot_clickedRook()
{
    emit signals_pieceSelected(3);
    accept();
}

void PawnSelection::slot_clickedElephant()
{
    emit signals_pieceSelected(4);
    accept();
}

void PawnSelection::slot_clickedHorse()
{
    emit signals_pieceSelected(5);
    accept();
}
