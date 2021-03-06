#include <stdio.h>

#define MAX_BALLS_ALLOWED 3
#define MIN_BALLS_ALLOWED 1
#define NUM_OF_PLAYERS 2
#define NUM_OF_BOXES 2
// Minimal number of balls in a box.
#define MIN_BALLS_IN_BOX 1
#define STARTING_PLAYER 1
// The length (number of chars) of the seperator line.
#define SEPERATOR_LENGTH 15
#define SEPERATOR "-"
#define BALL_SHAPE "o"
#define TRUE 1
#define FALSE 0

int gBox[NUM_OF_BOXES], gCurrentPlayer = STARTING_PLAYER;

void printXTimes(char textToPrint[], int numOfCopies);
void printBoxes();
int getBallsForBoxes();
int chooseBox();
int chooseAmount(int chosenBox);
void handlePlayerTurn();
int checkWin();
int main();

/**
 * @brief Prints a line of X tokens of a given text in a row.
 * @param textToPrint[] The string (charArray) to print in a row.
 * @param numOfCopies The number of copies of the given text.
 */
void printXTimes(char textToPrint[], int numOfCopies)
{
	int i;
	for (i = 0; i < numOfCopies; i++)
	{
		printf(textToPrint);
	}
	printf("\n");
}

/**
 * @brief Prints the current state of all boxes.
 * note that i counts the actual box number, the box index in the gBox array is i-1.
 */
void printBoxes()
{
	unsigned int i;
	printXTimes(SEPERATOR, SEPERATOR_LENGTH);
	for (i = 1; i <= NUM_OF_BOXES; i++)
	{
		printf("Box %d: ", i);
		printXTimes(BALL_SHAPE, gBox[i - 1]); // Prints "o" as the number of balls in the box
	}
	printXTimes(SEPERATOR, SEPERATOR_LENGTH);
	return;
}

/**
 * @brief Gets the user input for the number of balls in each box.
 * @return True (number other than zero) if a legal number was entered, False (zero) otherwise.
 */
int getBallsForBoxes()
{
	int i, numOfBalls; // i counts the actual box number, the index of the box is i-1
	for (i = 1; i <= NUM_OF_BOXES; i++)
	{
		printf("How many balls in box %d?\n", i);
		scanf("%d", &numOfBalls);
		if (numOfBalls >= MIN_BALLS_IN_BOX)
		{
			gBox[i - 1] = numOfBalls;
		}
		else
		{
			printf("Number of balls in box must be positive.\n");
			return FALSE;
		}
	}
	return TRUE;
}

/**
 * @brief Gets the user input for the chosen box.
 * @return The number of the box, as written by the user! (1 to NUM_OF_BOXES)
 */
int chooseBox() 
{
	int chosenBox = 0;
	do 
	{
		printf("Player %d, it's your turn.\n", gCurrentPlayer);
		printf("Player %d, choose a box (1 or 2):\n", gCurrentPlayer);
		scanf("%d", &chosenBox);
	}
	while(chosenBox < 1 || chosenBox > NUM_OF_BOXES);
	return chosenBox;
}

/**
 * @brief Gets the user input for the chosen amount of balls to remove from the box, and
 *		  prints error messages if needed.
 * @param chosenBox The INDEX of the box to remove balls from.
 * @return The amount of balls the user chose to remove.
 */
int chooseAmount(int chosenBox)
{
	int chosenBalls = 0, shouldStop = 0;
	do
	{
		printf("Player %d, how many balls do you want to take from box %d?\n",
		       gCurrentPlayer, chosenBox + 1);
		scanf("%d", &chosenBalls);
		if (chosenBalls < MIN_BALLS_ALLOWED)
		{
			printf("Number of balls to take must be positive.\n");
		}
		else if (chosenBalls > MAX_BALLS_ALLOWED)
		{
			printf("Cannot take more than %d balls at a time.\n", MAX_BALLS_ALLOWED);
		}
		else if (chosenBalls > gBox[chosenBox])
		{
			printf("Cannot take more balls than what's in the box.\n");
		}
		else
		{
			shouldStop = TRUE;
		}
	} 
	while (!shouldStop);
	return chosenBalls;
}

/**
 * @brief Runs the current player's turn.
 */
void handlePlayerTurn()
{
	unsigned int chosenBox = 0, chosenBalls = 0;
	chosenBox = chooseBox() - 1; // Gets the INDEX of the chosen box.
	chosenBalls = chooseAmount(chosenBox);
	gBox[chosenBox] -= chosenBalls;
	return;
}

/**
 * @brief Checks if the game is over (all boxes are empty).
 * @return True (1) if the game is over, False (0) otherwise.
 */
int checkWin()
{
	unsigned int i;
	for (i = 0; i < NUM_OF_BOXES; i++)
	{
		if (gBox[i] <= 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * @brief The manager function of this game.
 */
int main()
{
	if (!getBallsForBoxes())
	{
		return 0; // Quit if an error has occured
	}
	printBoxes();
	while (!checkWin()) // As long as there is a non-empty box
	{
		handlePlayerTurn();
		printBoxes();
		gCurrentPlayer++;
		// gCurrentPlayer holds the actual player number. 
		// The following formula gives us the numbers: 1, 2, ... , NUM_OF_PLAYERS, 1, 2, ... etc
		gCurrentPlayer = ((gCurrentPlayer - 1) % NUM_OF_PLAYERS) + 1;
	}
	// The player that plays after the loser is the winner.
	// (in our case, the player who didn't take the last ball in a box)
	printf("Player %d wins the game.\n", gCurrentPlayer);
	return 0;
}