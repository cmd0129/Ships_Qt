#ifndef CREATESHIP_H
#define CREATESHIP_H

#include <QList>
#include <QPalette>
#include "clickablelabel.h"

/*! \file createship.h
 *   \brief Plik nagłówkowy zawierający klasę CreateShip.
 */

/*!
 * \brief Klasa CreateShip jest odpowiedzialna za utworzenie planszy wraz z statkami.
 *
 * Klasa odpowiada za wygenerowanie planszy, utworzenie statków, sprawdzanie czy statki są zatopione.
 */
class CreateShip
{
protected:
    /*!
     * \brief Tablica statków gracza po lewej stronie.
     */
    int playerShipTable[100];

    /*!
     * \brief Tablica statków komputera po prawej stronie.
     */
    int enemyShipTable[100];

    /*!
     * \brief Tablica trafionych pól statków przez gracza.
     */
    QList<int> shipFieldsShotByPlayer;

    /*!
     * \brief Tablica typów trafionych pól statków przez gracza.
     */
    QList<int> shipFieldsShotByPlayerTypes;

    /*!
     * \brief Tura gracza lub komputera.
     */
    int whoShoots;
public:
    /*!
     * \brief Konstruktor klasy CreateShip
     */
    CreateShip();

    /*!
     * \brief Metoda czyści (zeruje) tablice statków gracza.
     *
     * Pobiera tablicę typu int i nie zwraca żadnej wartości.
     *
     * \param shipTable[] tablica statków gracza.
     */
     void resetShipTable(int shipTable[]);

     /*!
      * \brief Metoda sprawdza czy pola po danej stronie są puste.
      *
      * Pobiera trzy wartości typu int i zwraca wartość typu bool.
      *
      * \param id indeks sprawdzanego pola.
      * \param shipTable[] tablica statków gracza.
      * \param direction kierunek budowania statku.
      * \return true gdy można postawić kolejne pole statku.
      * \return false jeżeli nie można postawić kolejnego pola statku.
      */
     bool checkIfFieldIsEmpty(int id, int shipTable[], int direction);

     /*!
      * \brief Metoda sprawdza czy sprawdza czy dane 3x3 jest puste.
      *
      * Pobiera dwie wartości typu int i zwraca wartość typu bool.
      *
      * \param id indeks sprawdzanego pola
      * \param shipTable[] tablica statków gracza
      * \return true gdy 3x3 jest puste, false jeżeli 3x3 jest zajęte
      */
     bool is3x3Empty(int id, int shipTable[]);

     /*!
      * \brief Metoda generuje statek czteromasztowy.
      *
      * Pobiera tablicę w której będzie generowany statek oraz id pierszego pola.
      *
      * \param shipTable[] tablica w której statek będzie generowany.
      * \param id ID pierwszego pola.
      */
     bool generateShip4(int shipTable[], int id);

     /*!
      * \brief Metoda generuje statek trójmasztowy.
      *
      * Pobiera tablicę w której będzie generowany statek, id pierszego pola oraz numer generowanego statku.
      *
      * \param shipTable[] tablica w której statek będzie generowany.
      * \param id ID pierwszego pola.
      * \param shipNumber numer generowanego statku.
      */
     bool generateShip3(int shipTable[], int id, int shipNumber);

     /*!
      * \brief Metoda generuje statek dwumasztowy.
      *
      * Pobiera tablicę w której będzie generowany statek, id pierszego pola oraz numer generowanego statku.
      *
      * \param shipTable[] tablica w której statek będzie generowany.
      * \param id ID pierwszego pola.
      * \param shipNumber numer generowanego statku.
      */
     bool generateShip2(int shipTable[], int id, int shipNumber);

     /*!
      * \brief Metoda generuje statek jednomasztowy.
      *
      * Pobiera tablicę w której będzie generowany statek, id pierszego pola oraz numer generowanego statku.
      *
      * \param shipTable[] tablica w której statek będzie generowany.
      * \param id ID pierwszego pola.
      * \param shipNumber numer generowanego statku.
      */
     bool generateShip1(int shipTable[], int id, int shipNumber);

     /*!
      * \brief Metoda odpowiedzialna za wygenerowanie wszystkich statków w danej tablicy.
      *
      * Pobiera tablicę w której będą wygenerowane statki.
      *
      * \param shipTable[] tablica w której zostaną wygenerowane statki.
      */
     void shipGenerator(int shipTable[]);

     /*!
      * \brief Metoda sprawdza czy komputer lub gracz zatopił wszystkie statki z danej tablicy.
      *
      * Jako argument przyjmuje tablicę statków i zwraca true w przypadku zatopienia wszystkich statków,
      * lub false w przypadku niezatopienia wszystkich statków.
      *
      * \param table[] tablica statków.
      * \return true jeśli wszystkie statki są zatopione.
      * \return false jeśli wszystkie statki nie są zatopione.
      */
     bool checkTable(int table[]);

     /*!
      * \brief Metoda sprawdza czy statek o danym ID został zatopiony w danej tablicy.
      *
      * Pozyskuje tablicę oraz ID statku i zwraca true w przypadku zatopienia statku w tablicy,
      * lub false w przypadku niezatopienia statku.
      *
      * \param shipTable[] tablica statków.
      * \param id ID statku.
      * \return true jeśli statek został zatopiony.
      * \return false jeśli statek nie został zatopiony.
      */
     bool checkIfSunk(int shipTable[], int id);

     /*!
      * \brief Metoda odpowiedzialna za pokazanie statków na danej planszy.
      *
      * Otrzymuje tablice statków oraz wskaźnik do tablicy w której mają zostać wyświetlone statki.
      *
      * \param shipTable[] tablica/plansza statków.
      * \param *pointerShipTable[] wskaźnik do tablicy statków/planszy.
      */
     void showShips(int shipTable[], ClickableLabel *pointerShipTable[]);
};

#endif // CREATESHIP_H
