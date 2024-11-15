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
	int oldX = 100 , oldY = 0;
    int x = 0, y = 0;
	int direction = 0;
	int jump = 0;
	int jumpHeight = 0;
	bool colision = 0;
	bool win = false;
	int color = GREEN;
	char sign = 'a';
} Frog;

typedef struct Tree {
	int x = 0 , y = 0;
	int color = LIGHTGREEN;
	int sign = 'a';
	int rangeX = 0;
	bool colision = false;
};

typedef struct Bush {
	int x = 0 , y = 0;
	int color = LIGHTGREEN;
	int sign = 'a';
	int rangeX = 0;
	int * rangeY;
	bool colision = false;
};
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
	int spawn_car_x = 0;
	int numberOfBush = 0;
	int numberOfTrees = 0;
}Board;

bool canGo ( Frog & frog , Board * board , char input , Tree * trees , Bush * bushes ) {
	int newX = frog.x;
	int newY = frog.y;

	if ( input == 'w' ) {
		newY--;
	}
	else if ( input == 's' ) {
		newY++;
	}
	else if ( input == 'a' ) {
		newX--;
	}
	else if ( input == 'd' ) {
		newX++;
	}
	if ( newY == START_BOARD || newY == board->y_end_board - 1 ||
		 newX == START_BOARD || newX == board->x_end_board - 1 ) {
		return false;
	}
	for ( int i = 0; i < board->numberOfTrees; i++ ) {
		if ( newX == trees[i].x && newY == trees[i].y ) {
			return false;
		}
	}
	for ( int i = 0; i < board->numberOfBush; i++ ) {
		if ( newX == bushes[i].x && newY == bushes[i].y ) {
			return false;
		}
	}
	return true;
}

void movingFrog ( Frog & frog , Board * board , char input , Tree * trees , Bush * bushes ) {
	if ( canGo ( frog , board , input , trees , bushes ) ) {
		frog.oldY = frog.y;
		frog.oldX = frog.x;
		if ( input == 'w'  ) {
			frog.y--;
		}
		else if ( input == 's' ) {
			frog.y++;
		}
		else if ( input == 'a' ) {
			frog.x--;
		}
		else if ( input == 'd') {
			frog.x++;
		}
	}
}


void spawnCars ( Board * board , Car * cars ) {
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		if (!cars[i].active ) {
			cars[i].lenght = rand () % cars[i].max_lenght + 1;
			cars[i].x = new int[cars[i].lenght];
			cars[i].active = true;
			cars[i].speed = rand () % 3 + 1;
			cars[i].y = board->roadY[i];
			if(!cars[i].dissapear )
				cars[i].dissapear = rand () % 2 == 0 ? true : false;
			cars[i].direction = rand () % 2 == 0 ? LEFT : RIGHT;
			int start = ( cars[i].direction == RIGHT ) ? START_BOARD + cars[i].lenght : START_BOARD + board->width - cars[i].lenght - 1;

			if ( cars[i].dissapear ) {
				if ( cars[i].direction == RIGHT ) {
					for ( int k = 0; k < cars[i].lenght; k++ ) {
						cars[i].x[k] = start - k;
					}
				}
				else {
					for ( int k = cars[i].lenght - 1; k >= 0; k-- ) {
						cars[i].x[k] = start + k;
					}
				}
			}
			else {
				cars[i].direction = ( rand () % 2 == 0 ) ? LEFT : RIGHT;

				for ( int k = 0; k < cars[i].lenght; k++ ) {
					cars[i].x[k] = start + ( cars[i].direction == RIGHT ? k : -k );
				}
			}
		}
	}
}


void moveCar ( Board * board , Car * cars ) {
	
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		
		if ( !cars[i].active) {
			continue;
		}
		for ( int k = 1; k < board->width - 1; k++ ) { //clearing street
			int posX = START_BOARD + k;
			if ( posX != START_BOARD && posX != board->width + START_BOARD - 1 ) {
				gotoxy ( posX , cars[i].y );
				putchar ( ' ' );
			}
		}
		for ( int k = 0; k < cars[i].lenght; k++ ) { //moving
			cars[i].x[k] += cars[i].direction * cars[i].speed;
		}
		bool hitBorder = false;
		for ( int k = 0; k < cars[i].lenght; k++ ) {
			if ( cars[i].x[k] >= START_BOARD + board->width - 1 || cars[i].x[k] <= START_BOARD ) {
				hitBorder = true;
				break;
			}
		}
		if ( hitBorder ) {
			if ( cars[i].dissapear ) {
					delete[] cars[i].x;
					cars[i].x = nullptr;
					cars[i].y = 0;
					cars[i].lenght = 0;
					cars[i].active = false;
			}
			else {
				cars[i].direction = -cars[i].direction;//riverse direction 
				int position = ( cars[i].direction == RIGHT ) ? START_BOARD + cars[i].lenght : START_BOARD + board->width - cars[i].lenght - 1;
				for ( int k = 0; k < cars[i].lenght; k++ ) {
					cars[i].x[k] = position + ( cars[i].direction == RIGHT ? -k : k );
				}
			}
		}
	}
}

void spawnTreeBush ( Board * board , Tree * trees , Bush * bushes ) {
	for ( int k = 0; k < board->numberOfTrees; k++) {
		bool validPos = false;
		while ( !validPos ) {
			trees[k].y = rand() % ( board->height - 2 ) + START_BOARD + 2;
			validPos = true;
			for ( int i = 0; i < board->numberOfStreets; i++ ) {
				if ( trees[k].y == board->roadY[i] || trees[k].y == board->roadY[i] + 1 ||
					 trees[k].y == board->roadY[i] - 1 ) {
					validPos = false;
					break;
				}
			}
			trees[k].x = rand () % ( board->width - 2 ) + START_BOARD + 1;

		}
	}

	for ( int k = 0; k < board->numberOfBush; k++ ) {
		bool validPos = false;
		while ( !validPos ) {
			bushes[k].y = rand () % ( board->height - 2 ) + START_BOARD + 2;
			validPos = true;
			for ( int i = 0; i < board->numberOfStreets; i++ ) {
				if ( bushes[k].y == board->roadY[i] || bushes[k].y == board->roadY[i] + 1 ||
					 bushes[k].y == board->roadY[i] - 1 ) {
					validPos = false;
					break;
				}
			}
			bushes[k].x = rand () % ( board->width - 2 ) + START_BOARD + 1;

		}
	}
}

bool win ( Frog & frog , Board * board ) {
	if ( frog.y == START_BOARD + 1 ) {
		return true;
	}
	return false;
}

bool colisionWithCar (Frog&frog, Board*board, Car*cars) {
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		for ( int k = 0; k < cars[i].lenght; k++ ) {
			if ( frog.y == cars[i].y && frog.x == cars[i].x[k] ) {
				return true;
			}
		}
	}
	return false;
}

void loadFile (Board*board,Frog*frog,Car*car, Car *& cars ,Tree*tree, Bush*bush, Tree*&trees, Bush*&bushes) {
	FILE * file = fopen("config.txt","r");
	if ( file == NULL ) {
		printf("Error while opening file");
		exit ( 1 );
	}
	fscanf ( file ,
			 "%d %d %d %d %d %d %d %d %d %d %c %c %d %d %d %d %c %d %c" ,
			 &board->width ,
			 &board->height ,
			 &board->numberOfStreets ,
			 &board->numberOfCars ,
			 &frog->x ,
			 &frog->y ,
			 &frog->direction ,
			 &frog->jump ,
			 &frog->jumpHeight ,
			 &frog->colision ,
			 &frog->sign ,
			 &car->sign ,
			 &car->max_lenght ,
			 &board->firstYRoad ,
			 &car->speed,
			 &board->numberOfTrees,
			 &tree->sign,
			 &board->numberOfBush,
			 &bush->sign
	);
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
	trees = new Tree[board->numberOfTrees];
	bushes = new Bush[board->numberOfBush];
	for ( int i = 0; i < board->numberOfTrees; i++ ) {
		trees[i].sign = tree->sign;
	}
	for ( int i = 0; i < board->numberOfBush; i++ ) {
		bushes[i].sign = bush->sign;

	}
}

void printingStaticBoard(Board*board, Tree*trees, Bush*bushes){
	gotoxy ( 0 , 0 );
	int y = START_BOARD + board->height;
	int x = START_BOARD + board->width;

	for ( int i = START_BOARD; i < y; i++ ) {
		for ( int j = START_BOARD; j < x; j++ ) {
			textcolor ( WHITE );
			gotoxy ( j , i );
			if ( i == START_BOARD || i == y - 1 || j == START_BOARD || j == x - 1 ) {
				putch ( '*' );
			}
			for ( int k = 0; k < board->numberOfStreets; k++ ) {
				if ( i == board->roadY[k] - 1 && j < x - 1 ) {
					textcolor ( RED );
					putch ( '-' );
				}
				else if ( i == board->roadY[k] + 1 && j < x - 1 ) {
					textcolor ( BLUE );
					putch ( '-' );
				}
			}
			for ( int k = 0; k < board->numberOfBush; k++ ) {
				if ( i == bushes[k].y && j == bushes[k].x ) {
					textcolor ( bushes[k].color );
					putch ( bushes[k].sign );
				}
			}
			for ( int k = 0; k < board->numberOfTrees; k++ ) {
				if ( i == trees[k].y && j == trees[k].x ) {
					textcolor ( trees[k].color );
					putch ( trees[k].sign );
				}
			}
		}
	}
	gotoxy ( 1 , 1 );
	printf ( "press q to exit   " );
	printf ( "w - up, a - left, s - down, d - right" );
}
void printingFrogCars (Board*board, Frog&frog, Car*cars, clock_t start_time) {
	textcolor ( frog.color );
	gotoxy ( frog.x , frog.y );
	putch ( frog.sign );
	gotoxy ( frog.oldX , frog.oldY );
	putch ( ' ' );
	for ( int k = 0; k < board->numberOfStreets; k++ ) {
		gotoxy ( frog.oldX , frog.oldY );
		if ( frog.oldY == board->roadY[k] - 1 ) {
			textcolor ( RED );
			putch ( '-' );
		}
		else if ( frog.oldY == board->roadY[k] + 1 ) {
			textcolor ( BLUE );
			putch ( '-' );
		}
	}
	for ( int k = 0; k < board->numberOfCars; k++ ) {
			for ( int l = 0; l < cars[k].lenght; l++ ) {
				if ( cars[k].x[l] > START_BOARD && cars[k].x[l] < board->width + START_BOARD - 1 ) {
					textcolor ( cars[k].color );
					gotoxy ( cars[k].x[l] , cars[k].y );
					putch ( cars[k].sign );
				}
			}
	}
	gotoxy ( 1 , 2 );
	textcolor ( RED );
	clock_t current_time = clock ();
	double elapsed_time = ( double ) ( current_time - start_time ) / CLOCKS_PER_SEC;
	printf ( "your time: %.2f s" , elapsed_time );

}

void handelInput ( Board*board , Frog&frog , Car*cars, Tree*trees, Bush*bushes ) {
	const int frameDelayFrog = 300;
	const int frameDelayCar = 500;
	bool keyHeld = false;
	char input = 'j';
	clock_t lastFrogMove = clock ();
	clock_t lastCarMove = clock ();
	clock_t start_time = clock ();
	spawnTreeBush ( board , trees , bushes );
	printingStaticBoard ( board, trees, bushes );
	while (true && !frog.colision && !frog.win) {
		clock_t current_time = clock ();
		double elapsedFrogTime = ( double ) ( current_time - lastFrogMove ) / CLOCKS_PER_SEC * 1000; // ms
		double elapsedCarTime = ( double ) ( current_time - lastCarMove ) / CLOCKS_PER_SEC * 1000;   // ms
		frog.colision = colisionWithCar ( frog , board , cars );
		frog.win = win ( frog , board );
		spawnCars (  board , cars );
		if(elapsedCarTime>=frameDelayCar ){
			moveCar ( board , cars );
			lastCarMove = clock ();
		}
		printingFrogCars ( board , frog , cars,  start_time);
		if ( kbhit () ) {
			if ( !keyHeld ) {
				input = getch ();
				if ( input == 'q' ) {
					break;
				}
				movingFrog ( frog , board , input , trees, bushes);
				lastFrogMove = clock ();
			}
			keyHeld = true;
		}
		else {
			keyHeld = false;
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
	Tree * tree = new Tree;
	Bush * bush = new Bush;
	Car * cars = nullptr;
	Tree * trees = nullptr;
	Bush * bushes = nullptr;
	settitle ( "Krzysztof Szudy - Jumping Frog" );
	_setcursortype ( _NOCURSOR );
	loadFile ( board , &frog , car, cars,tree, bush, trees, bushes);
	delete car;
	delete tree , bush;
	handelInput ( board , frog , cars, trees, bushes );
	delete[] cars;
	delete board;
}
