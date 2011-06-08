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
							const char* strPath, unsigned int uiLength,
							float fAverage, const float fRate[],
							const float fCoord[])
{
	int i;

	assert (psTrack != NULL);
	assert (iTID >= 0);
	assert (uiLength >= 0);
	assert (fAverage >= 0 && fAverage <= 22050);

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

	/* Si l'un des deux tableaux est à NULL, on remplis le champs
	correspondant avec des 0 */

	for (i = 0; i < 3; i++)
	{
		if (fRate == NULL)
		{
			psTrack->pfRate[i] = 0;
		}
		else
		{
			psTrack->pfRate[i] = fRate[i];
		}

		if (fCoord == NULL)
		{
			psTrack->pfCoord[i] = 0;
		}
		else
		{
			psTrack->pfCoord[i] = fCoord[i];
		}
	}

	/* On assigne le reste des valeurs */

	psTrack->iTID = iTID;
	psTrack->bAnalyzed = 1 ;
	psTrack->bAdded = 0 ;
	psTrack->uiLength = uiLength;
	psTrack->fAverage = fAverage;


	return EXIT_SUCCESS;
}
int analyzedTrackInit(AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	analyzedTrackInitWithData(psTrack, 0, NULL, 0, 0.0, NULL, NULL);
	analyzedTrackSetAnalyzed(psTrack, 0);

	return EXIT_SUCCESS;
}
int analyzedTrackRelease(AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	psTrack->iTID = 0;
	psTrack->bAnalyzed = 0;
	psTrack->fAverage = 0;
	psTrack->pfCoord[0] = psTrack->pfCoord[1] = psTrack->pfCoord[2] = 0;
	psTrack->pfRate[0] = psTrack->pfRate[1] = psTrack->pfRate[2] = 0;
	psTrack->bAdded = 0 ;
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


AnalyzedTrack* analyzedTrackCreateWithData(int iTID,
							const char* strPath, unsigned int uiLength,
							float fAverage, const float fRate[],
							const float fCoord[])
{
	AnalyzedTrack* psTrack = NULL;

	psTrack = (AnalyzedTrack*) malloc (sizeof(AnalyzedTrack));
	analyzedTrackInitWithData(psTrack, iTID, strPath,
							uiLength, fAverage, fRate, fCoord);

	return psTrack;
}
AnalyzedTrack* analyzedTrackCreate (void)
{
	AnalyzedTrack* psTrack = NULL;
	psTrack = analyzedTrackCreateWithData(0, NULL, 0, 0.0, NULL, NULL);
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

const char* analyzedTrackConstGetPath (const AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	return (const char*) psTrack->strPath;
}
char* analyzedTrackGetPath (const AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	return psTrack->strPath;
}
int analyzedTrackSetPath (AnalyzedTrack* psTrack, const char* strNewPath)
{
	assert (psTrack != NULL);

	if (strNewPath != NULL)
	{
		psTrack->strPath = malloc((strlen(strNewPath)+1)*sizeof(char));
		strcpy(psTrack->strPath, strNewPath);
	}

	return EXIT_SUCCESS;
}

unsigned int analyzedTrackGetLength(const AnalyzedTrack* psTrack)
{
	assert(psTrack != NULL);

	return psTrack->uiLength;
}
int analyzedTrackSetLength(AnalyzedTrack* psTrack,
						unsigned int uiValue)
{
	assert(psTrack != NULL);
	assert(uiValue >= 0);

	psTrack->uiLength = uiValue;

	return EXIT_SUCCESS;
}


float analyzedTrackGetAverage (const AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	return psTrack->fAverage;
}
int analyzedTrackSetAverage (AnalyzedTrack* psTrack,
										float fValue)
{
	assert (psTrack != NULL);
	assert (fValue >= 0.0 && fValue <= 22050);

	psTrack->fAverage = fValue;

	return EXIT_SUCCESS;
}


const float* analyzedTrackConstGetRate(const AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	return (const float*) psTrack->pfRate;
}
float* analyzedTrackGetRate (const AnalyzedTrack* psTrack)
{
	assert(psTrack != NULL);

	return (float*) psTrack->pfRate;
}
int analyzedTrackSetRate(AnalyzedTrack* psTrack,
									const float fRate[])
{
	int i;

	assert(psTrack != NULL);

	if (fRate == NULL)
	{
		for (i = 0; i < 3; i++)
		{
			psTrack->pfRate[i] = 0;
		}
	}
	else
	{
		for (i = 0; i < 3; i++)
		{
			psTrack->pfRate[i] = fRate[i];
		}
	}

	return EXIT_SUCCESS;
}

const float* analyzedTrackConstGetCoord (const AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	return (const float*) (psTrack->pfCoord);
}
float* analyzedTrackGetCoord (const AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	return (float*) (psTrack->pfCoord);
}

int analyzedTrackSetCoord(AnalyzedTrack* psTrack,
						const float pfCoord[])
{
	assert (psTrack != NULL);

	if (pfCoord != NULL)
	{
		psTrack->pfCoord[0] = pfCoord[0];
		psTrack->pfCoord[1] = pfCoord[1];
		psTrack->pfCoord[2] = pfCoord[2];
	}
	else
	{
		psTrack->pfCoord[0] = 0;
		psTrack->pfCoord[1] = 0;
		psTrack->pfCoord[2] = 0;

	}

	return EXIT_SUCCESS;
}
int analyzedTrackSetCoords(AnalyzedTrack* psTrack,
						float fX,
						float fY,
						float fZ)
{
        assert (psTrack != NULL);


        psTrack->pfCoord[0] = fX;
        psTrack->pfCoord[1] = fY;
        psTrack->pfCoord[2] = fZ;


        return EXIT_SUCCESS;
}

unsigned char analyzedTrackGetbAdded (const AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL) ;
	
	return psTrack->bAdded ;
}

int analyzedTrackSetbAdded (AnalyzedTrack* psTrack, unsigned char bAdded)
{
	assert (psTrack != NULL && (bAdded == 0 || bAdded == 1)) ;

	psTrack->bAdded = bAdded ;
	
	return EXIT_SUCCESS ;
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
	psTrack = analyzedTrackCreateWithData(100,
										NULL,
										0,
										0,
										NULL,
										NULL);
	assert (psTrack != NULL &&
			psTrack->iTID == 100 &&
			psTrack->strPath == NULL &&
			psTrack->bAnalyzed == 1 &&
			psTrack->uiLength == 0 &&
			psTrack->fAverage == 0 &&
			psTrack->pfRate[0]==0 &&
			psTrack->pfRate[1]== 0 &&
			psTrack->pfRate[2]==0 &&
			psTrack->pfCoord[0]== 0 &&
			psTrack->pfCoord[0]== 0 &&
			psTrack->pfCoord[0]==0);
	printf("\tFAIT !!\n");

	printf("Initialisation d'un morceau...\n");
	analyzedTrackInitWithData(&sTrack2, 50, "test", 25000, 0.5, NULL, NULL);
	assert (sTrack2.iTID == 50 &&
			strcmp(sTrack2.strPath, "test")==0 &&
			sTrack2.bAnalyzed == 1 &&
			sTrack2.uiLength == 25000 &&
			sTrack2.fAverage == 0.5 &&
			psTrack->pfRate[0]==0 &&
			psTrack->pfRate[1]== 0 &&
			psTrack->pfRate[2]==0 &&
			psTrack->pfCoord[0]== 0 &&
			psTrack->pfCoord[0]== 0 &&
			psTrack->pfCoord[0]==0);
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
	analyzedTrackSetAverage(psTrack, 0.5);
	analyzedTrackSetTID(psTrack, 555);
	assert (psTrack->fAverage == 0.5 &&
			psTrack->iTID == 555);
	printf("\tFAIT !!\n");

	printf("Récupération des données...\n");
	printf("TID (555) :              %d\n", analyzedTrackGetTID(psTrack));
	printf("PATH (/test2/filepath) : %s\n", analyzedTrackGetPath(psTrack));
	printf("Average (0.5) :          %f\n",
			analyzedTrackGetAverage(psTrack));
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

