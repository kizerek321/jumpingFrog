#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "conio2.h"


typedef struct Frog {
    int x = 0, y = 0;
	int speed = 0;
	int direction = 0;
	int jump = 0;
	int jumpHeight = 0;
	bool colision = 0;
	int color = GREEN;
	char sign = 'a';
} Frog;

typedef struct Car {
	int x = 0, y = 0;
	int speed = 0;
	int direction = 0;
	int typeOfCar = 0; // 0 - friendly, 1 - enemy
	bool colision = 0;
	int movmentType = 0; // 0 - normal 1 - speed changes 2 - dissapearing
	int friendlycolor = BLUE;
	int enemycolor = RED;
	char sign = 'a';
 }Car;

typedef struct Board {
	int width = 0, height = 0;
	int roadHeight = 0, roadWidth = 0;
	int numberOfStreets = 0;
	int numberOfCars = 0; // number of cars on the street
}Board;

void loadFile (Board*board,Frog*frog,Car*car) {
	FILE * file = fopen("config.txt","r");
	if ( file == NULL ) {
		std::cout << "Error while opening file";
		exit ( 1 );
	}
	fscanf ( file , 
			 "%d %d %d %d %d %d %d %d %d %d %d %d %d %c" ,
			 &board->width , 
			 &board->height, 
			 &board->roadHeight ,
			 &board->roadWidth ,
			 &board->numberOfStreets ,
			 &board->numberOfCars ,
			 &frog->x ,
			 &frog->y ,
			 &frog->speed ,
			 &frog->direction ,
			 &frog->jump ,
			 &frog->jumpHeight ,
			 &frog->colision ,
			 &frog->sign );
	fclose ( file );
}
void startingStage (Board &board, Frog&frog) {

}

int main()
{
	Board board;
	Frog frog;
	Car car;
	settitle ( "Krzysztof Szudy - Jumping Frog" );
	_setcursortype ( _NOCURSOR );
	textmode ( C4350 );
	loadFile ( &board , &frog , &car);
	char input = 'a';
	while ( input != 'q' ) {
		clrscr ();
		gotoxy ( 1 , 1 );
		std::cout << "Press q to quit";
		gotoxy ( frog.x , frog.y );
		textcolor ( frog.color );
		std::cout << frog.sign;
		input = getch ();
	}
}
