#include "LineSeparator.h"
#define READ_LINE(l,f) fgets(l, MAX_LINE_LENGTH, f)
#define EPSILON 0.00001
#define EOL '\n'
#define STR_END '\0'

Vector *parseLine(Vector * p_v, const char * line, int dim)
{
    int i;
    char currNum[MAX_LINE_LENGTH];
    for (int j = 0; j < dim; j++)
    {
        i = 0;
        while (*line != ',')
        {
            currNum[i] = *line;
            i++;
            line++;
        }
        line++;
        currNum[i] = STR_END;
        sscanf(currNum, "%lf", &(p_v->data[j]));        
    }
    if (*line != EOL)
    {
        line++;
        sscanf(line, "%d", &(p_v->tag));
    }
    return p_v;
}

void parseFile(FILE *inFile)
{
    char line[MAX_LINE_LENGTH + 1]; // +1 for '\0'
    int dim, numOfPts;
    READ_LINE(line, inFile); // First Line
    sscanf(line, "%d", &dim); // Read the dimension (n)
    assert(dim <= MAX_DIMENSION);
    READ_LINE(line, inFile); // Second Line
    sscanf(line, "%d", &numOfPts); // Read the number of points (m)

    Vector v, w;
    Tag tempTag;
    // Training set:
    for (int i = 0; i < numOfPts; i++)
    {
        READ_LINE(line, inFile);
        parseLine(&v, line, dim);
        tempTag = dotProduct(&w, &v, dim) < EPSILON ? MINUS : PLUS;
        if (tempTag != v.tag)
        {
            addVector(&w, &v, dim);
        }
    }
    // Predictions:
    while (!feof(inFile))
    {
        READ_LINE(line, inFile);
        parseLine(&v, line, dim);
        printf("%d\n", dotProduct(&w, &v, dim) < EPSILON ? MINUS : PLUS);
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: LineSeparator <file_name>\n");
        return 0;
    }

    FILE *inFile = fopen(argv[1], "r");
    if (inFile == 0)
    {
        printf("Unable to open input file: %s\n", argv[1]);
        return 0;
    }
    parseFile(inFile);
    fclose(inFile);
    return 0;
}