#include "LineSeparator.h"
#define READ_LINE(l,f) fgets(l, MAX_LINE_LENGTH, f)
#define EPSILON 0.00001
#define EOL '\n'
#define STR_END '\0'
#define SEPARATOR ','

char *readLine(char * destLine, FILE * inFile)
{
    int i = -1;
    do
    {
        i++;
    }
    while ((destLine[i] = fgetc(inFile)) != EOL);
    return destLine;
}

Vector *parseLine(Vector * p_v, const char * line, int dim)
{
    if (*line == EOL)
    {
        return p_v;
    }
    int i;
    char currNum[MAX_LINE_LENGTH];
    for (int j = 0; j < dim; j++)
    {
        i = 0;
        while (*line != SEPARATOR && *line != EOL)
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
        sscanf(line, "%d", &(p_v->tag));
    }
    return p_v;
}

Vector * train(Vector *p_w, Vector *p_v, FILE *inFile, int dim, int numOfPts)
{
    char line[MAX_LINE_LENGTH + 1];
    Tag tempTag = PLUS;
    for (int i = 0; i < numOfPts; i++)
    {
        readLine(line, inFile);
        parseLine(p_v, line, dim);
        tempTag = dotProduct(p_w, p_v, dim) < EPSILON ? MINUS : PLUS;
        if (tempTag != p_v->tag)
        {
            addVector(p_w, p_v, dim);
        }
    }
    return p_w;
}

void parseFile(FILE *inFile)
{
    char line[MAX_LINE_LENGTH + 1]; // +1 for '\0'
    int dim, numOfPts;
    readLine(line, inFile); // First Line
    sscanf(line, "%d", &dim); // Read the dimension (n)
    assert(dim <= MAX_DIMENSION);
    readLine(line, inFile); // Second Line
    sscanf(line, "%d", &numOfPts); // Read the number of points (m)

    Vector v, w;
    // Training set:
    train(&w, &v, inFile, dim, numOfPts);
    // Predictions:
    while (!feof(inFile))
    {
        readLine(line, inFile);
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