#include <stdio.h>

// The difference between lower and upper case letters in ASCII
#define LETTER_CASE_INTERVAL ('a' - 'A')
#define MAX_INPUT_LENGTH 50
// The null ascii value
#define ASCII_NULL 0
#define NEW_LINE '\n'
// The range of numbers to change (MIN_ to MID_, MID_ to MAX_)
#define MID_VALUE '5'
#define MIN_VALUE '0'
#define MAX_VALUE '9'
// The numbers that existing numbers will be changed to
#define SMALL_NUM '0'
#define BIG_NUM '8'
// The range of letters to change (starting from UPPER/LOWER_START and ending with _END)
#define LOWER_START 'a'
#define LOWER_END 'z'
#define UPPER_START 'A'
#define UPPER_END 'Z'

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
	if (letter >= UPPER_START && letter <= UPPER_END)
	{
		letter += LETTER_CASE_INTERVAL;
	}
	else if (letter >= LOWER_START && letter <= LOWER_END)
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
		letter = SMALL_NUM;
	}
	else if (letter >= MID_VALUE && letter <= MAX_VALUE)
	{
		letter = BIG_NUM;
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