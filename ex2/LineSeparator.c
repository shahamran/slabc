#include "LineSeparator.h"
#define SEPARATOR ","
// Macro for a function for reading a line.
#define READ_LINE(l, f) fgets(l, MAX_LINE_LENGTH, f)
// The 'read' token for the fopen function
#define READ_ONLY "r"

// Enums for the command line arguments that are given to this program.
enum 
{
    PROG_NAME = 0,
    FILE_NAME = 1
} CommandLineArgument;

/**
 * Reads a line and extract a vector's initialization from it,
 * which includes the data of the vector and perhaps a tag value.
 * @param p_v A pointer to a vector to be initialized.
 * @param line The string of a line to be parsed.
 * @param dim The dimension of the vector.
 * @return The pointer of the vector that was initialized.
 */
static Vector * parseLine(Vector* p_v, char* line, int dim)
{
    assert (line != NULL && *line != EOL);
    char *currNum;
    currNum = strtok(line, SEPARATOR);
    for (int j = 0; j < dim; j++)
    {
        sscanf(currNum, "%lf", &(p_v->_data[j]));
        currNum = strtok(NULL, SEPARATOR);
    }
    if (currNum != NULL) // This means a tag value exists
    {
        sscanf(currNum, "%d", &(p_v->_tag));
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
 * Note that in our case: |<w,v>| < EPSILON <=> <w,v> = 0 => <w,v> < EPSILON ==> Tag = -1
 */
static Tag predict(const Vector* p_w, char* line, int dim)
{
    Vector v;
    parseLine(&v, line, dim);
    return getTag(p_w, &v, dim);
}

/**
 * Gets a classification vector and trains it according to a given number of points.
 * @param p_w A pointer to the classification vector to be trained.
 * @param inFile The file that specifies the training set.
 * @param dim The dimension of the vector.
 * @param numOfPts The number of training points (m).
 * @return A pointer to the same training vector (p_w).
 */
Vector * trainVector(Vector* p_w, FILE* inFile, int dim, int numOfPts)
{
    assert (p_w != NULL && inFile != NULL && numOfPts >= 0);
    initVector(p_w, dim); // Classifier is initially set to be the 0 vector.
    Vector v;
    char line[MAX_LINE_LENGTH + 1]; // +1 for '\0'
    Tag tempTag; // The predicted Tag value.
    for (int i = 0; i < numOfPts; i++)
    {
        READ_LINE(line, inFile);
        parseLine(&v, line, dim);
        tempTag = getTag(p_w, &v, dim);
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
    assert(inFile != NULL && dim <= MAX_DIMENSION && dim > 1);
    char line[MAX_LINE_LENGTH + 1]; // Again, +1 for '\0'
    while (!feof(inFile))
    {
        if (READ_LINE(line, inFile) != NULL)
        {// Prints predictions
            printf("%d\n", predict(p_w, line, dim));
        }
    }
    return;
}

/**
 * Reads the file that was given and train a classifier vector to make predictions.
 * @param inFile The given file.
 */
void parseFile(FILE* inFile)
{
    assert (inFile != NULL);
    char line[MAX_LINE_LENGTH + 1]; // +1 for '\0'
    int dim, numOfPts; // Dimension and number of training points
    READ_LINE(line, inFile);
    sscanf(line, "%d", &dim); // Read the dimension (n) - first line
    assert(dim <= MAX_DIMENSION && dim > 1);
    READ_LINE(line, inFile);
    sscanf(line, "%d", &numOfPts); // Read the number of points (m) - second line

    Vector w; // Our classifier (the vector perp. to the linear classifier)
    trainVector(&w, inFile, dim, numOfPts);
    getPredictions(&w, inFile, dim);
    return;
}

/**
 * The main function.
 * Checks the validity of the command line argument that was given and 
 * calls the parse function.
 */
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <file_name>\n", argv[PROG_NAME]);
        return 0;
    }

    FILE *inFile = fopen(argv[FILE_NAME], READ_ONLY); // Open
    if (inFile == NULL)
    {
        printf("Unable to open input file: %s\n", argv[FILE_NAME]);
        return 0;
    }

    parseFile(inFile);
    fclose(inFile); // Close
    return 0;
}