/**
  * @file preferences.c
  * @brief Fichier d'implémentation du module \em preferences.
  *
  * Module de manipulation des préférences de l'application.
  * @author Alexis BRENON in STELLODY TEAM
  */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "preferences.h"
#include "files.h"


/* ********************************************************************* */
/*                                                                       */
/*           Fonctions relatives à la structure Preferences              */
/*                                                                       */
/* ********************************************************************* */


int preferencesInit (Preferences* psPref, int iX, int iY, int iRate)
{
	assert(psPref != NULL);
	assert(iX >= 0 && iY >= 0 && iRate > 0 && iRate <= 100);

	psPref->iAnalysisRate = iRate;
	psPref->iWindowXSize = iX;
	psPref->iWindowYSize = iY;

	return EXIT_SUCCESS;
}
int preferencesInitFromFile(Preferences* psPref, GKeyFile* ppsContext[])
{
	int iRate, iXSize, iYSize;

	assert (psPref != NULL);
	assert (ppsContext != NULL);

	iRate = g_key_file_get_integer(ppsContext[CONFIG],
									"DEFAULT",
									"iAnalysisRate", NULL);
	iXSize = g_key_file_get_integer(ppsContext[CONFIG],
									"DEFAULT",
									"iWindowXSize", NULL);
	iYSize = g_key_file_get_integer(ppsContext[CONFIG],
									"DEFAULT",
									"iWindowYSize", NULL);

	preferencesInit(psPref, iXSize, iYSize, iRate);

	return EXIT_SUCCESS;
}
int preferencesRelease (Preferences* psPref)
{
	assert(psPref != NULL);

	psPref->iAnalysisRate = -1;
	psPref->iWindowXSize = psPref->iWindowYSize = -1;

	return EXIT_SUCCESS;
}

Preferences* preferencesCreate (int iX, int iY, int iRate)
{
	Preferences* psPref = NULL;

	assert(iX >= 0 && iY >= 0 && iRate > 0 && iRate <= 100);

	psPref = (Preferences*) malloc(sizeof(Preferences));
	preferencesInit(psPref, iX, iY, iRate);

	return psPref;
}
Preferences* preferencesCreateFromFile (GKeyFile* ppsContext[])
{
	int iRate, iXSize, iYSize;

	assert (ppsContext != NULL);

	iRate = g_key_file_get_integer(ppsContext[CONFIG],
									"DEFAULT",
									"iAnalysisRate", NULL);
	iXSize = g_key_file_get_integer(ppsContext[CONFIG],
									"DEFAULT",
									"iWindowXSize", NULL);
	iYSize = g_key_file_get_integer(ppsContext[CONFIG],
									"DEFAULT",
									"iWindowYSize", NULL);

	return preferencesCreate(iXSize, iYSize, iRate);
}
int preferencesDestroy (Preferences** ppsPref)
{
	assert(*ppsPref != NULL);

	preferencesRelease(*ppsPref);
	free(*ppsPref);
	*ppsPref = NULL;

	return EXIT_SUCCESS;
}


int preferencesGetWindowXSize (const Preferences* psPref)
{
	assert(psPref != NULL);

	return psPref->iWindowXSize;
}
int preferencesSetWindowXSize (Preferences* psPref, int iValue)
{
	assert(psPref != NULL);
	assert(iValue >= 0);

	psPref->iWindowXSize = iValue;

	return EXIT_SUCCESS;
}

int preferencesGetWindowYSize (const Preferences* psPref)
{
	assert(psPref != NULL);

	return psPref->iWindowYSize;
}
int preferencesSetWindowYSize (Preferences* psPref, int iValue)
{
	assert(psPref != NULL);
	assert(iValue >= 0);

	psPref->iWindowYSize = iValue;

	return EXIT_SUCCESS;
}

int preferencesGetAnalysisRate (const Preferences* psPref)
{
	assert(psPref != NULL);

	return psPref->iAnalysisRate;
}
int preferencesSetAnalysisRate (Preferences* psPref, int iValue)
{
	assert(psPref != NULL);
	assert(iValue > 0 && iValue <= 100);

	psPref->iAnalysisRate = iValue;

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

	printf("\n\t -- MODULE PREFERENCES --\n\n");

	printf("Création d'une structure...\n");
	psPref = preferencesCreate(800, 600, 50);
	assert(psPref != NULL &&
			psPref->iWindowXSize == 800 &&
			psPref->iWindowYSize == 600 &&
			psPref->iAnalysisRate == 50);
	printf("\tFAIT !!\n");

	printf("Modification des valeurs...\n");
	preferencesSetAnalysisRate(psPref, 100);
	preferencesSetWindowXSize(psPref, 0);
	preferencesSetWindowYSize(psPref, 0);
	assert(psPref->iWindowXSize == 0 &&
			psPref->iWindowYSize == 0 &&
			psPref->iAnalysisRate == 100);
	printf("\tFAIT !!\n");

	printf("Récupération des valeurs...\n");
	assert(psPref->iWindowXSize == preferencesGetWindowXSize(psPref) &&
			psPref->iWindowYSize == preferencesGetWindowYSize(psPref) &&
			psPref->iAnalysisRate == preferencesGetAnalysisRate(psPref));
	printf("\tFAIT !!\n");

	printf("Destruction de la structure...\n");
	preferencesDestroy(&psPref);
	assert(psPref == NULL);
	printf("\tFAIT !!\n");

	printf("\n\t -- Fin du test de régression --\n");

	return EXIT_SUCCESS;
}
