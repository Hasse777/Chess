#include <QApplication>
#include "mainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *game = new MainWindow();

    game->show();
    return a.exec();
}
