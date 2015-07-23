#include <stdio.h>

#define MAX_BALLS_ALLOWED 3
#define MIN_BALLS_ALLOWED 1

#define NUM_OF_PLAYERS 2
#define NUM_OF_BOXES 2
#define STARTING_PLAYER 1
#define SEPERATOR_LENGTH 15

#define SEPERATOR "-"
#define BALL_SHAPE "o"
#define NEGATIVE_NUM_OF_BALLS_MSG "Number of balls in box must be positive.\n"


int ballsAmount[NUM_OF_BOXES], currentPlayer = STARTING_PLAYER;

void printXTimes(char charToPrint[], int numOfCopies)
{
	int i;
	for (i = 0; i < numOfCopies; i++)
	{
		printf(charToPrint);
	}
	printf("\n");
}

void printBalls()
{
	int i;
	printXTimes(SEPERATOR, SEPERATOR_LENGTH);
	for (i = 1; i <= NUM_OF_BOXES; i++)
	{
		printf("Box %d: ", i);
		printXTimes(BALL_SHAPE, ballsAmount[i-1]);
	}
	printXTimes(SEPERATOR, SEPERATOR_LENGTH);
	return;
}

int getBallsAmount()
{
	int i, numOfBalls;
	for (i = 1; i <= NUM_OF_BOXES; i++)
	{
		printf("How many balls in box %d?\n", i);
		scanf("%d", &numOfBalls);
		if (numOfBalls > 0)
		{
			ballsAmount[i-1] = numOfBalls;
		}
		else
		{
			printf(NEGATIVE_NUM_OF_BALLS_MSG);
			return 0;
		}
	}
	return 1;
}

int chooseBox() 
{
	int chosenBox = 0;
	do 
	{
		printf("Player %d, choose a box (1 or 2):\n", currentPlayer);
		scanf("%d", &chosenBox);
	}
	while(chosenBox < 1 || chosenBox > NUM_OF_BOXES || ballsAmount[chosenBox-1] == 0);
	return chosenBox;
}

int chooseAmount(int chosenBox)
{
	int chosenBalls = 0, shouldStop = 0;
	do
	{
		printf("Player %d, how many balls do you want to take from box %d?\n"
		       ,currentPlayer, chosenBox + 1);
		scanf("%d", &chosenBalls);
		if (chosenBalls <= 0)
		{
			printf("Number of balls to take must be positive.\n");
		}
		else if (chosenBalls > MAX_BALLS_ALLOWED)
		{
			printf("Cannot take more than %d balls at a time.\n", MAX_BALLS_ALLOWED);
		}
		else if (chosenBalls > ballsAmount[chosenBox])
		{
			printf("Cannot take more balls than what's in the box.\n");
		}
		else
		{
			shouldStop = 1;
		}
	} 
	while (!shouldStop);
	return chosenBalls;
}

void handlePlayerTurn()
{
	int chosenBox = 0, chosenBalls = 0;
	printf("Player %d, it's your turn.\n", currentPlayer);
	chosenBox = chooseBox() - 1;
	chosenBalls = chooseAmount(chosenBox);
	ballsAmount[chosenBox] -=  chosenBalls;
	return;
}

int checkWin()
{
	int i;
	for (i = 0; i < NUM_OF_BOXES; i++)
	{
		if (ballsAmount[i] > 0)
		{
			return 0;
		}
	}
	return 1;
}

int main()
{
	if (!getBallsAmount())
	{
		return 0; // Quit if an error has occured
	}
	while (!checkWin())
	{
		printBalls();
		handlePlayerTurn();
		currentPlayer++;
		currentPlayer = ((currentPlayer - 1) % NUM_OF_PLAYERS) + 1;
	}
	printf("Player %d wins the game.\n", currentPlayer);
	return 0;
}