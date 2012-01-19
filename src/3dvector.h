#ifndef __3DVECTOR_H__
#define __3DVECTOR_H__

struct _Vector
{
	double x;
	double y;
	double z;
};
typedef struct _Vector Vector;

Vector* vectorCreate (double dX, double dY, double dZ);
void vectorDestroy (Vector** ppVector);

void vectorInit (Vector* const pVector, double dX, double dY, double dZ);
void vectorRelease (Vector* const pVector);

Vector vectorCopy (Vector const* const pVector);
Vector vectorFromTo (Vector const* const pFrom, Vector const* const pTo);

Vector vectorSum (Vector const* const pVector1, Vector const* const pVector2);
Vector vectorDiff (Vector const* const pVector1, Vector const* const pVector2);
Vector vectorProduct (Vector const* const pVector1, double dCoef);
Vector vectorDividide (Vector const* const pVector1, double dCoef);
Vector vectorCrossProduct (Vector const* const pVector1, Vector const* const pVector2);
void vectorNormalize (Vector* const pVector);

double vectorGetX (Vector const* const pVector);
double vectorGetY (Vector const* const pVector);
double vectorGetZ (Vector const* const pVector);
double vectorGetLength (Vector const* const pVector);

#endif
