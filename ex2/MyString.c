#include "MyString.h"
#define EOS '\0' // EOS stands for End-of-String
#define NO_APPEAR 0
#define MAX_INPUT 50 // For the main function

// Enum for Boolean type.
typedef enum 
{
    VERY_FALSE = -1,
    FALSE = 0,
    TRUE = 1
} Boolean;

/**
 * Checks if a string (str2) matches another (str1) when starting to read str1 from
 * a certain point (i).
 * @param str1 The source string pointer
 * @param i The starting point for str1
 * @param str2 The string to match to str1 substrings
 * @param isCyclic Same as before..
 * @return TRUE str2 matches str1[i:] substring, FALSE otherwise.
 * Note that TRUE = 1 and FALSE = 0
 */
static Boolean checkMatch(const char* str1, int i, const char* str2, int isCyclic)
{
    int sourceStrLen = 0; // Keeps the number: K * length_of_str1 where
                          // K is the number of times we passed the end of str1 in cyclic mode.
    for (int j = 0; str2[j] != EOS; j++)
    {
        // Check if the end of str1 was reached
        if (str1[i + j - sourceStrLen] == EOS)
        {
            // If in cyclic mode, go back to the start of the string (i + j - sourceStrLen = 0)
            if (isCyclic)
            {
                sourceStrLen = i + j;
            }
            else /* If not in cyclic, the substring str1[i:] is shorter than str2,
                    So there is no point to keep checking. */
            {
                return VERY_FALSE;
            }
        }
        // If 2 chars differ, current substring does not match. stop checking.
        if (str2[j] != str1[i + j - sourceStrLen])
        {
            return FALSE;
        }
    }
    return TRUE; // If this is reached, all the chars that were read were identical.
}

/**
 * Counts the amount of str1 substrings that are equal to str2.
 * In case one (or two) of the strings is empty- returns 0.
 * @str1 - the string
 * @str2 -  the substring
 * @isCycic != 0 - search also for cyclic appearnce 
 * @return number appearance of str2 as substring of str1
 */
unsigned int countSubStr(const char* str1, const char* str2, int isCyclic)
{
    // Counts the number of appearances of str2 in str1.
    int numOfAppear = NO_APPEAR;
    Boolean isMatch;
    // Searching an empty string or in an empty string results in 0.
    if (str1 == NULL || str2 == NULL || *str2 == EOS)
    {
        return NO_APPEAR;
    }

    // Check if str2 matches str1[i:] substring, for i = 0 to length(str1)
    for (int i = 0; str1[i] != EOS; i++)
    {
        isMatch = checkMatch(str1, i, str2, isCyclic);
        if (isMatch == VERY_FALSE) // This means there is no point to keep checking.
        {
            return numOfAppear;
        }
        numOfAppear += isMatch; // Remember that TRUE = 1, FALSE = 0.
    }
    return numOfAppear;
}

/**
 * The main function. A simple test that requests input from the user.
 * @return 0
 */
int main()
{
    char str1[MAX_INPUT], str2[MAX_INPUT];
    int isCyclic;
    printf("str1: ");
    scanf("%s", str1);
    printf("str2: ");
    scanf("%s", str2);
    printf("is cyclic? (1 or 0): ");
    scanf("%d", &isCyclic);
    printf("%s appears %d times in %s\n", str2,
           countSubStr(str1, str2, isCyclic), str1);
    return 0;
}