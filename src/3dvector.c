#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "3dvector.h"

Vector* vectorCreate (double dX, double dY, double dZ)
{
	Vector* pVect = NULL;

	pVect = (Vector*) malloc(sizeof(Vector));
	vectorInit(pVect, dX, dY, dZ);

	return pVect;
}
void vectorDestroy (Vector** ppVector)
{
	assert (ppVector != NULL);
	assert (*ppVector != NULL);

	free(*ppVector);
	*ppVector = NULL;
}

void vectorInit (Vector* const pVector,
				double dX, double dY, double dZ)
{
	assert (pVector != NULL);

	pVector->x = dX;
	pVector->y = dY;
	pVector->z = dZ;
}
void vectorRelease (Vector* const pVector)
{
	assert (pVector != NULL);

	pVector->x = 0;
	pVector->y = 0;
	pVector->z = 0;
}

Vector vectorCopy (Vector const* const pVector)
{
	Vector vect;

	assert (pVector != NULL);

	vectorInit(&vect,
				pVector->x, pVector->y, pVector->z);
	return vect;
}
Vector vectorFromTo (Vector const* const pFrom,
					Vector const* const pTo)
{
	Vector vect;

	assert (pFrom != NULL);
	assert (pTo != NULL);

	vect.x = pTo->x - pFrom->x;
	vect.y = pTo->y - pFrom->y;
	vect.z = pTo->z - pFrom->z;

    return vect;
}


Vector vectorSum (Vector const* const pVector1,
				Vector const* const pVector2)
{
	Vector Sum;

	Sum.x = pVector1->x + pVector2->x;
	Sum.y = pVector1->y + pVector2->y;
	Sum.z = pVector1->z + pVector2->z;

	return Sum;
}
Vector vectorDiff (Vector const* const pVector1,
					Vector const* const pVector2)
{
	Vector Diff;

	Diff.x = pVector1->x - pVector2->x;
	Diff.y = pVector1->y - pVector2->y;
	Diff.z = pVector1->z - pVector2->z;

	return Diff;
}

Vector vectorProduct (Vector const* const pVector1, double dCoef)
{
	Vector Product;

	Product.x = pVector1->x * dCoef;
	Product.y = pVector1->y * dCoef;
	Product.z = pVector1->z * dCoef;

	return Product;
}
Vector vectorDividide (Vector const* const pVector1, double dCoef)
{
	Vector Div;

	Div.x = pVector1->x / dCoef;
	Div.y = pVector1->y / dCoef;
	Div.z = pVector1->z / dCoef;

	return Div;
}

Vector vectorCrossProduct (Vector const* const pVector1,
						Vector const* const pVector2)
{
	Vector Cross;

	Cross.x = pVector1->y*pVector2->z - pVector1->z*pVector2->y;
	Cross.y = pVector1->z*pVector2->x - pVector1->x*pVector2->z;
	Cross.z = pVector1->x*pVector2->y - pVector1->y*pVector2->x;

	return Cross;
}

void vectorNormalize (Vector* const pVector)
{
	assert (pVector != NULL);

	*pVector = vectorDividide(pVector,
							vectorGetLength(pVector));
}

double vectorGetX (Vector const* const pVector)
{
	assert (pVector != NULL);

	return pVector->x;
}
double vectorGetY (Vector const* const pVector)
{
	assert (pVector != NULL);

	return pVector->y;
}
double vectorGetZ (Vector const* const pVector)
{
	assert (pVector != NULL);

	return pVector->z;
}

double vectorGetLength (Vector const* const pVector)
{
	assert (pVector != NULL);

	return sqrt(pVector->x * pVector->x +
				pVector->y * pVector->y +
				pVector->z * pVector->z);
}

