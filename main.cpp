#include "mainwindow.h"
#include <QApplication>

/*!
 * \author Biskup Przemysław, Chwistek Dawid, Łośko Jakub
 * \date 15.05.2018
 * \version 1.0.0
 */

/*! \file main.cpp
    \brief Główny plik programu.
*/

/*!
    \brief Główny funkcja programu inicjalizująca UI.
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
