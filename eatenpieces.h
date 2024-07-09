#ifndef EATENPIECES_H
#define EATENPIECES_H

#include <QLabel>
#include <QGridLayout>
#include <QPixmap>
#include <Qlist>

class EatenPieces : public QLabel
{
private:
    QGridLayout* m_layout;
    QList<QLabel*> m_list_label;
public:
    EatenPieces(int square_size, QWidget* parent = nullptr);

public slots:
    void slot_addImage(const QPixmap img);
};

#endif // EATENPIECES_H
