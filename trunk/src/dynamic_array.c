/**
  * @brief Fichier d'implementation du module @em DYNAMIC_ARRAY.
  * Implémente les tableaux dynamiques.
  *
  * @author Alexis BRENON in STELLODY TEAM
  * @file dynamic_array.c
  */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "dynamic_array.h"


/* ********************************************************************* */
/*                                                                       */
/*                 Fonctions relatives à la structure                    */
/*                                                                       */
/* ********************************************************************* */


DynamicArray* dynamicArrayCreate (void)
{
	DynamicArray* pArray = NULL;

	pArray = (DynamicArray*) malloc(sizeof(DynamicArray));

	dynamicArrayInit(pArray);

	return pArray;
}

ArrayError dynamicArrayDestroy (DynamicArray** ppArray)
{
	if (ppArray == NULL || *ppArray == NULL) {return ARRAY_NULL_POINTER;}

	dynamicArrayRelease(*ppArray);
	free(*ppArray); *ppArray = NULL;

	return ARRAY_OK;
}

ArrayError dynamicArrayInit (DynamicArray* pArray)
{
	if (pArray == NULL) {return ARRAY_NULL_POINTER;}

	pArray->ppFisrtCell = (void**) malloc(10*sizeof(void*));
	pArray->iAllocatedSize = 10;
	pArray->iFirstEmptyCell = 0;

	return ARRAY_OK;
}

ArrayError dynamicArrayRelease (DynamicArray* pArray)
{
	if (pArray == NULL) {return ARRAY_NULL_POINTER;}

	free(pArray->ppFisrtCell); pArray->ppFisrtCell = NULL;
	pArray->iAllocatedSize = 0;
	pArray->iFirstEmptyCell = -1;

	return ARRAY_OK;
}

ArrayError dynamicArraySet (DynamicArray* pArray,
							int iIndex,
							void* pData)
{
	if (pArray == NULL) {return ARRAY_NULL_POINTER;}

	if (iIndex >= pArray->iFirstEmptyCell) {return ARRAY_INDEX_OVERFLOW;}

	(pArray->ppFisrtCell)[iIndex] = pData;

	return ARRAY_OK;
}

ArrayError dynamicArrayGet (const DynamicArray* pArray,
							int iIndex,
							void** pData)
{
	if (pArray == NULL) {return ARRAY_NULL_POINTER;}

	if (iIndex >= pArray->iFirstEmptyCell) {return ARRAY_INDEX_OVERFLOW;}

	if (pData == NULL) {return ARRAY_BAD_ARGUMENTS;}

	*pData = (pArray->ppFisrtCell)[iIndex];

	return ARRAY_OK;
}


ArrayError dynamicArrayPush (DynamicArray* pArray,
				void* pData)
{
	if (pArray == NULL) {return ARRAY_NULL_POINTER;}

	if (pArray->iFirstEmptyCell >= pArray->iAllocatedSize)
	{
		void** ppNewFirstCell = NULL;
		int iNewAllocatedSize = (pArray->iAllocatedSize)*2;
		int i = 0;

		ppNewFirstCell = (void**) malloc(iNewAllocatedSize*sizeof(void*));

		for (i = 0; i < pArray->iFirstEmptyCell; i++)
		{
			ppNewFirstCell[i] = (pArray->ppFisrtCell)[i];
		}
		free(pArray->ppFisrtCell); pArray->ppFisrtCell = NULL;

		pArray->ppFisrtCell = ppNewFirstCell;
		pArray->iAllocatedSize = iNewAllocatedSize;
	}

	pArray->ppFisrtCell[pArray->iFirstEmptyCell] = pData;
	pArray->iFirstEmptyCell = pArray->iFirstEmptyCell+1;

	return ARRAY_OK;
}

ArrayError dynamicArrayPop (DynamicArray* pArray)
{
	int iReductionSize = 0;

	if (pArray == NULL) {return ARRAY_NULL_POINTER;}

	pArray->ppFisrtCell[pArray->iFirstEmptyCell-1] = NULL;
	pArray->iFirstEmptyCell = pArray->iFirstEmptyCell-1;

	iReductionSize = pArray->iAllocatedSize;
	iReductionSize = iReductionSize*2;
	iReductionSize = iReductionSize/3;

	if (pArray->iFirstEmptyCell <= iReductionSize)
	{
		void** ppNewFirstCell = NULL;
		int iNewAllocatedSize = iReductionSize;
		int i = 0;

		ppNewFirstCell = (void**) malloc(iNewAllocatedSize*sizeof(void*));

		for (i = 0; i < pArray->iFirstEmptyCell; i++)
		{
			ppNewFirstCell[i] = (pArray->ppFisrtCell)[i];
		}
		free(pArray->ppFisrtCell); pArray->ppFisrtCell = NULL;

		pArray->ppFisrtCell = ppNewFirstCell;
		pArray->iAllocatedSize = iNewAllocatedSize;
	}

	return ARRAY_OK;
}

ArrayError dynamicArrayGetSize (const DynamicArray* pArray,
								int* piSize)
{
	if (pArray == NULL) {return ARRAY_NULL_POINTER;}
	if (piSize == NULL) {return ARRAY_BAD_ARGUMENTS;}

	*piSize = pArray->iFirstEmptyCell;

	return ARRAY_OK;
}

ArrayError dynamicArrayGetMaxSize (const DynamicArray* pArray,
								int* piMaxSize)
{
	if (pArray == NULL) {return ARRAY_NULL_POINTER;}
	if (piMaxSize == NULL) {return ARRAY_BAD_ARGUMENTS;}

	*piMaxSize = pArray->iAllocatedSize;

	return ARRAY_OK;
}


/* ********************************************************************* */
/*                                                                       */
/*                          Test de regression                           */
/*                                                                       */
/* ********************************************************************* */


int dynamicArrayRegressionTest ()
{
	DynamicArray* pArray = NULL;
	int iTest = 0;
	int i = 0;
	int* piData = NULL;

	printf("\n\t\t == TEST DYNAMIC_ARRAY == \n\n");

	printf("Création...\n");
	pArray = dynamicArrayCreate();
	assert(pArray != NULL);

	printf("Interrogation taille max...\n");
	assert (dynamicArrayGetMaxSize(pArray, &iTest) == ARRAY_OK);
	assert (iTest == 10);

	printf("Interrogation taille actuelle...\n");
	assert (dynamicArrayGetSize(pArray, &iTest) == ARRAY_OK);
	assert (iTest == 0);

	printf("Insertion...\n");
	for (i = 0; i < 15; i++)
	{
		assert (dynamicArrayPush(pArray, &iTest) == ARRAY_OK);
	}

	printf("Interrogation taille max...\n");
	assert (dynamicArrayGetMaxSize(pArray, &iTest) == ARRAY_OK);
	assert (iTest == 20);

	printf("Interrogation taille actuelle...\n");
	assert (dynamicArrayGetSize(pArray, &iTest) == ARRAY_OK);
	assert (iTest == 15);

	printf("Obtention...\n");
	assert (dynamicArrayGet(pArray, 13, (void**) &piData) == ARRAY_OK);
	assert(piData == &iTest);
	assert(*piData == iTest);

	printf("Diminution...\n");
	for (i = 15; i > 10; i--)
	{
		assert(dynamicArrayPop(pArray) == ARRAY_OK);
	}
	printf("Interrogation taille actuelle...\n");
	assert(dynamicArrayGetSize(pArray, &iTest) == ARRAY_OK);
	assert(iTest == 10);
	printf("Interrogation taille max...\n");
	assert(dynamicArrayGetMaxSize(pArray, &iTest) == ARRAY_OK);
	assert(iTest == 13);

	printf("Destruction...\n");
	assert(dynamicArrayDestroy(&pArray) == ARRAY_OK);


	printf("\n\t == FIN == \n");
	return EXIT_SUCCESS;
}

