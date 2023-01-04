#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

#define COLS 40
#define ROWS 25
//Initialize game
void init(int* ptrX, int* ptrY, int* ptrFruitX, int* ptrFruitY, int* ptrTailLen, int* ptrScore, char* ptrDir) {
	*ptrX = COLS / 2;
	*ptrY = ROWS / 2;
	*ptrFruitX = rand() % COLS;
	*ptrFruitY = rand() % ROWS;
	*ptrTailLen = 0;
	*ptrScore = 0;
	*ptrDir = ' ';
}
//Resets screen
void ResetScreenPosition() {
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}
//Draws screen
void drawScreen(int x, int y, int fruitX, int fruitY, int tailLength, int tailX[255], int tailY[255], int score, int highscore) {
	for (int i = 0; i <= COLS + 1; i++) {
		if (i == 0)
			printf("%c", 201);
		else if (i == COLS + 1)
			printf("%c", 187);
		else
			printf("%c", 205);
	}
	printf("\n");
	for (int i = 0; i < ROWS; i++) {
		printf("%c", 186);
		for (int j = 0; j < COLS; j++) {
			if (i == y && j == x) {
				printf("%c", 178);
			}
			else if (i == fruitY && j == fruitX) {
				printf("%c", 15);
			}
			else {
				int printing = 0;
				for (int k = 0; k < tailLength; k++) {
					if (j == tailX[k] && i == tailY[k]) {
						printf("%c", 176);
						printing = 1;
					}
				}
				if (j % 2 == 0 && !printing)
					printf("-");
				else if (j % 2 == 1 && !printing)
					printf(" ");
			}
		}
		printf("%c", 186);
		printf("\n");
	}
	for (int i = 0; i <= COLS + 1; i++) {
		if (i == 0)
			printf("%c", 200);
		else if (i == COLS + 1)
			printf("%c", 188);
		else
			printf("%c", 205);
	}
	printf("\n");
	printf("Score: %d\n", score);
	printf("Highscore: %d\n", highscore);
}
//Gets player input
void getInput(char* dir, int tailLength, int* gameOver) {
	if (_kbhit()) {
		switch (_getch()) {
		case 'w':
			if (*dir == 's' && tailLength > 0)
				break;
			*dir = 'w';
			break;
		case 'a':
			if (*dir == 'd' && tailLength > 0)
				break;
			*dir = 'a';
			break;
		case 's':
			if (*dir == 'w' && tailLength > 0)
				break;
			*dir = 's';
			break;
		case 'd':
			if (*dir == 'a' && tailLength > 0)
				break;
			*dir = 'd';
			break;
		case 'q':
			*gameOver = 1;
			break;
		}
	}
}
//Calculates snake movement
void movementLogic(int* tailLength, int* tailX, int* tailY, int* x, int* y, char dir, int* gameOver, int* fruitX, int* fruitY, int* score) {
	for (int i = *tailLength; i > 0; i--)
	{
		tailX[i] = tailX[i - 1];
		tailY[i] = tailY[i - 1];
	}
	tailX[0] = *x;
	tailY[0] = *y;
	switch (dir) {
	case 'w':
		*y-=1;
		break;
	case 'a':
		*x-=1;
		break;
	case 's':
		*y = (*y + 1) % ROWS;
		break;
	case 'd':
		*x = (*x + 1) % COLS;
		break;
	}
	for (int i = 0; i < *tailLength; i++)
		if (tailX[i] == *x && tailY[i] == *y)
			*gameOver = 1;
	if (*x < 0) {
		*x = COLS - 1;
	}
	if (*y < 0) {
		*y = ROWS - 1;
	}
	if (*x == *fruitX && *y == *fruitY) {
		*score += 10;
		*fruitX = rand() % COLS;
		*fruitY = rand() % ROWS;
		*tailLength+=1;
	}
}
//Checks for a new highscore
void newBest(int currentScore, int* highscore, FILE** fscore) {
	if (*highscore < currentScore) {
		*highscore = currentScore;
		if ((*fscore = fopen("highscore.txt", "w")) == NULL) {
			printf("Error\n");
		}
		else {
			printf("New best!!!\n");
			fprintf(*fscore, "%d", *highscore);
		}
		fclose(*fscore);
	}
}
//Quit game menu
void quit(int* quitGame, int* gameOver) {
	int validInput;
	char playAgain;
	do {
		printf("Would you like to play again? Yes or No (Y or N). ");
		validInput = scanf(" %c", &playAgain);
		//read past invalid input
		scanf("%*[^\n]");
		if (playAgain != 'Y' && playAgain != 'N') {
			printf("Type Y or N.\n");
			//validInput = 0;
		}
		else if (playAgain == 'N') {
			printf("Thanks for playing.\n");
			//validInput = 1;
			*quitGame = 1;
		}
		else if (playAgain == 'Y') {
			printf("New game starting.\n");
			//validInput = 1;
			*quitGame = 0;
			*gameOver = 0;
			system("pause");
			system("cls");
		}
	} while (validInput == 0);
}
int main() {
	srand(time(NULL));
	int x, y, fruitX, fruitY, tailLength, tailX[255], tailY[255], score, highscore, gameOver = 0, quitGame = 0;
	char dir;
	//Read highscore from text file
	FILE* fscore = fopen("highscore.txt", "r");
	fscanf(fscore, "%d", &highscore);
	fclose(fscore);
	while (quitGame == 0) {
		//Initialize game
		init(&x, &y, &fruitX, &fruitY, &tailLength, &score, &dir);
		while (gameOver == 0) {
			//Draw game screen
			drawScreen(x, y, fruitX, fruitY, tailLength, tailX, tailY, score, highscore);
			//Input
			getInput(&dir, tailLength, &gameOver);
			//Logic
			movementLogic(&tailLength, tailX, tailY, &x, &y, dir, &gameOver, &fruitX, &fruitY, &score);
			//New best highscore
			newBest(score, &highscore, &fscore);
			//Resets screen
			ResetScreenPosition();
			Sleep(99);
		}
		//Play again?
		quit(&quitGame, &gameOver);
	}
	system("pause");
	return 0;
}