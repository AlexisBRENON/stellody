/**
  * @brief Fichier d'implémentation du module \em AnalyzedTrack.
  *
  * @author Alexis BRENON in STELLODY TEAM
  * @file analyzed_track.c
  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "analyzed_track.h"


/* ********************************************************************* */
/*                                                                       */
/*         Fonctions relatives à la structure AnalyzedTrack              */
/*                                                                       */
/* ********************************************************************* */


int analyzedTrackCompare(const int* iTID1,
						const int* iTID2)
{
	assert (iTID1 != NULL &&
			iTID2 != NULL);

	return analyzedTrackDataCompare(iTID1, iTID2, NULL);
}
int analyzedTrackDataCompare(const int* iTID1,
						const int* iTID2,
						void* pData)
{
	assert (iTID1 != NULL);
	assert (iTID2 != NULL);

	if (pData == NULL)
	{
		if (*iTID1 == *iTID2)
		{
			return 0;
		}
		else if (*iTID1 <= *iTID2)
		{
			return -1;
		}
		else if (*iTID1 > *iTID2)
		{
			return 1;
		}
	} else {
		printf("pData != 0 !!!!!!!\n");
		printf("*pData = %d", *((int* )pData));
	}

	return EXIT_FAILURE;
}


int analyzedTrackInitWithData(AnalyzedTrack* psTrack, int iTID,
							const char* strPath, float fAverage,
							float fMedian)
{
	assert (psTrack != NULL);
	assert (iTID >= 0);

	/* Si on lui passe une chaine de caractère (même vide), on la copie.*/
	if (strPath != NULL)
	{
		psTrack->strPath = (char*) malloc((strlen(strPath)+1)*sizeof(char));
		strcpy(psTrack->strPath, strPath);
	}
	else
	{
		psTrack->strPath = NULL;
	}
	psTrack->iTID = iTID;
	psTrack->bAnalyzed = 1;
	psTrack->fFrequenciesAverage = fAverage;
	psTrack->fFrequenciesMedian = fMedian;

	return EXIT_SUCCESS;
}
int analyzedTrackInit(AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	analyzedTrackInitWithData(psTrack, 0, NULL, 0.0, 0.0);
	analyzedTrackSetAnalyzed(psTrack, 0);

	return EXIT_SUCCESS;
}
int analyzedTrackRelease(AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	psTrack->iTID = 0;
	psTrack->bAnalyzed = 0;
	psTrack->fFrequenciesAverage = 0;
	psTrack->fFrequenciesMedian = 0;
	if (psTrack->strPath != NULL)
	{
		free(psTrack->strPath);
		psTrack->strPath = NULL;
	}

	return EXIT_SUCCESS;
}
gboolean analyzedTrackReleaseFromTree(gpointer pKey, gpointer pValue,
									gpointer pData)
{
	assert (pValue != NULL);

	analyzedTrackRelease(pValue);

	return FALSE;
}


AnalyzedTrack* analyzedTrackCreateWithData(int iTID, const char* strPath,
								float fAverage,	float fMedian)
{
	AnalyzedTrack* psTrack = NULL;

	psTrack = (AnalyzedTrack*) malloc (sizeof(AnalyzedTrack));
	analyzedTrackInitWithData(psTrack, iTID, strPath, fAverage, fMedian);

	return psTrack;
}
AnalyzedTrack* analyzedTrackCreate (void)
{
	AnalyzedTrack* psTrack = NULL;
	psTrack = analyzedTrackCreateWithData(0, NULL, 0.0, 0.0);
	analyzedTrackSetAnalyzed(psTrack, 0);

	return psTrack;
}
int analyzedTrackDestroy(AnalyzedTrack** ppsTrack)
{
	assert (ppsTrack != NULL);
	assert (*ppsTrack != NULL);

	analyzedTrackRelease(*ppsTrack);
	free(*ppsTrack);
	*ppsTrack = NULL;

	return EXIT_SUCCESS;
}

int analyzedTrackGetTID (const AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	return psTrack->iTID;
}
int analyzedTrackSetTID (AnalyzedTrack* psTrack, int iValue)
{
	assert (psTrack != NULL);
	assert (iValue >= 0);

	psTrack->iTID = iValue;

	return EXIT_SUCCESS;
}

char analyzedTrackGetAnalyzed (const AnalyzedTrack *psTrack)
{
	assert(psTrack != NULL);

	return psTrack->bAnalyzed;
}

int analyzedTrackSetAnalyzed (AnalyzedTrack* psTrack, char bValue)
{
	assert(psTrack != NULL);
	assert(bValue == 0 || bValue == 1);

	psTrack->bAnalyzed = bValue;

	return EXIT_SUCCESS;
}


const char* analyzedTrackGetPath (const AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	return psTrack->strPath;
}
int analyzedTrackSetPath (AnalyzedTrack* psTrack, const char* strNewPath)
{
	assert (psTrack != NULL);
	assert (strNewPath != NULL);

	if (psTrack->strPath != NULL)
	{
		free(psTrack->strPath);
	}
	psTrack->strPath = malloc((strlen(strNewPath)+1)*sizeof(char));
	strcpy(psTrack->strPath, strNewPath);

	return EXIT_SUCCESS;
}

float analyzedTrackGetFrequenciesAverage (const AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	return psTrack->fFrequenciesAverage;
}
int analyzedTrackSetFrequenciesAverage (AnalyzedTrack* psTrack,
										float fValue)
{
	assert (psTrack != NULL);

	if (fValue < 0)
	{
		fValue = fValue * (-1);
	}

	psTrack->fFrequenciesAverage = fValue;

	return EXIT_SUCCESS;
}

float analyzedTrackGetFrequenciesMedian (const AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	return psTrack->fFrequenciesMedian;
}
int analyzedTrackSetFrequenciesMedian (AnalyzedTrack *psTrack, float fValue)
{
	assert (psTrack != NULL);

	if (fValue < 0)
	{
		fValue = fValue * (-1);
	}

	psTrack->fFrequenciesMedian = fValue;

	return EXIT_SUCCESS;
}


/* ********************************************************************* */
/*                                                                       */
/*                          Tests de regressions                         */
/*                                                                       */
/* ********************************************************************* */


int analyzedTrackRegressionTest(void)
{
	AnalyzedTrack *psTrack = NULL;
	AnalyzedTrack sTrack2;

	printf("\n\t -- MODULE ANALYZED_TRACK --\n\n");

	printf("Création d'un morceau...\n");
	psTrack = analyzedTrackCreateWithData(100, NULL, 0, 0);
	assert (psTrack != NULL &&
			psTrack->iTID == 100 &&
			psTrack->strPath == NULL &&
			psTrack->fFrequenciesAverage == 0 &&
			psTrack->fFrequenciesMedian == 0);
	printf("\tFAIT !!\n");

	printf("Initialisation d'un morceau...\n");
	analyzedTrackInitWithData(&sTrack2, 50, "test", 0.5, 0.5);
	assert (sTrack2.iTID == 50 &&
			sTrack2.fFrequenciesAverage == 0.5 &&
			sTrack2.fFrequenciesMedian == 0.5 &&
			strcmp(sTrack2.strPath, "test")==0);
	printf("\tFAIT !!\n");

	printf("Comparaison de morceaux...\n");
	assert (analyzedTrackCompare(&(psTrack->iTID), &(sTrack2.iTID)) == 1);
	printf("\tFAIT !!\n");

	printf("Changement du chemin...\n");
	analyzedTrackSetPath(psTrack, "/test/path");
	assert (strcmp(psTrack->strPath, "/test/path") == 0);
	printf("\tFAIT !!\n");

	printf("Rechangement du chemin...\n");
	analyzedTrackSetPath(psTrack, "/test2/filepath");
	assert (strcmp(psTrack->strPath, "/test2/filepath") == 0);
	printf("\tFAIT !!\n");

	printf("Changement des valeurs...\n");
	analyzedTrackSetFrequenciesAverage(psTrack, 0.5);
	analyzedTrackSetFrequenciesMedian(psTrack, 0.25);
	analyzedTrackSetTID(psTrack, 555);
	assert (psTrack->fFrequenciesAverage == 0.5 &&
			psTrack->fFrequenciesMedian == 0.25 &&
			psTrack->iTID == 555);
	printf("\tFAIT !!\n");

	printf("Récupération des données...\n");
	printf("TID (555) :              %d\n", analyzedTrackGetTID(psTrack));
	printf("PATH (/test2/filepath) : %s\n", analyzedTrackGetPath(psTrack));
	printf("Average (0.5) :          %f\n",
			analyzedTrackGetFrequenciesAverage(psTrack));
	printf("Median (0.25) :          %f\n",
			analyzedTrackGetFrequenciesMedian(psTrack));
	printf("\tFAIT !!\n");

	printf("Libération d'un morceau...\n");
	analyzedTrackRelease(&sTrack2);
	assert (sTrack2.strPath == NULL);
	printf("\tFAIT !!\n");

	printf("Destruction du morceau...\n");
	analyzedTrackDestroy(&psTrack);
	assert (psTrack == NULL);
	printf("\tFAIT !!\n");

	printf("\n\t -- Fin du test de régression --\n");

	return EXIT_SUCCESS;
}

