#include "createship.h"
#include <cstdlib>
#include <ctime>

/*! \file createship.cpp
	\brief Plik źródłowy zawierający klasę CreateShip, odpowiadający za obsługę tworzenia statków.
*/

CreateShip::CreateShip()
{
	srand(time(NULL));
	whoShoots = 0;
}

void CreateShip::resetShipTable( int shipTable[] )
{
    for( int i = 0; i < 100; ) {
        shipTable[i++] = 0;
	}
}

bool CreateShip::checkIfFieldIsEmpty( int id, int shipTable[], int direction )
{
	switch( direction ) {
		case 0: /*!< Po prawej */
            if( id - 9  > -1  && shipTable[id-9]  != 0 ) { return false; }
            if( id + 1  < 100 && shipTable[id+1]  != 0 ) { return false; }
            if( id + 11 < 100 && shipTable[id+11] != 0 ) { return false; }
            return true;
			break;
		case 1: /*!< Po lewej */
            if( id + 10 < 100 && shipTable[id+10] != 0 ) { return false; }
            if( id + 9  < 100 && shipTable[id+9]  != 0 ) { return false; }
            if( id + 11 < 100 && shipTable[id+11] != 0 ) { return false; }
            return true;
			break;
		case 2: /*!< Na dole */
            if( id - 11 > -1  && shipTable[id-11] != 0 ) { return false; }
            if( id + 9  < 100 && shipTable[id+9]  != 0 ) { return false; }
            if( id - 1  > -1  && shipTable[id-1]  != 0 ) { return false; }
            return true;
			break;
		case 3: /*!< U góry */
            if( id - 11 > -1 && shipTable[id-11] != 0 ) { return false; }
            if( id - 10 > -1 && shipTable[id-10] != 0 ) { return false; }
            if( id - 9  > -1 && shipTable[id-9]  != 0 ) { return false; }
            return true;
			break;
	}
	return false;
}

bool CreateShip::is3x3Empty( int id, int shipTable[] )
{
	if( shipTable[id] != 0 ) { return false; }
    if( id - 11 > -1  && shipTable[id-11] != 0 ) { return false; }
    if( id - 10 > -1  && shipTable[id-10] != 0 ) { return false; }
    if( id - 9  > -1  && shipTable[id-9]  != 0 ) { return false; }
    if( id - 1  > -1  && shipTable[id-1]  != 0 ) { return false; }
    if( id + 1  < 100 && shipTable[id+1]  != 0 ) { return false; }
    if( id + 9  < 100 && shipTable[id+9]  != 0 ) { return false; }
    if( id + 10 < 100 && shipTable[id+10] != 0 ) { return false; }
    if( id + 11 < 100 && shipTable[id+11] != 0 ) { return false; }
	return true;
}

bool CreateShip::generateShip4( int shipTable[], int id )
{
	int direction = ( std::rand() % 4 ); /*!< 0 prawo, 1 dół, 2 lewo, 3 góra */

	if( !is3x3Empty(id, shipTable) ) { return true; }

	if( direction == 0 ) { /*!< w prawo */
		int id2 = id+1;
		if( id2 > 99 ) { return true; }
		if( (id2/10) == (id/10) && checkIfFieldIsEmpty(id2, shipTable, direction) ) { //czy 1 w prawo jest ok?
			int id3 = id2+1;
			if( id3 > 99 ) { return true; }
			if( (id3/10) == (id2/10) && checkIfFieldIsEmpty(id3, shipTable, direction) ) { //czy 2 w prawo jest ok?
				int id4 = id3+1;
				if( id4 > 99 ) { return true; }
				if( (id4/10) == (id3/10) && checkIfFieldIsEmpty(id4, shipTable, direction) ) { //czy 3 w prawo jest ok?
                    shipTable[id]  = 41;
					shipTable[id2] = 41;
					shipTable[id3] = 41;
					shipTable[id4] = 41;
					return false;
				}
			}
		}
	} else if( direction == 1 ) { //w dół
		int id2 = id+10;
		if( id2 > 99 ) { return true; }
		if( checkIfFieldIsEmpty(id2, shipTable, direction) ) { //czy 1 w dół jest ok?
			int id3 = id2+10;
			if( id3 > 99 ) { return true; }
			if( checkIfFieldIsEmpty(id3, shipTable, direction) ) { //czy 2 w dół jest ok?
				int id4 = id3+10;
				if( id4 > 99 ) { return true; }
				if( checkIfFieldIsEmpty(id4, shipTable, direction) ) { //czy 3 w dół jest ok?
                    shipTable[id]  = 41;
					shipTable[id2] = 41;
					shipTable[id3] = 41;
					shipTable[id4] = 41;
					return false;
				}
			}
		}
	} else if( direction == 2 ) { //w lewo
		int id2 = id-1;
		if( id2 < 0 ) { return true; }
		if( (id2/10) == (id/10) && checkIfFieldIsEmpty(id2, shipTable, direction) ) { //czy 1 w lewo jest ok?
			int id3 = id2-1;
			if( id3 < 0 ) { return true; }
			if( (id3/10) == (id2/10) && checkIfFieldIsEmpty(id3, shipTable, direction) ) { //czy 2 w lewo jest ok?
				int id4 = id3-1;
				if( id4 < 0 ) { return true; }
				if( (id4/10) == (id3/10) && checkIfFieldIsEmpty(id4, shipTable, direction) ) { //czy 3 w lewo jest ok?
                    shipTable[id]  = 41;
					shipTable[id2] = 41;
					shipTable[id3] = 41;
					shipTable[id4] = 41;
					return false;
				}
			}
		}
	} else { //w góre
		int id2 = id-10;
		if( id2 < 0 ) { return true; }
		if( checkIfFieldIsEmpty(id2, shipTable, direction) ) { //czy 1 w góre jest ok?
			int id3 = id2-10;
			if( id3 < 0 ) { return true; }
			if( checkIfFieldIsEmpty(id3, shipTable, direction) ) { //czy 2 w góre jest ok?
				int id4 = id3-10;
				if( id4 < 0 ) { return true; }
				if( checkIfFieldIsEmpty(id4, shipTable, direction) ) { //czy 3 w góre jest ok?
                    shipTable[id]  = 41;
					shipTable[id2] = 41;
					shipTable[id3] = 41;
					shipTable[id4] = 41;
					return false;
				}
			}
		}
	}
	return true;
}

bool CreateShip::generateShip3( int shipTable[], int id, int shipNumber)
{
	int direction = ( std::rand() % 4 ); //0 prawo, 1 dół, 2 lewo, 3 góra

	if( !is3x3Empty(id, shipTable) ) { return true; }

	if( direction == 0 ) { //w prawo
		int id2 = id+1;
		if( id2 > 99 ) { return true; }
		if( (id2/10) == (id/10) && checkIfFieldIsEmpty(id2, shipTable, direction) ) { //czy 1 w prawo jest ok?
			int id3 = id2+1;
			if( id3 > 99 ) { return true; }
			if( (id3/10) == (id2/10) && checkIfFieldIsEmpty(id3, shipTable, direction) ) { //czy 2 w prawo jest ok?
                shipTable[id]  = 30 + shipNumber;
				shipTable[id2] = 30 + shipNumber;
				shipTable[id3] = 30 + shipNumber;
				return false;
			}
		}
	} else if( direction == 1 ) { //w dół
		int id2 = id+10;
		if( id2 > 99 ) { return true; }
		if( checkIfFieldIsEmpty(id2, shipTable, direction) ) { //czy 1 w dół jest ok?
			int id3 = id2+10;
			if( id3 > 99 ) { return true; }
			if( checkIfFieldIsEmpty(id3, shipTable, direction) ) { //czy 2 w dół jest ok?
                shipTable[id]  = 30 + shipNumber;
				shipTable[id2] = 30 + shipNumber;
				shipTable[id3] = 30 + shipNumber;
				return false;
			}
		}
	} else if( direction == 2 ) { //w lewo
		int id2 = id-1;
		if( id2 < 0 ) { return true; }
		if( (id2/10) == (id/10) && checkIfFieldIsEmpty(id2, shipTable, direction) ) { //czy 1 w lewo jest ok?
			int id3 = id2-1;
			if( id3 < 0 ) { return true; }
			if( (id3/10) == (id2/10) && checkIfFieldIsEmpty(id3, shipTable, direction) ) { //czy 2 w lewo jest ok?
                shipTable[id]  = 30 + shipNumber;
				shipTable[id2] = 30 + shipNumber;
				shipTable[id3] = 30 + shipNumber;
				return false;
			}
		}
	} else { //w góre
		int id2 = id-10;
		if( id2 < 0 ) { return true; }
		if( checkIfFieldIsEmpty(id2, shipTable, direction) ) { //czy 1 w góre jest ok?
			int id3 = id2-10;
			if( id3 < 0 ) { return true; }
			if( checkIfFieldIsEmpty(id3, shipTable, direction) ) { //czy 2 w góre jest ok?
                shipTable[id]  = 30 + shipNumber;
				shipTable[id2] = 30 + shipNumber;
				shipTable[id3] = 30 + shipNumber;
				return false;
			}
		}
	}
	return true;
}

bool CreateShip::generateShip2( int shipTable[], int id, int shipNumber )
{
	int direction = ( std::rand() % 4 ); //0 prawo, 1 dół, 2 lewo, 3 góra

	if( !is3x3Empty(id, shipTable) ) { return true; }

	if( direction == 0 ) { //w prawo
		int id2 = id+1;
		if( id2 > 99 ) { return true; }
		if( (id2/10) == (id/10) && checkIfFieldIsEmpty(id2, shipTable, direction) ) { //czy 1 w prawo jest ok?
            shipTable[id]  = 20 + shipNumber;
			shipTable[id2] = 20 + shipNumber;
			return false;
		}
	} else if( direction == 1 ) { //w dół
		int id2 = id+10;
		if( id2 > 99 ) { return true; }
		if( checkIfFieldIsEmpty(id2, shipTable, direction) ) { //czy 1 w dół jest ok?
            shipTable[id]  = 20 + shipNumber;
			shipTable[id2] = 20 + shipNumber;
			return false;
		}
	} else if( direction == 2 ) { //w lewo
		int id2 = id-1;
		if( id2 < 0 ) { return true; }
		if( (id2/10) == (id/10) && checkIfFieldIsEmpty(id2, shipTable, direction) ) { //czy 1 w lewo jest ok?
            shipTable[id]  = 20 + shipNumber;
			shipTable[id2] = 20 + shipNumber;
			return false;
		}
	} else { //w góre
		int id2 = id-10;
		if( id2 < 0 ) { return true; }
		if( checkIfFieldIsEmpty(id2, shipTable, direction) ) { //czy 1 w góre jest ok?
            shipTable[id]  = 20 + shipNumber;
			shipTable[id2] = 20 + shipNumber;
			return false;
		}
	}
	return true;
}

bool CreateShip::generateShip1( int shipTable[], int id, int shipNumber )
{
	if( !is3x3Empty(id, shipTable) ) { return true; }
	shipTable[id] = 10 + shipNumber;
	return false;
}

void CreateShip::shipGenerator( int shipTable[] )
{
	int num;

	//1 statek 4-polowy
	do { num = ( std::rand() % 100 ); }
	while( generateShip4( shipTable, num ) );

	//2 statki 3-polowe
	do { num = ( std::rand() % 100 ); }
	while( generateShip3( shipTable, num, 1 ) );
	do { num = ( std::rand() % 100 ); }
	while( generateShip3( shipTable, num, 2 ) );

	//3 statki 2-polowe
	do { num = ( std::rand() % 100 ); }
	while( generateShip2( shipTable, num, 1 ) );
	do { num = ( std::rand() % 100 ); }
	while( generateShip2( shipTable, num, 2 ) );
	do { num = ( std::rand() % 100 ); }
	while( generateShip2( shipTable, num, 3 ) );

	//4 statki 1-polowe
	do { num = ( std::rand() % 100 ); }
	while( generateShip1( shipTable, num, 1 ) );
	do { num = ( std::rand() % 100 ); }
	while( generateShip1( shipTable, num, 2 ) );
	do { num = ( std::rand() % 100 ); }
	while( generateShip1( shipTable, num, 3 ) );
	do { num = ( std::rand() % 100 ); }
	while( generateShip1( shipTable, num, 4 ) );
}

bool CreateShip::checkTable( int table[] ) //sprawdza czy tablica ma tylko zera
{
    for( int i = 0; i < 100; ) {
        if( table[i++] > 0 ) { //ma jakieś liczby
            return false;
        }
	}
	return true; //ma tylko zera
}

bool CreateShip::checkIfSunk( int shipTable[], int id ) //sprawdza czy statek o ID zostal zatopiony
{
	for( int i = 0; i < 100; ) {
        if( shipTable[i++] == id ) {
            return false;
        }
	}
	return true;
}

void CreateShip::showShips( int shipTable[], ClickableLabel *pointerShipTable[] ) //wyświetla wszystkie statki w danej tablicy
{
	QPalette pstatek;
    pstatek.setBrush(QPalette::Window, QPixmap(":/StatkiZasoby/statek.png"));
	for( int i = 0 ; i < 100 ; ++i ) {
		if( shipTable[i] > 0 ) {
			pointerShipTable[i]->setPalette(pstatek);
		}
	}
}
