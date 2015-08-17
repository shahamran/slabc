#ifndef LINE_SEPARATOR_H

// Includes:
#include <stdio.h>
#include <assert.h>
#include <string.h>

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


// Enums:
/**
 * A tag enum that represents the classification of a vector.
 */
typedef enum 
{
	PLUS = 1,
	MINUS = -1
} Tag;

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

// Functions:

/**
 * Reads the file that was given and train a classifier vector to make predictions.
 * @param inFile The given file.
 */
void parseFile(FILE* inFile);

/**
 * Gets a classification vector and trains it according to a given number of points.
 * Note that this function trains a vector from scratch - meaning any data on it will be lost.
 * @param p_w A pointer to the classification vector.
 * @param inFile The file that specifies the training set.
 * @param dim The dimension of the vector.
 * @param numOfPts The number of training points (m).
 * @return A pointer to the same training vector (p_w).
 */
Vector * trainVector(Vector* p_w, FILE* inFile, int dim, int numOfPts);

/**
 * Gets a classifier and prints its predictions to the the vectors specified in the rest of
 * the file.
 * @param p_w A pointer to the classifier vector.
 * @param inFile The file to read.
 * @param dim The dimension of the vector.
 */
void getPredictions(Vector* p_w, FILE* inFile, int dim);

// Implementations:

/**
 * Sets all coordinates of a vector to be START_VALUE (0).
 * @param p_v The vector to be initialized.
 * @param dim The dimension of the vector.
 * @return The pointer to the vector that was initialized.
 */
Vector* initVector(Vector* p_v, int dim)
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
Vector* addVector(Vector* p_v, const Vector* p_w, int dim)
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
double dotProduct(const Vector* p_v, const Vector* p_w, int dim)
{
	int i;
	double total = 0.0;
	for (i = 0; i < dim; i++)
	{
		total += p_v->_data[i] * p_w->_data[i];
	}
	return total;
}

/**
 * Gets the tag of a target vector determined by the dot product of it
 * and a given classifier vector.
 * @param p_w A pointer to the classifier vector.
 * @param p_v A pointer to the vector to be tagged.
 * @param dim The dimension of the vectors.
 * @return The tag of the target vector as predicted by the classifier.
 */
Tag getTag(const Vector* p_w, const Vector* p_v, int dim)
{
	return dotProduct(p_w, p_v, dim) < EPSILON ? MINUS : PLUS;
}

#endif //LINE_SEPARATOR_H