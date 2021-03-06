#include "MyString.h"
#include <stdlib.h>

#define MAX_INPUT_LENGTH 501
#define WRITE_ONLY "w"
#define FILE_NAME "test.out"
#define SUCCESS 0
#define FAILURE 1
#define EQUAL 0

/**
 * Displays a message given by msg C-String and writes user input to dest.
 */
static char* getUserInput(const char* msg, char* dest)
{
	printf("%s\n", msg);
	scanf("%s", dest);
	return dest;
}

/**
 * Gets input for two MyString objects and compares them using myStringCompare.
 * Writes the output to the given file.
 */
static int test1(const char* fileName)
{
	FILE *inFile = fopen(fileName, WRITE_ONLY);
	if (inFile == NULL)
	{
		printf("Problem with opening the file: %s\n", fileName);
		return FAILURE;
	}
	char userInput[MAX_INPUT_LENGTH], *big, *small;
	MyString *a = myStringAlloc(),
			 *b = myStringAlloc();

	myStringSetFromCString(a, getUserInput("Enter first string:", userInput));
	myStringSetFromCString(b, getUserInput("Enter second string:", userInput));

	int result = myStringCompare(a, b);
	if (result == MYSTR_ERROR_CODE)
	{ // If an error has occured, clean up and quit.
		myStringFree(a); myStringFree(b);
		fclose(inFile);
		return FAILURE;
	}
	if (result <= EQUAL)
	{
		big   = myStringToCString(b);
		small = myStringToCString(a);
	}
	else
	{
		big   = myStringToCString(a);
		small = myStringToCString(b);
	} // Write to file
	fprintf(inFile, "%s is smaller than %s\n", small, big);

	// Cleanup
	free(big);       free(small);
	myStringFree(a); myStringFree(b);
	fclose(inFile);
	return SUCCESS;
}

/**
 * Runs the test.
 */
int main()
{
	return test1(FILE_NAME);
}