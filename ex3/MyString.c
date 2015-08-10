// ---------------------------- includes --------------------------------

#include "MyString.h"
#include <stdlib.h> // For malloc..
#include <assert.h>
#include <string.h> // memcmp and memcpy

// ---------------------------- constants -------------------------------

#define EOS '\0' // End-of-String
#define ZERO_CHAR '0'
#define INT_BASE 10
#define MINUS_SIGN '-'
#define LESSER -1
#define GREATER 1
#define EQUAL 0

struct _MyString
{
	char* _str;
	size_t _len;
};

// ------------------------ helper functions ----------------------------

static char digToChar(int digit)
{
	return digit + ZERO_CHAR;
}

static int charToDig(char chr)
{
	int charVal = chr - ZERO_CHAR;
	if (charVal > 9 || charVal < 0)
	{
		return MYSTR_ERROR_CODE;
	}
	return charVal;
}

static int pow(int base, unsigned int powerVal)
{
	if (powerVal == 0)
	{
		return 1;
	}
	int result = base;
	for (int i = 1; i < powerVal; i++)
	{
		result *= base;
	}
	return result;
}

static int defaultComparator(const MyString *str1, const MyString *str2)
{
	if (str1 == NULL || str2 == NULL)
	{
		return MYSTR_ERROR_CODE;
	}

	if (str1->_len == str2->_len)
	{ // If the string have the same size, compare chars
		for (int i = 0; i < str1->_len; i++)
		{
			if (str1->_str[i] < str2->_str[i])
			{
				return LESSER;
			}
			else if (str1->_str[i] > str2->_str[i])
			{
				return GREATER;
			}
		}
		return EQUAL;
	}
	else
	{ // Otherwise, the longer string will be the greater one.
		return str1->_len < str2->_len ? LESSER : GREATER;
	}
}

static MyStringRetVal strAlloc(char** dest, size_t strLen)
{
	char* temp = *dest;
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
MyString * myStringAlloc()
{
	MyString* newString = (MyString*)malloc(sizeof(MyString));
	if (!newString == NULL)
	{ // These are the empty string values (that I chose).
		newString->_len = 0;
		newString->_str = NULL;
	}
	return newString;
}

void myStringFree(MyString *str)
{
	if (str == NULL)
	{
		return;
	}
	free(str->_str);
	str->_str = NULL;
	free(str);
	return;
}

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

MyStringRetVal myStringSetFromMyString(MyString *str, const MyString *other)
{
	if (str == NULL || other == NULL)
	{
		return MYSTRING_ERROR;
	}
	if (strAlloc(&(str->_str), other->_len) == MYSTRING_ERROR)
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

MyStringRetVal myStringFilter(MyString *str, bool (*filt)(const char *))
{
	if (str == NULL || filt == NULL || str->_str == NULL)
	{
		return MYSTRING_ERROR;
	}
	// Go over the string from the end to the start
	for (int i = str->_len - 1; i >= 0; i--)
	{
		if (filt(str->_str[i]))
		{ // If a char should be filtered, put the next char instead of it and decrease the length
			if (i < str->_len - 1)
			{
				str->_str[i] = str->_str[i + 1];
			}
			str->_len--;
		}
	}
	// Try to resize the memory block, since the content may have gotten smaller
	char* temp = str->_str;
	str->_str = (char*)realloc(str->_str, str->_len); 
	if (str->_str != NULL)
	{
		free(temp);
	}
	else
	{
		str->_str = temp;
	}
	// Even if reallocation didn't succeed, the operation still succeeded.
	return MYSTRING_SUCCESS;
}

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
	while (n / i != 0)
	{
		i *= INT_BASE;
		len++;
	}
	if (n < 0)
	{
		len++;	
	}
	if (strAlloc(&(str->_str), len) == MYSTRING_ERROR)
	{
		return MYSTRING_ERROR;
	}
	int j = 0;
	if (n < 0)
	{
		str->_str[j] = MINUS_SIGN;
		j++;
		n = -n;
	}
	for (; j < len; j++)
	{
		i /= INT_BASE;
		str->_str[j] = digToChar(n / i);
		n %= i;
	}
	str->_len = len;
	return MYSTRING_SUCCESS;
}

int myStringToInt(const MyString *str)
{
	if (str == NULL || str->_str == NULL)
	{
		return MYSTR_ERROR_CODE;
	}
	int total = 0, charValue, i = 0;
	bool isNegative = false;
	if (str->_str[i] == MINUS_SIGN)
	{
		isNegative = true;
		i++;
	}
	for (; i < str->_len; i++)
	{
		if (charValue = charToDig(str->_str[i]) != MYSTR_ERROR_CODE)
		{
			total += charValue * pow(INT_BASE, str->_len - i - 1);
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

char * myStringToCString(const MyString *str)
{
	if (str == NULL)
	{
		return NULL;
	}
	char* newStr = (char*)malloc(sizeof(char) * (str->_len + 1)); // + 1 for EOS
	if (newStr != NULL)
	{
		int i = 0;
		for (; i < str->_len; i++)
		{
			newStr[i] = str->_str[i];
		}
		newStr[i] = EOS;
	}
	return newStr;
}

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
	free(temp);
	temp = NULL;
	// Start to copy (str2 to end of str1)
	for (int i = 0; i < src->_len; i++)
	{
		dest->_str[dest->_len + i] = src->_str[i];
	}
	dest->_len = newLen;
	return MYSTRING_SUCCESS;
}

MyStringRetVal myStringCatTo(const MyString *str1, const MyString *str2, MyString *result)
{
	if (result == NULL || str1 == NULL || str2 == NULL)
	{
		return MYSTRING_ERROR;
	}
	size_t newLen = str1->_len + str2->_len;
	if (result->_len < newLen)
	{ // if the dest. size is too small, allocate a new MyString
		myStringFree(result);
		if ((result = myStringClone(str1)) != NULL) // Clone the str1
		{
			return myStringCat(result, str2); // Then use regular cat to append str2
		}
		else
		{
			return MYSTRING_ERROR;
		}
	} 
	else // This means the result size is OK
	{
		for (int i = 0; i < str1->_len; i++)
		{
			result->_str[i] = str1->_str[i];
		}
		for (int i = 0; i < str2->_len; i++)
		{
			result->_str[str1->_len + i] = str2->_str[i];
		}
		result->_len = newLen; // Try to reallocate the block because
							   // It might be smaller now.
		result->_str = (char*)realloc(result->_str, sizeof(char) * newLen);
		return MYSTRING_SUCCESS;
	}
}

int myStringCompare(const MyString *str1, const MyString *str2)
{
	return myStringCustomCompare(str1, str2, &defaultComparator); // Use custom compare with default comparator
}

int myStringCustomCompare(const MyString *str1, const MyString *str2,
						  int(*comp)(const MyString*, const MyString*))
{
	if (str1 == NULL || str2 == NULL)
	{ // Uncomparable strings
		return MYSTR_ERROR_CODE;
	}
	return comp(str1, str2);
}

int myStringEqual(const MyString *str1, const MyString *str2)
{
	return myStringCustomEqual(str1, str2, defaultComparator);
}

int myStringCustomEqual(const MyString *str1, const MyString *str2,
						int(*comp)(const MyString*, const MyString*))
{
	if (str1 == NULL || str2 == NULL)
	{
		return MYSTR_ERROR_CODE;
	}
	if (str1->_len != str2->_len)
	{
		return false;
	}
	// Note that true == 1 and false == 0 according to stdbool.h documentation
	return myStringCustomCompare(str1, str2, comp) == 0 ? true : false;
}


unsigned long myStringMemUsage(const MyString *str1)
{
	// The size of the struct MyString + the size allocated for the chars
	return (str1->_len * sizeof(char)) + sizeof(MyString);
}

unsigned long myStringLen(const MyString *str1)
{
	return str1->_len; // Hmm...
}

MyStringRetVal myStringWrite(const MyString *str, FILE *stream)
{
	if (str == NULL)
	{
		return MYSTRING_ERROR;
	}
	for (int i = 0; i < str->_len; i++)
	{
		if (fputc(str->_str[i], stream) == EOF) // EOF means the writing failed.
		{
			return MYSTRING_ERROR;
		}
	}
	return MYSTRING_SUCCESS;
}

void myStringCustomSort(MyString* arr[], int len, int (*comp)(const MyString*, const MyString*))
{
	qsort(arr, len, sizeof(MyString*), comp);
}

void myStringSort(MyString* arr[], int len)
{
	myStringCustomSort(arr, len, defaultComparator);
}