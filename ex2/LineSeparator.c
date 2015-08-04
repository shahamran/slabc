#include "LineSeparator.h"
#define SEPARATOR ','
#define READ_LINE(l,f) fgets(l, MAX_LINE_LENGTH, f)

/**
 * Reads a line and extract a vector's initialization from it,
 * which includes the data of the vector and perhaps a tag value.
 * @param p_v A pointer to a vector to be initialized.
 * @param line The string of a line to be parsed.
 * @param dim The dimension of the vector.
 * @return The pointer of the vector that was initialized.
 */
static Vector * parseLine(Vector* p_v, const char* line, int dim)
{
    if (*line == EOL) // Make sure this is not an empty line.
    {
        return p_v;
    }
    int i; // An index for the currNum string
    char currNum[MAX_LINE_LENGTH];
    for (int j = 0; j < dim; j++)
    {
        i = 0;
        // Read until a separator or the end of line is reached.
        while (*line != SEPARATOR && *line != EOL)
        {
            currNum[i] = *line;
            i++;
            line++;
        }
        // 
        if (*line == SEPARATOR)
        {
            line++;
        }
        currNum[i] = STR_END;
        sscanf(currNum, "%lf", &(p_v->_data[j])); // write the number that was read to the vector
    }
    // If the line end wasn't reached, there is a tag value to be read
    if (*line != EOL)
    {
        sscanf(line, "%d", &(p_v->_tag));
    }
    return p_v;
}

/**
 * Gets a classification vector and classifies the vector specified by the given line, i.e
 * checks on what side of the hyper-plane determined by p_w this vector is.
 * @param p_w A pointer to the classifier vector.
 * @param line The string of the line to be parsed.
 * @param dim The dimension of the vector.
 * @return The Tag value that is given by <w,v> >? 0
 * Note that in our case: |<w,v>| < EPSILON <=> <w,v> = 0
 */
static Tag predict(const Vector* p_w, const char* line, int dim)
{
    Vector v;
    parseLine(&v, line, dim);
    // <w,v> < EPSILON  ==> v.tag = -1
    // <w,v> >= EPSILON ==> v.tag = 1
    return dotProduct(p_w, &v, dim) < EPSILON ? MINUS : PLUS;
}

/**
 * Gets a classification vector and trains it according to a given number of points.
 * @param p_w A pointer to the classification vector.
 * @param inFile The file that specifies the training set.
 * @param dim The dimension of the vector.
 * @param numOfPts The number of training points (m).
 * @return A pointer to the same training vector (p_w).
 */
Vector * trainVector(Vector* p_w, FILE* inFile, int dim, int numOfPts)
{
    Vector v;
    char line[MAX_LINE_LENGTH + 1]; // +1 for '\0'
    Tag tempTag; // The predicted Tag value.
    for (int i = 0; i < numOfPts; i++)
    {
        READ_LINE(line, inFile);
        parseLine(&v, line, dim);
        tempTag = dotProduct(p_w, &v, dim) < EPSILON ? MINUS : PLUS;
        if (tempTag != v._tag) // Checks if an update to the vector is needed.
        {
            addVector(p_w, &v, dim);
        }
    }
    return p_w;
}

/**
 * Gets a classifier and prints its predictions to the the vectors specified in the rest of
 * the file.
 * @param p_w A pointer to the classifier vector.
 * @param inFile The file to read.
 * @param dim The dimension of the vector.
 */
void getPredictions(Vector* p_w, FILE* inFile, int dim)
{
    char line[MAX_LINE_LENGTH + 1];
    while (!feof(inFile))
    {
        if (READ_LINE(line, inFile) != NULL)
        {
            printf("%d\n", predict(p_w, line, dim));
        }
    }
    return;
}

/**
 * Reads the file that was given to train a classifier vector and make predictions.
 * @param inFile The given file.
 */
void parseFile(FILE* inFile)
{
    char line[MAX_LINE_LENGTH + 1]; // +1 for '\0'
    int dim, numOfPts; // Dimension and number of training points
    READ_LINE(line, inFile);
    sscanf(line, "%d", &dim); // Read the dimension (n) - first line
    assert(dim <= MAX_DIMENSION && dim > 1);
    READ_LINE(line, inFile);
    sscanf(line, "%d", &numOfPts); // Read the number of points (m) - second line

    Vector w; // Our classifier (the vector perp. to the linear classifier)
    initVector(&w, dim); // initially set to be the 0 vector.
    // Training set:
    trainVector(&w, inFile, dim, numOfPts);
    // Predictions:
    getPredictions(&w, inFile, dim);
    return;
}

/**
 * The main function.
 */
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <file_name>\n", argv[0]);
        return 0;
    }

    FILE *inFile = fopen(argv[1], "r");
    if (inFile == NULL)
    {
        printf("Unable to open input file: %s\n", argv[1]);
        return 0;
    }

    parseFile(inFile);
    fclose(inFile);
    return 0;
}