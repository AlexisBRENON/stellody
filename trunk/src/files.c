/**
  * @file files.c
  * @brief Fichier d'implémentation du module \em Files.
  *
  * @author Alexis BRENON in STELLODY TEAM
  */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <gtk/gtk.h>

#include "files.h"
#include "preferences.h"
#include "analyzed_tracks.h"
#include "analyzed_track.h"

/* ********************************************************************* */
/*                                                                       */
/*                               Constantes                              */
/*                                                                       */
/* ********************************************************************* */

#define CONFIG_FILE "data/.stellody_config" /**< Nom du fichier de config */
#define DATA_FILE "data/.stellody_data" /**< Nom du fichier de données */

/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions relatives aux fichiers                    */
/*                                                                       */
/* ********************************************************************* */



GKeyFile** filesOpen(void)
{
	GKeyFile** ppsContext;
	ppsContext = (GKeyFile**) malloc(2*sizeof(GKeyFile*));

	ppsContext[CONFIG] = g_key_file_new();
	g_key_file_load_from_file(ppsContext[CONFIG], CONFIG_FILE,
							G_KEY_FILE_NONE, NULL);

	ppsContext[DATA] = g_key_file_new();
	g_key_file_load_from_file(ppsContext[DATA], DATA_FILE,
									G_KEY_FILE_NONE, NULL);

	return ppsContext;
}

int filesClose(GKeyFile*** pppsContext)
{
	assert (pppsContext != NULL);

	g_key_file_free((*pppsContext)[CONFIG]);
	g_key_file_free((*pppsContext)[DATA]);
	(*pppsContext)[CONFIG]=NULL;
	(*pppsContext)[DATA]=NULL;
	free(*pppsContext);
	*pppsContext = NULL;

	return EXIT_SUCCESS;
}


int filesCloseAndSave(GKeyFile*** pppsContext,
					const Preferences* psPref,
					AnalyzedTracks* psTracks)
{
	assert (pppsContext != NULL);

	filesSave(*pppsContext,	psPref, psTracks);
	filesClose(pppsContext);

	return EXIT_SUCCESS;
}

int filesSave(GKeyFile** ppsContext,
			const Preferences* psPref,
			AnalyzedTracks* psTracks)
{
	FILE* pfConfig = NULL;
	FILE* pfData = NULL;
	int iSize = preferencesGetNbPath(psPref);

	if (psPref != NULL)
	{
		g_key_file_set_integer(ppsContext[CONFIG],
								"DEFAULT", "iAnalysisRate",
								preferencesGetAnalysisRate(psPref));
		g_key_file_set_integer(ppsContext[CONFIG],
								"DEFAULT", "iWindowXSize",
								preferencesGetWindowXSize(psPref));
		g_key_file_set_integer(ppsContext[CONFIG],
								"DEFAULT", "iWindowYSize",
								preferencesGetWindowYSize(psPref));
		g_key_file_set_integer(ppsContext[CONFIG],
								"DEFAULT", "i3DQuality",
								preferencesGet3DQuality(psPref));
		g_key_file_set_integer(ppsContext[CONFIG],
								"DEFAULT", "iTIDMax",
								preferencesGetMaxTID(psPref));
		g_key_file_set_integer(ppsContext[CONFIG],
								"DEFAULT", "iTIDMin",
								preferencesGetMinTID(psPref));
		g_key_file_set_integer(ppsContext[CONFIG],
								"DEFAULT", "iMoveCam",
								preferencesGetMoveCam(psPref));
		g_key_file_set_integer(ppsContext[CONFIG],
								"DEFAULT", "iDispGrid",
								preferencesGetGrid(psPref));
		g_key_file_set_string_list(ppsContext[CONFIG],
									"DEFAULT", "pstrPath",
			(const gchar* const*) preferencesGetFilesPath(psPref, &iSize),
									iSize);

		pfConfig = fopen(CONFIG_FILE, "w");
		assert (pfConfig != NULL);
		fprintf(pfConfig, "%s\n", g_key_file_to_data(ppsContext[CONFIG],
													NULL, NULL));
	}


	if (psTracks != NULL)
	{
		AnalyzedTrack* psTrack = NULL;
		int iNbTracks = 0;
		int i = 0;

		iNbTracks = analyzedTracksGetNbTracks(psTracks);
		g_key_file_free(ppsContext[DATA]);
		ppsContext[DATA] = g_key_file_new();

		for (i = 0; i < iNbTracks; i++)
		{
			psTrack = analyzedTracksGetTrackInArray(psTracks, i);
			analyzedTracksSaveTracks(psTrack, ppsContext[DATA]);
		}

		pfData = fopen(DATA_FILE, "w");
		assert (pfData != NULL);
		fprintf(pfData, "%s\n", g_key_file_to_data(ppsContext[DATA], NULL,
													NULL));
	}

	fclose(pfConfig);
	fclose(pfData);

	return EXIT_SUCCESS;
}


/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */


int filesRegressionTest(void)
{
	GKeyFile** ppsContext = NULL;

	printf("\n\t -- MODULES FILES --\n");

	printf("Ouverture d'un fichier...\n");
	ppsContext = filesOpen();
	assert (ppsContext[CONFIG] != NULL &&
			ppsContext[DATA] != NULL);
	printf("\tFAIT !!\n");

	printf("Fermeture et sauvegarde...\n");
	filesClose(&ppsContext);
	assert (ppsContext == NULL);
	printf("\tFAIT !!\n");

	printf("\n\t -- FIN TEST DE REGRESSION --\n");

	return EXIT_SUCCESS;

}
