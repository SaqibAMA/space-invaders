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


unsigned int spacePosX = 45;
unsigned int spacePosY = 20;

unsigned int currentLevel = 1;
unsigned int planeLife = 10;
unsigned int playerScore = 0;


void printSpaceShip(int clear = 0) {


	// This function puts 1s in the places where the spaceship
	// is supposed to be.

	// This function also acts as a reset button by putting 0
	// if the clear flag is on.


	board[spacePosY][spacePosX + 2] = !clear;


	board[spacePosY + 1][spacePosX + 1] = !clear;
	board[spacePosY + 1][spacePosX + 2] = !clear;
	board[spacePosY + 1][spacePosX + 3] = !clear;


	board[spacePosY + 2][spacePosX + 0] = !clear;
	board[spacePosY + 2][spacePosX + 1] = !clear;
	board[spacePosY + 2][spacePosX + 2] = !clear;
	board[spacePosY + 2][spacePosX + 3] = !clear;
	board[spacePosY + 2][spacePosX + 4] = !clear;


	board[spacePosY + 3][spacePosX + 0] = !clear;
	board[spacePosY + 3][spacePosX + 1] = !clear;
	board[spacePosY + 3][spacePosX + 2] = !clear;
	board[spacePosY + 3][spacePosX + 3] = !clear;
	board[spacePosY + 3][spacePosX + 4] = !clear;


	board[spacePosY + 4][spacePosX + 1] = !clear;
	board[spacePosY + 4][spacePosX + 2] = !clear;
	board[spacePosY + 4][spacePosX + 3] = !clear;



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


	// Printing the health bar.
	for (int i = 0; i < planeLife; i++) {
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


}

void fireBullet() {

	if (spacePosY > 0) {

		// If there is enemy ahead?
		if (board[spacePosY - 1][spacePosX + 2] == ENEMY) {


			// Killing the enemy.
			board[spacePosX - 1][spacePosX + 2] = BLANK;
		
			// Updating the score.
			playerScore++;

			
		}
		else {

			// Adding the bullet.
			board[spacePosY - 1][spacePosX + 2] = BULLET;

		}
	}

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

			// If this is an enemy bullet?
			if (board[i][j] == BULLET) {

				// If we are within bounds of the board.
				if (i > 0) {

					// If the next position is enemy?
					if (board[i - 1][j] == ENEMY) {

						// Delete that enemy.
						board[i - 1][j] = BLANK;

						// Update the score.
						playerScore++;

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

			}

		}

	}

}

// Holds the main game functionality.

void startGame() {


	printSpaceShip();										// Printing the spaceship initially.

	while (true) {


		printSpaceShip();									// Printing the spaceship after every turn.
		printBoard();										// Prints the board array on the screen.


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

		updateScreen();											// This function handles all the movement of enemies, bullets, etc.

		Sleep(5);												// Delay function.	
	
	}

}

int main() {

	startGame();

	return 0;
}