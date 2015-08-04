#ifndef LINE_SEPARATOR_H

#define LINE_SEPARATOR_H
#define MAX_LINE_LENGTH 150
#define MAX_DIMENSION ((MAX_LINE_LENGTH - 1) / 2)

#include <stdio.h>
#include <assert.h>

typedef enum {PLUS = 1, MINUS = -1} Tag;

typedef struct 
{
    double data[MAX_DIMENSION];
    Tag tag;
} Vector;

Vector *addVector(Vector *p_v, const Vector *p_w, int dim)
{
	if (p_v == NULL || p_w == NULL)
	{
		return p_v;
	}

	int i;
	for (i = 0; i < dim; i++)
	{
		p_v->data[i] += (p_w->tag) * (p_w->data[i]);
	}
	return p_v;
}

double dotProduct(const Vector *p_v, const Vector *p_w, int dim)
{
	int i;
	double total = 0.0;
	for (i = 0; i < dim; i++)
	{
		total += p_v->data[i] * p_w->data[i];
	}
	return total;
}

#endif //LINE_SEPARATOR_H