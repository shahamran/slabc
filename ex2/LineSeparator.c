#include "LineSeparator.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        return 0;
    }
    FILE *inFile = fopen(argv[1], "r");
    if (inFile == 0)
    {
        return 0;
    }
    int currChar;
    char* line;
    return 0;
}