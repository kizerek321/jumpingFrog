#include <iostream>
#include "conio2.h"

struct Frog {
    int x = 40 , y = 25;
	int speed;
	int direction;
	int jump;
	int jumpHeight;
	bool colision;
	int color = GREEN;
};

struct Car {
	int x , y;
	int speed;
	int direction;
	int typeOfCar; // 0 - friendly, 1 - enemy
	bool colision;
	int movmentType; // 0 - normal 1 - speed changes 2 - dissapearing
	int friendlycolor = BLUE;
	int enemycolor = RED;
 };

struct Board {
	int width = 80 , height = 25;
	int roadHeight, roadWidth;
	int numberOfStreets =5;
	int numberOfCars = 4; // number of cars on the street
};

void startingStage (Board &board, Frog&frog) {

}

int main()
{
	Board board;
	Frog frog;
	settitle ( "Krzysztof Szudy - Jumping Frog" );
	_setcursortype ( _NOCURSOR );
	textmode ( C4350 );
	char input = 'a';
	while ( input != 'q' ) {
		clrscr ();
		gotoxy ( 1 , 1 );
		std::cout << "Press q to quit";
		gotoxy ( frog.x , frog.y );
		textcolor ( frog.color );
		std::cout << "F";
		input = getch ();
	}
}
