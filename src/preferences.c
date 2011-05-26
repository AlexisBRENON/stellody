/**
  * @file preferences.c
  * @brief Fichier d'implémentation du module \em Preferences.
  *
  * Module de manipulation des préférences de l'application.
  * @author Alexis BRENON in STELLODY TEAM
  */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "preferences.h"
#include "files.h"


/* ********************************************************************* */
/*                                                                       */
/*           Fonctions relatives à la structure Preferences              */
/*                                                                       */
/* ********************************************************************* */


int preferencesInit (Preferences* psPref)
{
	assert (psPref != NULL);

	psPref->iAnalysisRate = 0;
	psPref->iWindowXSize = 0;
	psPref->iWindowYSize = 0;
	psPref->i3DQuality = 0;
	psPref->iMaxTID = 0;
	psPref->iMinTID = 0;
	psPref->iNbPath = 0;
	psPref->iMoveCam = 0;
	psPref->pstrFilesPath = NULL;

	return EXIT_SUCCESS;
}
int preferencesInitFromFile(Preferences* psPref, GKeyFile* ppsContext[])
{
	gchar** pstrTemp = NULL;
	int i = 0;
	int iSize = 0;

	assert (psPref != NULL);
	assert (ppsContext != NULL);

	psPref->iAnalysisRate = g_key_file_get_integer(ppsContext[CONFIG],
												"DEFAULT",
												"iAnalysisRate", NULL);
	psPref->iWindowXSize = g_key_file_get_integer(ppsContext[CONFIG],
												"DEFAULT",
												"iWindowXSize", NULL);
	psPref->iWindowYSize = g_key_file_get_integer(ppsContext[CONFIG],
												"DEFAULT",
												"iWindowYSize", NULL);
	psPref->i3DQuality = g_key_file_get_integer(ppsContext[CONFIG],
												"DEFAULT",
												"i3DQuality", NULL);
	psPref->iMaxTID = g_key_file_get_integer(ppsContext[CONFIG],
											"DEFAULT",
											"iTIDMax", NULL);
	psPref->iMinTID = g_key_file_get_integer(ppsContext[CONFIG],
											"DEFAULT",
											"iTIDMin", NULL);
	psPref->iMoveCam = g_key_file_get_integer(ppsContext[CONFIG],
											"DEFAULT",
											"iMoveCam", NULL);
	pstrTemp = g_key_file_get_string_list(ppsContext[CONFIG],
											"DEFAULT",
											"pstrPath",
											(gsize*) &(psPref->iNbPath),
											NULL);
	/* Copie le tableau de chaines de manière à ne pas mélanger allocations
	GTK et libération manuelle (on alloue manuellement et on libère avec
	GTK ce qui à été alloué par GTK).*/
	iSize = psPref->iNbPath;
	psPref->pstrFilesPath = (gchar**) malloc((iSize+1)*sizeof(gchar*));
	for (i = 0; i < iSize; i++)
	{
		int len;
		len = strlen(pstrTemp[i]);
		(psPref->pstrFilesPath)[i] = (gchar*) malloc((len+1)*
													sizeof(gchar));
		strcpy((psPref->pstrFilesPath)[i], pstrTemp[i]);
	}
	(psPref->pstrFilesPath)[iSize] = NULL;

	g_strfreev(pstrTemp);

	return EXIT_SUCCESS;
}
int preferencesRelease (Preferences* psPref)
{
	int i;

	assert (psPref != NULL);

	psPref->iAnalysisRate = -1;
	psPref->iWindowXSize = psPref->iWindowYSize = -1;
	psPref->i3DQuality = -1;
	psPref->iNbPath = -1;
	psPref->iMoveCam = -1;
	for (i = 0; psPref->pstrFilesPath[i] != NULL; i++)
	{
		free(psPref->pstrFilesPath[i]);
		psPref->pstrFilesPath[i] = NULL;
	}
	free(psPref->pstrFilesPath[i]);
	psPref->pstrFilesPath[i] = NULL;
	psPref->pstrFilesPath = NULL;

	return EXIT_SUCCESS;
}

Preferences* preferencesCreate (void)
{
	Preferences* psPref = NULL;

	psPref = (Preferences*) malloc(sizeof(Preferences));
	preferencesInit(psPref);

	return psPref;
}
Preferences* preferencesCreateFromFile (GKeyFile* ppsContext[])
{
	Preferences* psPref = NULL;

	assert (ppsContext != NULL);

	psPref = preferencesCreate();
	preferencesInitFromFile(psPref, ppsContext);

	return psPref;
}
int preferencesDestroy (Preferences** ppsPref)
{
	assert (ppsPref != NULL);
	assert (*ppsPref != NULL);

	preferencesRelease(*ppsPref);
	free(*ppsPref);
	*ppsPref = NULL;

	return EXIT_SUCCESS;
}


int preferencesGetWindowXSize (const Preferences* psPref)
{
	assert (psPref != NULL);

	return psPref->iWindowXSize;
}
int preferencesSetWindowXSize (Preferences* psPref, int iValue)
{
	assert (psPref != NULL);
	assert (iValue >= 0);

	psPref->iWindowXSize = iValue;

	return EXIT_SUCCESS;
}

int preferencesGetWindowYSize (const Preferences* psPref)
{
	assert (psPref != NULL);

	return psPref->iWindowYSize;
}
int preferencesSetWindowYSize (Preferences* psPref, int iValue)
{
	assert (psPref != NULL);
	assert (iValue >= 0);

	psPref->iWindowYSize = iValue;

	return EXIT_SUCCESS;
}

int preferencesGetAnalysisRate (const Preferences* psPref)
{
	assert (psPref != NULL);

	return psPref->iAnalysisRate;
}
int preferencesSetAnalysisRate (Preferences* psPref, int iValue)
{
	assert (psPref != NULL);
	assert (iValue > 0 && iValue <= 100);

	psPref->iAnalysisRate = iValue;

	return EXIT_SUCCESS;
}

int preferencesGetMaxTID(const Preferences* psPref)
{
	assert(psPref != NULL);

	return psPref->iMaxTID;
}
int preferencesSetMaxTID(Preferences* psPref, int iValue)
{
	assert(psPref != NULL);
	assert(iValue > 0 && iValue > psPref->iMaxTID);

	psPref->iMaxTID = iValue;

	return EXIT_SUCCESS;
}

int preferencesGetMinTID(const Preferences* psPref)
{
	assert(psPref != NULL);

	return psPref->iMinTID;
}
int preferencesSetMinTID(Preferences* psPref, int iValue)
{
	assert(psPref != NULL);
	assert(iValue > 0 && iValue < psPref->iMinTID);

	psPref->iMinTID = iValue;

	return EXIT_SUCCESS;
}

char** preferencesGetFilesPath(const Preferences* psPref,
									int* piSize)
{
	assert (psPref != NULL);
	if (piSize != NULL)
	{
		*piSize = psPref->iNbPath;
	}

	return psPref->pstrFilesPath;
}
int preferencesSetFilesPath(Preferences* psPref, int iSize,
							char* pstrPath[])
{
	int i;

	assert (psPref != NULL);
	assert (iSize > 0);
	assert (pstrPath != NULL);

	/* Libère les anciens chemins sauvegardés */
	for (i = 0; i < psPref->iNbPath; i++)
	{
		free(psPref->pstrFilesPath[i]);
		psPref->pstrFilesPath[i] = NULL;
	}
	free(psPref->pstrFilesPath);
	psPref->pstrFilesPath = NULL;

	/* Stocke les nouveaux chemins */
	psPref->iNbPath = iSize;
	psPref->pstrFilesPath = (gchar**) malloc((iSize+1)*sizeof(gchar*));
	for (i = 0; i < iSize; i++)
	{
		int len;
		len = strlen(pstrPath[i]);
		(psPref->pstrFilesPath)[i] = (gchar*) malloc((len+1)*
													sizeof(gchar));
		strcpy((psPref->pstrFilesPath)[i], pstrPath[i]);
	}
	(psPref->pstrFilesPath)[iSize] = NULL;


	return EXIT_SUCCESS;
}

int preferencesAddFilesPath(Preferences* psPref, const char* strPath)
{
	int iSize = 0;
	int i = 0;
	int len = 0;
	char** pstrNewPath = NULL;
	char** pstrOldPath = NULL;
	int iAnswer = -1;

	pstrOldPath = preferencesGetFilesPath(psPref, &iSize);

	while (iAnswer != 0 && i < iSize)
	{
		iAnswer = strcmp(pstrOldPath[i], strPath);
		i++;
	}

	if (iAnswer == 0)
	{
		return EXIT_FAILURE;
	}
	else
	{
		pstrNewPath = (char **) malloc((iSize+2)*sizeof(char*));
		for (i = 0; i < iSize; i++)
		{
			len = strlen(pstrOldPath[i]);
			pstrNewPath[i] = (char*) malloc((len+1)*sizeof(char));
			strcpy(pstrNewPath[i], pstrOldPath[i]);
		}

		len = strlen(strPath);
		pstrNewPath[iSize] = (char*) malloc((len+1)*sizeof(char));
		strcpy(pstrNewPath[iSize], strPath);

		pstrNewPath[iSize+1] = NULL;

		preferencesSetFilesPath(psPref, iSize+1, pstrNewPath);
	}


	return EXIT_SUCCESS;
}

int preferencesGet3DQuality(const Preferences* psPref)
{
	assert (psPref != NULL);

	return psPref->i3DQuality;
}

int preferencesSet3DQuality(Preferences* psPref, int iValue)
{
	assert (psPref != NULL);
	assert (iValue > 0 && iValue <= 100);

	psPref->i3DQuality = iValue;

	return EXIT_SUCCESS;
}

int preferencesGetMoveCam(const Preferences* psPref)
{
	assert (psPref != NULL);

	return psPref->iMoveCam;
}

int preferencesSetMoveCam(Preferences* psPref, int iValue)
{
	printf("%d\n", iValue);

	assert (psPref != NULL);
	assert (iValue == 0 || iValue == 1);

	psPref->iMoveCam = iValue;

	return EXIT_SUCCESS;
}


/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */


int preferencesRegressionTest (void)
{
	Preferences* psPref = NULL;
	char strPath1[] = "test1";
	char strPath2[] = "test2";
	char* pstrPath[2];
	int iNbPath;

	printf("\n\t -- MODULE PREFERENCES --\n\n");

	printf("Création d'une structure...\n");
	psPref = preferencesCreate();
	assert (psPref != NULL);
	printf("\tFAIT !!\n");

	printf("Modification des valeurs...\n");
	preferencesSetAnalysisRate(psPref, 100);
	preferencesSetWindowXSize(psPref, 0);
	preferencesSetWindowYSize(psPref, 0);
	pstrPath[0]=strPath1;
	pstrPath[1]=strPath2;
	preferencesSetFilesPath(psPref, 2, pstrPath);
	assert (psPref->iWindowXSize == 0 &&
			psPref->iWindowYSize == 0 &&
			psPref->iAnalysisRate == 100 &&
			strcmp((psPref->pstrFilesPath)[0],"test1") == 0 &&
			strcmp((psPref->pstrFilesPath)[1],"test2") == 0 &&
			psPref->iNbPath == 2);
	printf("\tFAIT !!\n");

	printf("Récupération des valeurs...\n");
	assert (psPref->iWindowXSize == preferencesGetWindowXSize(psPref) &&
			psPref->iWindowYSize == preferencesGetWindowYSize(psPref) &&
			psPref->iAnalysisRate == preferencesGetAnalysisRate(psPref) &&
			strcmp((psPref->pstrFilesPath)[0],
					(preferencesGetFilesPath(psPref, &iNbPath))[0]) == 0 &&
			strcmp((psPref->pstrFilesPath)[1],
					(preferencesGetFilesPath(psPref, NULL))[0]) == 1 &&
			iNbPath == 2);
	printf("\tFAIT !!\n");

	printf("Destruction de la structure...\n");
	preferencesDestroy(&psPref);
	assert (psPref == NULL);
	printf("\tFAIT !!\n");

	printf("\n\t -- Fin du test de régression --\n");

	return EXIT_SUCCESS;
}

