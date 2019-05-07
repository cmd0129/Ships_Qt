#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "audio.h"
#include "gamevscomputer.h"
#include "gamevsplayer.h"
#include "clickablelabel.h"
#include "firebase.h"
#include <QMainWindow>
#include <QMediaPlayer>
#include <QDate>

/*! \file mainwindow.h
 *   \brief Plik nagłówkowy zawierający klasę MainWindow.
 */

namespace Ui {

/*!
 * \brief Klasa MainWindow jest główną klasą programu, odpowiedzialna za UI.
 */
class MainWindow;
}

/*!
 * \brief Dziedziczenie klasy MainWindow po klasie QMainWindow.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief Konstruktor klasy MainWindow.
     *
     * \param parent odpowiada za rodzica obiektu.
     */
    explicit MainWindow(QWidget *parent = 0);

    /*!
     * \brief Destruktor klasy MainWindow.
     *
     *
     */
    ~MainWindow();

    /*!
     * \brief Metoda służąca do utworzenia pustej planszy.
     */
    void setBoard();

private slots:
    /*!
     * \brief Metoda odpowiadająca za obsługę przycisku służącego do zamknięcia aplikacji.
     */
    void on_actionCloseApp_triggered();

    /*!
     * \brief Metoda odpowiadająca za włączenie lub wyłączenie dźwięku.
     */
    void on_actionChangeSound_triggered();

    /*!
     * \brief Metoda odpowiedzialna za obsługę przycisku Generuj Ponownie.
     */
    void on_pushButton_generateAgain_clicked();

    /*!
     * \brief Metoda odpowiedzialna za obsługę przycisku Rozpocznij Grę.
     */
    void on_pushButton_startGame_clicked();

    /*!
     * \brief Metoda odpowiedzialna za obsługę przycisku Zagraj z komputerem, który uruchamia tryb gry Gracz kontra komputer.
     */
    void on_actionGameVsComputer_triggered();

    /*!
     * \brief Metoda odpowiadająca za obsługę kliknięcia na pole.
     */
    void fieldClick();

	/*!
     * \brief Metoda odpowiedzialna za obsługę przycisku Zagraj z przeciwnikiem, który uruchamia tryb gry Gracz kontra Gracz.
     */
    void on_actionGameVsPlayer_triggered();

private:
    /*!
     * \brief Główna klasa odpowiedzialna za wyświatlanie interfejsu okienkowego aplikacji.
     */
    Ui::MainWindow *ui;

    /*!
     * \brief Konstruktor gry lokalnej.
     */
    GameVsComputer *gameLocal = NULL;

    /*!
     * \brief Konstruktor gry sieciowej.
     */
    GameVsPlayer *gameOnline = NULL;

    /*!
     * \brief Zmienna służąca do zapamiętania, który tryb gry jest obecnie uruchomiony.
     */
    bool localOrOnline;
};

#endif // MAINWINDOW_H
