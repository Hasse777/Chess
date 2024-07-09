#include "playerwidget.h"


PlayerWidget::PlayerWidget(QWidget* parent) : QWidget{parent}
{
    setFixedSize(250, 160);
    m_layout = new QHBoxLayout(this);

    QPixmap picture(":/figures_img/White_kingMove.png");
    m_whiteKingPixmap = picture.scaled(130, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    picture.load(":/figures_img/Black_kingMove.png");
    m_blackKingPixmap = picture.scaled(130, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    m_label_text = new QLabel("Move white:" ,this);
    m_label_picture = new QLabel(this);

    QFont font("Arial", 14, QFont::Bold);
    m_label_text->setFont(font);


    m_label_picture->setPixmap(m_whiteKingPixmap);

    m_layout->addWidget(m_label_text, 0, Qt::AlignTop | Qt::AlignLeft);
    m_layout->addWidget(m_label_picture, 1, Qt::AlignTop | Qt::AlignLeft);
    setLayout(m_layout);

}

void PlayerWidget::change_picture(bool color)
{
    if(color == 0)
    {
        m_label_picture->setPixmap(m_whiteKingPixmap);
        m_label_text->setText("Move white:");
    }
    else
    {
        m_label_picture->setPixmap(m_blackKingPixmap);
        m_label_text->setText("Move black:");
    }
}
