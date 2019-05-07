#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QBuffer>

/*! \file mainwindow.cpp
    \brief Plik źródłowy zawierający klasę MainWindow, obsługujący cały interface użytkownika.
*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMainWindow::showFullScreen();
    QPixmap bkgnd(":/StatkiZasoby/tlo.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);
    QPalette pwodatlo;
    pwodatlo.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/woda_tlo.jpg"));

    ui->pushButton_generateAgain->hide();
    ui->pushButton_startGame->hide();
    ui->gFl->setAutoFillBackground(true); // aby sie dało wyswietlić obrazek w okienku
    ui->gFl->setPalette(pwodatlo);
    ui->gFp->setAutoFillBackground(true);
    ui->gFp->setPalette(pwodatlo);
    ui->gFl->hide(); // ukrycie lewego okna
    ui->gFp->hide(); // ukrycie prawego okna
    ui->menu1->setTitle("");
    ui->menu1->setDisabled(true);
}

void MainWindow::setBoard() {
    QPalette pwoda;
    pwoda.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/woda.png"));

    ui->gFl->show();
    ui->pushButton_generateAgain->show();
    ui->pushButton_startGame->show();

    ClickableLabel *qLabelPlayerFields[100];
    ClickableLabel *qLabelEnemyFields[100];

    for( int i = 0; i < 10; ++i ) {
        for (int j = 0; j < 10; ++j ) {
            ClickableLabel *qLl = new ClickableLabel();
            ui->gridLayout->addWidget(qLl, i, j); // utworzenie statków na planszy
            qLl->setAutoFillBackground(true);
            qLl->setPalette(pwoda);
            qLabelPlayerFields[(i*10)+j] = qLl; // dodanie wskaznikow do tablicy
        }
    }

    for( int i = 0; i < 10; ++i ) {
        for( int j = 0; j < 10; ++j ) {
            ClickableLabel *qLp = new ClickableLabel();
            ui->gridLayout_2->addWidget(qLp, i, j); // utworzenie statków na planszy
            qLp->setAutoFillBackground(true);
            qLp->setPalette(pwoda);
            qLp->setObjectName(QString::number((i*10)+j)); // nadanie name dla QLabela
            connect(qLp, SIGNAL (clicked()), this, SLOT (fieldClick())); // ustawienie onClick
            qLabelEnemyFields[(i*10)+j] = qLp; // dodanie wskaznikow do tablicy
        }
    }

    if( localOrOnline ) {
        gameLocal = new GameVsComputer(qLabelPlayerFields, qLabelEnemyFields); // inicjalizacja gry lokalnej
    } else {
        gameOnline = new GameVsPlayer(qLabelPlayerFields, qLabelEnemyFields, (QMenu*)ui->menu1); // inicjalizacja gry online
    }
}

void MainWindow::fieldClick()
{
    ClickableLabel *field = qobject_cast<ClickableLabel *>(sender()); //trza zrzutować/zcastować by mieć dostęp do fukcji klas dziedziczących QObject
    if( localOrOnline ) {
        gameLocal->playerClick( field->objectName().toInt() );
    } else {
        gameOnline->playerClick( field->objectName().toInt() );
    }
}

MainWindow::~MainWindow()
{
    delete gameLocal;
    delete gameOnline;
    delete ui;
}

void MainWindow::on_actionCloseApp_triggered()
{
    MainWindow::close();
}

void MainWindow::on_actionChangeSound_triggered()
{
    if(ui->actionChangeSound->isChecked()) {
        Audio::setMute(true);
    } else {
        Audio::setMute(false);
    }
}

void MainWindow::on_pushButton_generateAgain_clicked()
{
    if( localOrOnline ) {
        gameLocal->generateAgain();
    } else {
        gameOnline->generateAgain();
    }
}

void MainWindow::on_pushButton_startGame_clicked()
{
    ui->pushButton_generateAgain->hide();
    ui->pushButton_startGame->hide();
    ui->gFp->show();
    if( localOrOnline ) {
        gameLocal->startGame();
    } else {
        gameOnline->startGame();
        ui->menu1->setTitle("Czekaj na połączenie z przeciwnikiem...");
    }
}

void MainWindow::on_actionGameVsComputer_triggered()
{
    localOrOnline = true;
    if( gameLocal != NULL ) {
        delete gameLocal;
        gameLocal = NULL;
    }
    if( gameOnline != NULL ) {
        delete gameOnline;
        gameOnline = NULL;
    }
    qDeleteAll(ui->gFl->findChildren<ClickableLabel*>());
    qDeleteAll(ui->gFp->findChildren<ClickableLabel*>());
    ui->gFp->hide();
    setBoard();
}

void MainWindow::on_actionGameVsPlayer_triggered()
{
    localOrOnline = false;
    if( gameLocal != NULL ) {
        delete gameLocal;
        gameLocal = NULL;
    }
    if( gameOnline != NULL ) {
        delete gameOnline;
        gameOnline = NULL;
    }
    qDeleteAll(ui->gFl->findChildren<ClickableLabel*>());
    qDeleteAll(ui->gFp->findChildren<ClickableLabel*>());
    ui->gFp->hide();
    setBoard();
}
