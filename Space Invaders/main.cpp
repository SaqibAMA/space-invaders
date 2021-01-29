#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include <cstring>
#include <fstream>

using namespace std;

// ----- Screen Printing Function -----
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void gotoxy(int x, int y) {

	COORD scrn;
	scrn.X = x;
	scrn.Y = y;

	SetConsoleCursorPosition(h, scrn);

}



// ----- Documentation -----


// Game Assets

#define BLANK 0
#define SPACESHIP 1
#define BULLET 2
#define ENEMY 3
#define ENEMY_BULLET 4
#define GIFT 5



// ----- Global Variables -----



// Initializing body of the spaceship.

const int screenHeight = 25;
const int screenWidth = 80;

int board[screenHeight][screenWidth] = { 0 };


unsigned int spacePosX = 50;				// Keeps the spaceship's X coordinate.
unsigned int spacePosY = 20;				// Keeps the spaceship's Y coordinate.

unsigned int currentLevel = 1;
int planeLife = 10;							// Health of the player.
int playerScore = 0;						// Score of the player.

unsigned int giftTaken = 0;					// Has the gift been accepted?
unsigned int orgTime = 0;					// This keeps the original time of the gift.
unsigned int bulletDuration = 0;			// This keeps the time for double bullets.


// Total 3 enemies at a time
// to keep xy-position of all enemies
// 500(out of scope) indicates enemy is not appeared yet
unsigned int enemiesPos[3][2] = {
									{500, 500},
									{500, 500},
									{500, 500}
								};





int randomNumber() {

	// This function generates a random number
	// for different functionality in code.

	// This generates the random number
	// with regard to the screen height.


	srand((unsigned)time(0));
	int i;
	i = (rand() % (screenHeight - 1));
	if (i < 25)
		i = i + 40;
	return i;


}

void createGift() {
	

	// This function generates a random spot using the randomNumber() function
	// and then places the gift on that spot.

	// There is validation that we are not placing the gift at a random spot.


	int random = NULL;						// Generating a random number.


	bool found = false;						// Flag to detect



	while (!found) {

		random = randomNumber();			// Generating a random number.
		found = true;						// Assuming that the row is good enough to place the gift.
		
		for (int i = 0; i < screenHeight; i++) {
			
			// This checks if the entire column is empty or not.

			if ((board[i][random] != BLANK && board[i][random] != SPACESHIP)
				&& board[i][random + 1] != BLANK && board[i][random + 1] != SPACESHIP) {

				found = false; 
			
			}
		
		}

	}


	// Placing the gift on the board.

	board[0][random] = GIFT;
	board[0][random + 1] = GIFT;
	board[1][random] = GIFT;
	board[1][random + 1] = GIFT;


}

char* getSystemTime() {
	
	// This is the standard function that gives us the system
	// time. This is useful in timing the entire game.

	time_t my_time = time(NULL);
	return ctime(&my_time);


}

int convertToMin(char* arr) {
	
	// This takes the system time that has been returned by the
	// getSystemTime function and converts it into minutes.

	int num1 = (int)arr[14] - 48;
	int num2 = (int)arr[15] - 48;
	int time = (num1 * 10) + num2;

	return time;
}

int calculateMin() {
	
	// This function gets the current minute from
	// your system time.

	char* temp = getSystemTime();
	return convertToMin(temp);
}

int newNum(int a) {

	// This is a helper function that helps
	// in gift time calculation.

	if (a + 15 >= 60) {
		return 15 - (60 - a);
	}
	else {
		return a + 15;
	}

}

int convertToSec(char* arr) {

	// This function takes the output from
	// getSystemTime function and converts it into seconds.

	int num1 = (int)arr[17] - 48;
	int num2 = (int)arr[18] - 48;
	int time = (num1 * 10) + num2;

	return time;
}

int calculateSec() {
	
	// This function calculates the number of seconds using
	// the helper function.

	char* temp = getSystemTime();
	return convertToSec(temp);
}

void printSpaceShip(int clear = 0) {


	// This function puts 1s in the places where the spaceship
	// is supposed to be.

	// This function also acts as a reset button by putting 0
	// if the clear flag is on.


	board[spacePosY][spacePosX + 2] = !clear;

	for (int i = 1; i < 4; i++) {

		board[spacePosY + 1][spacePosX + i] = !clear;
		board[spacePosY + 4][spacePosX + i] = !clear;

	}

	for (int i = 0; i < 5; i++) {

		board[spacePosY + 2][spacePosX + i] = !clear;
		board[spacePosY + 3][spacePosX + i] = !clear;

	}



}

void printBoard() {

	for (int i = 0; i < screenHeight; i++) {

		for (int j = 0; j < screenWidth; j++) {

			gotoxy(j, i);

			if (board[i][j] == BLANK) {

				// This prints blank space.
				
				cout << " ";
			
			
			}
			else if (board[i][j] == SPACESHIP) {

				// This prints spaceship body.

				// Setting color to light blue.
				SetConsoleTextAttribute(h, 9);
				cout << char(219);


			}
			else if (board[i][j] == BULLET) {

				// This prints the bullet.

				// Setting color to blue.
				SetConsoleTextAttribute(h, 1);
				cout << char(254);
			
			
			}
			else if (board[i][j] == ENEMY) {

				// This prints the enemy.

				// Setting the color to red.
				SetConsoleTextAttribute(h, 4);
				cout << char(219);

			}
			else if (board[i][j] == ENEMY_BULLET) {

				// This prints the enemy bullet.

				// Settign color to light red.
				SetConsoleTextAttribute(h, 12);
				cout << char(254);

			}
			else if (board[i][j] == GIFT) {

				// This prints the gift.
				SetConsoleTextAttribute(h, 3);
				cout << char(220);

			}

			// Resetting color to white.
			SetConsoleTextAttribute(h, 15);

		}

	}


	// Printing the score and life of the player.
	gotoxy(90, 0);
	cout << "Score: " << playerScore;
	gotoxy(90, 2);
	cout << "Health: ";
	
	// Changing color to light blue.
	SetConsoleTextAttribute(h, 9);


	gotoxy(98, 2);
	cout << "          ";
	// Printing the health bar.
	for (int i = 0; i < planeLife -1; i++) {
		gotoxy(98 + i, 2);
		cout << char(219);
	}

	// Resetting back to white.
	SetConsoleTextAttribute(h, 15);


	// Displaying right border.
	for (int i = 0; i < screenHeight; i++) {
		gotoxy(screenWidth, i);
		cout << char(219);
	}

	// Displaying bottom border.
	for (int i = 0; i <= screenWidth; i++) {
		gotoxy(i, screenHeight);
		cout << char(223);
	}

	// Displaying instructions
	gotoxy(90, 4);
	cout << "Press S to save!";
	gotoxy(90, 5);
	cout << "Press L to load!";


}

void fireBullet() {

	if (spacePosY > 0) {

		// If there is enemy ahead?
		if (board[spacePosY - 1][spacePosX + 2] == ENEMY) {


			// Killing the enemy.
			// board[spacePosX - 1][spacePosX + 2] = BLANK;
		
			// Updating the score.
			if (giftTaken == 1) {
				playerScore += 3;
			}
			else {
				playerScore++;
			}

			
		}
		else {

			// Adding the bullet.
			board[spacePosY - 1][spacePosX + 2] = BULLET;

			
			// If the gift has been taken, double the fire.
			if (giftTaken == 1) {

				if (spacePosY + 4 < screenWidth) {
					board[spacePosY - 1][spacePosX + 4] = BULLET;
				}

			}


		}
	}

}

void printAnEnemy(unsigned int x, unsigned int y, int draw = 3) {

	// This function allows to draw or clear an enemy from a specific
	// position.
	// draw == 0  --> Clear an enemy.
	// draw == 3  --> Draw an enemy.


	// If the coordinates are out of range, then don't execute the function.
	if (x >= screenWidth - 1 || y >= screenHeight - 1) return;

	board[y - 1][x - 1] = draw;
	board[y + 1][x - 1] = draw;

	board[y][x] = draw;

	board[y - 1][x + 1] = draw;
	board[y + 1][x + 1] = draw;


}

void updateScreen() {

	// This array keeps a track of all the indices that we don't
	// have to update.
	bool doNotUpdate[screenHeight][screenWidth] = { 0 };

	for (int i = 0; i < screenHeight; i++) {

		for (int j = 0; j < screenWidth; j++) {

			// Do not continue the iteration if the index has been marked.
			// as not to be updated.
			if (doNotUpdate[i][j] == true) continue;

			// If this is a bullet?
			if (board[i][j] == BULLET) {

				// If we are within bounds of the board.
				if (i > 0) {

					// If the next position is enemy?
					if (board[i - 1][j] == ENEMY) {

						//// Delete that enemy.
						//board[i - 1][j] = BLANK;

						// to get index number of enemey to be deleted
						int ind = (j + 2) / 26;

						// Killing the enemy.
						printAnEnemy(enemiesPos[ind][0], enemiesPos[ind][1], 0);
						enemiesPos[ind][0] = 500;
						enemiesPos[ind][1] = 500;

						// Updating the score.
						if (giftTaken == 1) {
							playerScore += 3;
						}
						else {
							playerScore++;
						}

					}
					else if (board[i - 1][j] == ENEMY_BULLET) {

						board[i - 1][j] = board[i][j] = BLANK;

					}
					else {

						// Clear previous position.
						board[i][j] = BLANK;

						// Move the bullet to next position.
						board[i - 1][j] = BULLET;

						// This index has been updated,
						// do not update it in the next iteration.
						doNotUpdate[i - 1][j] = true;

					}

				}
				else {

					// Erasing the bullet if it has reached
					// the end.
					board[i][j] = BLANK;
				
				}

			}

			// If this is an enemy bullet?
			if (board[i][j] == ENEMY_BULLET) {

				// If we are within bounds of the board.
				if (i < screenHeight - 1)
				{
					// If the next position is spaceship?
					if (board[i + 1][j] == SPACESHIP) {

						// Update the score.
						planeLife--;

					}
					else if (board[i + 1][j] == BULLET) {
					
						board[i][j] = board[i + 1][j] = BLANK;
					
					}
					else {

						// Clear previous position.
						board[i][j] = BLANK;

						// Move the enemy bullet to next position.
						board[i + 1][j] = ENEMY_BULLET;

						// This index has been updated,
						// do not update it in the next iteration.
						doNotUpdate[i + 1][j] = true;

					}

				}
				else {

					board[i][j] = BLANK;

				}
			}

			// Is the cell a gift?
			if (board[i][j] == GIFT && board[i][j + 1] == GIFT) {

				// Is the gift colliding with spaceship at any point?
				if (
					
					(board[i + 3][j] == SPACESHIP
					|| board[i + 3][j + 1] == SPACESHIP)
					
					||
					
					(board[i + 2][j] == SPACESHIP
					|| board[i + 2][j + 1] == SPACESHIP)
					
					) {


					// Setting the giftTaken flag to ON.
					giftTaken = 1;
					bulletDuration = calculateSec();

					// Removing the gift from
					// the place.
					board[i][j] = BLANK;
					board[i][j + 1] = BLANK;
					board[i + 1][j] = BLANK;
					board[i + 1][j + 1] = BLANK;
				
				
				}
				else {

					if (i + 3 < screenHeight) {

						// Move the gift to a position below.

						board[i][j] = BLANK;
						board[i][j + 1] = BLANK;
						board[i + 3][j] = GIFT;
						board[i + 3][j + 1] = GIFT;
						doNotUpdate[i + 3][j] = true;
						doNotUpdate[i + 3][j + 1] = true;
					
					}
					else {

						// Remove the gift if it has reached the end of the board.

						board[i][j] = BLANK;
						board[i][j + 1] = BLANK;
						board[i][j] = BLANK;
						board[i + 1][j + 1] = BLANK;                        

					}


				}


			}
			

		}

	}

}

void gameOver() {

	// This function prints the game over message.

	// Setting the color to black with white background.
	SetConsoleTextAttribute(h, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);


	// Filling the screen with white color.
	for (int i = 0; i < screenHeight; i++) {

		for (int j = 0; j < screenWidth; j++) {

			gotoxy(j, i);
			cout << " ";

		}

	}

	gotoxy(screenWidth / 2 - 5, screenHeight / 2 - 1);
	cout << "GAME OVER!";
	gotoxy(0, screenHeight + 1);


	// Resetting the screen to white on black.
	SetConsoleTextAttribute(h, 15);


}

void randomPosGenerator(unsigned int& x, unsigned int& y, int start, int end) {
	
	
	// This generates a random X and Y coordinate within the range
	// of the screen.

	// The variables have & sign so that the value is changed outside
	// the function as well.

	srand( (unsigned) time(0));

	x = (start + 1 + (rand() % (end - 2)));
	y = 4;

}

void generateEnemy(int enemyNum, bool randomPlaces = true) {

	// This function generates a new enemy.

	unsigned int start = 26 * enemyNum;
	srand( (unsigned) time(0));

	if (randomPlaces) {
		randomPosGenerator(enemiesPos[enemyNum][0], enemiesPos[enemyNum][1], start, 26);
	}
	else {

		int enemiesX[3] = { 25, 40, 55 };
		int enemiesY[3] = { 1, 1, 1 };

		if (enemyNum == 0) {
		
			enemiesPos[0][0] = enemiesX[0];
			enemiesPos[0][1] = enemiesY[0];

		}
		else if (enemyNum == 1) {

			enemiesPos[1][0] = enemiesX[1];
			enemiesPos[1][1] = enemiesY[1];

		}
		else if (enemyNum == 2) {

			enemiesPos[2][0] = enemiesX[2];
			enemiesPos[2][1] = enemiesY[2];

		}


	}

}

void generateAllEnemies() {

	// This function creates all 3 enemies.

	// Get random positions of all the three enemies
	for (int i = 0; i < 3; ++i) {
	
		// Create an enemy
		generateEnemy(i);
	
	}


	// Print enemies on the screen
	for (int i = 0; i < 3; ++i)
	{

		// Print an enemy.
		printAnEnemy(enemiesPos[i][0], enemiesPos[i][1]);

	}

}

bool isEnemyCollide(unsigned int x, unsigned int y) {

	// This function checks if there is a collision between
	// the enemy and the spaceship.

	if (x >= screenWidth - 2 || y >= screenHeight - 2) return false;

	return (
		board[y - 1][x - 2] == SPACESHIP ||
		board[y - 1][x + 2] == SPACESHIP ||
		board[y + 1][x] == SPACESHIP ||
		board[y + 1][x - 2] == SPACESHIP ||
		board[y + 1][x + 2] == SPACESHIP ||
		board[y + 2][x - 1] == SPACESHIP ||
		board[y + 2][x - 1] == SPACESHIP
		);
}

void controlEnemiesMovement() {

	for (unsigned int i = 0; i < 3; ++i) {

		if (enemiesPos[i][1] >= screenHeight - 1) {
			// Clearing the enemy from previous position.
			printAnEnemy(enemiesPos[i][0], enemiesPos[i][1], 0);

			enemiesPos[i][0] = 500;
			enemiesPos[i][1] = 500;
		}

		else {


			// check collision with spaceship
			if (isEnemyCollide(enemiesPos[i][0], enemiesPos[i][1])) {

				// Clearing enemy.
				printAnEnemy(enemiesPos[i][0], enemiesPos[i][1], 0);
				enemiesPos[i][0] = 500;
				enemiesPos[i][1] = 500;


				planeLife--;
			}

			else {
				// clear an enemy
				printAnEnemy(enemiesPos[i][0], enemiesPos[i][1], 0);

				// move enemy down
				enemiesPos[i][1]++;

				// print an enemy
				printAnEnemy(enemiesPos[i][0], enemiesPos[i][1], 3);
			}
		}
	}
}

void enemyFireBullet(unsigned int x, unsigned int y) {

	// Handling the bullet firing by the enemies.

	if (y >= screenHeight - 1) return;

	// If there is spaceship ahead?
	if (board[y + 1][x] == SPACESHIP) {
		// Updating the score.
		planeLife--;
	}
	else {
		// Adding the bullet.
		board[y + 1][x] = ENEMY_BULLET;

	}
}

bool allEnemiesDown() {

	// This function checks if all of the enemies have been downed
	// or not.
	
	bool allDown = true;

	for (int i = 0; i < 3; i++) {

		if (enemiesPos[i][0] != 500) {
			
			allDown = false;
		
		}

	}

	return allDown;

}


void saveGame() {

	// This function deals with all of the game saving functionality.

	ofstream fout;
	fout.open("game.txt");


	// Saving the board
	for (int i = 0; i < screenHeight; i++) {

		for (int j = 0; j < screenWidth; j++) {
			
			fout << board[i][j] << " ";

		}

		fout << endl;

	}

	// Saving the health.
	fout << planeLife << endl;

	// Saving player score.
	fout << playerScore << endl;

	// Saving current level.
	fout << currentLevel << endl;

	// Saving spaceship position
	fout << spacePosX << " " << spacePosY << endl;


	// Saving metrics.
	fout << giftTaken << " " << orgTime << " " << bulletDuration << endl;

	// Saving enemy positions.
	for (int i = 0; i < 3; i++)
		fout << enemiesPos[i][0] << " " << enemiesPos[i][1] << endl;


	// Closing the file.
	fout.close();


}

void loadGame() {

	ifstream fin;
	fin.open("game.txt");

	if (fin) {


		// Saving the board
		for (int i = 0; i < screenHeight; i++) {

			for (int j = 0; j < screenWidth; j++) {

				fin >> board[i][j];

			}

		}

		// Saving the health.
		fin >> planeLife;

		// Saving player score.
		fin >> playerScore;

		// Saving current level.
		fin >> currentLevel;

		// Saving spaceship position
		fin >> spacePosX; fin >> spacePosY;


		// Saving metrics.
		fin >> giftTaken; fin >> orgTime; fin >> bulletDuration;

		// Saving enemy positions.
		for (int i = 0; i < 3; i++) {
			fin >> enemiesPos[i][0]; fin >> enemiesPos[i][1];
		}



	}
	else {

		gotoxy(90, 7);
		cout << "No such file!";
		Sleep(200);

	}

	fin.close();

}


// Holds the main game functionality.

void startGame() {

	bool sendGift = true;

	int enemyFIringPos[3] = { 3, 3, 3 };					// Keep record of firing position of each enemy.
	generateAllEnemies();

	while (true) {


		printSpaceShip();									// Printing the spaceship after every turn.
		printBoard();										// Prints the board array on the screen.

		if (currentLevel <= 2) {
		
			// Making enemies fire continuously.
			for (int i = 0; i < 3; i++) {

				// If enemy was removed
				if (enemiesPos[i][0] == 500) {

					enemyFIringPos[i] = 3;			// Resetting positon.
					generateEnemy(i);				// Generating a new enemy.

				}

				enemyFireBullet(enemiesPos[i][0], enemiesPos[i][1] + enemyFIringPos[i]++);
			}

			controlEnemiesMovement();

		
		}
		else {


			// Only initiate the re-generation if all of the enemies have been downed.
			if (allEnemiesDown()) {

				// Making enemies fire continuously.
				for (int i = 0; i < 3; i++) {

					// If enemy was removed

					enemyFIringPos[i] = 3;			// Resetting positon.
					generateEnemy(i, false);		// Generating a new enemy.


				}
			
			}

			for (int i = 0; i < 3; i++)
				enemyFireBullet(enemiesPos[i][0], enemiesPos[i][1] + enemyFIringPos[i]++);


			controlEnemiesMovement();


		}


		if (GetAsyncKeyState(VK_RIGHT) & 0x27000) {			// checking if right key has been pressed.

			printSpaceShip(1);								// clearing previous spaceship.
			if (spacePosX < screenWidth - 5)
				spacePosX++;

		}
		
		if (GetAsyncKeyState(VK_LEFT) & 0x25000) {		// checking if the left key has been pressed.

			printSpaceShip(1);								// clearing previous spaceship.
			if (spacePosX > 0)
				spacePosX--;

		}
		
		if (GetAsyncKeyState(VK_UP) & 0x26000) {		// checking if up key has been pressed.
			
			printSpaceShip(1);								// clearing previous spaceship.
			if (spacePosY > 0)
				spacePosY--;


		}

		if (GetAsyncKeyState(VK_DOWN) & 0x28000) {

			printSpaceShip(1);								// clearing previous spaceship.
			if (spacePosY < screenHeight - 5)
				spacePosY++;

		}

		if (GetAsyncKeyState(VK_SPACE) & 0x20000) {

			fireBullet();

		}


		// Saving game
		if (GetAsyncKeyState(0x53) & 0x53000) {

			saveGame();
			gotoxy(90, 7);
			cout << "Game Saved!";
			Sleep(200);
			gotoxy(90, 7);
			cout << "            ";


		}

		// Loading the game
		if (GetAsyncKeyState(0x4C) & 0x4C000) {

			loadGame();

		}
		

		updateScreen();											// This function handles all the movement of enemies, bullets, etc.


		// Upper level functionality.
		// Level > 1

		if (currentLevel > 1) {

			if (sendGift) {
				createGift();
				orgTime = calculateMin();
				sendGift = false;
			}

			if (!sendGift) {

				int _time = calculateMin();
				if (_time - orgTime == 1) {

					sendGift = true;

				}
				else if (orgTime == 59 && _time == 0) {

					sendGift = true;

				}

			}

			if (giftTaken == 1) {

				int _time = calculateSec();
				if (newNum(bulletDuration) < _time)
					giftTaken = 0;

			}
		
		}

		if (currentLevel <= 2) {
			Sleep(10);													// Delay function.	
		}
		else {
			Sleep(5);
		}

		// Changing levels based on certain criteria.
		if (playerScore >= 10 && currentLevel == 1) {
			
			currentLevel++;
			playerScore = 0;

		}
		else if (playerScore >= 20 && currentLevel == 2) {

			currentLevel++;
			playerScore = 0;

		}
		else if (playerScore >= 20 && currentLevel == 3) {

			gameOver();
			return;

		}


																	// Checking if the game has ended?
		if (planeLife <= 0) {

			gameOver();
			return;

		}


	}

}

int main() {

	startGame();

	return 0;
}