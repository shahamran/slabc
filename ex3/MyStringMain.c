#include "MyString.h"

#define MAX_INPUT_LENGTH 501
#define WRITE_ONLY "w"
#define FILE_NAME "test.out"
#define SUCCESS 0
#define FAILURE 1
#define EQUAL 0

static char* getUserInput(const char* msg, char* dest)
{
	printf("%s\n", msg);
	scanf("%s", dest);
	return dest;
}

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
		big   = myStringSetToCString(b);
		small = myStringSetToCString(a);
	}
	else
	{
		big   = myStringSetToCString(a);
		small = myStringSetToCString(b);
	} // Write to file
	fprintf(inFile, "%s is smaller than %s\n", small, big);

	// Cleanup
	free(big);       free(small);
	myStringFree(a); myStringFree(b);
	fclose(inFile);
	return SUCCESS;
}

int main()
{
	return test1(FILE_NAME);
}