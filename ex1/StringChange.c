#include <stdio.h>
#define LETTER_CASE_INTERVAL ('a' - 'A')
#define MAX_INPUT_LENGTH 50
#define ASCII_NULL 0

// Change the letter case from upper to lower or vise versa
// Other characters remain unchanged
int changeLetterCase(int chr)
{
	if (chr >= 'A' && chr <= 'Z')
	{
		chr += LETTER_CASE_INTERVAL;
	}
	else if (chr >= 'a' && chr <= 'z')
	{
		chr -= LETTER_CASE_INTERVAL;
	}
	return chr;
}

// Change every digit < 5 to 0 and every digit >= 5 to 8
// Other characters remain unchanged
int changeNumber(int chr)
{
	if (chr >= '0' && chr < '5')
	{
		chr = '0';
	}
	else if (chr >= '5' && chr <= '9')
	{
		chr = '8';
	}
	return chr;
}

int main()
{
	char output[MAX_INPUT_LENGTH], input[MAX_INPUT_LENGTH];
	int currentChar, i = 0;

	// Get input from user.
	while ((i < MAX_INPUT_LENGTH) && ((currentChar = getchar()) != '\n'))
	{
		input[i] = currentChar;
		currentChar = changeLetterCase(currentChar);
		currentChar = changeNumber(currentChar);
		output[i] = currentChar;
		i++;
	}
	
	// Fill remaining cells with null values.
	for (; i < MAX_INPUT_LENGTH; i++)
	{
		input[i] = ASCII_NULL;
		output[i] = ASCII_NULL;
	}

	printf("\"%s\" -> \"%s\"\n", &input, &output);
	return 0;
}