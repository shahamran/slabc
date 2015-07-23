#include <stdio.h>
#define LETTER_CASE_INTERVAL ('a' - 'A')
#define MAX_INPUT_LENGTH 50
#define ASCII_NULL 0
#define NEW_LINE '\n'
#define MID_VALUE '5'
#define MIN_VALUE '0'
#define MAX_VALUE '9'

// Change the letter case from upper to lower or vise versa
// Other characters remain unchanged
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

// Change every digit < 5 to 0 and every digit >= 5 to 8
// Other characters remain unchanged
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

int main()
{
	char output[MAX_INPUT_LENGTH], input[MAX_INPUT_LENGTH];
	int currentChar, i = 0;

	// Get input from user.
	while ((i < MAX_INPUT_LENGTH) && ((currentChar = getchar()) != NEW_LINE))
	{
		input[i] = currentChar;
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