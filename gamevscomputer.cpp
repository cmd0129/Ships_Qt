#include "gamevscomputer.h"
#include <QtDebug>
#include <QMediaPlayer>

/*! \file gamevscomputer.cpp
    \brief Plik źródłowy zawierający klasę GameVsComputer, odpowiada za obsługę trybu gry Gracz kontra Komputer.
*/

GameVsComputer::GameVsComputer()
{

}

GameVsComputer::GameVsComputer(ClickableLabel *qLabelPlayerFields[], ClickableLabel *qLabelEnemyFields[])
{
    for( int i = 0; i < 100; ++i ) {
        this->qLabelPlayerFields[i] = qLabelPlayerFields[i];
        this->qLabelEnemyFields[i] = qLabelEnemyFields[i];
    }

    aiLastDirection = -1;
    aiLastShotShipField = -1;
    aiFirstShotShipField = -1;
    aiTwoGoodShotsSameDirection = -1;
    whoShoots = -1; // tura nikogo
    resetShipTable(playerShipTable);
    shipGenerator(playerShipTable);
    showShips(playerShipTable, this->qLabelPlayerFields);
}

void GameVsComputer::generateAgain()
{
    resetShipTable(playerShipTable);
    shipGenerator(playerShipTable);
    QPalette pwoda;
    pwoda.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/woda.png"));
    QPalette pstatek;
    pstatek.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/statek.png"));
    for( int i = 0; i < 100; ++i ) {
        if( playerShipTable[i] == 0 ) {
            qLabelPlayerFields[i]->setPalette(pwoda);
        } else {
            qLabelPlayerFields[i]->setPalette(pstatek);
        }
    }
}

void GameVsComputer::startGame()
{
    resetShipTable(enemyShipTable);
    shipGenerator(enemyShipTable);
    for( int i = 0; i < 4; ) { aiDirections.append(i++); }
    for( int i = 0; i < 100; ) { aiPlayerCoordinates.append(i++); }
    whoShoots = 0; // tura gracza
}

void GameVsComputer::playerClick( int fieldId )
{
    if( whoShoots == 1 ) { return; }

    disconnect(qLabelEnemyFields[fieldId], 0, 0, 0); // usuwanie onClicka

    if( enemyShipTable[fieldId] == 0 ) { //jeżeli pudło
        whoShoots = 1;
        audio.playMiss();
        QPalette ppudlo;
        ppudlo.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/pudlo.png"));
        qLabelEnemyFields[fieldId]->setPalette(ppudlo);
        QTimer::singleShot(1250,this,[=]{ai();});

    }
    else { //jezeli trafiony
        shipFieldsShotByPlayer.append( fieldId );
        shipFieldsShotByPlayerTypes.append( enemyShipTable[fieldId] );

        QPalette ptrafiony;
        ptrafiony.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/trafiony.png"));
        qLabelEnemyFields[fieldId]->setPalette(ptrafiony);

        int oldId = enemyShipTable[ fieldId ];
        enemyShipTable[ fieldId ] = -1;

        if( checkIfSunk(enemyShipTable, oldId) ) { //jeżeli statek został zatopiony
            playerRemoveAdjacentFields();
        }

        whoShoots = 0;
        audio.playHit();
    }

    if( checkTable(enemyShipTable) ) {
        deleteAllOnClicks();
        QMessageBox::information(NULL,"Wygrałeś!","Pokonałeś mnie, gratulacje!");
    }
}

void GameVsComputer::ai() //strzelanie do pól gracza
{
    int elem;
    if( checkTable(playerShipTable) ) {
        QMessageBox::information(NULL,"Przegrałeś","Przykro mi, jestem lepszy!");
        deleteAllOnClicks();
        showShips(enemyShipTable,qLabelEnemyFields);
        return;
    }

    if( whoShoots == 1 ) { //jeżeli tura komputera
        if( aiLastShotShipField == -1 ) { //strzelanie losowo
            elem = aiRandomShoot();
        }
        else { //strzelanie ai
            int elem1;
            do {
                elem1 = aiShootShip();
            } while( elem1 == -1 );
            elem = elem1;
        }

        if( playerShipTable[elem] == 0 ) { //jeżeli pudło
            QPalette ppudlo;
            ppudlo.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/pudlo.png"));
            qLabelPlayerFields[elem]->setPalette(ppudlo);
            audio.playMiss();
            if( aiTwoGoodShotsSameDirection != -1 ) { //zawracanie ostrzału po 2 udanych i 1 nieudanym strzale w tym samym kierunku
                switch( aiTwoGoodShotsSameDirection ) {
                    case 0:
                        aiLastDirection = 2;
                        break;
                    case 1:
                        aiLastDirection = 3;
                        break;
                    case 2:
                        aiLastDirection = 0;
                        break;
                    case 3:
                        aiLastDirection = 1;
                        break;
                }
                aiLastShotShipField = aiFirstShotShipField;
            }
            else {
                aiLastDirection = -1; //spudłowaliśmy, więc następny strzał ma być w inną stronę
            }
            whoShoots = 0;
        }
        else { //jeżeli trafiony
            int id = elem;

            aiShipFields.append( id );
            QPalette ptrafiony;
            ptrafiony.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/trafiony.png"));
            qLabelPlayerFields[elem]->setPalette(ptrafiony);

            int tempId = playerShipTable[ id ];
            playerShipTable[ id ] = 0;
            aiLastShotShipField = id;
            if( aiFirstShotShipField != -1 && aiTwoGoodShotsSameDirection == -1 ) {
                aiTwoGoodShotsSameDirection = aiLastDirection;
            }
            if( aiFirstShotShipField == -1 ) { //id pierwszego trafionego punktu statku
                aiFirstShotShipField = id;
            }
            if( checkIfSunk(playerShipTable, tempId) ) { //jeżeli statek zatopiony
                aiRemoveAdjacentFields();
                aiLastShotShipField = -1;
                aiFirstShotShipField = -1;
                aiLastDirection = -1;
                aiTwoGoodShotsSameDirection = -1;
            }
            aiDirections.clear();
            aiDirections << 0 << 1 << 2 << 3;
            audio.playHit();
            whoShoots = 1;
            QTimer::singleShot(1250,this,[=]{ai();});
        }
    }
}

int GameVsComputer::aiShootShip()
{
    int temp;
    if( aiLastDirection == -1 ) {
        temp = (std::rand() % aiDirections.size());
        aiLastDirection = aiDirections.at(temp);
        aiDirections.removeAt( temp );
    }

    aiGoBack();

    switch( aiLastDirection ) {
        case 0:
            temp = aiLastShotShipField+1;
            if( aiPlayerCoordinates.contains(temp) && aiLastShotShipField/10 == temp/10 ) { //jeżeli istnieje i nie wyjdzie do następnego wiersza
                aiPlayerCoordinates.removeOne(temp);
                return temp;
            }
            aiLastDirection = -1;
            return -1;
        case 1:
            temp = aiLastShotShipField+10;
            if( aiPlayerCoordinates.contains(temp) && temp < 100 ) { //jeżeli istnieje i nie wyjdzie za tabelę
                aiPlayerCoordinates.removeOne(temp);
                return temp;
            }
            aiLastDirection = -1;
            return -1;
        case 2:
            temp = aiLastShotShipField-1;
            if( aiPlayerCoordinates.contains(temp) && aiLastShotShipField/10 == temp/10 ) { //jeżeli istnieje i nie wyjdzie do poprzedniego wiersza
                aiPlayerCoordinates.removeOne(temp);
                return temp;
            }
            aiLastDirection = -1;
            return -1;
        case 3:
            temp = aiLastShotShipField-10;
            if( aiPlayerCoordinates.contains(temp) && temp > -1 ) { //jeżeli istnieje i nie wyjdzie przed tabelę
                aiPlayerCoordinates.removeOne(temp);
                return temp;
            }
            aiLastDirection = -1;
            return -1;
        default:
            return -1;
    }
}

int GameVsComputer::aiRandomShoot() //zwraca wylosowane pole i usuwa je z aiPlayerCoordinates
{
    int num = (std::rand() % aiPlayerCoordinates.size()); //wylosowanie pola
    int field = aiPlayerCoordinates.at( num );
    aiPlayerCoordinates.removeAt( num ); //usuniecie pola z tablicy
    return field; //zwrocenie pola
}

void GameVsComputer::aiGoBack() //jeżeli wszystkie sąsiednie pola nie istnieją to wracamy do aiFirstShotShipField
{
    if( ( !aiPlayerCoordinates.contains( aiLastShotShipField+1 ) || (aiLastShotShipField/10) != ((aiLastShotShipField+1)/10) ) //nie istnieje lub gdy jesteśmy przy ścianie i istnieje w następnym wierszu
            && ( !aiPlayerCoordinates.contains( aiLastShotShipField-1 ) || (aiLastShotShipField/10) != ((aiLastShotShipField-1)/10) ) //nie istnieje lub gdy jesteśmy przy ścianie i istnieje w poprzednim wierszu
            && !aiPlayerCoordinates.contains( aiLastShotShipField-10 )
            && !aiPlayerCoordinates.contains( aiLastShotShipField+10 ) ) {
        switch( aiLastDirection ) {
            case 0:
                aiLastDirection = 2;
                break;
            case 1:
                aiLastDirection = 3;
                break;
            case 2:
                aiLastDirection = 0;
                break;
            case 3:
                aiLastDirection = 1;
                break;
        }
        aiDirections.clear();
        aiDirections << 0 << 1 << 2 << 3;
        aiLastShotShipField = aiFirstShotShipField;
    }
}

void GameVsComputer::aiRemoveAdjacentFields()
{
    QPalette ppudlo;
    ppudlo.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/pudlo.png"));
    int temp;

    for( int i = 0; i < aiShipFields.size(); ++i ) {
        temp = aiShipFields.at(i)-10;
        if( aiPlayerCoordinates.removeOne(temp) && playerShipTable[temp] == 0 ) {
            qLabelPlayerFields[temp]->setPalette(ppudlo);
        }

        temp = aiShipFields.at(i)-9;
        if( temp/10 == (aiShipFields.at(i)-10)/10 && aiPlayerCoordinates.removeOne(temp) && playerShipTable[temp] == 0 ) {
            qLabelPlayerFields[temp]->setPalette(ppudlo);
        }

        temp = aiShipFields.at(i)+1;
        if( temp/10 == aiShipFields.at(i)/10 && aiPlayerCoordinates.removeOne(temp) && playerShipTable[temp] == 0 ) {
            qLabelPlayerFields[temp]->setPalette(ppudlo);
        }

        temp = aiShipFields.at(i)+11;
        if( temp/10 == (aiShipFields.at(i)+10)/10 && aiPlayerCoordinates.removeOne(temp) && playerShipTable[temp] == 0 ) {
            qLabelPlayerFields[temp]->setPalette(ppudlo);
        }

        temp = aiShipFields.at(i)+10;
        if( aiPlayerCoordinates.removeOne(temp) && playerShipTable[temp] == 0 ) {
            qLabelPlayerFields[temp]->setPalette(ppudlo);
        }

        temp = aiShipFields.at(i)+9;
        if( temp/10 == (aiShipFields.at(i)+10)/10 && aiPlayerCoordinates.removeOne(temp) && playerShipTable[temp] == 0 ) {
            qLabelPlayerFields[temp]->setPalette(ppudlo);
        }

        temp = aiShipFields.at(i)-1;
        if( temp/10 == (aiShipFields.at(i))/10 && aiPlayerCoordinates.removeOne(temp) && playerShipTable[temp] == 0 ) {
            qLabelPlayerFields[temp]->setPalette(ppudlo);
        }

        temp = aiShipFields.at(i)-11;
        if( temp/10 == (aiShipFields.at(i)-10)/10 && aiPlayerCoordinates.removeOne(temp) && playerShipTable[temp] == 0 ) {
             qLabelPlayerFields[temp]->setPalette(ppudlo);
        }
    }
    aiShipFields.clear();
}

void GameVsComputer::playerRemoveAdjacentFields()
{
    QPalette ppudlo;
    ppudlo.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/pudlo.png"));
    int temp;

    for( int i = 0; i < shipFieldsShotByPlayer.size(); ) {
        if( checkIfSunk( enemyShipTable, shipFieldsShotByPlayerTypes.at(i) ) ) {
            temp = shipFieldsShotByPlayer.at(i)-10;
            if( temp > -1 && enemyShipTable[temp] == 0 ) {
                qLabelEnemyFields[temp]->setPalette(ppudlo);
                disconnect(qLabelEnemyFields[temp], 0, 0, 0);
            }

            temp = shipFieldsShotByPlayer.at(i)-9;
            if( temp > -1 && ( temp/10 == (shipFieldsShotByPlayer.at(i)-10)/10 ) && enemyShipTable[temp] == 0 ) {
                qLabelEnemyFields[temp]->setPalette(ppudlo);
                disconnect(qLabelEnemyFields[temp], 0, 0, 0);
            }

            temp = shipFieldsShotByPlayer.at(i)+1;
            if( temp < 100 && ( temp/10 == shipFieldsShotByPlayer.at(i)/10 ) && enemyShipTable[temp] == 0 ) {
                qLabelEnemyFields[temp]->setPalette(ppudlo);
                disconnect(qLabelEnemyFields[temp], 0, 0, 0);
            }

            temp = shipFieldsShotByPlayer.at(i)+11;
            if( temp < 100 && ( temp/10 == (shipFieldsShotByPlayer.at(i)+10)/10 ) && enemyShipTable[temp] == 0 ) {
                qLabelEnemyFields[temp]->setPalette(ppudlo);
                disconnect(qLabelEnemyFields[temp], 0, 0, 0);
            }

            temp = shipFieldsShotByPlayer.at(i)+10;
            if( temp < 100 && enemyShipTable[temp] == 0 ) {
                qLabelEnemyFields[temp]->setPalette(ppudlo);
                disconnect(qLabelEnemyFields[temp], 0, 0, 0);
            }

            temp = shipFieldsShotByPlayer.at(i)+9;
            if( temp < 100 && ( temp/10 == (shipFieldsShotByPlayer.at(i)+10)/10 ) && enemyShipTable[temp] == 0 ) {
                qLabelEnemyFields[temp]->setPalette(ppudlo);
                disconnect(qLabelEnemyFields[temp], 0, 0, 0);
            }

            temp = shipFieldsShotByPlayer.at(i)-1;
            if( temp > -1 && ( temp/10 == shipFieldsShotByPlayer.at(i)/10 ) && enemyShipTable[temp] == 0 ) {
                qLabelEnemyFields[temp]->setPalette(ppudlo);
                disconnect(qLabelEnemyFields[temp], 0, 0, 0);
            }

            temp = shipFieldsShotByPlayer.at(i)-11;
            if( temp > -1 && ( temp/10 == (shipFieldsShotByPlayer.at(i)-10)/10 ) && enemyShipTable[temp] == 0 ) {
                qLabelEnemyFields[temp]->setPalette(ppudlo);
                disconnect(qLabelEnemyFields[temp], 0, 0, 0);
            }

            shipFieldsShotByPlayer.removeAt(i);
            shipFieldsShotByPlayerTypes.removeAt(i);
        }
    }
}

void GameVsComputer::deleteAllOnClicks()
{
    for( int i = 0; i < 100; ) {
        disconnect(qLabelEnemyFields[i++], 0, 0, 0);
    }
}
