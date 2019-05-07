#ifndef GAMEVSCOMPUTER_H
#define GAMEVSCOMPUTER_H

#include "createship.h"
#include <QObject>
#include <QTimer>
#include <QMessageBox>
#include "audio.h"

/*! \file gamevscomputer.h
 *   \brief Plik nagłówkowy zawierający klasę GameVsComputer.
 */

/*!
 * \brief Klasa GameVsComputer odpowiada za działanie trybu gry Gracz kontra Komputer.
 *
 * Klasa dziedziczy z QObject w celu możliwości zrobienia disconnect'a.
 */
class GameVsComputer : public QObject, public CreateShip
{
private:
    /*!
     * \brief Tabela pól gracza.
     *
     * Komputer używa jej w algorytmie strzelania.
     */
    QList<int> aiPlayerCoordinates;

    /*!
     * \brief Ostatni kierunek, w którym AI trafiło pole statku.
     */
    int aiLastDirection;

    /*!
     * \brief Tablica kierunków, używana do losowego określania kierunku ostrzału AI po trafieniu w część statku.
     */
    QList<int> aiDirections;

    /*!
     * \brief ID ostatniego trafionego pola przez komputer.
     */
    int aiLastShotShipField;

    /*!
     * \brief ID pierwszego trafionego pola przez komputer. Służy do zawracania ostrzału.
     */
    int aiFirstShotShipField;

    /*!
     * \brief Zmienna służy do zawracania ostrzału, aby algorytm nie strzelał po bokach.
     */
    int aiTwoGoodShotsSameDirection;

    /*!
     * \brief Tablica trafionych pól statków przez komputer.
     */
    QList<int> aiShipFields;

    /*!
     * \brief Tablica wskaźników do pól na planszy gracza.
     */
    ClickableLabel *qLabelPlayerFields[100];

    /*!
     * \brief Tablica wskaźników do pól na planszy komputera.
     */
    ClickableLabel *qLabelEnemyFields[100];
public:
    /*!
     * \brief Pusty konstruktor, potrzebny do prawidłowej kompilacji gdy zostanie dodana zmienna tej klasy do UI.
     */
    GameVsComputer();

    /*!
     * \brief Konstruktor klasy GameVsComputer.
     *
     * \param *qLabelPlayerFields[]
     * \param *qLabelEnemyFields[]
     */
    GameVsComputer(ClickableLabel *qLabelPlayerFields[], ClickableLabel *qLabelEnemyFields[]);

    /*!
     * \brief Inicjalizacja klasy obsługującej dźwięki w grze.
     */
    Audio audio;

    /*!
     * \brief Główna metoda obsługująca cały algorytm AI, który odpowiada za strzelanie do gracza w trybie gry Gracz kontra Komputer.
     */
    void ai();

    /*!
     * \brief Metoda odpowiedzialna za algorytm AI,
     * który służy do ostrzeliwania pól gracza przez komputer.
     */
    int aiShootShip();

    /*!
     * \brief Metoda odpowiedzialna za wylosowanie pola do którego zostanie oddany strzał.
     *
     * \return ID pola.
     */
    int aiRandomShoot();

    /*!
     * \brief Metoda odpowiedzialna powrót na piersze trafione pole w przypadku gdy
     * algorytm trafił w statek i ostrzeliwał go w linii prostej aż do momentu spudłowania.
     * Następnie metoda odwraca kierunek ostrzału.
     */
    void aiGoBack();

    /*!
     * \brief Metoda odpowiedzialna za oznaczenie pól wokól statku jako nietrafione,
     * aby uniemożliwić strzelanie obok zatopionego statku.
     */
    void aiRemoveAdjacentFields();

    /*!
     * \brief Metoda odpowiedzialna za oznaczenie pól wokól statku jako nietrafione,
     * aby uniemożliwić strzelanie obok zatopionego statku.
     */
    void playerRemoveAdjacentFields();

    /*!
     * \brief Metoda odpowiedzialna za ponowne wylosowanie pozycji statków na planszy.
     */
    void generateAgain();

    /*!
     * \brief Metoda odpowiedzialna za rozpoczęcie gry po naciśnięciu przycisku Rozpocznij Grę.
     */
    void startGame();

    /*!
     * \brief Metoda odpowiada za możliwość naciskania na pole oraz komunikat o wygraniu gry.
     *
     * Jeśli po oddaniu strzału spudłowaliśmy to metoda odtwarza dźwięk miss.mp3, oznacza pole jako pudło,
     * zmienia turę na ruch komputera oraz blokuje możliwość strzelania dla gracza, aż do momentu ponownego ruchu.
     *
     * Jeśli trafiliśmy w statek to metoda odtwarza dźwięk hit.mp3, onacza pole jako trafione oraz daje nam możliwość
     * oddania kolejnego strzału.
     * W przypadku zatopienia statku zostaje uruchomiona metodaplayer RemoveAdjacentFields.
     *
     * Jeżeli wygraliśmy grę to zostaje uruchomiona metoda deleteAllOnClicks oraz zostaje wyświetlony komunikat o wygranej grze.
     *
     * \param fieldId odpowiada za pole do którego strzelaliśmy.
     */
    void playerClick(int fieldId);

    /*!
     * \brief Metoda odpowiadzialna za wyłączenie możliwości klikania w pola na planszy.
     */
    void deleteAllOnClicks();
};

#endif // GAMEVSCOMPUTER_H
