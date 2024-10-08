cmake_minimum_required(VERSION 3.14)

project(Chess LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(Chess
  main.cpp
  mainWindow.h
  mainWindow.cpp
  chessboard.h chessboard.cpp
  figures.qrc
  chesspiece.h chesspiece.cpp
  clickablerect.h clickablerect.cpp
  playerwidget.h playerwidget.cpp
  moveswidget.h moveswidget.cpp
  pawnselection.h pawnselection.cpp
  eatenpieces.h eatenpieces.cpp
  dialogendgame.h dialogendgame.cpp
  mainmenudialog.h mainmenudialog.cpp
  sideselectiondialog.h sideselectiondialog.cpp
  artificial_intelligence.h artificial_intelligence.cpp
  chesspieceforartifical.h chesspieceforartifical.cpp


)

target_link_libraries(Chess PRIVATE Qt6::Widgets)


include(GNUInstallDirs)
install(TARGETS Chess
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
