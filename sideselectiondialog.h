#ifndef SIDESELECTIONDIALOG_H
#define SIDESELECTIONDIALOG_H
#include <QDialog>
#include <QObject>


class SideSelectionDialog : public QDialog
{
    Q_OBJECT
private:
    void button_design(QPushButton* button);
public:
    SideSelectionDialog(QWidget* parent = nullptr);
protected:
    // void keyPressEvent(QKeyEvent *event) override;
private slots:
    void slot_playerWhite();
    void slot_playerBlack();
signals:
    void signal_playerColor(bool color); // 0 - white, 1 - black;
};

#endif // SIDESELECTIONDIALOG_H
