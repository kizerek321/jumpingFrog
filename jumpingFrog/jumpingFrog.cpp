#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "conio2.h"
#include <time.h>

#define START_BOARD 3
#define ROAD_HEIGHT 2
#define LEFT -1
#define RIGHT 1




typedef struct Frog {
	int oldX = 0 , oldY = 0;
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
	int max_lenght = 0;
	int lenght = 0;
	bool colision = 0;
	int movmentType = 0; // 0 - normal 1 - speed changes 2 - dissapearing
	int color = RED;
	char sign = 'a';
 }Car;

typedef struct Board {
	int width = 0, height = 0;
	int x_end_board = 0, y_end_board = 0;
	int numberOfStreets = 0;
	int firstYRoad = 0;
	int * roadY;
	int numberOfCars = 0; // number of cars on the street
	int * x_road , *y_road ;
}Board;

void movingFrog (Frog&frog, Board*board, char input) {
	frog.oldY = frog.y;
	frog.oldX = frog.x;
	if ( input == 'w'  && frog.y != START_BOARD + 1) {
		frog.y--;
	}
	else if ( input == 's' &&  frog.y != board->y_end_board-2) {
		frog.y++;
	}
	else if ( input == 'a' && frog.x != START_BOARD +1) {
		frog.x--;
	}
	else if ( input == 'd' && frog.x != board->x_end_board -2) {
		frog.x++;
	}
}

void spawnCars ( Board * board , Car * cars ) {
	int lastSpawn = 0;
	int currentTime = clock ();
	if ( ( currentTime - lastSpawn ) / CLOCKS_PER_SEC >= 2 ) {
		for ( int i = 0; i < board->numberOfCars; i++ ) {
			if ( cars[i].x == 0 && cars[i].y == 0 ) { //not active car will have cords 0,0
				cars[i].lenght = rand () % cars[i].max_lenght + 1;
				if ( cars[i].lenght > cars[i].max_lenght / 2 ) {
					cars[i].direction = LEFT;
					cars[i].x = board->x_end_board;
				}
				else {
					cars[i].direction = RIGHT;
					cars[i].x = START_BOARD;
				}
				cars[i].y = board->roadY[i];
				lastSpawn = currentTime;
			}
		}
	}
}


void moveCar ( Board * board , Car * cars ) {
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		if ( cars[i].x != 0 || cars[i].y != 0 ) {
			cars[i].x += cars[i].direction * cars[i].speed;
			if ( cars[i].x >= START_BOARD + board->width - 1 ) {
				cars[i].x = cars[i].y = 0;
			}
			else if ( cars[i].x <= START_BOARD ) {
				cars[i].x = cars[i].y = 0;
			}

		}
	}
}

void loadFile (Board*board,Frog*frog,Car*car, Car *& cars ) {
	FILE * file = fopen("config.txt","r");
	if ( file == NULL ) {
		printf("Error while opening file");
		exit ( 1 );
	}
	fscanf ( file , 
			 "%d %d %d %d %d %d %d %d %d %d %d %c %c %d %d" ,
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
			 &car->sign,
			 &car->max_lenght,
			 &board->firstYRoad);
	fclose ( file );
	board->roadY = new int[board->numberOfStreets];
	for ( int i = 0; i < board->numberOfStreets; i++ ) {
		board->roadY[i] = board->firstYRoad + i * 5;
	}
	board->x_end_board = board->width + START_BOARD;
	board->y_end_board = board->height + START_BOARD;
	cars = new Car[board->numberOfCars];
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		cars[i].sign= car->sign;
		cars[i].max_lenght = car->max_lenght;
	}
}

void printingBoard (Board*board, Frog&frog, Car*cars, clock_t start_time) {
	gotoxy ( 0 , 0 );
	int y = START_BOARD + board->height;
	int x = START_BOARD + board->width;
	
	for ( int i = START_BOARD; i < y; i++ ) {
		for ( int j = START_BOARD; j < x; j++ ) {
			textcolor ( WHITE );
			gotoxy ( j , i );
			if(i == START_BOARD || i == y - 1 || j == START_BOARD || j == x - 1){
				putch ( '*' );
			}
			
			for ( int k = 0; k < board->numberOfStreets; k++ ) {
				if ( i == board->roadY[k] - 1  &&  j<x -1) {
					textcolor ( RED );
					putch( '-' );
				}
				else if ( i == board->roadY[k] + 1 && j < x-1) {
					textcolor ( BLUE );
					putch ( '-' );
				}
			}
			if ( i == frog.y && j == frog.x ) {
				char sign = frog.sign;
				gotoxy ( frog.oldX , frog.oldY );
				putch ( ' ' );
				textcolor ( frog.color );
				gotoxy ( frog.x , frog.y );
				putch ( sign );
			}
			for ( int k = 0; k < board->numberOfCars; k++ ) {
				if ( i == cars[k].y && j == cars[k].x ) {
					textcolor ( cars[k].color );
					char sign = cars[k].sign;
					putch ( sign );
				}
			}
		}
	}
	gotoxy ( 1 , 1 );
	printf ( "press q to exit   " );
	printf ( "w - up, a - left, s - down, d - right\n" );
	textcolor ( RED );
	clock_t current_time = clock ();
	double elapsed_time = ( double ) ( current_time - start_time ) / CLOCKS_PER_SEC;
	printf ( "your time: %.2f s" , elapsed_time );

}

void handelInput ( Board*board , Frog&frog , Car*cars ) {
	char input = 'j';
	clock_t start_time = clock ();
	while (true) {
		spawnCars (  board , cars );
		printingBoard ( board , frog , cars,  start_time);
		if ( kbhit () ) { // Sprawdź, czy naciśnięto klawisz
			input = getch ();
			if ( input == 'q' ) {
				break; // Zakończ grę, jeśli naciśnięto 'q'
			}
			movingFrog ( frog ,board, input );
		}
	}
	clock_t end_time = clock ();
	double gameplay_time = ( double ) ( end_time - start_time ) / CLOCKS_PER_SEC;

}

int main()
{
	srand ( time ( NULL ) );
	Board* board = new Board;
	Frog frog;
	Car* car = new Car;
	Car * cars = nullptr;
	settitle ( "Krzysztof Szudy - Jumping Frog" );
	_setcursortype ( _NOCURSOR );
	loadFile ( board , &frog , car, cars);
	delete car;
	handelInput ( board , frog , cars );
	delete[] cars;
	delete board;
}
