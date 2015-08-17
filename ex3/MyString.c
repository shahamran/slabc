/**
 * ___Design_Details___
 * 
 * _MyString struct: I chose to define two variables in the struct:
 * an int to specify the string length and a pointer to a char which will hold the
 * address of the first char in our string. This allows dynamic and flexible memory
 * management since the variables don't need to be in a continuos block of memory
 * (except the string).
 *
 * Memory Allocations: Memory allocation for the struct itself is made naturally by the
 * myStringAlloc() function which returns a pointer to the allocated struct. This function
 * is also used by the Clone function and all the unit-tests.
 * Allocation for the actual chars-set is usually made by the static strAlloc() helper function 
 * which gets a pointer to our char pointer and the size of the block to allocate. 
 * Most of the functions that set strings (from MyString, from CString etc.) use this function
 * to allocate dynamic-heap memory block for the string.
 * The memory for the structs is being freed at the myStringFree() function which frees both
 * the memory allocated for the string and the memory allocated to the struct.
 * When a memory block needs resizing, realloc() function is used to allocate more memory to the
 * same block or move it if needed. According to the documentation this function considers
 * efficiency factors in the decision whether to really change the block-size or not (e.g 1 byte
 * changes) so my implementation does not contain those considerations.
 *  
 * Wherever possible, I have used the memcpy function provided by string.h library which is
 * supposed to be faster than other methods of copying data because of compiler-optimization
 * reasons.
 * 
 * Note that runtime-complexity described in the documentation uses notation like (w/o malloc)
 * and (w malloc). The complexity of malloc is undetermined thus my complexity description is
 * mainly about my implementation (w/o malloc).
 */

// ---------------------------- includes --------------------------------

#include "MyString.h"
#include <stdlib.h> // For malloc, free, etc..
#include <assert.h>
#include <string.h> // memcmp and memcpy

// ---------------------------- constants -------------------------------

#define EOS '\0' // End-of-String
#define ZERO_CHAR '0'
#define INT_BASE 10 // The decimal base
#define MINUS_SIGN '-'
#define LESSER -1
#define GREATER 1
#define EQUAL 0

/**
 * The struct that holds a pointer to the string data and an int indicating the length of
 * the string. _len = 0, _str = NULL indicates the empty string.
 */
struct _MyString
{
	char* _str;
	size_t _len;
};

// A typedef for comparators in this implementation
typedef int (*pComparator)(const char, const char);

// ------------------------ helper functions ----------------------------

/**
 * A simple helper function that returns the char value of a digit (0 <= int <= 9)
 */
inline static char digToChar(int digit)
{
	return (digit + ZERO_CHAR);
}

/**
 * Gets a digit char and returns the number it represents.
 * @param chr The char of the digit.
 * @return An int between 0 to 9 if the conversion succeeded, MYSTR_ERROR_CODE otherwise.
 */
static int charToDig(char chr)
{
	int charVal = chr - ZERO_CHAR;
	if (charVal > 9 || charVal < 0)
	{
		return MYSTR_ERROR_CODE;
	}
	return charVal;
}

/**
 * Returns base^powerVal.
 */
static int intPow(int base, unsigned int powerVal)
{
	if (powerVal == 0)
	{
		return 1;
	}
	int result = base;
	for (unsigned int i = 1; i < powerVal; i++)
	{
		result *= base;
	}
	return result;
}

/**
 * A comparator that compares the natural order of chars according to ascii values.
 * @param c1
 * @param c2
 * @return EQUAL (0) if c1 == c2, LESSER (-1) if c1 < c2, GREATER (1) otherwise.
 */
static int defaultComparator(const char c1, const char c2)
{
	if (c1 == c2)
	{
		return EQUAL;
	}
	else if (c1 < c2)
	{
		return LESSER;
	}
	else
	{
		return GREATER;
	}
}

/**
 * Attempts to allocate dynamic-heap memory in a given size for a string.
 * @param dest A pointer to the pointer to the first char of the string.
 * @param strLen The length of the string.
 * @return MYSTRING_SUCCESS if the allocation was successful (*dest was changed),
 *         MYSTRING_ERROR otherwise (*dest remains unchanged).
 */
static MyStringRetVal strAlloc(char** dest, size_t strLen)
{
	if (strLen == 0)
	{
		free(*dest);
		*dest = NULL;
		return MYSTRING_SUCCESS;
	}
	char* temp = *dest; // save previous memory
	*dest = (char*)malloc(sizeof(char) * strLen);
	if (*dest == NULL)
	{
		*dest = temp;
		return MYSTRING_ERROR;
	}
	else
	{
		free(temp);
		return MYSTRING_SUCCESS;
	}
}

// ------------------------- implementations ----------------------------

/**
 * This function's complexity is the complexity of malloc(sizeof(MyString)) function, which
 * is undetermined. Without the malloc it runs in O(1).
 */
MyString * myStringAlloc()
{
	MyString* newString = (MyString*)malloc(sizeof(MyString));
	if (newString != NULL)
	{ // These are the empty string values (that I chose).
		newString->_len = 0;
		newString->_str = NULL;
	}
	return newString;
}

/**
 * Runs in O(1).
 */
void myStringFree(MyString *str)
{
	if (str == NULL)
	{ // Do nothing for NULL pointers.
		return;
	}
	free(str->_str);
	str->_str = NULL;
	free(str);
	return;
}

/**
 * Uses myStringSetFromMyString which Runs in O(n) where n is the length of the string that is 
 * being cloned (w/o malloc), or undetermined amount of time (w malloc)
 */
MyString * myStringClone(const MyString *str)
{
	assert(str != NULL);
	MyString* newString = myStringAlloc();
	if (newString != NULL)
	{
		if (myStringSetFromMyString(newString, str) == MYSTRING_ERROR)
		{
			myStringFree(newString);
			newString = NULL;
		}		
	}
	return newString;
}

/**
 * Runs in O(n) where n is the length of the (other) string (w/o malloc)
 */
MyStringRetVal myStringSetFromMyString(MyString *str, const MyString *other)
{
	if (str == NULL || other == NULL)
	{
		return MYSTRING_ERROR;
	}
	else if (strAlloc(&(str->_str), other->_len) == MYSTRING_ERROR)
	{
		return MYSTRING_ERROR;
	}
	else
	{
		str->_len = other->_len;
		memcpy(str->_str, other->_str, sizeof(char) * str->_len);
		return MYSTRING_SUCCESS;
	}
}

/**
 * Runs in O(n) where n is the length of str's content.
 */
MyStringRetVal myStringFilter(MyString *str, bool (*filt)(const char *))
{
	if (str == NULL || filt == NULL || str->_str == NULL)
	{
		return MYSTRING_ERROR;
	}
	// Go over the string from the end to the start
	for (int i = str->_len - 1; i >= 0; i--)
	{
		if (filt(&(str->_str[i])))
		{ // If a char should be filtered, put the next char instead of it and decrease the length
			if (i < (int) (str->_len - 1))
			{
				str->_str[i] = str->_str[i + 1];
			}
			str->_len--;
		}
	}
	// Try to resize the memory block, since the content may have gotten smaller
	char* temp = str->_str;
	str->_str = (char*)realloc(str->_str, str->_len); 
	if (str->_str == NULL)
	{
		str->_str = temp;
	}
	return MYSTRING_SUCCESS;
}

/**
 * Runs in O(len(cString)).
 */
MyStringRetVal myStringSetFromCString(MyString *str, const char * cString)
{
	if (str == NULL || cString == NULL)
	{
		return MYSTRING_ERROR;
	}
	size_t len = 0;
	while (cString[len] != EOS)
	{ // Check the c string length
		len++;
	}
	// Try to allocate memory for the dest string
	if (strAlloc(&(str->_str), len) == MYSTRING_ERROR)
	{
		return MYSTRING_ERROR;
	}
	else
	{ // Copy the content excluding the EOS char (len == strlen)
		memcpy(str->_str, cString, sizeof(char) * len);
		str->_len = len;
		return MYSTRING_SUCCESS;
	}
}

/**
 * Runs in O(n) where n is the number of digits the given int consists of.
 */
MyStringRetVal myStringSetFromInt(MyString *str, int n)
{
	if (str == NULL)
	{
		return MYSTRING_ERROR;
	}
	if (n == 0) // SPECIAL CASE: n == 0
	{
		if (strAlloc(&(str->_str), 1) == MYSTRING_ERROR)
		{
			return MYSTRING_ERROR;
		}
		str->_len = 1;
		*(str->_str) = ZERO_CHAR;
		return MYSTRING_SUCCESS;
	} // END OF n == 0
	size_t i = 1, len = 0;
	while (n / i != 0) // This finds the number of digits the input number consists of (len)
	{
		i *= INT_BASE;
		len++;
	}
	if (n < 0)
	{
		len++; // ++ for the MINUS sign
	}
	if (strAlloc(&(str->_str), len) == MYSTRING_ERROR)
	{
		return MYSTRING_ERROR;
	}
	unsigned int j = 0;
	if (n < 0)
	{
		str->_str[j] = MINUS_SIGN;
		j++;
		n = -n; 
	}
	// From now on, n is a positive number
	for (; j < len; j++)
	{ // This extracts the MSD (most-significant-digit) from the number
		i /= INT_BASE;
		str->_str[j] = digToChar(n / i);
		n %= i;
	}
	str->_len = len;
	return MYSTRING_SUCCESS;
}

/**
 * Runs in O(n) where n is the number of digits the string consists of.
 */
int myStringToInt(const MyString *str)
{
	if (str == NULL || str->_str == NULL)
	{
		return MYSTR_ERROR_CODE;
	}
	int total = 0, charValue;
	unsigned int i = 0;
	bool isNegative = false;
	if (str->_str[i] == MINUS_SIGN)
	{
		isNegative = true;
		i++;
	}
	for (; i < str->_len; i++)
	{
		// Tries to parse the char as a digit and then multiply it by 10^(location_of_digit)
		if ((charValue = charToDig(str->_str[i])) != MYSTR_ERROR_CODE)
		{
			total += charValue * intPow(INT_BASE, str->_len - i - 1);
		}
		else
		{
			return MYSTR_ERROR_CODE;
		}
	}
	if (isNegative)
	{
		total = -total;
	}
	return total;
}

/**
 * Runs in O(str->_len) (linear in the length of the string).
 */
char * myStringToCString(const MyString *str)
{
	if (str == NULL)
	{
		return NULL;
	}
	char* newStr = (char*)malloc(sizeof(char) * (str->_len + 1)); // + 1 for EOS
	if (newStr != NULL)
	{
		memcpy(newStr, str->_str, sizeof(char) * str->_len);
		newStr[str->_len] = EOS;
	}
	return newStr;
}

/**
 * Let n = len(dest), m = len(src). If realloc succeeds (enlarges the memory block in-place),
 * runs in O(m), otherwise, runs in O(n + m).
 */
MyStringRetVal myStringCat(MyString * dest, const MyString * src)
{
	size_t newLen = dest->_len + src->_len;
	// Reallocate memory for the combined string.
	char* temp = dest->_str;
	dest->_str = (char*)realloc(dest->_str, sizeof(char) * newLen);
	if (dest->_str == NULL)
	{
		dest->_str = temp;
		return MYSTRING_ERROR;
	}
	// Start to copy (str2 to end of str1)
	memcpy(&(dest->_str[dest->_len]), src->_str, sizeof(char) * src->_len);
	dest->_len = newLen;
	return MYSTRING_SUCCESS;
}

/**
 * Like before, n = len(str1), m = len(str2). Runs in O(n + m)
 */
MyStringRetVal myStringCatTo(const MyString *str1, const MyString *str2, MyString *result)
{
	if (result == NULL || str1 == NULL || str2 == NULL)
	{
		return MYSTRING_ERROR;
	}
	size_t newLen = str1->_len + str2->_len;
	if ((result->_len < newLen) || (result->_len > newLen))
	{ // if the dest. size is too small (or big), allocate a new block for the string
		result->_str = (char*)realloc(result->_str, sizeof(char) * newLen);
		if (result->_str == NULL)
		{
			return MYSTRING_ERROR;
		}
	}
	// At this point, result->_str has the right length.
	memcpy(result->_str, str1->_str, sizeof(char) * str1->_len); // Copy str1 --> result
	memcpy(&(result->_str[str1->_len]), str2->_str, sizeof(char) * str2->_len); // str2 --> result
	result->_len = newLen;
	return MYSTRING_SUCCESS;
}

/**
 * Runs the same as myStringCustomCompare.
 */
int myStringCompare(const MyString *str1, const MyString *str2)
{
	return myStringCustomCompare(str1, str2, &defaultComparator); // Use custom compare with default comparator
}

/**
 * Let n = len(str1), m = len(str2), k = min{n, m}. runs in O(k).
 */
int myStringCustomCompare(const MyString *str1, const MyString *str2, pComparator comp)
{
	int result;
	if (str1 == NULL || str2 == NULL)
	{ // Uncomparable strings
		return MYSTR_ERROR_CODE;
	} // Check all chars of the smaller string to see if there's a difference
	for (unsigned int i = 0; (i < str1->_len) && (i < str2->_len); i++)
	{
		result = comp(str1->_str[i], str2->_str[i]);
		if (result < EQUAL)
		{
			return LESSER;
		}
		else if (result > EQUAL)
		{
			return GREATER;
		}
	} // If this is reached, the chars on both strings match until the end of ther shorter
	if (str1->_len == str2->_len)
	{ // If they share the same length, they are therefore equal.
		return EQUAL;
	}
	else
	{ // Otherwise, the longer is the larger (since EVERY_CHAR > NULL)
		return str1->_len < str2->_len ? LESSER : GREATER;
	}
}

/**
 * Same as myStringCustomEqual.
 */
int myStringEqual(const MyString *str1, const MyString *str2)
{
	return myStringCustomEqual(str1, str2, &defaultComparator);
}

/**
 * If len(str1) != len(str2), runs in O(1). Otherwise, runs in O(n).
 */
int myStringCustomEqual(const MyString *str1, const MyString *str2, pComparator comp)
{
	if (str1 == NULL || str2 == NULL)
	{
		return MYSTR_ERROR_CODE;
	} // Different lengths are not equal.
	if (str1->_len != str2->_len)
	{
		return false;
	}
	// Note that true == 1 and false == 0 according to stdbool.h documentation
	return myStringCustomCompare(str1, str2, comp) == 0 ? true : false;
}

/**
 * Runs in O(1).
 */
unsigned long myStringMemUsage(const MyString *str1)
{
	// The size of the struct MyString + the size allocated for the chars
	return (str1->_len * sizeof(char)) + sizeof(MyString);
}

/**
 * Runs in O(1).
 */
unsigned long myStringLen(const MyString *str1)
{
	return str1->_len; // Hmm...
}

/**
 * Runs in O(len(str)).
 */
MyStringRetVal myStringWrite(const MyString *str, FILE *stream)
{
	if (str == NULL)
	{
		return MYSTRING_ERROR;
	}
	for (unsigned int i = 0; i < str->_len; i++)
	{
		if (fputc(str->_str[i], stream) == EOF) // EOF means the writing has failed.
		{
			return MYSTRING_ERROR;
		}
	}
	return MYSTRING_SUCCESS;
}

/**
 * Let n be the average length of a string in the array, This function runs in
 * (Complexity-of-qsort) * n <= n * len * log(len)
 */
void myStringCustomSort(MyString* arr[], int len, pComparator comp)
{
	// A function that gets 2 pointers to a pointer to MyString and compares them.
	int strCompare(const void* a, const void* b)
	{
		return myStringCustomCompare(*(MyString**)a, *(MyString**)b, comp);
	}
	qsort(arr, (size_t) len, sizeof(MyString*), strCompare);
}

/**
 * Same as myStringCustomSort.
 */
void myStringSort(MyString *arr[], int len)
{
	myStringCustomSort(arr, len, &defaultComparator);
}

// ------------------------ Unit-testing --------------------------------

#ifndef NDEBUG

#include <stdarg.h> // for freeAll function

#define SEPARATOR "--------------------\n"
#define MAX_LINE_LENGTH 150
#define TEST_FILE_NAME "unitTest.out"
#define WRITE_ONLY "w"
#define READ_ONLY "r"

/**
 * Toy comparator for chars. Checks if the chars are close enough on the ascii table.
 * @param a
 * @param b
 * @return EQUAL if |a - b| <= EPSILON, LESSER if a < b - EPSILON, GREATER if a > b + EPSILON
 * EPSILON == 3.
 */
static int customComp(const char a, const char b)
{
	int EPSILON = 3;
	if (a - b <= EPSILON && a - b >= -EPSILON)
	{
		return EQUAL;
	}
	else if (a - b > EPSILON)
	{
		return GREATER;
	}
	else
	{
		return LESSER;
	}
}

/**
 * Allocates a memory block in the dynamic-heap for a given cstring.
 * @param str A C-String (sequence of char that ends with EOS[=='\0'])
 * @return A pointer to a memory block containing a copy of the given string if allocation
 * succeeds, NULL otherwise.
 */
static char* getHeapStr(char* str)
{
	int len = strlen(str) + 1; // + 1 for '\0'
	char* dest = (char*)malloc(sizeof(char) * len);
	if (dest != NULL)
	{
		memcpy(dest, str, sizeof(char) * len);
	}
	return dest;
}

/**
 * Frees all MyString pointers that are given.
 * @param numOfItems The number of MyString pointers following this parameter.
 */
static void freeAll(int numOfItems, ...)
{
	va_list ap;
	va_start(ap, numOfItems);
	for (int i = 0; i < numOfItems; i++)
	{
		myStringFree(va_arg(ap, MyString*));
	}
	va_end(ap);
}

/**
 * Tests myStringAlloc() function.
 */
bool testAlloc()
{
	MyString* a = myStringAlloc();
	if (a == NULL)
	{
		printf("Could not allocate memory for a MyString object\n");
		return false;
	}
	myStringFree(a);
	return true;
}

// Not sure how to test the myStringFree() function.

/**
 * Tests myStringClone() function.
 */
bool testClone()
{
	char* txt = getHeapStr("bye");
	if (txt == NULL)
	{
		printf("Could not allocate memory for a string\n");
		return false;
	}
	MyString * s1 = myStringAlloc();
	s1->_len = 4; s1->_str = txt; // txt is bye (+ '\0')
	MyString * s2 = myStringClone(s1);
	if (s2 == NULL)
	{
		printf("Could not allocate memory for a clone\n");
		myStringFree(s1);
		return false;
	}
	if (strcmp(s1->_str, s2->_str) != 0)
	{
		printf("Didn't clone the right values.\n source: %s \t dest: %s\n",
			   s1->_str, s2->_str);
		freeAll(2, s1, s2);
		return false;
	}
	freeAll(2, s1, s2);
	return true;
}

/**
 * Tests myStringSetFromMyString() function.
 */
bool testSetFromMyString()
{
	MyString *s1 = myStringAlloc(), *s2 = myStringAlloc();
	MyStringRetVal ret = myStringSetFromMyString(s2, s1);
	freeAll(2, s1, s2);
	if (ret == MYSTRING_ERROR)
	{
		printf("Failed to set MyString from another\n");
		return false;
	}
	return true;
}

/**
 * A toy filter function that filters the chars 'o' and '!'.
 */
static bool myFilt(const char* ch)
{
	return (*ch == 'o' || *ch == '!');
}

/**
 * Tests myStringFilter() function.
 */
bool testStringFilter()
{
	char *ORIGINAL = "Hello!", *EXPECTED = "Hell";
	char* txt = getHeapStr(ORIGINAL);
	size_t len = 7;
	if (txt == NULL)
	{
		printf("Could not allocate memory for a string\n");
		return false;
	}	
	MyString *str = myStringAlloc();
	str->_len = len;
	str->_str = txt;
	MyStringRetVal ret = myStringFilter(str, myFilt);
	if (ret == MYSTRING_ERROR)
	{
		printf("Failed to filter string\n");
		myStringFree(str);
		return false;
	}
	if (strcmp(str->_str, EXPECTED) != 0)
	{
		printf("Error in filter.\n Expected: %s \t Got: %s\n",
			   EXPECTED, str->_str);
		myStringFree(str);
		return false;
	}
	myStringFree(str);
	return true;
}

/**
 * Tests myStringSetFromCString() function.
 */
bool testSetFromCString()
{
	char* cString = "Hello!";
	MyString *str = myStringAlloc(); 
	MyStringRetVal ret = myStringSetFromCString(str, cString);
	myStringFree(str);
	if (ret == MYSTRING_ERROR)
	{
		printf("Failed to set string from C string: %s\n", cString);
		return false;
	}
	return true;
}

/**
 * Tests myStringSetFromInt() function.
 */
bool testSetFromInt()
{
	int myInt = 1908; // The year Kinneret Moshava was founded
	MyString *str = myStringAlloc();
	MyStringRetVal ret = myStringSetFromInt(str, myInt);
	if (ret == MYSTRING_ERROR)
	{
		printf("Failed to set string from int: %d\n", myInt);
		myStringFree(str);
		return false;
	}
	char* cString = myStringToCString(str);
	if (atoi(cString) != myInt)
	{
		printf("Error converting from int.\nExpected %d \t Got: %d\n",
			   myInt, atoi(cString));
		free(cString);
		myStringFree(str);
		return false;
	}
	free(cString);
	myStringFree(str);
	return true;
}

/**
 * Tests myStringToInt() function.
 */
bool testStringToInt()
{
	int len = 4, myInt = 1908;
	char* intStr = malloc(sizeof(char) * len);
	if (intStr == NULL)
	{
		printf("Could not allocate memory for a string\n");
		return false;
	}
	intStr[0] = '1'; intStr[1] = '9';
	intStr[2] = '0'; intStr[3] = '8';
	MyString *str = myStringAlloc();
	str->_len = len;
	str->_str = intStr;
	int result = myStringToInt(str);
	myStringFree(str);
	if (result != myInt)
	{
		printf("Error converting to int.\nExpected %d \t Got: %d\n", 1908, result);
		return false;
	}
	return true;
}

/**
 * Tests myStringToCString() function.
 */
bool testStringToCString()
{
	int len = 6;
	char* txt = (char*)malloc(sizeof(char) * len), *EXPECTED = "blalab";
	if (txt == NULL)
	{
		printf("Could not allocate memory for a string\n");
		return false;
	}
	txt[0] = 'b'; txt[1] = 'l'; txt[2] = 'a';
	txt[3] = 'l'; txt[4] = 'a'; txt[5] = 'b';
	MyString *str = myStringAlloc();
	str->_len = 6;
	str->_str = txt;
	char* cString = myStringToCString(str);
	myStringFree(str);
	if (strcmp(cString, EXPECTED) != 0)
	{
		printf("Error converting to C String.\nExpected %s \t Got: %s\n", EXPECTED, cString);
		free(cString);
		return false;
	}
	free(cString);
	return true;
}

/**
 * Tests myStringCat() function.
 */
bool testCat()
{
	char *txt1 = "Hello ", *txt2 = "World!", *EXPECTED = "Hello World!";
	MyString *s1 = myStringAlloc(),
		     *s2 = myStringAlloc();
	myStringSetFromCString(s1, txt1);
	myStringSetFromCString(s2, txt2);
	MyStringRetVal result = myStringCat(s1, s2);
	if (result == MYSTR_ERROR_CODE)
	{
		printf("Failed appending \"%s\" to \"%s\"\n", txt2, txt1);
		freeAll(2, s1, s2);
		return false;
	}
	char *actual = myStringToCString(s1);
	if (strcmp(EXPECTED, actual) != 0)
	{
		printf("Failed appending \"%s\" to \"%s\"\n", txt2, txt1);
		printf("Expected:\"%s\" \t Got:\"%s\"\n", EXPECTED, actual);
		freeAll(2, s1, s2); free(actual);
		return false;
	}
	freeAll(2, s1, s2); free(actual);
	return true;
}

/**
 * Tests myStringCatTo() function.
 */
bool testCatTo()
{
	char *txt1 = "Hello ", *txt2 = "World!", *EXPECTED = "Hello World!";
	MyString *s1 = myStringAlloc(),
		     *s2 = myStringAlloc(),
		     *s3 = myStringAlloc();
	myStringSetFromCString(s1, txt1);
	myStringSetFromCString(s2, txt2);
	if (myStringCatTo(s1, s2, s3) == MYSTRING_ERROR)
	{
		printf("Failed appending \"%s\" to \"%s\"\n", txt2, txt1);
		freeAll(3, s1, s2, s3);
		return false;
	}
	char *actual = myStringToCString(s3);
	freeAll(3, s1, s2, s3);
	if (strcmp(EXPECTED, actual) != 0)
	{
		printf("Failed appending \"%s\" to \"%s\"\n", txt2, txt1);
		printf("Expected:\"%s\" \t Got:\"%s\"\n", EXPECTED, actual);
		free(actual);
		return false;
	}
	free(actual);
	return true;
}

// myStringCompare uses myStringCustomCompare so checking it is pointless.

/**
 * Tests myStringCustomCompare() function.
 */
bool testCustomCompare()
{
	MyString *s1 = myStringAlloc(),
		     *s2 = myStringAlloc();
	myStringSetFromInt(s1, 92);
	myStringSetFromInt(s2, 70);
	int result = myStringCustomCompare(s1, s2, &customComp);
	freeAll(2, s1, s2);
	if (result == MYSTR_ERROR_CODE)
	{
		printf("Function reported an error.\n");
		return false;
	}
	if (result != EQUAL)
	{
		printf("Error comparing using custom compare.\n");
		printf("Expected %d \t Got %d\n", EQUAL, result);
		return false;
	}
	return true;
}

// Same for equal and customEqual.

/**
 * Tests myStringCustomEqual() function.
 */
bool testCustomEqual()
{
	char *eqString = "We both are equal.",
		 *notEqString = "I'm not, actually.";
	MyString *s1 = myStringAlloc(),
		     *s2 = myStringAlloc();
	myStringSetFromCString(s1, eqString);
	myStringSetFromCString(s2, eqString);
	int result = myStringCustomEqual(s1, s2, &customComp);
	if (result == MYSTR_ERROR_CODE)
	{
		printf("Function reported an error.\n");
		freeAll(2, s1, s2);
		return false;
	}
	if (result == 0)
	{
		printf("Function says strings aren't equal though they are.\n");
		freeAll(2, s1, s2);
		return false;
	}
	myStringSetFromCString(s2, notEqString);
	result = myStringCustomEqual(s1, s2, &customComp);
	freeAll(2, s1, s2);
	if (result != 0)
	{
		printf("Function says strings are equal though they aren't.\n");
		return false;
	}
	return true;
}

/**
 * Tests myStringMemUsage() function.
 */
bool testMemUsage()
{
	char *str = "Memory";
	size_t len = 6;
	MyString *s1 = myStringAlloc();
	myStringSetFromCString(s1, str);
	size_t result = myStringMemUsage(s1),
		   actual = len * sizeof(char) + sizeof(MyString);
	myStringFree(s1);
	if (result != actual)
	{
		printf("Returned unexpected value: %lu for the memory usage which is: %lu.\n", result, actual);
		return false;
	}
	return true;
}

/**
 * Tests myStringLen() function.
 */
bool testLen()
{
	char *str = "12345678";
	size_t len = 8;
	MyString *s1 = myStringAlloc();
	myStringSetFromCString(s1, str);
	size_t result = myStringLen(s1);
	myStringFree(s1);
	if (result != len)
	{
		printf("Unexpected string length for string: %s.\nExpected: %lu, got: %lu", str, len, result);
		return false;
	}
	return true;
}

/**
 * Tests myStringWrite() function.
 */
bool testWrite()
{
	char *str = "This is a test.\n", *fileName = TEST_FILE_NAME;
	MyString *s1 = myStringAlloc();
	MyStringRetVal ret = myStringSetFromCString(s1, str);
	if (ret != MYSTRING_SUCCESS)
	{
		printf("Error setting string.\n");
		myStringFree(s1);
		return false;
	}
	FILE* inFile = fopen(fileName, WRITE_ONLY);
	if (inFile == NULL)
	{
		printf("Error opening file: %s\n", fileName);
		myStringFree(s1);
		return false;
	}
	ret = myStringWrite(s1, inFile);
	myStringFree(s1);
	if (ret != MYSTRING_SUCCESS)
	{
		printf("Error writing to file.\n");
		return false;
	}
	fclose(inFile);
	inFile = fopen(fileName, READ_ONLY);
	char line[MAX_LINE_LENGTH];
	fgets(line, MAX_LINE_LENGTH, inFile);
	fclose(inFile);
	if (strcmp(line, str) != 0)
	{
		printf("Unexpected file content.\nExpected: %s, got: %s.\n", str, line);
		return false;		
	}
	return true;
}

/**
 * Prints all the contents of an array of MyString*
 */
static void printArr(MyString* arr[], int len)
{
	printf("Printing array:\n");
	char* temp;
	for (int i = 0; i < len; i++)
	{
		temp = myStringToCString(arr[i]);
		printf("str[%d]: %s\n", i + 1, temp);
		free(temp);
		temp = NULL;
	}
}

/**
 * Tests myStringCustomSort() function.
 */
bool testCustomSort()
{
	int len = 6;
	char *txt1 = " nothing", *txt2 = "Aa", *txt3 = "Shalom",
		 *txt4 = "Something", *txt5 = "a", *txt6 = "eb";
	char* sorted[] = { txt1, txt2, txt3, txt4, txt5, txt6 }; // This is a sorted array.
	MyString *s1 = myStringAlloc(), *s2 = myStringAlloc(), *s3 = myStringAlloc(),
		   	 *s4 = myStringAlloc(), *s5 = myStringAlloc(), *s6 = myStringAlloc();
	// Set all MyString objects to a random permutation of the strings above.
	myStringSetFromCString(s1, txt6); myStringSetFromCString(s2, txt3); 
	myStringSetFromCString(s3, txt2); myStringSetFromCString(s4, txt5); 
	myStringSetFromCString(s5, txt1); myStringSetFromCString(s6, txt4);
	MyString* arr[] = { s1, s2, s3, s4, s5, s6 };
	myStringCustomSort(arr, len, customComp); // Sort.
	char* result = NULL;
	for (int i = 0; i < len; i++)
	{
		result = myStringToCString(arr[i]);
		if (strcmp(sorted[i], result) != EQUAL)
		{
			printf("Array isn't sorted as expected.\n");
			printArr(arr, len);
			free(result);
			freeAll(len, s1, s2, s3, s4, s5, s6);
			return false;
		}
		free(result);
		result = NULL;
	}
	freeAll(len, s1, s2, s3, s4, s5, s6);
	return true;
}

// myStringSort function uses CustomSort function, so checking it is pointless.

/**
 * Runs one test. Prints SUCCEEDED if the test was successful, FAILED otherwise.
 */
void runTest(char* testName, bool (*test)(void))
{
	static unsigned int testNum = 1;
	char *result = "SUCCEEDED";
	printf("Running test %u: %s\n", testNum, testName);
	if (!(*test)()) // i.e test() returned false - meaning the test has failed
	{
		result = "FAILED";
	}
	printf("%s!\n", result);
	testNum++;
	printf(SEPARATOR);
}

/**
 * Runs all unit-tests written above.
 */
void runAllTests()
{
	printf("Start unit-tests...\n");
	printf(SEPARATOR);

	runTest("myStringAlloc", testAlloc);
	runTest("myStringClone", testClone);
	runTest("myStringSetFromMyString", testSetFromMyString);
	runTest("myStringFilter", testStringFilter);
	runTest("myStringSetFromCString", testSetFromCString);
	runTest("myStringSetFromInt", testSetFromInt);
	runTest("myStringToInt", testStringToInt);
	runTest("myStringToCString", testStringToCString);
	runTest("myStringCat", testCat);
	runTest("myStringCatTo", testCatTo);
	runTest("myStringCustomCompare", testCustomCompare);
	runTest("myStringCustomEqual", testCustomEqual);
	runTest("myStringMemUsage", testMemUsage);
	runTest("myStringLen", testLen);
	runTest("myStringWrite", testWrite);
	runTest("myStringCustomSort", testCustomSort);

	printf("Finish unit-tests...\n");
	return;
}

/**
 * The main function that calls runAllTests()
 */
int main()
{
	runAllTests();
	return 0;
}

#endif