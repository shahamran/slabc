#ifndef LINE_SEPARATOR_H

// Defines:
#define LINE_SEPARATOR_H
#define MAX_LINE_LENGTH 150
// Since the numbers are separated by commas, the following is the maximal dimension
// of the vectors that are given.
#define MAX_DIMENSION ((MAX_LINE_LENGTH - 1) / 2)
#define EPSILON 0.00001
#define EOL '\n' // Stands for End-of-Line
#define EOS '\0' // Stands for End-of-String
#define START_VALUE 0.0

// Includes:
#include <stdio.h>
#include <assert.h>

// Functions:
void parseFile(FILE *inFile);
Vector * trainVector(Vector *p_w, FILE *inFile, int dim, int numOfPts);
void getPredictions(Vector * p_w, FILE * inFile, int dim);

// Enums:
/**
 * A tag enum that represents the classification of a vector.
 */
typedef enum {PLUS = 1, MINUS = -1} Tag;

// Structs:
/**
 * This struct is a vector that holds a data array (every cell is a coordinate in R^n)
 * and a tag value (see Tag above)
 */
typedef struct Vector
{
    double _data[MAX_DIMENSION];
    Tag _tag;
} Vector;

// Implementations:

/**
 * Sets all coordinates of a vector to be START_VALUE.
 * @param p_v The vector to be initialized.
 * @param dim The dimension of the vector.
 * @return The pointer to the vector that was initialized.
 */
Vector *initVector(Vector *p_v, int dim)
{
	for (int i = 0; i < dim; i++)
	{
		p_v->_data[i] = START_VALUE; // Which is 0.0
	}
	return p_v;
}

/**
 * Adds or subtracts the second vector to the first, according to the second's tag.
 * @param p_v A pointer to the vector to be added to (destination)
 * @param p_w A pointer to the vector that will be added
 * @param dim The dimension of the vectors
 * @return A pointer to the result of the addition/subtraction - p_v
 */
Vector *addVector(Vector *p_v, const Vector *p_w, int dim)
{
	if (p_v == NULL || p_w == NULL)
	{
		return p_v;
	}

	int i;
	for (i = 0; i < dim; i++)
	{
		p_v->_data[i] += (p_w->_tag) * (p_w->_data[i]);
	}
	return p_v;
}

/**
 * Gets two vectors and returns their dot product.
 * @param p_v A pointer to a vector.
 * @param p_w A pointer to another vector.
 * @param dim The dimension of the vectors.
 * @return The dot product of the vectors.
 */
double dotProduct(const Vector *p_v, const Vector *p_w, int dim)
{
	int i;
	double total = 0.0;
	for (i = 0; i < dim; i++)
	{
		total += p_v->_data[i] * p_w->_data[i];
	}
	return total;
}

#endif //LINE_SEPARATOR_H