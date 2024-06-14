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
)

target_link_libraries(Chess PRIVATE Qt6::Widgets)


include(GNUInstallDirs)
install(TARGETS Chess
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
