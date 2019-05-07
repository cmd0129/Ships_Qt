#include "gamevsplayer.h"
#include <sys/time.h>

/*!
 * \brief URL odpowiada za link bazy danych Firebase do której jest podłączona gra.
 */
#define URL "https://project27database.firebaseio.com/"

/*! \file gamevsplayer.cpp
    \brief Plik źródłowy zawierający klasę GameVsPlayer, odpowiada za obsługę trybu gry Gracz kontra Gracz.
*/

GameVsPlayer::GameVsPlayer()
{
}

GameVsPlayer::GameVsPlayer(ClickableLabel *qLabelPlayerFields[], ClickableLabel *qLabelEnemyFields[], QMenu *m1)
{
    for( int i = 0; i < 100; ++i ) {
        this->qLabelPlayerFields[i] = qLabelPlayerFields[i];
        this->qLabelEnemyFields[i] = qLabelEnemyFields[i];
        this->m1 = m1;
    }

    resetShipTable(playerShipTable);
    shipGenerator(playerShipTable);
    showShips(playerShipTable, this->qLabelPlayerFields);
}

GameVsPlayer::~GameVsPlayer() {
    firebaseDeleteData("/boards/" + myUser + "/");
    firebaseDeleteData("/lobby/" + game + "/gameover");
}

void GameVsPlayer::generateAgain()
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

void GameVsPlayer::startGame()
{
    createUserInDatabase();
}

void GameVsPlayer::playerClick( int fieldId )
{
    m1->setTitle("");

    if( alreadyShotOne ) { return; }
    alreadyShotOne = true;

    if( enemyUser == "ul" || enemyUser == "placeholder" ) {
        alreadyShotOne = false;
        return;
    }
    tempFieldId = fieldId;

    firebaseWhoseTurn = new Firebase(URL, "/lobby/" + game + "/whoseTurn.json");
    firebaseWhoseTurn->getValue();
    connect(firebaseWhoseTurn,SIGNAL(eventResponseReady(QByteArray)),this,SLOT(onCheckWhoseTurn(QByteArray)));
}

void GameVsPlayer::onCheckWhoseTurn(QByteArray data)
{
    disconnect(firebaseWhoseTurn, 0, 0, 0);
    QString temp = data;
    whoseTurn = temp.toInt();

    if( whichUserInitializedGame == "myUser" && whoseTurn != 1 ) { //nie możemy strzelać
        alreadyShotOne = false;
        m1->setTitle("Ruch przeciwnika!");
        QTimer::singleShot(2000,this,[=]{m1->setTitle("");});
        return;
    } else if ( whichUserInitializedGame == "enemyUser" && whoseTurn != 2 ) { //nie możemy strzelać
        alreadyShotOne = false;
        m1->setTitle("Ruch przeciwnika!");
        QTimer::singleShot(2000,this,[=]{m1->setTitle("");});
        return;
    }
    disconnect(qLabelEnemyFields[tempFieldId], 0, 0, 0); // usuwanie onClicka

    if( enemyShipTable[tempFieldId] == 0 ) { //jeżeli pudło
        QPalette ppudlo;
        ppudlo.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/pudlo.png"));
        qLabelEnemyFields[tempFieldId]->setPalette(ppudlo);

        if( whoseTurn == 1 ) {
            firebaseSetData("/lobby/" + game,"whoseTurn",2);
        } else {
            firebaseSetData("/lobby/" + game,"whoseTurn",1);
        }

        firebaseSetData("/boards/"+enemyUser+"/board/",QString::number(tempFieldId),1);
        audio.playMiss();
    }
    else { //jezeli trafiony
        shipFieldsShotByPlayer.append( tempFieldId );
        shipFieldsShotByPlayerTypes.append( enemyShipTable[tempFieldId] );

        QPalette ptrafiony;
        ptrafiony.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/trafiony.png"));
        qLabelEnemyFields[tempFieldId]->setPalette(ptrafiony);

        int oldId = enemyShipTable[ tempFieldId ];
        enemyShipTable[ tempFieldId ] = 0;
        firebaseSetData("/boards/"+enemyUser+"/board/",QString::number(tempFieldId),3);

        if( checkIfSunk(enemyShipTable, oldId) ) { //jeżeli statek został zatopiony
            playerRemoveAdjacentFields();
        }

        audio.playHit();
    }

    if( checkTable(enemyShipTable) ) {
        gameover = true;
        deleteAllOnClicks();
        QMessageBox::information(NULL,"SYSTEM","Wygrałeś!");
        disconnect(firebaseOver, 0, 0, 0);
        firebaseSetData("/lobby/" + game + "/","gameover",true);
        disconnect(firebaseBoardChange, 0, 0, 0);
        firebaseDeleteData("/lobby/" + game + "/");
        firebaseDeleteData("/boards/" + myUser + "/");
    }
    alreadyShotOne = false;
}

void GameVsPlayer::deleteAllOnClicks()
{
    for( int i = 0; i < 100; ) {
        disconnect(qLabelEnemyFields[i++], 0, 0, 0);
    }
}

void GameVsPlayer::playerRemoveAdjacentFields()
{
    QPalette ppudlo;
    ppudlo.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/pudlo.png"));
    int temp;
        for( int i = 0; i < shipFieldsShotByPlayer.size(); ) {
            if( checkIfSunk(enemyShipTable, shipFieldsShotByPlayerTypes.at(i) ) ) {
                temp = shipFieldsShotByPlayer.at(i)-10;
                if( temp > -1 && oldEnemyShipTable[temp] == 0 && enemyShipTable[temp] == 0 ) {
                    qLabelEnemyFields[temp]->setPalette(ppudlo);
                    disconnect(qLabelEnemyFields[temp], 0, 0, 0);
                    firebaseSetData("/boards/"+enemyUser+"/board/",QString::number(temp),4);
                }

                temp = shipFieldsShotByPlayer.at(i)-9;
                if( temp > -1 && ( temp/10 == (shipFieldsShotByPlayer.at(i)-10)/10 ) && oldEnemyShipTable[temp] == 0 && enemyShipTable[temp] == 0 ) {
                    qLabelEnemyFields[temp]->setPalette(ppudlo);
                    disconnect(qLabelEnemyFields[temp], 0, 0, 0);
                    firebaseSetData("/boards/"+enemyUser+"/board/",QString::number(temp),4);
                }

                temp = shipFieldsShotByPlayer.at(i)+1;
                if( temp < 100 && ( temp/10 == shipFieldsShotByPlayer.at(i)/10 ) && oldEnemyShipTable[temp] == 0 && enemyShipTable[temp] == 0 ) {
                    qLabelEnemyFields[temp]->setPalette(ppudlo);
                    disconnect(qLabelEnemyFields[temp], 0, 0, 0);
                    firebaseSetData("/boards/"+enemyUser+"/board/",QString::number(temp),4);
                }

                temp = shipFieldsShotByPlayer.at(i)+11;
                if( temp < 100 && ( temp/10 == (shipFieldsShotByPlayer.at(i)+10)/10 ) && oldEnemyShipTable[temp] == 0 && enemyShipTable[temp] == 0 ) {
                    qLabelEnemyFields[temp]->setPalette(ppudlo);
                    disconnect(qLabelEnemyFields[temp], 0, 0, 0);
                    firebaseSetData("/boards/"+enemyUser+"/board/",QString::number(temp),4);
                }

                temp = shipFieldsShotByPlayer.at(i)+10;
                if( temp < 100 && oldEnemyShipTable[temp] == 0 && enemyShipTable[temp] == 0 ) {
                    qLabelEnemyFields[temp]->setPalette(ppudlo);
                    disconnect(qLabelEnemyFields[temp], 0, 0, 0);
                    firebaseSetData("/boards/"+enemyUser+"/board/",QString::number(temp),4);
                }

                temp = shipFieldsShotByPlayer.at(i)+9;
                if( temp < 100 && ( temp/10 == (shipFieldsShotByPlayer.at(i)+10)/10 ) && oldEnemyShipTable[temp] == 0 && enemyShipTable[temp] == 0 ) {
                    qLabelEnemyFields[temp]->setPalette(ppudlo);
                    disconnect(qLabelEnemyFields[temp], 0, 0, 0);
                    firebaseSetData("/boards/"+enemyUser+"/board/",QString::number(temp),4);
                }

                temp = shipFieldsShotByPlayer.at(i)-1;
                if( temp > -1 && ( temp/10 == (shipFieldsShotByPlayer.at(i))/10 ) && oldEnemyShipTable[temp] == 0 && enemyShipTable[temp] == 0 ) {
                    qLabelEnemyFields[temp]->setPalette(ppudlo);
                    disconnect(qLabelEnemyFields[temp], 0, 0, 0);
                    firebaseSetData("/boards/"+enemyUser+"/board/",QString::number(temp),4);
                }

                temp = shipFieldsShotByPlayer.at(i)-11;
                if( temp > -1 && ( temp/10 == (shipFieldsShotByPlayer.at(i)-10)/10 ) && oldEnemyShipTable[temp] == 0 && enemyShipTable[temp] == 0 ) {
                    qLabelEnemyFields[temp]->setPalette(ppudlo);
                    disconnect(qLabelEnemyFields[temp], 0, 0, 0);
                    firebaseSetData("/boards/"+enemyUser+"/board/",QString::number(temp),4);
                }
                shipFieldsShotByPlayer.removeAt(i);
                shipFieldsShotByPlayerTypes.removeAt(i);
            }
        }
    }

void GameVsPlayer::periodicallyCheckConnection()
{
    if( !gameover ) {
        firebaseSetData("/boards/"+myUser+"/","timestamp", QDateTime::currentDateTime().toMSecsSinceEpoch() );
    }

    if( !gameover ) {
        firebaseGet = new Firebase(URL, "/boards/" + enemyUser + "/timestamp.json" );
        firebaseGet->getValue();
        connect(firebaseGet,SIGNAL(eventResponseReady(QByteArray)),this,SLOT(onCheckConnection(QByteArray)));

        if( !connectionLost && !gameover ) {
            QTimer::singleShot(10000,this,[=]{periodicallyCheckConnection();});
        }
    }
}

void GameVsPlayer::onCheckConnection(QByteArray data)
{
    disconnect(firebaseGet, 0, 0, 0);

    if ( QDateTime::currentDateTime().toMSecsSinceEpoch() - data.toLongLong() > 30000 ) {
        m1->setTitle("Utracono połączenie z przeciwnikiem!");

        deleteAllOnClicks();

        firebaseDeleteData("/lobby/"+game+"/gameover");
        firebaseDeleteData("/boards/"+myUser+"/board");
        firebaseDeleteData("/boards/"+enemyUser+"/");
        firebaseDeleteData("/lobby/"+game+"/");
        firebaseDeleteData("/boards/"+myUser+"/");

        connectionLost = true;
        gameover = true;
    }
}

void GameVsPlayer::createUserInDatabase()
{
    firebaseTemp = new Firebase(URL, "/boards/.json");
    firebaseTemp->getValue();
    connect(firebaseTemp,SIGNAL(eventResponseReady(QByteArray)),this,SLOT(onUserCreate(QByteArray)));
}

void GameVsPlayer::onUserCreate(QByteArray data) // slot
{
    disconnect(firebaseTemp, 0, 0, 0);
    QJsonObject jsonData = QJsonDocument::fromJson(data).object();
    int i = 0;
    int j = 0;
    int check = 0;
    QString userIteratori = "user" + QString::number(i);
    while( jsonData[userIteratori].isObject() )
    {
        userIteratori = "user" + QString::number(++i); //iteracja pętli
    }

    QJsonArray qJAPlayerShipTable;
    for( int k = 0; k < 100; ) {
        qJAPlayerShipTable.append( playerShipTable[k++] );
    }

    // user nie istnieje, tworzymy nowego usera
    if( jsonData["user0"].isNull() ){
        firebaseSetData("boards/user0","board",qJAPlayerShipTable);
        firebaseSetData("boards/user0","timestamp",QDateTime::currentDateTime().toMSecsSinceEpoch());
        check = 0;
        myUser = "user0";
    }

    // sprawdzamy czas userow
    QString userIteratorj = "user" + QString::number(j);
    while( j < i ) {
        QJsonObject user = jsonData[userIteratorj].toObject();
        double temptimestamp = user["timestamp"].toDouble();
        if( temptimestamp + 1200000 < QDateTime::currentDateTime().toMSecsSinceEpoch() ){ //stary user, przejmujemy go jako nasz
            firebaseSetData("boards/user"+QString::number(j),"board",qJAPlayerShipTable);
            firebaseSetData("boards/user"+QString::number(j),"timestamp",QDateTime::currentDateTime().toMSecsSinceEpoch());
            check = 1;
            myUser = "user" + QString::number(j);
            break;
        }
        userIteratorj = "user" + QString::number(++j); //iteracja pętli
    }

    if( check == 0 ) {
        firebaseSetData("boards/user"+QString::number(i),"board",qJAPlayerShipTable);
        firebaseSetData("boards/user"+QString::number(i),"timestamp",QDateTime::currentDateTime().toMSecsSinceEpoch());
        myUser = "user" + QString::number(i);
    }

    createGameInDatabase();
}

void GameVsPlayer::createGameInDatabase() //tworzymy games
{
    firebaseTemp = new Firebase(URL, "/lobby/.json");
    firebaseTemp->getValue();
    connect(firebaseTemp,SIGNAL(eventResponseReady(QByteArray))
            ,this,SLOT(onGameCreate(QByteArray)));
}

void GameVsPlayer::onGameCreate(QByteArray data) // slot
{
    disconnect(firebaseTemp, 0, 0, 0);
    QJsonObject jsonData = QJsonDocument::fromJson(data).object();
    int i = 0;
    int j = 0;
    int check = 0;
    QString gameIteratori = "game" + QString::number(i);
    while( jsonData[gameIteratori].isObject() )
    {
        gameIteratori = "game" + QString::number(++i); //iteracja pętli
    }

    // game nie istnieje, tworzymy nowego usera
    if( jsonData["game0"].isNull() ) {
        firebaseSetData("lobby/game0","freeToJoin",true);
        firebaseSetData("lobby/game0","gameover",false);
        firebaseSetData("lobby/game0","timestamp",QDateTime::currentDateTime().toMSecsSinceEpoch());
        firebaseSetData("lobby/game0","user1",myUser);
        firebaseSetData("lobby/game0","user2","placeholder");
        firebaseSetData("lobby/game0","whoseTurn",1);
        check = 0;
        whichUserInitializedGame = "myUser";
    }

    // sprawdzamy czas game
    QString gameIteratorj = "game" + QString::number(j);
    while( j < i ) {
        QJsonObject gameTemp = jsonData[gameIteratorj].toObject();
        double temptimestamp = gameTemp["timestamp"].toDouble();

        if( temptimestamp + 1200000 < QDateTime::currentDateTime().toMSecsSinceEpoch() ){ //stary game, przejmujemy go jako nasz
            QString path = "lobby/" + gameIteratorj;
            firebaseSetData( path,"freeToJoin",true );
            firebaseSetData( path,"gameover",false );
            firebaseSetData( path,"timestamp",QDateTime::currentDateTime().toMSecsSinceEpoch() );
            firebaseSetData( path,"user1",myUser );
            firebaseSetData( path,"user2","placeholder" );
            firebaseSetData( path,"whoseTurn",1 );
            game = "game" + QString::number(j);
            whichUserInitializedGame = "myUser";
            check = 1;
            break;
        }

        if( gameTemp["freeToJoin"].toBool() == true ){ //stary game, przejmujemy go jako nasz
            QString path = "lobby/" + gameIteratorj;
            firebaseSetData( path,"freeToJoin",false );
            firebaseSetData( path,"timestamp",QDateTime::currentDateTime().toMSecsSinceEpoch() );
            firebaseSetData( path,"user2",myUser );
            game = "game" + QString::number(j);
            enemyUser = gameTemp["user1"].toString();
            whichUserInitializedGame = "enemyUser";
            check = 1;
            break;
        }

        gameIteratorj = "game" + QString::number(++j); //iteracja pętli
    }

    if( check == 0 ) {
        QString path = "lobby/"+gameIteratorj;
        firebaseSetData( path,"freeToJoin",true );
        firebaseSetData( path,"gameover",false );
        firebaseSetData( path,"timestamp",QDateTime::currentDateTime().toMSecsSinceEpoch() );
        firebaseSetData( path,"user1",myUser );
        firebaseSetData( path,"user2","placeholder" );
        firebaseSetData( path,"whoseTurn",1 );
        game = "game" + QString::number(j);
        whichUserInitializedGame = "myUser";
    }

    firebaseTemp = new Firebase(URL, "/lobby/"+game+"/user2.json");
    firebaseTemp->listenEvents();
    connect(firebaseTemp,SIGNAL(eventPut(QJsonObject)),this,SLOT(onSetEnemyUser(QJsonObject)));
}

void GameVsPlayer::onSetEnemyUser(QJsonObject data)
{

    QString enemy = NULL;
    if( data["data"].isString() ) {
        enemy = data["data"].toString();
    }

    if( enemy.isNull() || enemy == "placeholder" ) { return; }

    disconnect(firebaseTemp, 0, 0, 0);

    if (whichUserInitializedGame == "myUser") { //user, który stworzył lobby
        enemyUser = enemy;
    }

    firebaseTemp = new Firebase(URL, "/boards/" + enemyUser + "/board.json");
    firebaseTemp->getValue();
    connect(firebaseTemp,SIGNAL(eventResponseReady(QByteArray)),this,SLOT(onGetEnemyBoard(QByteArray)));
}

void GameVsPlayer::onGetEnemyBoard(QByteArray data)
{
    disconnect(firebaseTemp, 0, 0, 0);

    QString dataString = data;
    dataString.remove(0, 1);
    dataString.remove(dataString.size()-1, 1);
    QStringList temp = dataString.split( "," );
    for( int i = 0; i < 100; ++i ) {
        enemyShipTable[i] = temp.at(i).toInt();
        oldEnemyShipTable[i] = enemyShipTable[i];
    }
    m1->setTitle("Połączyłeś się z przeciwnikiem");

    if (whichUserInitializedGame == "myUser") {
        m1->setTitle("Połączyłeś się z przeciwnikiem, zaczynasz grę!");
        if ( connectionLost ) {
            connectionLost = false;
            periodicallyCheckConnection();
        }
    }
    else {
        m1->setTitle("Połączyłeś się z przeciwnikiem, ruch przeciwnika!");
        if ( connectionLost ) {
            connectionLost = false;
            QTimer::singleShot(15000,this,[=]{periodicallyCheckConnection();});
        }
    }

    firebaseBoardChange = new Firebase(URL, "/boards/" + myUser + "/board.json");
    firebaseBoardChange->listenEvents();
    connect(firebaseBoardChange,SIGNAL(eventPut(QJsonObject)),this,SLOT(onBoardChange(QJsonObject)));

    firebaseOver = new Firebase(URL, "/lobby/" + game + "/gameover.json");
    firebaseOver->listenEvents();
    connect(firebaseOver,SIGNAL(eventPut(QJsonObject)),this,SLOT(onOver(QJsonObject)));
}

void GameVsPlayer::onBoardChange(QJsonObject data)
{
    QPalette pwoda;
    pwoda.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/woda.png"));
    QPalette ppudlo;
    ppudlo.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/pudlo.png"));
    QPalette ptrafiony;
    ptrafiony.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/trafiony.png"));
    QPalette pstatek;
    pstatek.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/statek.png"));

    QJsonObject temp = data["data"].toObject();
    QList<QString> list = temp.keys();
    for( int i = 0; i < list.size(); ++i ) {
        switch (temp[list.at(i)].toInt()) {
            case 0: //woda
                qLabelPlayerFields[list.at(i).toInt()]->setPalette(pwoda);
                break;
            case 1: //pudło
                qLabelPlayerFields[list.at(i).toInt()]->setPalette(ppudlo);
                audio.playMiss();
                firebaseSetData("/boards/" + myUser + "/board/",QString::number(list.at(i).toInt()),4);
                break;
            case 3: //trafiony statek
                qLabelPlayerFields[list.at(i).toInt()]->setPalette(ptrafiony);
                audio.playHit();
                playerShipTable[list.at(i).toInt()] = 0;
                firebaseSetData("/boards/" + myUser + "/board/",QString::number(list.at(i).toInt()),5);
                break;
            case 4: //pudło stare (nie ma odtwarzać dźwięku)
                qLabelPlayerFields[list.at(i).toInt()]->setPalette(ppudlo);
                break;
            case 5: //trafiony statek stary (nie ma odtwarzać dźwięku)
                qLabelPlayerFields[list.at(i).toInt()]->setPalette(ptrafiony);
                break;
            default:
                qLabelPlayerFields[list.at(i).toInt()]->setPalette(pstatek);
        }
    }
}

void GameVsPlayer::onOver(QJsonObject data)
{
    if( data["data"].isNull() ) { return; }

    if( data["data"].toBool() ) {
        deleteAllOnClicks();
        disconnect(firebaseBoardChange, 0, 0, 0);
        disconnect(firebaseOver, 0, 0, 0);
        QMessageBox::information(NULL,"SYSTEM","Przegrałeś!");
        gameover = true;
        showShips(enemyShipTable, qLabelEnemyFields);
        firebaseDeleteData("/boards/" + myUser + "/");
        firebaseDeleteData("/lobby/" + game + "/gameover");
    }
}


// FIREBASE METODY
template <typename T> void GameVsPlayer::firebaseSetData(QString path, QString name, T value)
{
    QJsonObject jsonObj;
    jsonObj[name] = value;
    QJsonDocument uploadDoc(jsonObj);
    Firebase *firebaseSet = new Firebase(URL, path);
    firebaseSet->setValue(uploadDoc, "PATCH");
}

void GameVsPlayer::firebaseDeleteData(QString path)
{
    QJsonObject jsonObj;
    QJsonDocument uploadDoc(jsonObj);
    Firebase *firebaseDelete = new Firebase(URL, path);
    firebaseDelete->setValue(uploadDoc, "DELETE");
}
