#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "conio2.h"
#include <time.h>



typedef struct Const {
	int START_BOARD = 0;
	int ROAD_HEIGHT = 0;
	int LEFT = 0;
	int RIGHT = 0;
	int X_END_INFO = 0;
	int Y_END_INFO = 0;
	int STREET_HEIGHT = 0;
	int SCORE_BOOST = 0;
	int SAFE_DISTANCE = 0;
	int CORECTOR2 = 0;
	int CORECTOR1 = 0;
	int CORECTOR3 = 0;
	int CORECTOR4 = 0;
	int UP = 0;
	int DOWN = 0;
	int CURRENT = 0;
	int CORECTOR_HEIGHT = 0;
	int HALF_MINUTE = 0;
	int FOURTY_FIVE_S = 0;
	int MINUTE = 0;
	int TIME_X = 0;
	int TIME_Y = 0;
};

typedef struct Game {
	double timeForLastLVL = 0;
	double firstLvl = 0;
	double secondLvl = 0;
	double thirdLvl = 0;
	int score1lvl = 0;
	int score2lvl = 0;
	int score3lvl = 0;
	int lastScore = 0;
	clock_t lastFrogMove;
	clock_t lastCarMove;
	clock_t start_time;
	clock_t lastStorkMove;
	clock_t current_time;
};

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
	int x = 0 , y = 0;
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
	int width = 0 , height = 0;
	int right_end_board = 0 , bottom_end_board = 0;
	int numberOfStreets = 0;
	int firstYRoad = 0;
	int * roadY = nullptr;
	int numberOfCars = 0;
	int numberOfBush = 0;
	int numberOfTrees = 0;
	int numberOfCoins = 0;
	int score = 0;
	int numberOfFreeSpace = 0;
	int * freeSpace = nullptr;
	int spaceBetweenRoads = 0;
	int leftStartRoad = 0;
	int rightStartRoad = 0;
	int * topCurb = nullptr;
	int * bottomCurb = nullptr;
	int startingY = 0;
	int metaY = 0;
}Board;

typedef struct Coin {
	int x = 0 , y = 0;
	char sign = 'a';
	bool colected = false;
	int value = 0;
}Coin;

bool canGo ( Const & constant , Frog & frog , Board * board , char input , Tree * trees , Bush * bushes ) { // funtion to check if frog can go(if there is no tree,bush on the way
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
	if ( newY == constant.START_BOARD || newY == board->bottom_end_board ||
		 newX == constant.START_BOARD || newX == board->rightStartRoad ) {
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

bool canFly ( Const & constant , Stork & stork , Tree * trees , Bush * bushes , Board * board ) {//Checking if stork can fly, if there is no tree, bush on the way
	int newX = stork.x + stork.directionX * stork.speed;
	int newY = stork.y + stork.directionY * stork.speed;

	if ( newY == constant.START_BOARD || newY == board->bottom_end_board ||
		 newX == constant.START_BOARD || newX == board->rightStartRoad ) {
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

void movingFrog ( Const & constant , Frog & frog , Board * board , char input , Tree * trees , Bush * bushes ) {
	if ( canGo ( constant , frog , board , input , trees , bushes ) ) {
		frog.oldY = frog.y;
		frog.oldX = frog.x;
		if ( input == 'w' ) {
			frog.y--;
		}
		else if ( input == 's' ) {
			frog.y++;
		}
		else if ( input == 'a' ) {
			frog.x--;
		}
		else if ( input == 'd' ) {
			frog.x++;
		}
	}
}

void settingTypeOfCar ( Const & constant , Board * board , Car * cars, int i ) {
	if ( cars[i].friendly ) {
		cars[i].color = LIGHTBLUE;
		cars[i].stoping = rand () % 2 == 0 ? true : false; //some of friendly cars will stop and let frog move across street
		if ( cars[i].stoping )
			cars[i].color = WHITE;
	}
	else {
		cars[i].color = RED;
	}
	if ( !cars[i].dissapear )
		cars[i].dissapear = rand () % 2 == 0 ? true : false;
	cars[i].direction = rand () % 2 == 0 ? constant.LEFT : constant.RIGHT;
	// setting start point of car based on direction and lenght of car
	int start = ( cars[i].direction == constant.RIGHT ) ? constant.START_BOARD + cars[i].lenght : board->rightStartRoad - cars[i].lenght;

	if ( cars[i].dissapear ) {
		if ( cars[i].direction == constant.RIGHT ) {
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
		cars[i].direction = ( rand () % 2 == 0 ) ? constant.LEFT : constant.RIGHT;

		for ( int k = 0; k < cars[i].lenght; k++ ) {
			cars[i].x[k] = start + ( cars[i].direction == constant.RIGHT ? k : -k );
		}
	}
}

void spawnCars ( Const & constant , Board * board , Car * cars ) {
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		if ( !cars[i].active ) {
			cars[i].lenght = rand () % cars[i].max_lenght + 1;
			cars[i].x = new int[cars[i].lenght];
			cars[i].frameDelay = rand () % cars[i].frameDelayMax + cars[i].frameDelayMax;
			cars[i].active = true;
			cars[i].y = board->roadY[i];
			cars[i].friendly = rand () % 2 == 0 ? true : false; // setting randomly if car is friendly - will pick up frog and move it 
			settingTypeOfCar ( constant , board , cars , i );
		}
	}
}

void moveStork ( Const & constant , Stork & stork , Board * board , Frog & frog , Tree * trees , Bush * bushes ) {

	if ( frog.x < stork.x ) {
		stork.directionX = constant.LEFT;
	}
	else if ( frog.x > stork.x ) {
		stork.directionX = constant.RIGHT;
	}
	else {
		stork.directionX = constant.CURRENT;
	}
	if ( frog.y < stork.y ) {
		stork.directionY = constant.UP;
	}
	else if ( frog.y > stork.y ) {
		stork.directionY = constant.DOWN;
	}
	else {
		stork.directionY = constant.CURRENT;
	}
	if ( canFly ( constant , stork , trees , bushes , board ) ) {
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

void carLogic ( Const & constant , Board * board , Car * cars, int i) {
	for ( int k = 1; k < board->width; k++ ) { //clearing street
		int posX = constant.START_BOARD + k;
		if ( posX != constant.START_BOARD && posX != board->rightStartRoad ) {
			gotoxy ( posX , cars[i].y );
			putchar ( ' ' );
		}
	}
	for ( int k = 0; k < cars[i].lenght; k++ ) { //moving
		cars[i].x[k] += cars[i].direction * cars[i].speed;
	}
	bool hitBorder = false;
	for ( int k = 0; k < cars[i].lenght; k++ ) {
		if ( cars[i].x[k] >= board->rightStartRoad || cars[i].x[k] <= constant.START_BOARD ) {
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
			int position = ( cars[i].direction == constant.RIGHT ) ? constant.START_BOARD + cars[i].lenght : board->rightStartRoad - cars[i].lenght;
			for ( int k = 0; k < cars[i].lenght; k++ ) {
				cars[i].x[k] = position + ( cars[i].direction == constant.RIGHT ? -k : k );
			}
		}
	}
	cars[i].lastMove = clock ();
}

void moveCar ( Const & constant , Board * board , Car * cars , clock_t current_time ) {
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		double elapsedTime = ( double ) ( current_time - cars[i].lastMove ) / CLOCKS_PER_SEC * 1000;   // ms
		if ( elapsedTime >= cars[i].frameDelay ) {
			if ( !cars[i].active ) {
				continue;
			}
			carLogic ( constant , board , cars , i );
		}
		else {
			break;
		}
	}
}

void spawnTreeBush ( Const & constant , Board * board , Tree * trees , Bush * bushes , Coin * coins ) {
	int index = 0;
	bool ** freeSpaceGrid = new bool * [board->bottom_end_board];
	for ( int i = 0; i < board->bottom_end_board; i++ ) {
		freeSpaceGrid[i] = new bool[board->right_end_board];
		for ( int j = 0; j < board->right_end_board; j++ ) {
			freeSpaceGrid[i][j] = true;
		}
	}
	int modulo = board->rightStartRoad - board->leftStartRoad;
	for ( int i = 0; i < board->numberOfTrees; i++ ) {
		do {
			index = rand () % board->numberOfFreeSpace;
			trees[i].y = board->freeSpace[index];
			trees[i].x = rand () % ( modulo ) + board->leftStartRoad;
			if( freeSpaceGrid[trees[i].y][trees[i].x]) {
				freeSpaceGrid[trees[i].y][trees[i].x] = false;
			}
		} while ( freeSpaceGrid[trees[i].y][trees[i].x] );
	}
	for ( int i = 0; i < board->numberOfBush; i++ ) {
		do {
			index = rand () % board->numberOfFreeSpace;
			bushes[i].y = board->freeSpace[index];
			bushes[i].x = rand () % ( modulo ) + constant.START_BOARD + constant.CORECTOR1;
			if( freeSpaceGrid[bushes[i].y][bushes[i].x])
				freeSpaceGrid[bushes[i].y][bushes[i].x] = false;
		} while ( freeSpaceGrid[bushes[i].y][bushes[i].x]);
	}

	for ( int i = 0; i < board->bottom_end_board; i++ ) {
		delete[] freeSpaceGrid[i];
	}
	delete[] freeSpaceGrid;
}

bool win ( Frog & frog , Board * board ) {
	if ( frog.y == board->metaY ) {
		return true;
	}
	return false;
}

bool colisionWithEnemyCar ( Frog & frog , Board * board , Car * cars ) {
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		for ( int k = 0; k < cars[i].lenght; k++ ) {
			if ( frog.y == cars[i].y && frog.x == cars[i].x[k] && !cars[i].friendly ) {
				return true;
			}
		}
	}
	return false;
}

bool colisionWithFriendlyCar ( Frog & frog , Board * board , Car * cars , int & index ) {
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		if ( cars[i].friendly && !cars[i].stoping ) {
			if ( frog.y == cars[i].y && frog.x == cars[i].x[0] ) {
				index = i;
				return true;
			}
		}
	}
	return false;
}

bool colisionWithStoppingCar ( Const & constant , Frog & frog , Board * board , Car * cars , int & index ) {
	for ( int i = 0; i < board->numberOfCars; i++ ) {
		if ( cars[i].stoping ) {
			if ( cars[i].direction == constant.LEFT ) {
				if ( ( frog.y == cars[i].y || frog.y - constant.CORECTOR1 == cars[i].y ) && cars[i].x[0] - frog.x <= constant.SAFE_DISTANCE && 0 <= cars[i].x[0] - frog.x ) {
					index = i;
					return true;
				}
			}
			else {
				if ( ( frog.y == cars[i].y || frog.y - constant.CORECTOR1 == cars[i].y ) && frog.x - cars[i].x[0] <= constant.SAFE_DISTANCE && 0 <= frog.x - cars[i].x[0] ) {
					index = i;
					return true;
				}
			}
		}
	}
	return false;
}

void spawnCoin ( Const & constant , Board * board , Coin * coin , Coin *& coins ) {
	for ( int i = 0; i < board->numberOfCoins; i++ ) {
		coins[i].sign = coin->sign;
		coins[i].value = rand () % 5 + constant.CORECTOR1;
		int index = rand () % board->numberOfStreets;
		coins[i].y = index % 2 == 0 ? board->topCurb[index] : board->bottomCurb[index];
		coins[i].x = rand () % ( board->rightStartRoad - board->leftStartRoad) + board->leftStartRoad;
	}
}

void settingUpActors ( Const & constant , Board * board , Frog * frog , Car * car , Car *& cars , Tree * tree , Bush * bush , Tree *& trees ,
					   Bush *& bushes , Coin * coin , Coin *& coins , Stork * stork ) {
	frog->x = constant.START_BOARD + board->width / 2;
	frog->y = board->startingY;
	stork->x = constant.START_BOARD + board->width / 2;
	stork->y = board->metaY;
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
	spawnCoin ( constant , board , coin , coins );
}

void savingFreeSpace (Board*board, Const&constant) {
	board->numberOfFreeSpace = board->startingY -constant.CORECTOR1 - board->metaY - board->numberOfStreets * constant.STREET_HEIGHT;
	board->freeSpace = new int[board->numberOfFreeSpace];
	int i = 1;
	int y = i + board->metaY;
	int index = 0;
	while ( index < board->numberOfFreeSpace && y < board->bottom_end_board ) {
		bool isFreeSpace = true;
		for ( int k = 0; k < board->numberOfStreets; k++ ) {
			if ( y == board->roadY[k] || y == board->bottomCurb[k] || y == board->topCurb[k] || y == board->startingY) {
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
}

void settingUpBoard ( Const & constant , Board * board) { // setting up all board, frog,stork,cars, trees and bushes paramiters
	board->roadY = new int[board->numberOfStreets];
	board->topCurb = new int[board->numberOfStreets];
	board->bottomCurb = new int[board->numberOfStreets];
	for ( int i = 0; i < board->numberOfStreets; i++ ) {
		board->roadY[i] = board->firstYRoad + i * board->spaceBetweenRoads;
		board->bottomCurb[i] = board->roadY[i] + constant.CORECTOR1;
		board->topCurb[i] = board->roadY[i] - constant.CORECTOR1;
	}
	board->startingY = constant.START_BOARD+board->height - constant.CORECTOR1;
	board->metaY = constant.START_BOARD + constant.CORECTOR1;
	board->rightStartRoad = board->width + constant.START_BOARD;
	board->leftStartRoad = constant.START_BOARD + constant.CORECTOR1;
	board->right_end_board = board->width + constant.START_BOARD;
	board->bottom_end_board = board->height + constant.START_BOARD;
	savingFreeSpace ( board , constant );
}

void colectingCoin ( Frog & frog , Coin * coins , Board * board ) {
	for ( int i = 0; i < board->numberOfCoins; i++ ) {
		if ( frog.x == coins[i].x && frog.y == coins[i].y && !coins[i].colected ) {
			coins[i].colected = true;
			board->score += coins[i].value;
		}
	}
}

void loadConstants ( Const * constants ) {
	FILE * file = fopen ( "constFile.txt" , "r" );
	if ( file == NULL ) {
		printf ( "Error loading constants file\n" );
		exit ( 1 );
	}

	fscanf ( file , "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d" ,
			 &constants->START_BOARD ,
			 &constants->ROAD_HEIGHT ,
			 &constants->LEFT ,
			 &constants->RIGHT ,
			 &constants->X_END_INFO ,
			 &constants->Y_END_INFO ,
			 &constants->STREET_HEIGHT ,
			 &constants->SCORE_BOOST ,
			 &constants->SAFE_DISTANCE ,
			 &constants->CORECTOR2 ,
			 &constants->CORECTOR1 ,
			 &constants->CORECTOR3 ,
			 &constants->CORECTOR4 ,
			 &constants->UP ,
			 &constants->DOWN ,
			 &constants->CURRENT ,
			 &constants->CORECTOR_HEIGHT ,
			 &constants->HALF_MINUTE ,
			 &constants->FOURTY_FIVE_S ,
			 &constants->MINUTE ,
			 &constants->TIME_X ,
			 &constants->TIME_Y
	);
	fclose ( file );
}

void loadFile ( Const & constant , const char * fileName , Board * board , Frog * frog , Car * car , Car *& cars , Tree * tree , Bush * bush , Tree *& trees ,
				Bush *& bushes , Coin * coin , Coin *& coins , Stork * stork ) { //function to load lvls
	FILE * file = fopen ( fileName , "r" );
	if ( file == NULL ) {
		printf ( "Error while opening file" );
		exit ( 1 );
	}
	fscanf ( file ,
			 "%d %d %d %d %d %c %c %d %d %d %d %c %d %c %d %c %c %d %d %d %d %d" ,
			 &board->width ,
			 &board->height ,
			 &board->numberOfStreets ,
			 &board->numberOfCars ,
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
			 &stork->speed ,
			 &frog->frameDelay ,
			 &car->frameDelayMax ,
			 &stork->frameDelay ,
			 &board->spaceBetweenRoads
	);
	fclose ( file );
	settingUpBoard ( constant , board);
	settingUpActors ( constant , board , frog , car , cars , tree , bush , trees , bushes , coin , coins , stork );
}

void printingStaticBoard ( Const & constant , Board * board , Tree * trees , Bush * bushes ) {
	gotoxy ( 0 , 0 );

	for ( int i = constant.START_BOARD; i < board->bottom_end_board + constant.CORECTOR1; i++ ) {
		for ( int j = constant.START_BOARD; j < board->right_end_board + constant.CORECTOR1; j++ ) {
			textcolor ( WHITE );
			gotoxy ( j , i );
			if ( i == constant.START_BOARD || i == board->bottom_end_board || j == constant.START_BOARD || j == board->right_end_board ) {
				putch ( '*' );
			}
			else if ( i == board->metaY ) {
				if ( j % 2 == 0 )
					textbackground ( WHITE );
				putchar ( ' ' );
			}
			textbackground ( BLACK );
			for ( int k = 0; k < board->numberOfStreets; k++ ) {
				if ( i == board->topCurb[k] && j < board->right_end_board ) {
					textcolor ( RED );
					putch ( '-' );
				}
				else if ( i == board->bottomCurb[k] && j < board->right_end_board ) {
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

void printingCarsCoins ( Const&constant, Board*board, Car * cars , Coin * coins , Game&game) {
	for ( int k = 0; k < board->numberOfCars; k++ ) {
		for ( int l = 0; l < cars[k].lenght; l++ ) {
			if ( cars[k].x[l] > constant.START_BOARD && cars[k].x[l] < board->rightStartRoad ) {
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
	gotoxy ( constant.TIME_X , constant.TIME_Y - constant.CORECTOR1 );
	textcolor ( RED );
	clock_t current_time = clock ();
	double elapsed_time = ( double ) ( current_time - game.start_time ) / CLOCKS_PER_SEC;
	printf ( "your time: %.2f s" , elapsed_time );
	gotoxy ( constant.TIME_X , constant.TIME_Y );
	textcolor ( YELLOW );
	printf ( "your score: %d" , board->score );
	textcolor ( WHITE );
}

void printingFrogStork ( Const & constant , Board * board , Frog & frog ,  Game & game , Stork & stork ) {
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
		if ( frog.oldY == board->topCurb[k] ) {
			textcolor ( RED );
			putch ( '-' );
		}
		else if ( frog.oldY == board->bottomCurb[k] ) {
			textcolor ( BLUE );
			putch ( '-' );
		}
		gotoxy ( stork.oldX , stork.oldY );
		if ( stork.oldY == board->topCurb[k] ) {
			textcolor ( RED );
			putch ( '-' );
		}
		else if ( stork.oldY == board->bottomCurb[k] ) {
			textcolor ( BLUE );
			putch ( '-' );
		}
	}
}

void friendlyCar ( Const & constant , Board * board , Frog & frog , Car * cars , int index , clock_t start_time , Tree * trees , Bush * bushes ) {
	do {
		int newX = cars[index].x[0] + cars[index].speed * cars[index].direction;
		if ( newX > constant.START_BOARD && newX < board->right_end_board ) {
			movingFrog ( constant , frog , board , ( cars[index].direction == constant.RIGHT ? 'd' : 'a' ) , trees , bushes );
		}
		else {
			frog.y--;
			break;
		}
	} while ( colisionWithFriendlyCar ( frog , board , cars , index ) );
}

void stoppingCar ( Game & game , Const & constant , Board * board , Frog & frog , Car * cars , int & index , int & oldSpeed , Coin * coins , Stork & stork ) {
	if ( cars[index].speed > 0 )
		oldSpeed = cars[index].speed;
	if ( colisionWithStoppingCar ( constant , frog , board , cars , index ) ) {
		cars[index].speed = 0;
		printingFrogStork ( constant , board , frog  , game  , stork );
		printingCarsCoins ( constant , board , cars , coins , game );
	}
	if ( frog.y < cars[index].y || ( frog.x - cars[index].x[0] > constant.SAFE_DISTANCE && cars[index].direction == constant.RIGHT ) || 
		 ( cars[index].x[0] - frog.x > constant.SAFE_DISTANCE && cars[index].direction == constant.LEFT ) )
		cars[index].speed = oldSpeed;
}

void colisionChecker ( Game & game , Const & constant , Board * board , Frog & frog , Car * cars , Tree * trees , Bush * bushes 
					   , int & indexOfCar , int & oldSpeed , Coin * coins , Stork & stork ) {
	if ( colisionWithFriendlyCar ( frog , board , cars , indexOfCar ) ) {
		friendlyCar ( constant , board , frog , cars , indexOfCar , game.start_time , trees , bushes );
	}
	if ( colisionWithStoppingCar ( constant , frog , board , cars , indexOfCar ) ) {
		stoppingCar ( game , constant , board , frog , cars , indexOfCar , oldSpeed , coins , stork );
	}
	else if ( cars[indexOfCar].speed == 0 ) {
		stoppingCar ( game , constant , board , frog , cars , indexOfCar , oldSpeed , coins , stork );
	}
	double elsapsedStorkTime = ( double ) ( game.current_time - game.lastStorkMove ) / CLOCKS_PER_SEC * 1000;   // ms
	if ( elsapsedStorkTime >= stork.frameDelay ) {
		moveStork ( constant , stork , board , frog , trees , bushes );
		game.lastStorkMove = clock ();
	}
}

//main loop of the game
void loop ( Game & game , Const & constant , Board * board , Frog & frog , Car * cars , Tree * trees , Bush * bushes ,
			int & indexOfCar , int & oldSpeed , bool & playing , Coin * coins , Stork & stork ) {
	game.current_time = clock ();
	bool keyHeld = false;
	char input = 'j';
	double elapsedFrogTime = ( double ) ( game.current_time - game.lastFrogMove ) / CLOCKS_PER_SEC * 1000; // ms
	

	frog.colision = colisionWithEnemyCar ( frog , board , cars );
	frog.win = win ( frog , board );
	spawnCars ( constant , board , cars );

	moveCar ( constant , board , cars , game.current_time );

	stork.eatFrog = colisionStork ( frog , stork );
	colectingCoin ( frog , coins , board );
	colisionChecker ( game , constant , board , frog , cars , trees , bushes , indexOfCar , oldSpeed , coins , stork );
	if ( kbhit () ) {
		if ( !keyHeld ) { //prevents stocking inpput from user
			input = getch ();
			if ( input == 'q' ) {
				playing = false;
			}
			if ( elapsedFrogTime >= frog.frameDelay ) {
				movingFrog ( constant , frog , board , input , trees , bushes );
				game.lastFrogMove = clock ();
			}
		}
		keyHeld = true;
	}
	else {
		keyHeld = false;
	}
	printingFrogStork ( constant , board , frog , game , stork );
	printingCarsCoins ( constant , board , cars , coins , game );
}

void gameHandler ( Const & constant , Game & game , Board * board , Frog & frog , Car * cars , Tree * trees , Bush * bushes , Coin * coins , Stork & stork , bool & wantToPlay) {
	game.start_time = clock ();
	spawnTreeBush ( constant , board , trees , bushes , coins );
	printingStaticBoard ( constant , board , trees , bushes );
	int indexOfCar = 0;
	int oldSpeed = 0;
	bool playing = true;
	while ( playing && !frog.colision && !frog.win && !stork.eatFrog ) {
		loop ( game , constant , board , frog , cars , trees , bushes , indexOfCar , oldSpeed , playing , coins , stork );
	}
	clock_t end_time = clock ();
	double gameplay_time = ( double ) ( end_time - game.start_time ) / CLOCKS_PER_SEC;
	if ( frog.win ) {
		gotoxy ( constant.X_END_INFO , constant.Y_END_INFO );
		textcolor ( GREEN );
		printf ( "You win! Your time: %.2f s" , gameplay_time );
		game.timeForLastLVL = gameplay_time;
		game.lastScore = board->score;

	}
	else if ( frog.colision ) {
		gotoxy ( constant.X_END_INFO , constant.Y_END_INFO );
		textcolor ( RED );
		printf ( "You lose! Your time: %.2f s" , gameplay_time );
	}
}

void finalInfo (Game&game, Const&constant,bool&wantToPlay) {
	clrscr ();
	if ( game.firstLvl < constant.HALF_MINUTE )
		game.score1lvl *= constant.SCORE_BOOST;
	if ( game.secondLvl < constant.FOURTY_FIVE_S )
		game.score2lvl *= constant.SCORE_BOOST;
	if ( game.thirdLvl < constant.MINUTE )
		game.score3lvl *= constant.SCORE_BOOST;

	gotoxy ( constant.X_END_INFO , constant.Y_END_INFO );
	printf ( "YOU WON THE WHOLE GAME WITH TIMES AND SCORES:" );
	gotoxy ( constant.X_END_INFO , constant.Y_END_INFO + constant.CORECTOR1 );
	printf ( "lvl1 - % .2f s " , game.firstLvl );
	printf ( " and score: % d" , game.score1lvl );
	gotoxy ( constant.X_END_INFO , constant.Y_END_INFO + constant.CORECTOR2 );
	printf ( "lvl2 - %.2f s" , game.secondLvl );
	printf ( " score - %d" , game.score2lvl );
	gotoxy ( constant.X_END_INFO , constant.Y_END_INFO + constant.CORECTOR3 );
	printf ( "lvl3 - %.2f s" , game.thirdLvl );
	printf ( " score - %d" , game.score3lvl );
	gotoxy ( constant.X_END_INFO , constant.Y_END_INFO + constant.CORECTOR4 );
	printf ( "click q to exit" );
	getch ();
	wantToPlay = false;
}

//levels manager, cheking if player won previous level so they can play next one
void nextLvl ( Const & constant , Game & game , Frog & frog , bool & hasWon1 , bool & hasWon2 , bool & hasWon3 , bool & wantToPlay ) {
	if ( frog.win && !hasWon1 ) {
		hasWon1 = true;
		game.firstLvl = game.timeForLastLVL;
		game.score1lvl = game.lastScore;
	}
	else if ( frog.win && !hasWon2 ) {
		hasWon2 = true;
		game.secondLvl = game.timeForLastLVL;
		game.score2lvl = game.lastScore;
	}
	else if ( frog.win && !hasWon3 ) {
		hasWon3 = true;
		game.thirdLvl = game.timeForLastLVL;
		game.score3lvl = game.lastScore;
	}
	if ( hasWon3 ) {
		finalInfo ( game , constant , wantToPlay );
	}
	else {
		char input = 'x';
		while ( input != 'y' && input != 'q' ) {
			textcolor ( LIGHTBLUE );
			gotoxy ( constant.X_END_INFO , constant.Y_END_INFO + constant.CORECTOR1 );
			printf ( "Thank you for playing! Do you want to play again? y-new game, q-exit" );
			textcolor ( WHITE );
			input = getch ();
			if ( input == 'q' )
				wantToPlay = false;
		}
	}
}

void loadingLevels ( Const & constants , Board * board , Frog *frog , Car * car , Car *& cars , Tree * tree , Bush * bush , Tree *& trees ,
					 Bush *& bushes , Coin * coin , Coin *& coins , Stork * stork, bool&hasWon1, bool & hasWon2, bool & hasWon3) {
	if ( !hasWon1 ) {
		const char * lvl1 = "config1.txt";
		loadFile ( constants , lvl1 , board , frog , car , cars , tree , bush , trees , bushes , coin , coins , stork );
	}
	else if ( hasWon1 && !hasWon2 ) {
		const char * lvl2 = "config2.txt";
		loadFile ( constants , lvl2 , board , frog , car , cars , tree , bush , trees , bushes , coin , coins , stork );

	}
	else if ( hasWon2 && !hasWon3 ) {
		const char * lvl3 = "config3.txt";
		loadFile ( constants , lvl3 , board , frog , car , cars , tree , bush , trees , bushes , coin , coins , stork );
	}
}

void menu ( Game & game ) {
	bool wantToPlay = true;
	bool hasWon1 = false;
	bool hasWon2 = false;
	bool hasWon3 = false;
	Const constants;
	loadConstants ( &constants );
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
		loadingLevels ( constants , board , &frog , car , cars , tree , bush , trees , bushes , coin , coins , &stork , hasWon1 , hasWon2 , hasWon3 );
		delete car , coin;
		delete tree , bush;
		gameHandler ( constants , game , board , frog , cars , trees , bushes , coins , stork , wantToPlay);
		nextLvl ( constants , game , frog , hasWon1 , hasWon2 , hasWon3 , wantToPlay );
		delete[] cars;
		delete board;
		clrscr ();
	}
	gotoxy ( constants.X_END_INFO , constants.Y_END_INFO );
	printf ( "see you next time! (press any key to close window...)" );
	getch ();
}

int main () {
	settitle ( "Krzysztof Szudy s197771 - Jumping Frog" );
	_setcursortype ( _NOCURSOR );
	srand ( time ( NULL ) );
	Game game;
	menu ( game );
}
