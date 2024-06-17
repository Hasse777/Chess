#include "mainWindow.h"


MainWindow::MainWindow(QWidget *parent)
{
    this->setWindowState(Qt::WindowMaximized);
    this->m_central_widget = new QWidget;
    this->m_chessBoard = new ChessBoard;
    this->m_gridLayout = new QGridLayout;
    m_gridLayout->addWidget(m_chessBoard, 0, 0);
    m_central_widget->setLayout(m_gridLayout);
    this->setCentralWidget(m_central_widget);

}
