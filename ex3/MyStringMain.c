#include "MyString.h"

#define MAX_INPUT_LENGTH 501
#define WRITE_ONLY "w"
#define FILE_NAME "test.out"

static char* getUserInput(const char* msg, char* dest)
{
	printf("%s\n", msg);
	scanf("%s", dest);
	return dest;
}

static void test1(const char* fileName)
{
	FILE *inFile = fopen(fileName, WRITE_ONLY);
	if (inFile == NULL)
	{
		printf("Problem with opening the file: %s\n", fileName);
		return;
	}
	char userInput[MAX_INPUT_LENGTH];
	MyString *a = myStringAlloc(), *b = myStringAlloc(),
			 *smaller, *bigger;
	myStringSetFromCString(a, getUserInput("Enter first string:", userInput));
	myStringSetFromCString(b, getUserInput("Enter second string:", userInput));
	int result = myStringCompare(a, b);
	if (result <= 0)
	{
		smaller = a; bigger = b;
	}
	else
	{
		smaller = b; bigger = a;
	}
	fprintf(inFile, "%s is smaller than %s\n", 
			myStringToCString(smaller), myStringToCString(bigger));
	myStringFree(a); myStringFree(b);
	fclose(inFile);
}

int main()
{
	test1(FILE_NAME);
	return 0;
}