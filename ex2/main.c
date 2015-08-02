#include <stdio.h>
#include <string.h>

#define MAX_USER_INPUT 50
#define MY_STRING "str"
#define LINE_SEPARATOR "line"
#define END "end"

void testMyString()
{
    return;
}

void testLineSeparator()
{
    return;
}

int main() {
    char test[MAX_USER_INPUT] = {NULL};
    while (strcmp(scanf("%s", test), END))
    {
        if (!strcmp(test, MY_STRING))
        {
            testMyString();
        }
        else if (!strcmp(test, LINE_SEPARATOR))
        {
            testLineSeparator();
        }
        else
        {
            printf("unknown test. try again:\n");
        }
    }
    return 0;
}