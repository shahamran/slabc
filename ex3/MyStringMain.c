#include "MyString.h"
#define MAX_INPUT_LENGTH 501
#define FILE_NAME "test.out"

char* getUserInput(const char* msg, char* dest)
{
	printf("%s\n", msg);
	scanf("%s", dest);
	return dest;
}

void test1(const char* fileName)
{
	FILE *inFile = fopen(fileName, "w");
	if (inFile == NULL)
	{
		printf("Problem with opening the file: %s\n", fileName);
		return;
	}
	char A[MAX_INPUT_LENGTH];
	char B[MAX_INPUT_LENGTH];
	getUserInput("Enter string A:", A);
	getUserInput("Enter string B:", B);
	MyString *myA = myStringAlloc();
	MyString *myB = myStringAlloc();
	myStringSetFromCString(myA, A);
	myStringSetFromCString(myB, B);
	int compResult = myStringCompare(myA, myB);
	char smaller[5];
	smaller = compResult <
	fclose(inFile);
}

int main()
{
	test1(FILE_NAME);
	return 0;
}