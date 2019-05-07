#ifndef AUDIO_H
#define AUDIO_H
#include <QMediaPlayer>
#include <QObject>

/*! \file audio.h
 *   \brief Plik nagłówkowy zawierający klasę Audio.
 */

/*!
 * \brief Klasa Audio odpowiada za odtwarzanie dźwięku w grze.
 */
class Audio : public QObject
{
    Q_OBJECT
    static bool sound;

private:
    QMediaPlayer* player; /*!< Wskaznik na obiekt klasy QMediaPlayer */
public:
    /*!
     * \brief Konstruktor klasy Audio.
     */
    Audio(); // konstruktor
    /*!
     * \brief Metoda uruchamia dźwięk odpowiadający za pudło.
     */
    void playMiss();

    /*!
     * \brief Metoda uruchamia dźwięk odpowadający za trafienie w statek.
     */
    void playHit();
	
	/*!
     * \brief Metoda służy do wyciszenia dźwięku.
     * 
     * \param temp przyjmuje true jeśli dźwięk jest wyciszony, false jeśli nie jest wyciszony.
     */
    static void setMute(bool temp);
};

#endif // AUDIO_H
