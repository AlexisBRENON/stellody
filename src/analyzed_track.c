/**
  * @brief Implémentation d'un morceau analysé
  *
  * Fichier d'implémentation de la structure d'un morceau analysé.
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


int analyzedTrackInit(AnalyzedTrack* psTrack, const char* strPath,
					float fAverage, float fMedian)
{
	assert (psTrack != NULL);
	assert (fAverage >= 0.0 && fAverage <= 1.0);
	assert (fMedian >= 0.0 && fMedian <= 1.0);

	/* Si on lui passe une chaine de caractère (même vide), on la copie.*/
	if (strPath != NULL)
	{
		psTrack->strPath = (char*) malloc(strlen(strPath)*sizeof(char));
		strcpy(psTrack->strPath, strPath);
	}
	else
	{
		psTrack->strPath = NULL;
	}
	psTrack->fFrequenciesAverage = fAverage;
	psTrack->fFrequenciesMedian = fMedian;

	return EXIT_SUCCESS;
}
int analyzedTrackRelease(AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	psTrack->fFrequenciesAverage = 0;
	psTrack->fFrequenciesMedian = 0;
	if (psTrack->strPath != NULL)
	{
		free(psTrack->strPath);
		psTrack->strPath = NULL;
	}

	return EXIT_SUCCESS;
}

AnalyzedTrack* analyzedTrackCreate(const char* strPath, float fAverage,
								float fMedian)
{
	AnalyzedTrack* psTrack = NULL;

	psTrack = (AnalyzedTrack*) malloc (sizeof(AnalyzedTrack));
	analyzedTrackInit(psTrack, strPath, fAverage, fMedian);

	return psTrack;
}
int analyzedTrackDestroy(AnalyzedTrack** ppsTrack)
{
	assert(ppsTrack != NULL);
	assert(*ppsTrack != NULL);

	analyzedTrackRelease(*ppsTrack);
	free(*ppsTrack);
	*ppsTrack = NULL;

	return EXIT_SUCCESS;
}

const char* analyzedTrackGetPath (const AnalyzedTrack* psTrack)
{
	assert(psTrack != NULL);

	return psTrack->strPath;
}
int analyzedTrackSetPath (AnalyzedTrack* psTrack, const char* strNewPath)
{
	assert(psTrack != NULL);
	assert(strNewPath != NULL);

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
	assert(psTrack != NULL);

	return psTrack->fFrequenciesAverage;
}
int analyzedTrackSetFrequenciesAverage (AnalyzedTrack* psTrack,
										float fValue)
{
	assert(psTrack != NULL);
	assert(fValue >= 0.0 && fValue <= 1.0);

	psTrack->fFrequenciesAverage = fValue;

	return EXIT_SUCCESS;
}

float analyzedTrackGetFrequenciesMedian (const AnalyzedTrack* psTrack)
{
	assert(psTrack != NULL);

	return psTrack->fFrequenciesMedian;
}
int analyzedTrackSetFrequenciesMedian (AnalyzedTrack *psTrack, float fValue)
{
	assert(psTrack != NULL);
	assert(fValue >= 0.0 && fValue <= 1.0);

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

	printf("\n\t -- MODULE ANALYZED_TRACK --\n\n");

	printf("Création d'un morceau...\n");
	psTrack = analyzedTrackCreate(NULL, 0, 0);
	assert(psTrack != NULL &&
			psTrack->strPath == NULL &&
			psTrack->fFrequenciesAverage == 0 &&
			psTrack->fFrequenciesMedian == 0);
	printf("\tFAIT !!\n");

	printf("Changement du chemin...\n");
	analyzedTrackSetPath(psTrack, "/test/path");
	assert(strcmp(psTrack->strPath, "/test/path") == 0);
	printf("\tFAIT !!\n");

	printf("Rechangement du chemin...\n");
	analyzedTrackSetPath(psTrack, "/test2/filepath");
	assert(strcmp(psTrack->strPath, "/test2/filepath") == 0);
	printf("\tFAIT !!\n");

	printf("Changement des valeurs...\n");
	analyzedTrackSetFrequenciesAverage(psTrack, 0.5);
	analyzedTrackSetFrequenciesMedian(psTrack, 0.25);
	assert(psTrack->fFrequenciesAverage == 0.5 &&
			psTrack->fFrequenciesMedian == 0.25);
	printf("\tFAIT !!\n");

	printf("Récupération des données...\n");
	printf("PATH (/test2/filepath) : %s\n", analyzedTrackGetPath(psTrack));
	printf("Average (0.5) :          %f\n",
			analyzedTrackGetFrequenciesAverage(psTrack));
	printf("Median (0.25) :          %f\n",
			analyzedTrackGetFrequenciesMedian(psTrack));
	printf("\tFAIT !!\n");


	printf("Destruction du morceau...\n");
	analyzedTrackDestroy(&psTrack);
	assert(psTrack == NULL);
	printf("\tFAIT !!\n");

	printf("\n\t -- Fin du test de régression --\n");

	return EXIT_SUCCESS;
}

