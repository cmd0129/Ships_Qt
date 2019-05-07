#include "audio.h"
#include <QFileDialog>

/*! \file audio.cpp
    \brief Plik zródłowy zawierający klasę Audio, odpowiedzialny za dźwięki w grze.
*/

Audio::Audio()
{
     player = new QMediaPlayer();
}

void Audio::setMute(bool temp)
{
    sound = temp;
}

void Audio::playMiss()
{
    if(sound){
        player->setMedia(QUrl("qrc:/StatkiZasoby/miss.mp3"));
        player->setVolume(100);
        player->play();
    }
}

void Audio::playHit()
{
    if(sound){
        player->setMedia(QUrl("qrc:/StatkiZasoby/hit.mp3"));
        player->setVolume(100);
        player->play();
    }
}

bool Audio::sound = true;
