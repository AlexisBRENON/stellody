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


/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions relatives aux fichiers                    */
/*                                                                       */
/* ********************************************************************* */


GKeyFile** filesOpen(void)
{
	GKeyFile** ppsContext;
	ppsContext = (GKeyFile**) malloc(3*sizeof(GKeyFile*));

	ppsContext[CONFIG] = g_key_file_new();
	g_key_file_load_from_file(ppsContext[CONFIG], "stellody_config",
							G_KEY_FILE_KEEP_COMMENTS, NULL);

	ppsContext[DATA] = g_key_file_new();
	g_key_file_load_from_file(ppsContext[DATA], "stellody_data",
									G_KEY_FILE_KEEP_COMMENTS, NULL);

	return ppsContext;
}

int filesClose(GKeyFile*** pppsContext)
{
	assert(pppsContext != NULL);

	g_key_file_free((*pppsContext)[CONFIG]);
	g_key_file_free((*pppsContext)[DATA]);
	(*pppsContext)[CONFIG]=NULL;
	(*pppsContext)[DATA]=NULL;

	return EXIT_SUCCESS;
}


int filesCloseAndSave(GKeyFile*** pppsContext)
{
	assert(pppsContext != NULL);

	filesSave(*pppsContext);
	filesClose(pppsContext);

	return EXIT_SUCCESS;
}

int filesSave(GKeyFile** ppsContext)
{
	FILE* pfConfig = NULL;
	FILE* pfData = NULL;

	pfConfig = fopen("stellody_config", "w");
	assert(pfConfig != NULL);
	fprintf(pfConfig, "%s\n", g_key_file_to_data(ppsContext[CONFIG], NULL,
												NULL));

	pfData = fopen("stellody_data", "w");
	assert(pfData != NULL);
	fprintf(pfData, "%s\n", g_key_file_to_data(ppsContext[DATA], NULL,
												NULL));

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
	assert(ppsContext[CONFIG] != NULL &&
			ppsContext[DATA] != NULL);
	printf("\tFAIT !!\n");

	printf("Fermeture et sauvegarde...\n");
	filesCloseAndSave(&ppsContext);
	assert(ppsContext[CONFIG] == NULL &&
			ppsContext[DATA] == NULL);
	printf("\tFAIT !!\n");

	printf("\n\t -- FIN TEST DE REGRESSION --\n");

	return EXIT_SUCCESS;

}
