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
	pData->pLibraryBuilder = gtk_builder_new();

	pData->iIncrementTimerID = 0;

	pData->iXMousePosition = 0;
	pData->iYMousePosition = 0;

	return EXIT_SUCCESS;
}

int guiDataInitWithData (GuiData* pData,
						const char* strMainBuilder,
						const char* strStellariumBuilder,
						const char* strPreferencesBuilder,
						const char* strLibraryBuilder,
						const char* strAboutBuilder)
{
	GtkTreePath* psPath = NULL;
	GtkTreeModel* psModel = NULL;

	assert (pData != NULL);

	guiDataInit (pData);
	guiDataSetBuilderByName(pData,
							MAIN, strMainBuilder);
	guiDataSetBuilderByName(pData,
							STELLARIUM, strStellariumBuilder);
	guiDataSetBuilderByName(pData,
							PREFERENCE, strPreferencesBuilder);
	guiDataSetBuilderByName(pData,
							LIBRARY, strLibraryBuilder);
	guiDataSetBuilderByName(pData,
							ABOUT, strAboutBuilder);

	psModel = GTK_TREE_MODEL(gtk_builder_get_object(pData->pMainBuilder,
													"liststore1"));
	psPath = gtk_tree_path_new_first();
	pData->psRowRef = gtk_tree_row_reference_new(psModel, psPath);

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
	g_object_unref(pData->pLibraryBuilder);
	pData->pLibraryBuilder = NULL;
	g_object_unref(pData->pAboutBuilder);
	pData->pAboutBuilder = NULL;

	pData->iIncrementTimerID = 0;

	gtk_tree_row_reference_free(pData->psRowRef);

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
								const char* strLibraryBuilder,
								const char* strAboutBuilder)
{
	GtkTreePath* psPath = NULL;
	GtkTreeModel* psModel = NULL;
	GuiData* pData = NULL;

	pData = guiDataCreate();
	guiDataSetBuilderByName(pData,
							MAIN, strMainBuilder);
	guiDataSetBuilderByName(pData,
							STELLARIUM, strStellariumBuilder);
	guiDataSetBuilderByName(pData,
							PREFERENCE, strPreferencesBuilder);
	guiDataSetBuilderByName(pData,
							LIBRARY, strLibraryBuilder);
	guiDataSetBuilderByName(pData,
							ABOUT, strAboutBuilder);

	psModel = GTK_TREE_MODEL(gtk_builder_get_object(pData->pMainBuilder,
													"liststore1"));
	psPath = gtk_tree_path_new_first();
	pData->psRowRef = gtk_tree_row_reference_new(psModel, psPath);

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


int guiDataConnectSignals (GuiData* pData,
						void* pConnectData)
{
	assert (pData != NULL);

	gtk_builder_connect_signals(pData->pMainBuilder, pConnectData);
	gtk_builder_connect_signals(pData->pStellariumBuilder, pConnectData);
	gtk_builder_connect_signals(pData->pPreferencesBuilder, pConnectData);
	gtk_builder_connect_signals(pData->pLibraryBuilder, pConnectData);
	gtk_builder_connect_signals(pData->pAboutBuilder, pConnectData);

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
		case PREFERENCE:
			if (pData->pPreferencesBuilder != NULL)
			{
				g_object_unref(pData->pPreferencesBuilder);
			}
			pData->pPreferencesBuilder = pBuilder;
			break;
		case LIBRARY:
			if (pData->pLibraryBuilder != NULL)
			{
				g_object_unref(pData->pLibraryBuilder);
			}
			pData->pLibraryBuilder = pBuilder;
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
		case PREFERENCE:
			gtk_builder_add_from_file(pData->pPreferencesBuilder,
										strBuilder, NULL);
			break;
		case LIBRARY:
			gtk_builder_add_from_file(pData->pLibraryBuilder,
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
		case PREFERENCE:
			return pData->pPreferencesBuilder;
			break;
		case LIBRARY:
			return pData->pLibraryBuilder;
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


GtkTreeRowReference* guiDataGetTreeRowReference (const GuiData* pData)
{
	assert (pData != NULL);

	return (pData->psRowRef);
}

int guiDataSetTreeRowReference (GuiData* pData,
								GtkTreeRowReference* psRowRef)
{
	assert (pData != NULL);

	gtk_tree_row_reference_free(pData->psRowRef);
	pData->psRowRef = NULL;

	if (psRowRef != NULL)
	{
		pData->psRowRef = gtk_tree_row_reference_copy(psRowRef);
	}


	return EXIT_SUCCESS;
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





































