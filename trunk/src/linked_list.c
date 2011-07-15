/**
  * @brief Fichier d'implementation du module @em LINKED_LIST.
  * Implémente les liste doublement chainées.
  *
  * @author Alexis BRENON in STELLODY TEAM
  * @file linked_list.c
  */


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "linked_list.h"


/* ********************************************************************* */
/*                                                                       */
/*                 Fonctions relatives à la structure                    */
/*                                                                       */
/* ********************************************************************* */


LinkedList* linkedListCreate()
{
	LinkedList* pList = NULL;

	pList = (LinkedList*) malloc(sizeof(LinkedList));
	linkedListInit(pList);

	return pList;
}

ListError linkedListDestroy (LinkedList** ppList)
{
	if (ppList == NULL || *ppList == NULL) {return LIST_NULL_ADRESS;}

	linkedListRelease(*ppList);
	free(*ppList); *ppList = NULL;

	return LIST_OK;
}

ListError linkedListInit (LinkedList* pList)
{
	if (pList == NULL) {return LIST_NULL_ADRESS;}

	pList->pData = pList;
	pList->pNext = NULL;
	pList->pPrevious = NULL;

	return LIST_OK;
}

ListError linkedListRelease (LinkedList* pList)
{
	if (pList == NULL) {return LIST_NULL_ADRESS;}

	while (pList->pPrevious != NULL && pList->pNext != NULL)
	{
		linkedListRemoveFirst(pList, NULL);
	}

	pList->pData = NULL;
	pList->pNext = NULL;
	pList->pPrevious = NULL;

	return LIST_OK;
}


ListError linkedListPrepend (LinkedList* pList,
							void* pData)
{
	LinkedList* pCell = NULL;
	LinkedList* pFirst = NULL;

	int iLength = 0;

	if (pList == NULL) {return LIST_NULL_ADRESS;}

	pCell = (LinkedList*) malloc(sizeof(LinkedList));
	pFirst = pList->pNext;

	pCell->pData = pData;
	pCell->pPrevious = NULL;
	pCell->pNext = pFirst;

	pFirst->pPrevious = pCell;
	pList->pNext = pCell;

	linkedListGetLength(pList, &iLength);
	if (iLength == 0)
	{
		pList->pPrevious = pCell;
	}

	return LIST_OK;
}

ListError linkedListAppend (LinkedList* pList,
							void* pData)
{
	LinkedList* pCell = NULL;
	int iLength = 0;

	if (pList == NULL) {return LIST_NULL_ADRESS;}

	pCell = (LinkedList*) malloc(sizeof(LinkedList));
	linkedListGetLength(pList, &iLength);
	if (iLength == 0)
	{
		pList->pNext = pCell;
	}

	pCell->pData = pData;
	pCell->pNext = NULL;
	pCell->pPrevious = pList->pPrevious;

	pList->pPrevious = pCell;

	if (iLength > 0)
	{
		pCell->pPrevious->pNext = pCell;
	}


	return LIST_OK;
}

ListError linkedListInsert(LinkedList* pList,
						int iIndex,
						void* pData)
{
	LinkedList* pCell = NULL;
	LinkedList* pPrevious = NULL;
	LinkedList* pNext = NULL;
	int iLength = 0;

	if (pList == NULL) {return LIST_NULL_ADRESS;}

	linkedListGetLength(pList, &iLength);
	if (iIndex >= iLength) {return LIST_INDEX_OVERFLOW;}

	if (iIndex == 0)
	{
		linkedListPrepend(pList, pData);
	}
	else if (iIndex == iLength)
	{
		linkedListAppend(pList, pData);
	}
	else
	{
		pCell = (LinkedList*) malloc(sizeof(LinkedList));
		linkedListGetIeme(pList, iIndex-1, &pPrevious);
		linkedListGetIeme(pList, iIndex, &pNext);

		pCell->pData = pData;
		pCell->pPrevious = pPrevious;
		pCell->pNext = pNext;

		pPrevious->pNext = pCell;
		pNext->pPrevious = pCell;
	}

	return LIST_OK;
}

ListError linkedListRemoveFirst(LinkedList* pList,
								void** pData)
{
	LinkedList* pFirst = NULL;
	int iLength = 0;

	if (pList == NULL) {return LIST_NULL_ADRESS;}

	linkedListGetLength(pList, &iLength);
	if (iLength == 1)
	{
		pList->pPrevious = NULL;
	}

	linkedListGetFirst(pList, &pFirst);

	if (pData != NULL) {*pData = pFirst->pData;}
	pList->pNext = pFirst->pNext;
	if (iLength > 1)
	{
		pFirst->pNext->pPrevious = NULL;
	}

	free(pFirst); pFirst = NULL;

	return LIST_OK;
}

ListError linkedListRemoveLast (LinkedList* pList,
								void** pData)
{
	LinkedList* pLast = NULL;
	int iLength = 0;

	if (pList == NULL) {return LIST_NULL_ADRESS;}

	linkedListGetLength(pList, &iLength);
	if (iLength == 1)
	{
		pList->pNext = NULL;
	}

	linkedListGetLast(pList, &pLast);

	if (pData != NULL) {*pData = pLast->pData;}
	pList->pPrevious = pLast->pPrevious;
	if (iLength > 1)
	{
		pLast->pPrevious->pNext = NULL;
	}

	free(pLast); pLast = NULL;

	return LIST_OK;
}

ListError linkedListRemoveIeme (LinkedList* pList,
								int iIndex,
								void** pData)
{
	LinkedList* pCell = NULL;
	LinkedList* pPrevious = NULL;
	LinkedList* pNext = NULL;

	int iLength = 0;

	if (pList == NULL) {return LIST_NULL_ADRESS;}

	linkedListGetLength(pList, &iLength);
	if (iIndex >= iLength) {return LIST_INDEX_OVERFLOW;}

	if (iIndex == 0)
	{
		linkedListRemoveFirst(pList, pData);
	}
	else if (iIndex == (iLength-1))
	{
		linkedListRemoveLast(pList, pData);
	}
	else
	{
		linkedListGetIeme(pList, iIndex, &pCell);
		linkedListGetIeme(pList, iIndex-1, &pPrevious);
		linkedListGetIeme(pList, iIndex+1, &pNext);

		if (pData != NULL) {*pData = pCell->pData;}
		pPrevious->pNext = pCell->pNext;
		pNext->pPrevious = pCell->pPrevious;

		free(pCell); pCell = NULL;
	}

	return LIST_OK;
}

ListError linkedListRemoveByData (LinkedList* pList,
								const void* pData)
{
	LinkedList* pCell = NULL;
	int i = 0;

	if (pList == NULL) {return LIST_NULL_ADRESS;}

	pCell = pList->pNext;

	while (pCell != NULL)
	{
		if (pCell->pData == pData)
		{
			pCell = pCell->pNext;
			linkedListRemoveIeme(pList, i, NULL);
		}
		else
		{
			pCell = pCell->pNext;
		}
		i++;
	}

	return LIST_OK;
}


ListError linkedListGetLength (const LinkedList* pList,
							int* iLength)
{
	LinkedList* pCell = NULL;
	int i = 0;

	if (pList == NULL) {return LIST_NULL_ADRESS;}
	if (iLength == NULL) {return LIST_BAD_ARGUMENTS;}

	pCell = pList->pNext;

	while (pCell != NULL)
	{
		i++;
		pCell = pCell->pNext;
	}

	*iLength = i;

	return LIST_OK;
}

ListError linkedListGetFirst (const LinkedList* pList,
							LinkedList** ppFirst)
{
	if (pList == NULL) {return LIST_NULL_ADRESS;}
	if (ppFirst == NULL) {return LIST_BAD_ARGUMENTS;}

	*ppFirst = pList->pNext;

	return LIST_OK;
}

ListError linkedListGetLast (const LinkedList* pList,
							LinkedList** ppLast)
{
	if (pList == NULL) {return LIST_NULL_ADRESS;}
	if (ppLast == NULL) {return LIST_BAD_ARGUMENTS;}

	*ppLast = pList->pPrevious;

	return LIST_OK;
}

ListError linkedListGetIeme (const LinkedList* pList,
							int iIndex,
							LinkedList** ppIeme)
{
	LinkedList* pCell = NULL;
	int iLength = 0;
	int i = 0;

	if (pList == NULL) {return LIST_NULL_ADRESS;}
	if (ppIeme == NULL) {return LIST_BAD_ARGUMENTS;}

	linkedListGetLength(pList, &iLength);
	if (iIndex >= iLength) {return LIST_INDEX_OVERFLOW;}

	pCell = pList->pNext;

	while (i < iIndex)
	{
		i++;
		pCell = pCell->pNext;
	}

	*ppIeme = pCell;

	return LIST_OK;
}


/* ********************************************************************* */
/*                                                                       */
/*                          Test de regression                           */
/*                                                                       */
/* ********************************************************************* */


int linkedListRegressionTest (void)
{
	LinkedList* pList = NULL;
	LinkedList* pCell = NULL;
	int i = 0;
	int piNumbers[10] = {1,2,3,4,5,6,7,8,9,10};
	int iTest = 0;

	printf("\n\t\t == TEST LINKED_LIST == \n\n");

	printf("Création...\n");
	pList = linkedListCreate();
	assert(pList != NULL);

	printf("Interrogation Taille...\n");
	assert(linkedListGetLength(pList, &iTest) == LIST_OK);
	assert(iTest == 0);

	printf("Insertion en fin...\n");
	for (i = 6; i <= 10; i++)
	{
		assert(linkedListAppend(pList, &(piNumbers[i-1])) == LIST_OK);
	}
	printf("Interrogation Taille...\n");
	assert(linkedListGetLength(pList, &iTest) == LIST_OK);
	assert(iTest == 5);

	printf("Insertion en début...\n");
	for (i = 4; i >= 1; i--)
	{
		assert(linkedListPrepend(pList, &(piNumbers[i-1])) == LIST_OK);
	}
	printf("Interrogation Taille...\n");
	assert(linkedListGetLength(pList, &iTest) == LIST_OK);
	assert(iTest == 9);

	printf("Insertion...\n");
	assert(linkedListInsert(pList, 4, &(piNumbers[4])) == LIST_OK);
	printf("Interrogation Taille...\n");
	assert(linkedListGetLength(pList, &iTest) == LIST_OK);
	assert(iTest == 10);

	printf("Interrogation Premier...\n");
	assert(linkedListGetFirst(pList, &pCell) == LIST_OK);
	assert((int*) pCell->pData == &(piNumbers[0]));
	assert(*((int*) pCell->pData) == piNumbers[0]);

	printf("Interrogation Dernier...\n");
	assert(linkedListGetLast(pList, &pCell) == LIST_OK);
	assert((int*) pCell->pData == &(piNumbers[9]));
	assert(*((int*) pCell->pData) == piNumbers[9]);

	printf("Interrogation ième...\n");
	assert(linkedListGetIeme(pList, 5, &pCell) == LIST_OK);
	assert((int*) pCell->pData == &(piNumbers[5]));
	assert(*((int*) pCell->pData) == piNumbers[5]);


	printf("Suppression...\n");
	assert(linkedListRemoveIeme(pList, 5, NULL) == LIST_OK);

	printf("Interrogation ième...\n");
	assert(linkedListGetIeme(pList, 5, &pCell) == LIST_OK);
	assert((int*) pCell->pData == &(piNumbers[6]));
	assert(*((int*) pCell->pData) == piNumbers[6]);


	printf("Suppression Premier...\n");
	assert(linkedListRemoveFirst(pList, NULL) == LIST_OK);

	printf("Interrogation Premier...\n");
	assert(linkedListGetFirst(pList, &pCell) == LIST_OK);
	assert((int*) pCell->pData == &(piNumbers[1]));
	assert(*((int*) pCell->pData) == piNumbers[1]);

	printf("Interrogation ième...\n");
	assert(linkedListGetIeme(pList, 5, &pCell) == LIST_OK);
	assert((int*) pCell->pData == &(piNumbers[7]));
	assert(*((int*) pCell->pData) == piNumbers[7]);


	printf("Suppression Dernier...\n");
	assert(linkedListRemoveLast(pList, NULL) == LIST_OK);

	printf("Interrogation Dernier...\n");
	assert(linkedListGetLast(pList, &pCell) == LIST_OK);
	assert((int*) pCell->pData == &(piNumbers[8]));
	assert(*((int*) pCell->pData) == piNumbers[8]);


	printf("Suppression par donnée...\n");
	assert(linkedListRemoveByData(pList, &(piNumbers[6])) == LIST_OK);

	printf("Interrogation ième...\n");
	assert(linkedListGetIeme(pList, 5, &pCell) == LIST_OK);
	assert((int*) pCell->pData == &(piNumbers[8]));
	assert(*((int*) pCell->pData) == piNumbers[8]);


	printf("Destruction...\n");
	assert(linkedListDestroy(&pList) == LIST_OK);


	printf("\n\t == FIN == \n");
	return EXIT_SUCCESS;
}







































