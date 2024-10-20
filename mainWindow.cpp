#include "mainWindow.h"
#include "mainmenudialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setStyleSheet("background-color: #F5F5DC;");
    // Устанавливаем центральный виджет
    setWindowState(Qt::WindowMaximized);
    m_central_widget = new QWidget(this);
    setCentralWidget(m_central_widget);
    setWindowIcon(QIcon(":/figures_img/mainPiece.png"));

    // Создаем виджеты
    m_chessBoard = new ChessBoard(m_central_widget);
    m_playerWidget = new PlayerWidget(m_central_widget);
    m_moveWidget = new MovesWidget(m_central_widget);
    m_eatenPieces = new EatenPieces(m_chessBoard->get_square_Size(), m_central_widget);

    // Подключаем сигналы от шахматной доски к виджетам
    connect(m_chessBoard, &ChessBoard::signal_Change_picture, m_playerWidget, &PlayerWidget::change_picture);
    connect(m_chessBoard, &ChessBoard::signal_addMove, m_moveWidget, &MovesWidget::slot_addText);
    connect(m_chessBoard, &ChessBoard::signal_newGame, m_moveWidget, &MovesWidget::slot_clearText);
    connect(m_chessBoard, &ChessBoard::signal_addEatenImages, m_eatenPieces, &EatenPieces::slot_addImage);
    connect(m_chessBoard, &ChessBoard::signal_newGame, m_eatenPieces, &EatenPieces::slot_clearImage);
    connect(this, &MainWindow::signal_newGame, m_chessBoard, &ChessBoard::slot_newGame);

    // Создаем макет и добавляем виджеты
    m_gridLayout = new QGridLayout(m_central_widget);
    m_gridLayout->setSpacing(15);
    m_gridLayout->addWidget(m_playerWidget, 0, 0, Qt::AlignTop | Qt::AlignLeft);
    m_gridLayout->addWidget(m_moveWidget, 1, 0, Qt::AlignTop | Qt::AlignLeft);
    m_gridLayout->addWidget(m_chessBoard, 0, 1, 3, 1, Qt::AlignCenter);
    m_gridLayout->addWidget(m_eatenPieces, 0, 2, 3, 1, Qt::AlignRight);
    m_gridLayout->setColumnStretch(2, 1);


    // Устанавливаем макет на центральный виджет
    m_central_widget->setLayout(m_gridLayout);
}

void MainWindow::newGame()
{

    emit signal_newGame();
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        MainMenuDialog* d = new MainMenuDialog(this);
        connect(d, &MainMenuDialog::signal_newGame, m_chessBoard, &ChessBoard::slot_newGame);
        d->exec();
        delete d;
    }
}
