#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "conio2.h"

#define START_BOARD 5





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
	int numberOfStreets = 0;
	int numberOfCars = 0; // number of cars on the street
	int * x_road , *y_road ;
}Board;

void loadFile (Board*board,Frog*frog,Car*car) {
	FILE * file = fopen("config.txt","r");
	if ( file == NULL ) {
		std::cout << "Error while opening file";
		exit ( 1 );
	}
	fscanf ( file , 
			 "%d %d %d %d %d %d %d %d %d %d %d %c %c" ,
			 &board->width , 
			 &board->height, 
			 &board->numberOfStreets ,
			 &board->numberOfCars ,
			 &frog->x ,
			 &frog->y ,
			 &frog->speed ,
			 &frog->direction ,
			 &frog->jump ,
			 &frog->jumpHeight ,
			 &frog->colision ,
			 &frog->sign, 
			 &car->sign);
	fclose ( file );
}

void printingBoard (Board&board, Frog&frog, Car&car) {
	int y = START_BOARD + board.height;
	int x = START_BOARD + board.width;
	int *
	for ( int i = START_BOARD; i < y; i++ ) {
		for ( int j = START_BOARD; j < x; j++ ) {
			gotoxy ( i , j );
			if(i == START_BOARD || i == y - 1 || j == START_BOARD || j == x - 1){
				printf ( "*" );
			}
		}
	}

}
void startingStage (Board &board, Frog&frog) {

}

int main()
{
	srand ( time ( NULL ) );
	Board board;
	Frog frog;
	Car car;
	settitle ( "Krzysztof Szudy - Jumping Frog" );
	_setcursortype ( _NOCURSOR );
	textmode ( C4350 );
	loadFile ( &board , &frog , &car);
	board.x_road = new int [board.numberOfStreets];
	board.y_road = new int[board.numberOfStreets];
	for ( int i = 0; i < board.numberOfStreets; i++ ) {
		board.x_road[i] = board.y_road[i] = rand () % 3 + 1;
	}
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
	delete[] board.x_road;
	delete[] board.y_road;
}
