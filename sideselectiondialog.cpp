#include "sideselectiondialog.h"
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QKeyEvent>
#include <QApplication>


SideSelectionDialog::SideSelectionDialog(QWidget *parent) : QDialog{parent}
{
    setFixedSize(200, 150);
    setStyleSheet("background-color: #F5F5DC; border: 2px solid #383636;");

    // Выключаем стандартную рамку окна и задаём флаг диалогового окна
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    QGridLayout* lay = new QGridLayout(this);
    QPushButton* white_king_button = new QPushButton(this);
    QPushButton* black_king_button = new QPushButton(this);
    button_design(white_king_button);
    button_design(black_king_button);
    connect(white_king_button, &QPushButton::clicked, this, &SideSelectionDialog::slot_playerWhite);
    connect(black_king_button, &QPushButton::clicked, this, &SideSelectionDialog::slot_playerBlack);

    QIcon white_kingIcon;
    QIcon black_kingIcon;

    white_kingIcon.addPixmap(QPixmap(":/figures_img/White_kingMove.png"));
    black_kingIcon.addPixmap(QPixmap(":/figures_img/Black_kingMove.png"));

    if(white_kingIcon.isNull() || black_kingIcon.isNull())
    {
        QMessageBox::critical(this, "Error", "Error inserting a picture in pawn selection");
        QApplication::quit();
    }

    QSize iconSize(64, 64);
    white_king_button->setIcon(white_kingIcon);
    white_king_button->setIconSize(iconSize);

    black_king_button->setIcon(black_kingIcon);
    black_king_button->setIconSize(iconSize);

    QLabel *textLabel = new QLabel("You will play for", this);
    textLabel->setFont(QFont("Arial", 14, QFont::Bold));
    textLabel->setStyleSheet("border: none;");

    lay->addWidget(textLabel, 0, 0, 1, 2, Qt::AlignCenter);
    lay->addWidget(white_king_button, 1, 0, 1, 1);
    lay->addWidget(black_king_button, 1, 1, 1, 1);

    setLayout(lay);
}

void SideSelectionDialog::slot_playerWhite()
{
    emit signal_playerColor(0);
    accept();
}

void SideSelectionDialog::slot_playerBlack()
{
    emit signal_playerColor(1);
    accept();
}



void SideSelectionDialog::button_design(QPushButton *button)
{
    button->setStyleSheet
        (
        "QPushButton {border: none;}"
        "QPushButton:hover {background-color: #cccccc;}"
        );
}

// void SideSelectionDialog::keyPressEvent(QKeyEvent *event)
// {

// }
