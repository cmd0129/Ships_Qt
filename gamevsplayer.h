#ifndef GAMEVSPLAYER_H
#define GAMEVSPLAYER_H

#include "audio.h"
#include "createship.h"
#include "firebase.h"
#include <QObject>
#include <QTimer>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QMessageBox>
#include <QJsonArray>
#include <QMenu>

/*! \file gamevsplayer.h
 *   \brief Plik nagłówkowy zawierający klasę GameVsPlayer.
 */

/*!
 * \brief Klasa GameVsPlayer odpowiada za działanie trybu gry Gracz kontra Komputer.
 */
class GameVsPlayer :public QObject, public CreateShip
{
    Q_OBJECT
public:

    /*!
     * \brief Pusty konstruktor klasy GameVsPlayer, potrzebny do poprawnej kompilacji po to aby można było wykorzystywać zmienne tej klasy w innych miejscach.
     */
    GameVsPlayer();

    /*!
     * \brief Konstruktor klasy GameVsPlayer.
     *
     * \param *qLabelPlayerFields[] wskaźnik do tablicy pól gracza.
     * \param *qLabelEnemyFields[] wskaźnik do tablicy pól przeciwnika.
     * \param *m1 wskażnik odpowiadający za menu aplikacji.
     */
    GameVsPlayer(ClickableLabel *qLabelPlayerFields[], ClickableLabel *qLabelEnemyFields[], QMenu *m1);

    /*!
     * \brief Inicjalizacja klasy obsługującej dźwięki w grze.
     */
    Audio audio;

    /*!
     * \brief Destruktor klasy GameVsPlayer.
     */
    ~GameVsPlayer();

    /*!
     * \brief Metoda odpowiada za obsługę przycisku służącego do wygenerowania nowych pozycji statków.
     */
    void generateAgain();

    /*!
     * \brief Metoda odpowiedzialna za osbługę przycisku slużącego do rozpoczęcia gry.
     */
    void startGame();

    /*!
     * \brief Metoda odpowiedzialna za oznaczenie pól wokól statku jako nietrafione,
     * aby uniemożliwić strzelanie obok zatopionego statku.
     */
    void playerRemoveAdjacentFields();

    /*!
     * \brief Metoda odpowiedzialna za sprawdzanie czy nadal mamy połączenie z przeciwnikiem.
     */
    void periodicallyCheckConnection();

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

    /*!
     * \brief Metoda odpowiedzialna za utworzenie użytkownika w bazie danych Firebase.
     */
    void createUserInDatabase();

    /*!
     * \brief Metoda odpowiedzialna za utworzenie gry w bazie danych Firebase.
     */
    void createGameInDatabase();

    /*!
     * \brief Szablon metody odpowiedzialnej do wysyłanie danych do bazy danych Firebase.
     *
     * \param path ścieżka do obiektu.
     * \param name nazwa obiektu w bazie.
     * \param value wartość obiektu w bazie.
     */
    template <typename T> void firebaseSetData(QString path, QString name, T value);

    /*!
     * \brief Metoda odpowiedzialna za usuwanie danych z podanej ściezki w bazie danych Firebase.
     *
     * \param path ścieżka z której zostaną usunięte dane.
     */
    void firebaseDeleteData(QString path);

private slots:
    void onCheckConnection(QByteArray data);
    void onUserCreate(QByteArray data);
    void onGameCreate(QByteArray data);
    void onSetEnemyUser(QJsonObject data);
    void onGetEnemyBoard(QByteArray data);
    void onBoardChange(QJsonObject data);
    void onOver(QJsonObject data);
    void onCheckWhoseTurn(QByteArray data);

private:

    /*!
     * \brief Tablica statków gracza po lewej stronie.
     */
     int playerShipTable[100];

     /*!
      * \brief Tablica przechowywująca pierwsze ułożenie tablicy przeciwnika.
      */
     int oldEnemyShipTable[100];

     /*!
      * \brief Tablica statków przeciwnika po prawej stronie.
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
      * \brief Tablica wskaźników do pól na planszy lewej.
      */
     ClickableLabel *qLabelPlayerFields[100];

     /*!
      * \brief Tablica wskaźników do pól na planszy prawej.
      */
     ClickableLabel *qLabelEnemyFields[100];

     /*!
      * \brief Odpowiada za menu aplikacji.
      */
     QMenu *m1;

     // Firebase
     /*!
      * \brief ID użytkownika w bazie danych Firebase.
      */
     QString myUser = NULL;

     /*!
      * \brief ID przeciwnika w bazie danych Firebase.
      */
     QString enemyUser = "placeholder";

     /*!
      * \brief Informacja o tym, który użytkownik rozpoczął grę.
      */
     QString whichUserInitializedGame = NULL;

     /*!
      * \brief Nazwa bazy danych Firebase.
      */
     QString game = NULL;

     /*!
      * \brief Odpowiada za ruch przeciwnika.
      */
     int whoseTurn;

     /*!
      * \brief Odpowiada za informację o połączeniu z przeciwnikiem.
      */
     bool alreadyShotOne = false;

     /*!
      * \brief Jeżeli zmienna przyjmuje wartość true wtedy jesteśmy połączeni, w przypadku utraty połączenia zmienna przyjmuje wartość false.
      */
     bool connectionLost = true;

     /*!
      * \brief Zmienna przyjmuje wartość false gdy gra zakońcyła się.
      */
     bool gameover = false;

     Firebase *firebaseGet;
     Firebase *firebaseTemp;
     Firebase *firebaseOver;
     Firebase *firebaseSetEnemyUser;
     Firebase *firebaseBoardChange;
     Firebase *firebaseWhoseTurn;
     int tempFieldId;
};

#endif // GAMEVSPLAYER_H
