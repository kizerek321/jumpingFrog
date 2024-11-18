#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "conio2.h"
#include <time.h>

#define START_BOARD 2
#define ROAD_HEIGHT 2
#define LEFT -1
#define RIGHT 1
#define X_END_INFO 50
#define Y_END_INFO 20
#define STREET_HEIGHT 3


typedef struct Stork {
	int x = 0 , y = 0;
	int oldX = 0 , oldY = 0;
	int directionX = 0;
	int directionY = 0;
	char sign = 'a';
	int color = LIGHTRED;
	int speed = 0;
	bool eatFrog = false;
	int frameDelay = 0;
}Stork;

typedef struct Frog {
	int oldX = 0 , oldY = 0;
    int x = 0, y = 0;
	bool colision = 0;
	bool win = false;
	int color = GREEN;
	char sign = 'a';
	int frameDelay = 0;
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
	int color = RED;
	char sign = 'a';
	bool dissapear = false;
	bool active = false;
	bool stoping = false;
	bool friendly = false;
	int frameDelayMax = 0;
	int frameDelay = 0;
	clock_t lastMove;
 }Car;

typedef struct Board {
	int width = 0, height = 0;
	int x_end_board = 0, y_end_board = 0;
	int numberOfStreets = 0;
	int firstYRoad = 0;
	int * roadY = nullptr;
	int numberOfCars = 0;
	int spawn_car_x = 0;
	int numberOfBush = 0;
	int numberOfTrees = 0;
	int numberOfCoins = 0;
	int score = 0;
	int numberOfFreeSpace = 0;
	int * freeSpace = nullptr;
	int spaceBetweenRoads = 0;
}Board;

typedef struct Coin {
	int x = 0 , y = 0;
	char sign = 'a';
	bool colected = false;
	int value = 0;
}Coin;

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

bool canFly (Stork&stork, Tree*trees, Bush*bushes, Board*board) {
	int newX = stork.x + stork.directionX * stork.speed;
	int newY = stork.y + stork.directionY * stork.speed;

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
			cars[i].frameDelay = rand() % cars[i].frameDelayMax + cars[i].frameDelayMax;
			cars[i].active = true;
			cars[i].y = board->roadY[i];
			cars[i].friendly = rand () % 2 == 0 ? true : false;
			if ( cars[i].friendly ){
				cars[i].color = LIGHTBLUE;
				cars[i].stoping = rand () % 2 == 0 ? true : false;
				if ( cars[i].stoping )
					cars[i].color = WHITE;
			}
			else {
				cars[i].color = RED;
			}
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

void moveStork ( Stork & stork , Board * board,Frog&frog, Tree*trees, Bush*bushes ) {
	
	if ( frog.x < stork.x ) {
		stork.directionX = -1;
	}
	else if ( frog.x > stork.x ) {
		stork.directionX = 1;
	}
	else {
		stork.directionX = 0;
	}
	if ( frog.y < stork.y ) {
		stork.directionY = -1;
	}
	else if ( frog.y > stork.y ) {
		stork.directionY = 1;
	}
	else {
		stork.directionY = 0;
	}
	if ( canFly ( stork , trees , bushes , board ) ) {
		stork.oldX = stork.x;
		stork.oldY = stork.y;
		stork.x += stork.directionX * stork.speed;
		stork.y += stork.directionY * stork.speed;
	}
}

bool colisionStork ( Frog & frog , Stork & stork ) {
	if ( frog.x == stork.x && frog.y == stork.y ) {
		return true;
	}
	return false;
}

void moveCar ( Board * board , Car * cars, clock_t current_time ) {
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		double elapsedTime = ( double ) ( current_time - cars[i].lastMove ) / CLOCKS_PER_SEC * 1000;   // ms
		if ( elapsedTime >= cars[i].frameDelay ) {
			if ( !cars[i].active ) {
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
					cars[i].friendly = false;
					cars[i].stoping = false;
				}
				else {
					cars[i].direction = -cars[i].direction;//riverse direction 
					int position = ( cars[i].direction == RIGHT ) ? START_BOARD + cars[i].lenght : START_BOARD + board->width - cars[i].lenght - 1;
					for ( int k = 0; k < cars[i].lenght; k++ ) {
						cars[i].x[k] = position + ( cars[i].direction == RIGHT ? -k : k );
					}
				}
			}
			cars[i].lastMove = clock ();
		}
		else {
			break;
		}
	}
}

void spawnTreeBush ( Board * board , Tree * trees , Bush * bushes, Coin*coins ) {
	int index = 0;
	for ( int i = 0; i < board->numberOfTrees; i++ ) {
		index = rand () % board->numberOfFreeSpace;
		trees[i].y = board->freeSpace[index];
		trees[i].x = rand () % ( board->width - 2 ) + START_BOARD + 1;
		if ( coins[i].y == trees[i].y && coins[i].x == trees[i].x ) {
			trees[i].x++;
		}
	}
	for ( int i = 0; i < board->numberOfBush; i++ ) {
		int index = rand () % board->numberOfFreeSpace;		
		bushes[i].y = board->freeSpace[index];
		bushes[i].x = rand () % ( board->width - 2 ) + START_BOARD + 1;
		if ( coins[i].y == bushes[i].y && coins[i].x == bushes[i].x ) {
			bushes[i].x++;
		}
	}
}

bool win ( Frog & frog , Board * board ) {
	if ( frog.y == START_BOARD + 1 ) {
		return true;
	}
	return false;
}

bool colisionWithEnemyCar (Frog&frog, Board*board, Car*cars) {
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		for ( int k = 0; k < cars[i].lenght; k++ ) {
			if ( frog.y == cars[i].y && frog.x == cars[i].x[k] && !cars[i].friendly ) {
				return true;
			}
		}
	}
	return false;
}

bool colisionWithFriendlyCar ( Frog & frog , Board * board , Car * cars, int&index ) {
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		if ( cars[i].friendly && !cars[i].stoping ) {
			if ( cars[i].direction == LEFT ) {
				if ( frog.y == cars[i].y && frog.x - 1 == cars[i].x[0] ) {
					index = i;
					return true;
				}
			}
			else {
				if ( frog.y == cars[i].y && frog.x + 1 == cars[i].x[0] ) {
					index = i;
					return true;
				}
			}
		}
	}
	return false;
}

bool colisionWithStoppingCar ( Frog & frog , Board * board , Car * cars, int&index ) {
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		if ( cars[i].stoping ) {
			if ( cars[i].direction == LEFT ) {
				if ( ( frog.y == cars[i].y || frog.y - 1 == cars[i].y ) && cars[i].x[0]-frog.x <= 2 && 0 <= cars[i].x[0] - frog.x ) {
					index = i;
					return true;
				}
			}
			else {
				if ( ( frog.y == cars[i].y || frog.y - 1 == cars[i].y ) && frog.x-cars[i].x[0] <= 2 && 0 <= frog.x-cars[i].x[0]) {
					index = i;
					return true;
				}
			}
		}
	}
	return false;
}

void spawnCoin ( Board * board , Coin * coin , Coin *& coins ) {
	for ( int i = 0; i < board->numberOfCoins; i++ ) {
		coins[i].sign = coin->sign;
		coins[i].value = rand () % 5 + 1;
		bool validPos = false;
		while ( !validPos ) {
			coins[i].y = rand () % ( board->height - 2 ) + START_BOARD + 1;
			validPos = true;
			for ( int k = 0; k < board->numberOfStreets; k++ ) {
				if ( coins[i].y == board->roadY[k] || coins[k].y == board->roadY[k] + 1 ||
					 coins[i].y == board->roadY[k] - 1 ) {
					validPos = false;
					break;
				}
			}
			coins[i].x = rand () % ( board->width - 2 ) + START_BOARD + 1;
		}
	}
}

void settingUp ( Board * board , Frog * frog , Car * car , Car *& cars , Tree * tree , Bush * bush , Tree *& trees ,
				 Bush *& bushes, Coin*coin, Coin*&coins, Stork*stork ) {
	board->roadY = new int[board->numberOfStreets];
	for ( int i = 0; i < board->numberOfStreets; i++ ) {
		board->roadY[i] = board->firstYRoad + i * board->spaceBetweenRoads;
	}
	board->x_end_board = board->width + START_BOARD;
	board->y_end_board = board->height + START_BOARD;
	board->spawn_car_x = ( board->x_end_board - 1 ) / 2;
	board->numberOfFreeSpace = board->height - 3 - board->numberOfStreets*STREET_HEIGHT;
	board->freeSpace = new int[board->numberOfFreeSpace];
	int i = 0;
	int y = i + START_BOARD + 1;
	int index = 0;
	while ( i < board->numberOfFreeSpace ) {
		
		bool isFreeSpace = true;
		for ( int k = 0; k < board->numberOfStreets; k++ ) {
			if ( y == board->roadY[k] || y == board->roadY[k] + 1 || y == board->roadY[k] - 1 ) {
				isFreeSpace = false;
				break;
			}
		}
		if ( isFreeSpace ) {
			board->freeSpace[index++] = y;
			i++;
		}
		y++;
	}
	stork->x = board->width / 2;
	stork->y = START_BOARD + 2;
	cars = new Car[board->numberOfCars];
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		cars[i].speed = car->speed;
		cars[i].sign = car->sign;
		cars[i].max_lenght = car->max_lenght;
		cars[i].speed = car->speed;
		cars[i].frameDelayMax = car->frameDelayMax;
	}
	trees = new Tree[board->numberOfTrees];
	bushes = new Bush[board->numberOfBush];
	for ( int i = 0; i < board->numberOfTrees; i++ ) {
		trees[i].sign = tree->sign;
	}
	for ( int i = 0; i < board->numberOfBush; i++ ) {
		bushes[i].sign = bush->sign;

	}
	coins = new Coin[board->numberOfCoins];
	spawnCoin ( board , coin , coins );
}



void colectingCoin ( Frog & frog , Coin * coins , Board * board ) {
	for ( int i = 0; i < board->numberOfCoins; i++ ) {
		if ( frog.x == coins[i].x && frog.y == coins[i].y && !coins[i].colected ) {
			coins[i].colected = true;
			board->score += coins[i].value;
		}
	}
}

void loadFile (const char* fileName, Board*board,Frog*frog,Car*car, Car *& cars ,Tree*tree, Bush*bush, Tree*&trees,
				Bush*&bushes, Coin*coin, Coin*&coins, Stork*stork) {
	FILE * file = fopen(fileName,"r");
	if ( file == NULL ) {
		printf("Error while opening file");
		exit ( 1 );
	}
	fscanf ( file ,
			 "%d %d %d %d %d %d %d %c %c %d %d %d %d %c %d %c %d %c %c %d %d %d %d %d" ,
			 &board->width ,
			 &board->height ,
			 &board->numberOfStreets ,
			 &board->numberOfCars ,
			 &frog->x ,
			 &frog->y ,
			 &frog->colision ,
			 &frog->sign ,
			 &car->sign ,
			 &car->max_lenght ,
			 &board->firstYRoad ,
			 &car->speed ,
			 &board->numberOfTrees ,
			 &tree->sign ,
			 &board->numberOfBush ,
			 &bush->sign ,
			 &board->numberOfCoins ,
			 &coin->sign ,
			 &stork->sign ,
			 &stork->speed,
			 &frog->frameDelay,
			 &car->frameDelayMax,
			 &stork->frameDelay,
			 &board->spaceBetweenRoads
	);
	fclose ( file );
	settingUp ( board , frog , car , cars , tree , bush , trees , bushes, coin, coins, stork );
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
void printingFrogCars (Board*board, Frog&frog, Car*cars, clock_t start_time, Coin*coins, Stork&stork) {
	textcolor ( frog.color );
	gotoxy ( frog.x , frog.y );
	putch ( frog.sign );
	gotoxy ( frog.oldX , frog.oldY );
	putch ( ' ' );
	gotoxy ( stork.x , stork.y );
	textcolor ( stork.color );
	putch ( stork.sign );
	gotoxy ( stork.oldX , stork.oldY );
	putchar ( ' ' );
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
		gotoxy ( stork.oldX , stork.oldY );
		if ( stork.oldY == board->roadY[k] - 1 ) {
			textcolor ( RED );
			putch ( '-' );
		}
		else if ( stork.oldY == board->roadY[k] + 1 ) {
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
	for ( int i = 0; i < board->numberOfCoins; i++ ) {
		if ( coins[i].colected )
			continue;
		gotoxy ( coins[i].x , coins[i].y );
		textcolor ( YELLOW );
		putch ( coins[i].sign );
	}
	gotoxy ( 60 , 1 );
	textcolor ( RED );
	clock_t current_time = clock ();
	double elapsed_time = ( double ) ( current_time - start_time ) / CLOCKS_PER_SEC;
	printf ( "your time: %.2f s" , elapsed_time );
	gotoxy ( 60 , 2 );
	textcolor ( YELLOW );
	printf ( "your score: %d" , board->score );
	textcolor ( WHITE );
}

void friendlyCar (Board*board,Frog&frog,Car*cars, int index,clock_t start_time,Coin*coins,Stork&stork) {
	do {
		int newX = cars[index].x[0] + cars[index].speed * cars[index].direction;

		if ( newX > START_BOARD + 1 && newX < START_BOARD + board->width - 2 ) {
			frog.x = newX;
		}
		else { 
			frog.y--;
			break;
		}
		//printingFrogCars ( board , frog , cars , start_time, coins ,stork);
	} while ( colisionWithFriendlyCar ( frog , board , cars , index ) );
}

void stoppingCar ( Board * board , Frog & frog , Car * cars, int&index, clock_t start_time, int&oldSpeed, Coin*coins,Stork&stork ) {
	if ( cars[index].speed > 0 )
		oldSpeed = cars[index].speed;
	if ( colisionWithStoppingCar( frog , board , cars , index ) ) {
		cars[index].speed = 0;
		printingFrogCars ( board , frog , cars , start_time, coins,stork );
	}
	if ( frog.y < cars[index].y || ( frog.x - cars[index].x[0] > 2 && cars[index].direction == RIGHT ) || (cars[index].x[0] - frog.x > 2 && cars[index].direction == LEFT ))
		cars[index].speed = oldSpeed;
}

void loop ( Board * board , Frog & frog , Car * cars , Tree * trees , Bush * bushes , clock_t&lastFrogMove,
			clock_t&lastCarMove, clock_t&start_time, int&indexOfCar, int&oldSpeed, bool&game, Coin*coins,Stork&stork,clock_t&lastStorkMove ) {
	clock_t current_time = clock ();
	bool keyHeld = false;
	char input = 'j';
	double elapsedFrogTime = ( double ) ( current_time - lastFrogMove ) / CLOCKS_PER_SEC * 1000; // ms
	double elsapsedStorkTime = ( double ) ( current_time - lastStorkMove ) / CLOCKS_PER_SEC * 1000;   // ms

	frog.colision = colisionWithEnemyCar ( frog , board , cars );
	frog.win = win ( frog , board );
	spawnCars ( board , cars );
	
	moveCar ( board , cars, current_time);


	if ( colisionWithFriendlyCar ( frog , board , cars , indexOfCar ) ) {
		friendlyCar ( board , frog , cars , indexOfCar , start_time ,coins,stork);
	}
	if ( colisionWithStoppingCar ( frog , board , cars , indexOfCar ) ) {
		stoppingCar ( board , frog , cars , indexOfCar , start_time , oldSpeed,coins,stork );
	}
	else if ( cars[indexOfCar].speed == 0 ) {
		stoppingCar ( board , frog , cars , indexOfCar , start_time , oldSpeed,coins,stork );
	}
	if(elsapsedStorkTime >= stork.frameDelay ){
		moveStork ( stork , board , frog , trees , bushes );
		lastStorkMove = clock ();
	}
	stork.eatFrog = colisionStork (frog, stork );
	colectingCoin ( frog , coins , board );
	if ( kbhit () ) {
		if ( !keyHeld ) {
			input = getch ();
			if ( input == 'q' ) {
				game = false;
			}
			if(elapsedFrogTime >= frog.frameDelay){
				movingFrog ( frog , board , input , trees , bushes );
				lastFrogMove = clock ();
			}
		}
		keyHeld = true;
	}
	else {
		keyHeld = false;
	}
	printingFrogCars ( board , frog , cars , start_time, coins,stork );
	
}

void handelInput ( Board*board , Frog&frog , Car*cars, Tree*trees, Bush*bushes, Coin*coins,Stork&stork, bool& wantToPlay ) {
	clock_t lastFrogMove = clock ();
	clock_t lastCarMove = clock ();
	clock_t start_time = clock ();
	clock_t lastStorkMove = clock ();
	spawnTreeBush ( board , trees , bushes,coins );
	printingStaticBoard ( board, trees, bushes );
	int indexOfCar = 0;
	int oldSpeed = 0;
	bool game = true;
	while (game && !frog.colision && !frog.win && !stork.eatFrog) {
		loop ( board , frog , cars , trees , bushes , lastFrogMove , lastCarMove , start_time , indexOfCar , oldSpeed, game, coins, stork, lastStorkMove );
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
	char input = 'x';
	while(input != 'y' && input !='q' ) {
		textcolor ( BLUE );
		gotoxy ( X_END_INFO , Y_END_INFO + 1 );
		printf ( "Thank you for playing! Do you want to play again? y-new game, q-exit" );
		textcolor ( WHITE );
		input = getch ();
		if ( input == 'q' )
			wantToPlay = false;

	}
}

void game () {
	settitle ( "Krzysztof Szudy - Jumping Frog" );
	_setcursortype ( _NOCURSOR );
	const char * lvl1 = "config1.txt";
	const char * lvl2 = "config2.txt";
	const char * lvl3 = "config3.txt";
	bool wantToPlay = true;
	bool hasWon1 = false;
	bool hasWon2 = false;
	bool hasWon3 = false;
	while ( wantToPlay ) {
		Board * board = new Board;
		Frog frog;
		Car * car = new Car;
		Tree * tree = new Tree;
		Bush * bush = new Bush;
		Car * cars = nullptr;
		Tree * trees = nullptr;
		Bush * bushes = nullptr;
		Coin * coin = new Coin;
		Coin * coins = nullptr;
		Stork stork;
		if(!hasWon1){
			loadFile ( lvl1 , board , &frog , car , cars , tree , bush , trees , bushes , coin , coins , &stork );
		}
		else if ( hasWon1 && !hasWon2 ) {
			loadFile ( lvl2 , board , &frog , car , cars , tree , bush , trees , bushes , coin , coins , &stork );

		}
		else if ( hasWon2 && !hasWon3) {
			loadFile ( lvl3 , board , &frog , car , cars , tree , bush , trees , bushes , coin , coins , &stork );
		}
		else {
			printf ( "you have won every level! do you want to start again from 1st level?" );
		}
		delete car , coin;
		delete tree , bush;
		handelInput ( board , frog , cars , trees , bushes , coins , stork, wantToPlay );
		if ( frog.win )
			hasWon1 = true;
		delete[] cars;
		delete board;
		clrscr ();
	}
	gotoxy ( X_END_INFO , Y_END_INFO );
	printf ( "see you next time! (press any key to close window...)" );
	getch ();
}

int main()
{
	srand ( time ( NULL ) );
	
	game ( );
	
}
