#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H
#include <QLabel>
#include <QLayout>

class PlayerWidget : public QWidget
{
    Q_OBJECT
private:
    QPixmap m_whiteKingPixmap;
    QPixmap m_blackKingPixmap;
    QLabel* m_label_text;
    QLabel* m_label_picture;
    QHBoxLayout* m_layout;
public:
    PlayerWidget(QWidget* parent = nullptr);
public slots:
    void change_picture(bool color);
};

#endif // PLAYERWIDGET_H
