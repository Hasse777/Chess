#include "eatenpieces.h"
#include <QMessageBox>



EatenPieces::EatenPieces(int square_size, QWidget* parent) : QLabel{parent}
{
    setFixedSize(380, 760);
    setStyleSheet("border: 3px solid black;");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Создаём таблицу для правильного отображения сьеденных фигур
    // В каждой строке будет 4 столбца виджетов
    m_layout = new QGridLayout();
    // Создаем текст по умолчанию
    m_label = new QLabel("All the eaten figures will be here");
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setStyleSheet("font-size: 20px; font-weight: bold; color: black; border: none;");
    m_layout->addWidget(m_label, 0, 0, 8, 4);


    int row = 0;
    int col = 0;
    for(int i = 0; i < 32; i++, col++)
    {
        QLabel* label = new QLabel(this);
        label->setStyleSheet("border: none;"); // Отключаем отрисовку границы виджета
        label->setFixedSize(square_size, square_size); // Ставим размер одной шахматной клетке

        //Храним двухсвязный список для вставки картинки в каждый виджет
        m_list_label.push_back(label);
        m_layout->addWidget(label, row, col);

        if(col >= 3)
        {
            col = -1;
            row++;
        }
    }
    mainLayout->addLayout(m_layout);
}

void EatenPieces::slot_addImage(const QPixmap img)
{
    if(!m_list_label.isEmpty())
    {
        // Добавляем картинку в виджет и убираем его из списка
        // QList был выбран потому что сложность вставки и удаление элемента из начало и конца списка O(1)
        m_label->setVisible(false);
        m_list_label.first()->setPixmap(img);
        m_list_img.push_front(m_list_label.first()); // Добавляем виджет с картинкой для того чтобы удалять картинку при новой игре
        m_list_label.pop_front();
    }
    else
    {
        // Если что-то пошло не так, пишем сообщение в терминал
        QMessageBox::critical(this, "Error", "Vector of eaten figures is overflowing");
    }
}

void EatenPieces::slot_clearImage()
{
    while (!m_list_img.empty())
    {
        m_list_img.first()->setPixmap(QPixmap());
        m_list_label.push_front(m_list_img.first());
        m_list_img.pop_front();
    }
    m_label->setVisible(true);
}
