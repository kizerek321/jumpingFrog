#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "conio2.h"
#include <time.h>

#define START_BOARD 3
#define ROAD_HEIGHT 2
#define LEFT -1
#define RIGHT 1
#define X_END_INFO 50
#define Y_END_INFO 20



typedef struct Frog {
	int oldX = 0 , oldY = 0;
    int x = 0, y = 0;
	int direction = 0;
	int jump = 0;
	int jumpHeight = 0;
	bool colision = 0;
	bool win = false;
	int color = GREEN;
	char sign = 'a';
} Frog;

typedef struct Car {
	int y = 0;
	int * x = nullptr;
	int speed = 0;
	int direction = 0;
	int max_lenght = 0;
	int lenght = 0;
	bool colision = 0;
	int movmentType = 0; // 0 - normal 1 - speed changes 2 - dissapearing
	int color = RED;
	char sign = 'a';
	bool dissapear = false;
	bool active = false;
 }Car;

typedef struct Board {
	int width = 0, height = 0;
	int x_end_board = 0, y_end_board = 0;
	int numberOfStreets = 0;
	int firstYRoad = 0;
	int * roadY;
	int numberOfCars = 0;
	int * x_road , *y_road ;
	int spawn_car_x = 0;
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
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		if (!cars[i].active ) {
			cars[i].lenght = rand () % cars[i].max_lenght + 1;
			cars[i].x = new int[cars[i].lenght];
			cars[i].active = true;
			if(cars[i].dissapear ) {
				cars[i].direction = rand () % 2 == 0 ? LEFT : RIGHT;
				if ( cars[i].direction == RIGHT ) {
					for ( int k = cars[i].lenght - 1; k >= 0; k-- ) {
						cars[i].x[k] = ( rand () % board->width ) + START_BOARD + k;
					}
				}
				else {
					for ( int k = 0; k < cars[i].lenght; k++ ) {
						cars[i].x[k] = ( rand () % board->width ) + START_BOARD + k;
					}
				}
				
				cars[i].speed = rand()%3+1;
			}
			else if ( cars[i].lenght % 2 == 0) {
				cars[i].direction = LEFT;
				for ( int k = 0; k < cars[i].lenght; k++ ) {
					cars[i].x[k] = board->spawn_car_x + k;
				}
				cars[i].dissapear = true;
			}
			else {
				cars[i].direction = RIGHT;
				for ( int k = 0; k < cars[i].lenght; k++ ) {
					cars[i].x[k] = board->spawn_car_x - k;
				}
				cars[i].speed = 1.5;
			}
			cars[i].y = board->roadY[i];
		}
	}
}


void moveCar ( Board * board , Car * cars ) {
	static int moveCounter = 0;
	const int moveDelay = 4;    

	moveCounter++;
	if ( moveCounter < moveDelay ) {
		return; 
	}
	moveCounter = 0;
	
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		if ( cars[i].x == nullptr ) {
			continue;
		}
		
		for ( int k = 0; k < cars[i].lenght; k++ ) { //moving
			cars[i].x[k] += cars[i].direction * cars[i].speed;
		}
		for ( int k = 0; k < cars[i].lenght; k++ ) {
			if ( cars[i].dissapear ) {
				if ( cars[i].x[k] >= START_BOARD + board->width - 2 ||
					 cars[i].x[k] <= START_BOARD ) {
					delete[] cars[i].x;
					cars[i].x = nullptr;
					cars[i].y = 0;
					cars[i].lenght = 0;
				}
			}
			else {
				bool hitBorder = false;
				if ( cars[i].x[k] >= START_BOARD + board->width - 2 ||
					 cars[i].x[k] <= START_BOARD) {
					hitBorder = true;
				}
				if ( hitBorder ) {
					if ( cars[i].direction == RIGHT ) {
						cars[i].direction = LEFT;
					}
					else {
						cars[i].direction = RIGHT;
					}
				}
			}
		}
		for ( int k = 1; k < board->width - 1; k++ ) { //clearing street
			gotoxy ( START_BOARD + k , cars[i].y );
			putchar ( ' ' );
		}
	}
}

bool win ( Frog & frog , Board * board ) {
	if ( frog.y == START_BOARD + 1 ) {
		return true;
	}
	return false;
}

bool colision (Frog&frog, Board*board, Car*cars) {
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		for ( int k = 0; k < cars[i].lenght; k++ ) {
			if ( frog.y == cars[i].y && frog.x == cars[i].x[k] ) {
				return true;
			}
		}
	}
	return false;
}

void loadFile (Board*board,Frog*frog,Car*car, Car *& cars ) {
	FILE * file = fopen("config.txt","r");
	if ( file == NULL ) {
		printf("Error while opening file");
		exit ( 1 );
	}
	fscanf ( file , 
			 "%d %d %d %d %d %d %d %d %d %d %c %c %d %d %d" ,
			 &board->width , 
			 &board->height, 
			 &board->numberOfStreets ,
			 &board->numberOfCars ,
			 &frog->x ,
			 &frog->y ,
			 &frog->direction ,
			 &frog->jump ,
			 &frog->jumpHeight ,
			 &frog->colision ,
			 &frog->sign, 
			 &car->sign,
			 &car->max_lenght,
			 &board->firstYRoad,
			 &car->speed);
	fclose ( file );
	board->roadY = new int[board->numberOfStreets];
	for ( int i = 0; i < board->numberOfStreets; i++ ) {
		board->roadY[i] = board->firstYRoad + i * 5;
	}
	board->x_end_board = board->width + START_BOARD;
	board->y_end_board = board->height + START_BOARD;
	board->spawn_car_x = ( board->x_end_board - 1 )/2;
	cars = new Car[board->numberOfCars];
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		cars[i].sign= car->sign;
		cars[i].max_lenght = car->max_lenght;
		cars[i].speed = car->speed;
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
				for ( int l = 0; l < cars[k].lenght; l++ ) {
					if ( i == cars[k].y && j == cars[k].x[l] && cars[i].active ) {
						textcolor ( cars[k].color );
						char sign = cars[k].sign;
						putch ( sign );
					}
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
	while (true && !frog.colision && !frog.win) {
		frog.colision = colision ( frog , board , cars );
		frog.win = win ( frog , board );
		spawnCars (  board , cars );
		moveCar ( board , cars );
		printingBoard ( board , frog , cars,  start_time);
		if ( kbhit () ) { 
			input = getch ();
			if ( input == 'q' ) {
				break; 
			}
			movingFrog ( frog ,board, input );
		}
	}
	clock_t end_time = clock ();
	double gameplay_time = ( double ) ( end_time - start_time ) / CLOCKS_PER_SEC;
	if ( frog.win ) {
		gotoxy (X_END_INFO,Y_END_INFO );
		textcolor ( GREEN );
		printf ( "You win! Your time: %.2f s" , gameplay_time );
	}
	else if ( frog.colision ) {
		gotoxy ( X_END_INFO , Y_END_INFO );
		textcolor ( RED );
		printf ( "You lose! Your time: %.2f s" , gameplay_time );
	}
	textcolor ( BLUE );
	gotoxy ( X_END_INFO , Y_END_INFO + 1 );
	printf ( "Thank you for playing! Press any key to exit..." );
	textcolor ( WHITE );
	getch ();
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
