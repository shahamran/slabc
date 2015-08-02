#include "MyString.h"
#define STRING_END '\0'
#define NO_APPEAR 0

// Enum for Boolean type.
typedef enum {FALSE = 0, TRUE = 1} Boolean;

unsigned int countSubStr(const char* str1, const char* str2, int isCyclic)
{
    /* i,j are indices for the loops.
     * numOfAppear counts the number of appearances of str2 in str1
     * sourceLen holds the 'length' of str1 to be used if isCyclic is true. */
    int i, j, numOfAppear = NO_APPEAR, sourceLen = 0;
    Boolean isMatch;
    // Searching an empty string or in an empty string results in 0.
    if (str1 == NULL || str2 == NULL || *str2 == STRING_END)
    {
        return NO_APPEAR;
    }

    // __THE FUNCTION'S FUNCTIONALITY STARTS HERE__
    // Check if str2 matches str1[i:] substring, for i = 0 to length(str1)
    for (i = 0; *(str1 + i) != STRING_END; i++)
    {
        isMatch = TRUE; // <=> isMatch = 1;
        for (j = 0; *(str2 + j) != STRING_END; j++)
        {
            // Check if the end of str1 was reached
            if (*(str1 + i + j - sourceLen) == STRING_END)
            {
                // If in cyclic mode, go back to the start of the string (i + j - sourceLen = 0)
                if (isCyclic)
                {
                    sourceLen = i + j;
                }
                else /* If not in cyclic, the substring str1[i:] is shorter than str2,
                        So there is no point to keep checking. */
                {
                    return numOfAppear;
                }
            }
            // If 2 chars differ, current substring does not match. stop checking.
            if (*(str2 + j) != *(str1 + i + j - sourceLen))
            {
                isMatch = FALSE;
                break;
            }
        } // for loop (j) ends here
        // NOTE that isMatch is an enum which gets the value 0 for FALSE and 1 for TRUE.
        // If two chars differ between str1 and str2, isMatch will be FALSE at this point.
        numOfAppear += isMatch;
    } // for loop (i) ends here
    return numOfAppear;
}

int main()
{
    char str1[50] = "", str2[50] = "";
    int isCyclic = 0;
    printf("str1 str2: ");
    scanf("%s %s", str1, str2);
    printf("is cyclic? (1 or 0): ");
    scanf("%d", &isCyclic);
    printf("%s appears %d times in %s\n", str2,
           countSubStr(str1, str2, isCyclic), str1);
    return 0;
}

