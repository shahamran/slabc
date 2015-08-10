#include "MyString.h"
#include <stdlib.h> // For malloc..
#include <assert.h>
#include <string.h> // memcmp and memcpy

#define EOS '\0'
#define ZERO_CHAR '0'
#define INT_BASE 10
#define MINUS_SIGN '-'

typedef struct _Node
{
	char* _str;
	size_t _len;
	size_t _numOfStr;
} Node;

struct _MyString
{
	Node* _data;
};

// ------------------------ helper functions ----------------------------

static Node * nodeDeepCopy(const Node* node)
{
	char* newStr = (char*)malloc(node->_len);
	if (newStr == NULL)
	{
		return NULL;
	}
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL)
	{
		free(newStr);
		return NULL;
	}
	memcpy(newStr, node->_str, sizeof(char));
	newNode->_len = node->_len;
	newNode->_numOfStr = 1;
	newNode->_str = newStr;
	return newNode;
}

static void freeNode(Node *node)
{
	if (node == NULL)
	{
		return;
	}
	if (node->_numOfStr == 1)
	{
		free(node->_str);
		node->_str = NULL;
		free(node);
	}
	else
	{
		node->_numOfStr--;
	}	
	return;
}

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

// ------------------------- implementations ----------------------------
MyString * myStringAlloc()
{
	MyString* newString = NULL;
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode != NULL)
	{
		newNode->_len = 0;
		newNode->_str = NULL;
		newNode->_numOfStr = 1;
		newString = (MyString*)malloc(sizeof(MyString));
		if (newString == NULL)
		{
			free(newNode);
			newNode = NULL;
		}
		else
		{
			newString->_data = newNode;
		}
	}
	return newString;
}

void myStringFree(MyString *str)
{
	if (str == NULL)
	{
		return;
	}
	assert(str->_data != NULL);
	if (str->_data->_numOfStr == 1)
	{
		free(str->_data->_str);
		str->_data->_str = NULL;
		free(str->_data);
		str->_data = NULL;
		free(str);
	}
	else
	{
		str->_data->_numOfStr--;
		free(str);
	}
}

MyString * myStringClone(const MyString *str)
{
	assert(str != NULL);
	MyString* newString = (MyString*)malloc(sizeof(MyString));
	if (newString != NULL)
	{
		newString->_data = str->_data;
		newString->_data->_numOfStr++;
	}
	return newString;
}

MyStringRetVal myStringSetFromMyString(MyString *str, const MyString *other)
{
	if (str == NULL || other == NULL)
	{
		return MYSTRING_ERROR;
	}
	assert(str->_data != NULL && other->_data != NULL);
	MyString* temp = str;
	str->_data = other->_data;
	str->_data->_numOfStr++;
	myStringFree(temp);
	return MYSTRING_SUCCESS;
}

MyStringRetVal myStringFilter(MyString *str, bool (*filt)(const char *))
{
	if (str == NULL || filt == NULL || str->_data == NULL)
	{
		return MYSTRING_ERROR;
	}
	Node* data = nodeDeepCopy(str->_data);
	if (data == NULL)
	{
		return MYSTRING_ERROR;
	}
	for (int i = data->_len - 1; i >= 0; i--)
	{
		if (filt(data->_str[i]))
		{
			if (i < data->_len - 1)
			{
				data->_str[i] = data->_str[i + 1];
			}
			data->_len--;
		}
	}
	if (str->_data->_len == data->_len) // This means the filter did nothing.
	{
		freeNode(data);
		data = NULL;
	}
	else
	{
		freeNode(str->_data);
		str->_data = data;
	}
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
	{
		len++;
	}
	char* newStr = (char*)malloc(sizeof(char) * len);
	if (newStr == NULL)
	{
		return MYSTRING_ERROR;
	}
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL)
	{
		return MYSTRING_ERROR;
	}
	memcpy(newStr, cString, len);
	newNode->_len = len;
	newNode->_str = newStr;
	newNode->_numOfStr = 1;
	freeNode(str->_data);
	str->_data = newNode;
	return MYSTRING_SUCCESS;
}

MyStringRetVal myStringSetFromInt(MyString *str, int n)
{
	if (str == NULL)
	{
		return MYSTRING_ERROR;
	}
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL)
	{
		return MYSTRING_ERROR;
	}
	newNode->_numOfStr = 1;
	if (n == 0)
	{
		newNode->_len = 1;
		newNode->_str = (char*)malloc(sizeof(char));
		if (newNode->_str == NULL)
		{
			free(newNode);
			return MYSTRING_ERROR;
		}
		*(newNode->_str) = digToChar(0);
		str->_data = newNode;
		return MYSTRING_SUCCESS;
	}
	size_t i = 1, len = 0;
	while (n / i != 0)
	{
		i *= INT_BASE;
		len++;
	}
	char* newStr;
	if (n < 0)
	{
		len++;	
	}
	newStr = (char*)malloc(sizeof(char) * len);
	if (newStr == NULL)
	{
		free(newNode);
		return MYSTRING_ERROR;
	}
	int j = 0;
	if (n < 0)
	{
		newStr[j] = MINUS_SIGN;
		j++;
		n = -n;
	}
	for (; j < len; j++)
	{
		i /= INT_BASE;
		newStr[j] = digToChar(n / i);
		n %= i;
	}
	newNode->_len = len;
	newNode->_str = newStr;
	freeNode(str->_data);
	str->_data = newNode;
}

int myStringToInt(const MyString *str)
{
	int total = 0, charValue, i = 0;
	bool isNegative = false;
	Node* node = str->_data;
	if (node->_str[i] == MINUS_SIGN)
	{
		isNegative = true;
		i++;
	}
	for (; i < node->_len; i++)
	{
		if (charValue = charToDig(node->_str[i]) != MYSTR_ERROR_CODE)
		{
			total += charValue * pow(INT_BASE, node->_len - i - 1);
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
	if (str == NULL || str->_data == NULL)
	{
		return NULL;
	}
	char* newStr = (char*)malloc(sizeof(char) * (str->_data->_len + 1)); // + 1 for EOS
	if (newStr != NULL)
	{
		int i = 0;
		for (; i < str->_data->_len; i++)
		{
			newStr[i] = str->_data->_str[i];
		}
		newStr[i] = EOS;
	}
	return newStr;
}

MyStringRetVal myStringCat(MyString * dest, const MyString * src)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL)
	{
		return MYSTRING_ERROR;
	}
	newNode->_numOfStr = 1;
	newNode->_len = dest->_data->_len + src->_data->_len;
	char* newStr = (char*)malloc(sizeof(char) * newNode->_len);
	if (newStr == NULL)
	{
		free(newNode);
		return MYSTRING_ERROR;
	}
	for (int i = 0; i < dest->_data->_len; i++)
	{
		newStr[i] = dest->_data->_str[i];
	}
	for (int i = 0; i < src->_data->_len; i++)
	{
		newStr[dest->_data->_len + i] = src->_data->_str[i];
	}
	newNode->_str = newStr;
	freeNode(dest->_data);
	dest->_data = newNode;
}

MyStringRetVal myStringCatTo(const MyString *str1, const MyString *str2, MyString *result)
{

}

int myStringCompare(const MyString *str1, const MyString *str2)
{

}

//TODO insert myStringCustomCompare signature here

int myStringEqual(const MyString *str1, const MyString *str2)
{

}

//TODO insert myStringCustomEqual signature here

unsigned long myStringMemUsage(const MyString *str1)
{

}

unsigned long myStringLen(const MyString *str1)
{
	return str1->_data->_len;
}

MyStringRetVal myStringWrite(const MyString *str, FILE *stream)
{

}

//TODO insert myStringCoustomSort signature here

//TODO insert myStringSort signature here