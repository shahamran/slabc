#include <stdio.h>

#define LETTER_CASE_INTERVAL ('a' - 'A')
#define MAX_INPUT_LENGTH 50
#define ASCII_NULL 0
#define NEW_LINE '\n'
#define MID_VALUE '5'
#define MIN_VALUE '0'
#define MAX_VALUE '9'

int changeLetterCase(int letter);
int changeNumber(int letter);
int main();

/** 
 * @brief Change the letter case from upper to lower or vise versa
 * @param letter The letter character to change
 * @return If the input wasn't a letter, returns the input unchanged.
 * 		   Otherwise, changes an uppercase letter to a lowercase one and vise versa.
 */
int changeLetterCase(int letter)
{
	if (letter >= 'A' && letter <= 'Z')
	{
		letter += LETTER_CASE_INTERVAL;
	}
	else if (letter >= 'a' && letter <= 'z')
	{
		letter -= LETTER_CASE_INTERVAL;
	}
	return letter;
}

/** 
 * @brief Change every digit < 5 to 0 and every digit >= 5 to 8
 * @param letter The letter character to change
 * @return If the input wasn't a digit, returns the input unchanged.
 *		   otherwise, returns 0 if input < 5 or 8 if input >= 5.
 */
int changeNumber(int letter)
{
	if (letter >= MIN_VALUE && letter < MID_VALUE)
	{
		letter = '0';
	}
	else if (letter >= MID_VALUE && letter <= MAX_VALUE)
	{
		letter = '8';
	}
	return letter;
}

/**
 *  @brief The main manager function. Gets a line of input and prints the output.
 */
int main()
{
	char output[MAX_INPUT_LENGTH], input[MAX_INPUT_LENGTH]; // input saves the original input.
	int currentChar, i = 0;

	// Get input from user.
	while ((i < MAX_INPUT_LENGTH) && ((currentChar = getchar()) != NEW_LINE))
	{
		input[i] = currentChar;
		// Changes input if needed
		currentChar = changeLetterCase(currentChar);
		currentChar = changeNumber(currentChar);
		output[i] = currentChar;
		i++;
	}
	
	// Fill remaining cells with null values to avoid weird chars.
	for (; i < MAX_INPUT_LENGTH; i++)
	{
		input[i] = ASCII_NULL;
		output[i] = ASCII_NULL;
	}

	printf("\"%s\" -> \"%s\"\n", &input, &output);
	return 0;
}