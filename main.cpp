#include <QApplication>
#include <QDebug>
#include "mainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    try
    {
        MainWindow *game = new MainWindow();
        game->show();
        return a.exec();
    }
    catch (const std::runtime_error &e)
    {
        qDebug() << "Error" << e.what();
        return EXIT_FAILURE;
    }
    catch (...)
    {
        qDebug() << "Unknown Error";
        return EXIT_FAILURE;
    }
}
