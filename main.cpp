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
        game->newGame();
        return a.exec();
    }
    catch (const std::out_of_range& e)
    {
        qDebug() << "Exception caught:" << e.what();
        return EXIT_FAILURE;
    }
    catch (const std::runtime_error &e)
    {
        qDebug() << "Error " << e.what();
        return EXIT_FAILURE;
    }
    catch (...)
    {
        qDebug() << "Unknown Error";
        return EXIT_FAILURE;
    }
}
