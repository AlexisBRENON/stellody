/**
  * @file gui_data.c
  * @brief Fichier d'implémentation du module \em GUI_DATA.
  *
  * @author Alexis BRENON in STELLODY TEAM
  */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <gtk/gtk.h>

#include "gui_data.h"

/* ********************************************************************* */
/*                                                                       */
/*          Fonctions relatives à la structure de données                */
/*                                                                       */
/* ********************************************************************* */


int guiDataInit (GuiData* pData)
{
	assert (pData != NULL);

	pData->pMainBuilder = gtk_builder_new();
	pData->pStellariumBuilder = gtk_builder_new();
	pData->pPreferencesBuilder = gtk_builder_new();
	pData->pAboutBuilder = gtk_builder_new();

	pData->iIncrementTimerID = 0;

	pData->iXMousePosition = 0;
	pData->iYMousePosition = 0;

	return EXIT_SUCCESS;
}

int guiDataInitWithData (GuiData* pData,
						const char* strMainBuilder,
						const char* strStellariumBuilder,
						const char* strPreferencesBuilder,
						const char* strAboutBuilder)
{
	assert (pData != NULL);

	guiDataInit (pData);
	guiDataSetBuilderByName(pData,
							MAIN, strMainBuilder);
	guiDataSetBuilderByName(pData,
							STELLARIUM, strStellariumBuilder);
	guiDataSetBuilderByName(pData,
							PREFERENCES, strPreferencesBuilder);
	guiDataSetBuilderByName(pData,
							ABOUT, strAboutBuilder);

	return EXIT_SUCCESS;
}


int guiDataRelease (GuiData* pData)
{
	assert (pData != NULL);

	g_object_unref(pData->pMainBuilder);
	pData->pMainBuilder = NULL;
	g_object_unref(pData->pStellariumBuilder);
	pData->pStellariumBuilder = NULL;
	g_object_unref(pData->pPreferencesBuilder);
	pData->pPreferencesBuilder = NULL;
	g_object_unref(pData->pAboutBuilder);
	pData->pAboutBuilder = NULL;

	pData->iIncrementTimerID = 0;

	pData->iXMousePosition = 0;
	pData->iYMousePosition = 0;

	return EXIT_SUCCESS;
}

GuiData* guiDataCreate (void)
{
	GuiData* pData = NULL;

	pData = (GuiData*) malloc(sizeof(GuiData));
	guiDataInit(pData);

	return pData;
}


GuiData* guiDataCreateWithData (const char* strMainBuilder,
								const char* strStellariumBuilder,
								const char* strPreferencesBuilder,
								const char* strAboutBuilder)
{
	GuiData* pData = NULL;

	pData = guiDataCreate();
	guiDataSetBuilderByName(pData,
							MAIN, strMainBuilder);
	guiDataSetBuilderByName(pData,
							STELLARIUM, strStellariumBuilder);
	guiDataSetBuilderByName(pData,
							PREFERENCES, strPreferencesBuilder);
	guiDataSetBuilderByName(pData,
							ABOUT, strAboutBuilder);

	return pData;
}


int guiDataDestroy (GuiData** ppData)
{
	assert (ppData != NULL);
	assert (*ppData != NULL);

	guiDataRelease(*ppData);

	free(*ppData);
	*ppData = NULL;

	return EXIT_SUCCESS;
}


int guiDataSetBuilder (GuiData* pData,
					const BuilderName iBuilderName,
					GtkBuilder* pBuilder)
{
	assert (pData != NULL);
	assert (pBuilder != NULL);

	switch (iBuilderName)
	{
		case MAIN:
			if (pData->pMainBuilder != NULL)
			{
				g_object_unref(pData->pMainBuilder);
			}
			pData->pMainBuilder = pBuilder;
			break;
		case STELLARIUM:
			if (pData->pStellariumBuilder != NULL)
			{
				g_object_unref(pData->pStellariumBuilder);
			}
			pData->pStellariumBuilder = pBuilder;
			break;
		case PREFERENCES:
			if (pData->pPreferencesBuilder != NULL)
			{
				g_object_unref(pData->pPreferencesBuilder);
			}
			pData->pPreferencesBuilder = pBuilder;
			break;
		case ABOUT:
			if (pData->pAboutBuilder != NULL)
			{
				g_object_unref(pData->pAboutBuilder);
			}
			pData->pAboutBuilder = pBuilder;
			break;
		default:
			fprintf(stderr, "STELLODY-Warning **: guiDataSetBuilder : \
					BuilderName not recognized.\n");
			break;
	}

	return EXIT_SUCCESS;
}


int guiDataSetBuilderByName(GuiData* pData,
							const BuilderName iBuilderName,
							const char* strBuilder)
{
	assert (pData != NULL);
	assert (strBuilder != NULL);

	switch (iBuilderName)
	{
		case MAIN:
			gtk_builder_add_from_file(pData->pMainBuilder,
										strBuilder, NULL);
			break;
		case STELLARIUM:
			gtk_builder_add_from_file(pData->pStellariumBuilder,
										strBuilder, NULL);
			break;
		case PREFERENCES:
			gtk_builder_add_from_file(pData->pPreferencesBuilder,
										strBuilder, NULL);
			break;
		case ABOUT:
			gtk_builder_add_from_file(pData->pAboutBuilder,
										strBuilder, NULL);
			break;
		default:
			fprintf(stderr, "STELLODY-Warning **: guiDataSetBuilder : \
					BuilderName not recognized.\n");
			break;
	}

	return EXIT_SUCCESS;
}

GtkBuilder* guiDataGetBuilder (const GuiData* pData,
							BuilderName iBuilderName)
{
	assert (pData != NULL);

	switch (iBuilderName)
	{
		case MAIN:
			return pData->pMainBuilder;
			break;
		case STELLARIUM:
			return pData->pStellariumBuilder;
			break;
		case PREFERENCES:
			return pData->pPreferencesBuilder;
			break;
		case ABOUT:
			return pData->pAboutBuilder;
			break;
		default:
			fprintf(stderr, "STELLODY-Warning **: guiDataSetBuilder : \
					BuilderName not recognized.\n");
			break;
	}

	return NULL;
}

int guiDataSetIncrementTimerID (GuiData* pData,
								int iID)
{
	assert (pData != NULL);
	assert (iID >= 0);

	pData->iIncrementTimerID = iID;

	return EXIT_SUCCESS;
}

int guiDataGetIncrementTimerID (const GuiData* pData)
{
	assert (pData != NULL);

	return pData->iIncrementTimerID;
}


int guiDataSetMousePosition (GuiData* pData,
							int iX, int iY)
{
	assert (pData != NULL);

	if (iX >= 0) {pData->iXMousePosition = iX;}
	if (iY >= 0) {pData->iYMousePosition = iY;}

	return EXIT_SUCCESS;
}


int guiDataGetMousePosition (const GuiData* pData,
							int* iX, int* iY)
{
	assert (pData != NULL);

	if (iX != NULL) {*iX = pData->iXMousePosition;}
	if (iY != NULL) {*iY = pData->iYMousePosition;}

	return EXIT_SUCCESS;
}





































