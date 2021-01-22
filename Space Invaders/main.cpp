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


// ----- Global Variables -----



// Initializing body of the spaceship.

const int screenHeight = 25;
const int screenWidth = 100;

char spaceShip[5][5];


unsigned int spacePosX = 0;
unsigned int spacePosY = 0;


void loadAssets() {


	// --- Loading spaceship. ---

	// Loading the body of spaceship.

	strcpy(spaceShip[0], "     ");
	strcpy(spaceShip[1], "     ");
	strcpy(spaceShip[2], "     ");
	strcpy(spaceShip[3], "     ");
	strcpy(spaceShip[4], "     ");


	// Filling in the body.

	spaceShip[0][2] = char(177);

	spaceShip[1][1] = char(177);
	spaceShip[1][2] = char(177);
	spaceShip[1][3] = char(177);
	
	spaceShip[2][0] = char(177);
	spaceShip[2][1] = char(177);
	spaceShip[2][2] = char(177);
	spaceShip[2][3] = char(177);
	spaceShip[2][4] = char(177);

	spaceShip[3][0] = char(177);
	spaceShip[3][1] = char(177);
	spaceShip[3][2] = char(177);
	spaceShip[3][3] = char(177);
	spaceShip[3][4] = char(177);

	spaceShip[4][1] = char(177);
	spaceShip[4][2] = char(177);
	spaceShip[4][3] = char(177);
	
}



void printSpaceShip() {


	// Printing spaceship
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {

			gotoxy(spacePosX + j, spacePosY + i);
			cout << spaceShip[i][j];

		}
	}


}

void fillSkyWithStars() {


	srand(time(NULL));


	for (int i = 0; i < 5; i++) {

		for (int j = 0; j < 5; j++) {


			// Generating a random number to fill the sky.

			int rW = rand() % screenWidth;
			int rH = rand() % screenHeight;

			gotoxy(rW, rH);
			cout << char(167);

		}

	}

}


// Holds the main game functionality.

void startGame() {

	loadAssets();											// Calling this to load all of the data.

	bool keyPressed = false;								// This makes sure that we only print when we need to.

	printSpaceShip();										// Printing the spaceship initially.

	while (true) {

		if (keyPressed) {
			fillSkyWithStars();								// Filling the sky with stars.
			printSpaceShip();								// Printing the spaceship after every turn.
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x27000) {			// checking if right key has been pressed.

			if (spacePosX < screenWidth)
				spacePosX++;

			keyPressed = true;


		}
		else if (GetAsyncKeyState(VK_LEFT) & 0x25000) {		// checking if the left key has been pressed.

			if (spacePosX > 0)
				spacePosX--;
			
			keyPressed = true;


		}
		else if (GetAsyncKeyState(VK_UP) & 0x26000) {		// checking if up key has been pressed.

			if (spacePosY > 0)
				spacePosY--;

			keyPressed = true;

		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x28000) {

			if (spacePosY < screenHeight)
				spacePosY++;

			keyPressed = true;

		}
		else {

			keyPressed = false;
		
		}
		
		Sleep(10);												// Delay function.
		
		if (keyPressed)
			system("cls");										// Clearing screen.
	
	
	}

}

int main() {

	startGame();

	return 0;
}