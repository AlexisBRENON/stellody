/**
  * @file gui.c
  * @brief Fichier d'implémentation du module \em GUI.
  *
  * @author Alexis BRENON in STELLODY TEAM
  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#if defined(__linux) || defined (__APPLE__)
	#include <dirent.h>
#endif

#include <gtk/gtk.h>
#include <gtk/gtkgl.h>

#if defined(__linux)
#include <fmodex/fmod.h>
#endif
#if defined (__APPLE__) || defined (_WIN32)
#include <fmod.h>
#endif

#include "gui.h"
#include "gui_data.h"
#include "preferences.h"
#include "analyzed_tracks.h"
#include "analyzed_track.h"
#include "analysis.h"
#include "opengl_drawing.h"
#include "player.h"
#include "player_data.h"
#include "stellarium.h"
#include "files.h"


/* ********************************************************************* */
/*                                                                       */
/*                       Définitions de constantes                       */
/*                                                                       */
/* ********************************************************************* */

#define GUI_WIN "data/windows/Window.glade" /**< Nom du fichier GUI
principal. */
#define GUI_STELLARIUM "data/windows/Stellarium.glade" /**< Nom du fichier
GUI contenant la base pour le Stellarium. */
#define GUI_PREFERENCES "data/windows/Preferences.glade" /**< Nom du
fichier GUI contenant la gestion des préférences. */
#define GUI_ABOUT "data/windows/About.glade" /**< Nom du fichier GUI
contenant le A Propos. */
#define GUI_ADDTRACK "data/windows/AddTrack.glade" /**< Interface de
chargement d'un fichier. */
#define GUI_LIBRARY "data/windows/Library.glade" /**< Interface d'affichage
de la librairie */

#define GUI_MAIN_WIN "Stellody_Window" /**< Nom du widget de niveau
supérieur. */
#define GUI_MAIN_CONTAINER "Variable_Box" /**< Nom du widget contenant les
différentes fenêtres (variables). */
#define GUI_STELLARIUM_WIN "Stellarium_VBox" /**< Nom du conteneur du
stellarium. */
#define GUI_PREFERENCES_WIN "Preferences_VBox" /**< Nom du conteneur des
preferences. */
#define GUI_ABOUT_WIN "About_VBox" /**< Nom du conteneur de A Propos; */
#define GUI_LIBRARY_WIN "Library_Box" /**< Nom du conteneur de la librairie */
#define GUI_ADDTRACK_WIN "AddTrack_VBox" /**< Conteneneur d'ajout de
fichier. */


/* ********************************************************************* */
/*                                                                       */
/*                           Fonctions privées                           */
/*                                                                       */
/* ********************************************************************* */

/* ********************************************************************* */
/* Déclarations                                                          */
/* ********************************************************************* */

static int guiUnparent (GuiData* psData);

static gboolean guiTrackScaleIncrement (gpointer* pData);

static int guiPlayTrack (AnalyzedTrack* pTrack,
						GtkBuilder* pMainBuilder,
						FMOD_SYSTEM* pFmodContext,
						FMOD_CHANNEL** ppPlayingChannel,
						int iGoToStar,
						OpenGLData* pGLData);
static int guiStopTrack (GtkBuilder* psMainBuilder,
						FMOD_CHANNEL* psChannel,
						GuiData* psGuiData,
						OpenGLData* psGLData,
						int iTimeOutID);

static int guiAddTrackToPlaylist (GtkBuilder* pMainBuilder,
								AnalyzedTrack* pTrack,
								gpointer* pData);


static int guiAddFolderToAnalyze (char* strFolderName,
								gpointer* pData);


/* ********************************************************************* */
/* Implémentation                                                        */
/* ********************************************************************* */

static int guiUnparent (GuiData* psData)
{
	GtkBuilder* pMainBuilder = NULL;
	GtkBuilder* pPreferencesBuilder = NULL;
	GtkBuilder* pAboutBuilder = NULL;
	GtkBuilder* pStellariumBuilder = NULL;
	GtkBuilder* pLibraryBuilder = NULL;

	GtkWidget* psStellarium = NULL;
	GtkWidget* psPreference = NULL;
	GtkWidget* psAbout = NULL;
	GtkWidget* psLibrary = NULL;
	GtkWidget* psContainer = NULL;


	pMainBuilder = guiDataGetBuilder(psData, MAIN);
	pPreferencesBuilder = guiDataGetBuilder(psData, PREFERENCE);
	pAboutBuilder = guiDataGetBuilder(psData, ABOUT);
	pStellariumBuilder = guiDataGetBuilder(psData, STELLARIUM);
	pLibraryBuilder = guiDataGetBuilder(psData, LIBRARY);


	psContainer = GTK_WIDGET(
				gtk_builder_get_object(pMainBuilder,
										GUI_MAIN_CONTAINER));

	psPreference = GTK_WIDGET(
				gtk_builder_get_object(pPreferencesBuilder,
									GUI_PREFERENCES_WIN));
	psAbout = GTK_WIDGET(
			gtk_builder_get_object(pAboutBuilder,
									GUI_ABOUT_WIN));
	psStellarium = GTK_WIDGET(
					gtk_builder_get_object(pStellariumBuilder,
											GUI_STELLARIUM_WIN));
	psLibrary = GTK_WIDGET(
					gtk_builder_get_object(pLibraryBuilder,
											GUI_LIBRARY_WIN));

	gtk_widget_hide_all(psPreference);
	gtk_widget_hide_all(psAbout);
	gtk_widget_hide_all(psStellarium);
	gtk_widget_hide_all(psLibrary);


	if (gtk_widget_get_parent(psPreference) != NULL)
	{
		gtk_container_remove ((GtkContainer*)psContainer, psPreference);
	}
	if (gtk_widget_get_parent(psAbout) != NULL)
	{
		gtk_container_remove ((GtkContainer*)psContainer, psAbout);
	}
	if (gtk_widget_get_parent(psStellarium) != NULL)
	{
		gtk_container_remove ((GtkContainer*)psContainer, psStellarium);
	}
	if (gtk_widget_get_parent(psLibrary) != NULL)
	{
		gtk_container_remove ((GtkContainer*)psContainer, psLibrary);
	}

	return EXIT_SUCCESS;
}


static gboolean guiTrackScaleIncrement (gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	GuiData* psGuiData = pData[2];
	PlayerData* psPlayerData = pData[3];
	OpenGLData* psGLData = pData[4];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GtkObject* psAdjustment = NULL;
	GtkBuilder* psMainBuilder = NULL;
	FMOD_CHANNEL* psPlayingChannel = NULL;

/* ********************************************************************* */
/* Si un morceau est en lecture, on le met en pause et inversement       */
/* ********************************************************************* */

	assert (pData != NULL);

	psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);
	psPlayingChannel = playerDataGetPlayingChannel(psPlayerData);

	psAdjustment = GTK_OBJECT(gtk_builder_get_object(psMainBuilder,
										"Track_Adjustment"));

	if (playerIsPlaying(psPlayingChannel) == 0)	/* Si la lecture est stoppée, on réinitialise la barre de progression */
	{
		int iTimeOutID = 0;

		iTimeOutID = guiDataGetIncrementTimerID(psGuiData);

		guiStopTrack(psMainBuilder,
					psPlayingChannel,
					psGuiData,
					psGLData,
					iTimeOutID);

		return FALSE;
	}
	else if (playerIsPaused(psPlayingChannel) == 0)	/* Si la lecture n'est pas en pause, on incrémente */
	{
		int iActualValue = 0;

		iActualValue = gtk_adjustment_get_value(
											GTK_ADJUSTMENT(psAdjustment));
		gtk_adjustment_set_value(GTK_ADJUSTMENT(psAdjustment),
								iActualValue+1);

		return TRUE;
	}
	else /* Si la lecture est en pause */
	{
		/* On ne fait rien */
		return TRUE;
	}

	return FALSE;
}


static int guiPlayTrack (AnalyzedTrack* pTrack,
						GtkBuilder* pMainBuilder,
						FMOD_SYSTEM* pFmodContext,
						FMOD_CHANNEL** ppPlayingChannel,
						int iGoToStar,
						OpenGLData* pGLData)
{
	unsigned int uiTrackPerCent = 0;
	unsigned int uiTrackLength = 0;
	char* strSinger = NULL;
	char* strTitle = NULL;
	char* strTrackInfo = NULL;
	GtkWidget* pScale = NULL;
	GtkWidget* pLabel = NULL;
	GtkObject* pAdjustment = NULL;

/* ********************************************************************* */
/* ********************************************************************* */

	/* On récupère à partir de quelle partie commencer le morceau (%) */

	pScale = GTK_WIDGET(gtk_builder_get_object(pMainBuilder,
												"Track_Scale"));
	uiTrackPerCent = (unsigned int)
					gtk_range_get_value(GTK_RANGE(pScale));

/* ********************************************************************* */
/* ********************************************************************* */

	/* On lance la lecture */

	playerPlayTrack(analyzedTrackGetPath(pTrack),
					pFmodContext,
					ppPlayingChannel,
					0 /* FALSE */,
					uiTrackPerCent,
					&uiTrackLength,
					&strSinger, &strTitle);

/* ********************************************************************* */
/* ********************************************************************* */

	drawingGLSetPlayedTrack(pGLData, pTrack);
	if (iGoToStar == 1)
	{
		drawingGLSetNewDirection(pGLData, pTrack);
	}


/* ********************************************************************* */
/* ********************************************************************* */

	/* On met à l'échelle la barre de progression du morceau */

	pAdjustment = GTK_OBJECT(gtk_builder_get_object(
											pMainBuilder,
											"Track_Adjustment"));

	uiTrackPerCent = (uiTrackPerCent*uiTrackLength)/100;

	gtk_adjustment_set_upper(GTK_ADJUSTMENT(pAdjustment),
							(double) uiTrackLength/1000);
	gtk_range_set_value(GTK_RANGE(pScale),
						(double) uiTrackPerCent/1000);

/* ********************************************************************* */
/* ********************************************************************* */

	/* On concatène artiste et morceau */

	if (g_utf8_validate(strTitle, -1, NULL) == FALSE)
	{
		strTitle = NULL;
	}
	if (g_utf8_validate(strSinger, -1, NULL) == FALSE)
	{
		strSinger = NULL;
	}
	strTrackInfo = (char*) malloc((strlen(strTitle)+
									strlen(" - ") +
									strlen(strSinger)
									+1)
									*sizeof(char));
	strcpy(strTrackInfo, strSinger);
	strcat(strTrackInfo, " - ");
	strcat(strTrackInfo, strTitle);

	/* On les affiche dans le label */

	pLabel = GTK_WIDGET(gtk_builder_get_object(pMainBuilder,
												"TrackInfo_Label"));
	gtk_label_set_text(GTK_LABEL(pLabel),
						strTrackInfo);

	/* On libère la chaine */

	free(strTrackInfo);

/* ********************************************************************* */
/* ********************************************************************* */

	return EXIT_SUCCESS;

}

static int guiStopTrack (GtkBuilder* psMainBuilder,
						FMOD_CHANNEL* psChannel,
						GuiData* psGuiData,
						OpenGLData* psGLData,
						int iTimeOutID)
{
	GtkWidget* psRange = NULL;
	GtkWidget* psTrackLabel = NULL;
	GtkObject* psAdjustment = NULL;

/* Cessation de la lecture. */
	playerStopTrack(psChannel);
	drawingGLSetPlayedTrack(psGLData, NULL);

/* Arret du timer d'incrémentation de la barre d'avancement */
	g_source_remove(iTimeOutID);
	guiDataSetIncrementTimerID(psGuiData, 0);

/* Réinitialisation de l'affichage (barre de progression, label). */
	psRange = GTK_WIDGET(
				gtk_builder_get_object(psMainBuilder,
										"Track_Scale"));
	psAdjustment = GTK_OBJECT(
					gtk_builder_get_object(psMainBuilder,
										"Track_Adjustment"));
	psTrackLabel = GTK_WIDGET(
					gtk_builder_get_object(psMainBuilder,
											"TrackInfo_Label"));

	gtk_label_set_text(GTK_LABEL(psTrackLabel), "");

	gtk_adjustment_set_upper(GTK_ADJUSTMENT(psAdjustment), 100);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(psAdjustment), 0);
	gtk_range_set_value(GTK_RANGE(psRange), 0);



	return EXIT_SUCCESS;
}

static int guiAddTrackToPlaylist (GtkBuilder* pMainBuilder,
								AnalyzedTrack* pTrack,
								gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	Preferences* psPreferences = pData[1];
	GuiData* psGuiData = pData[2];
	PlayerData* psPlayerData = pData[3];
	OpenGLData* psGLData = pData[4];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GtkBuilder* psMainBuilder = NULL;
	FMOD_SYSTEM* psFmodContext = NULL;
	FMOD_CHANNEL* psPlayingChannel = NULL;
	int iMoveCam = 0;
	int iIncrementTimerID = 0;
	int iTID = 0;
	int iPlay = 0;

	GtkTreeModel* psListStore = NULL;
	GtkTreeView* psTreeView = NULL;
	GtkTreeIter Iter;
	char* strTrackPath = NULL;
	char* strTrackName = NULL;

/* ********************************************************************* */
/* ********************************************************************* */

	psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);
	psFmodContext = playerDataGetSystem(psPlayerData);
	psPlayingChannel = playerDataGetPlayingChannel(psPlayerData);
	iMoveCam = preferencesGetMoveCam(psPreferences);
	psListStore = GTK_TREE_MODEL( gtk_builder_get_object (psMainBuilder,
														"liststore1"));

	/* Si la liste de lecture est vide, il faut lancer la lecture */
	if (gtk_tree_model_get_iter_first(psListStore, &Iter) == FALSE)
	{
		iPlay = 1;
	}

	/* On ajoute le morceau à la playlist */
	strTrackPath = analyzedTrackGetPath(pTrack);
	strTrackName = strrchr(strTrackPath, '/');
	iTID = analyzedTrackGetTID(pTrack);

	psTreeView = (GtkTreeView*) gtk_builder_get_object(psMainBuilder,
													"PlayList_TreeView");


	gtk_list_store_append(GTK_LIST_STORE(psListStore), &Iter);
	gtk_list_store_set(GTK_LIST_STORE(psListStore), &Iter,
						0, &(strTrackName[1]), 1, iTID, -1);

	gtk_widget_show_all(GTK_WIDGET(psTreeView));

	/* On lance la lecture */
	if (iPlay == 1)
	{
		GtkTreePath* psPath = NULL;
		GtkTreeRowReference* psRowRef = NULL;

		on_Stop_Action_activate(NULL, pData);
		guiPlayTrack(pTrack,
					psMainBuilder,
					psFmodContext,
					&psPlayingChannel,
					iMoveCam,
					psGLData);
		playerDataSetPlayingChannel(psPlayerData, psPlayingChannel);

		iIncrementTimerID =
			g_timeout_add_seconds(1,
						(GSourceFunc) guiTrackScaleIncrement,
						pData);
		guiDataSetIncrementTimerID(psGuiData, iIncrementTimerID);


		/* On met à jour le morceau actif de la playlist */
		psPath = gtk_tree_path_new_first();
		psRowRef = gtk_tree_row_reference_new(GTK_TREE_MODEL(psListStore),
												psPath);
		guiDataSetTreeRowReference(psGuiData, psRowRef);
		gtk_tree_row_reference_free(psRowRef);
		gtk_tree_path_free(psPath);
	}

	return EXIT_SUCCESS;
}

static int guiAddFolderToAnalyze (char* strFolderName,
								gpointer* pData)
{
	DIR *dirp;
	struct dirent *dp;
	unsigned char ucFileType = 0;
	char* strFileName = NULL;
	char* strExtension = NULL;
	int iLength = 0;
	int i = 0;

	/* Ajoute tous les fichiers du dossier. */
	dirp = opendir(strFolderName);
	assert(dirp);
	while ((dp = readdir(dirp)) != NULL)
	{
		ucFileType = dp->d_type;
		strFileName = dp->d_name;

		/* S'il s'agit d'un dossier */
		if (ucFileType == 4 &&
			strcmp(strFileName, ".") != 0 &&
			strcmp(strFileName, "..") != 0)
		{
			char* strFullFileName = NULL;
			strFullFileName = (char*) malloc(strlen(strFolderName)+
												1+
												strlen(strFileName)+
												1);
			strcpy(strFullFileName, strFolderName);
			strcat(strFullFileName, "/");
			strcat(strFullFileName, strFileName);
			guiAddFolderToAnalyze(strFullFileName, pData);
			free(strFullFileName);
		}
		else
		{
			iLength = strlen(strFileName);
			strExtension = &(strFileName[iLength-4]);

			for (i = 1; i < 4; i++)
			{
				strExtension[i] = tolower(strExtension[i]);
			}

			/* On vérifie que ce soit un fichier pris en charge */
			if (strcmp(strExtension, ".mp3") == 0 ||
				strcmp(strExtension, ".mid") == 0 ||
				strcmp(strExtension, ".m3u") == 0 ||
				strcmp(strExtension, ".mp2") == 0 ||
				strcmp(strExtension, ".ogg") == 0 ||
				strcmp(strExtension, ".raw") == 0 ||
				strcmp(strExtension, ".wav") == 0)
			{
				char* strFullFileName = NULL;
				int iPathLength = 0;

				iPathLength = strlen(strFolderName);
				strFullFileName = (char*) malloc(
							(iLength+iPathLength+2)*sizeof(char));
				strcpy(strFullFileName, strFolderName);
				strcat(strFullFileName, "/");
				strcat(strFullFileName, strFileName);

				analysisTrack(strFullFileName, pData);

				free(strFullFileName);
				strFullFileName = NULL;
			}
		}
	}

	closedir(dirp);

	return EXIT_SUCCESS;
}

/* ********************************************************************* */
/* ********************************************************************* */


/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions relatives à la fenêtre                    */
/*                                                                       */
/* ********************************************************************* */

/* ********************************************************************* */
/*                           FENÊTRE PRINCIPALE                          */
/* ********************************************************************* */


int guiLoad (void** ppDatas)
{
	GKeyFile** ppsFiles = NULL;

	GtkBuilder* pMainBuilder = NULL;
	GtkWidget* pMainWindow = NULL;

	int iNbTracks = 0;
	int i = 0;
	AnalyzedTrack* psTrackToAnalyzed = NULL;

/* ********************************************************************* */
/*                                                                       */
/*                             CHARGEMENTS                               */
/*                                                                       */
/* ********************************************************************* */


/* ********************************************************************* */
/* Ouverture des fichiers...                                             */
/* ********************************************************************* */

	ppsFiles = filesOpen();

/* ********************************************************************* */
/* Chargements des morceaux...                                           */
/* ********************************************************************* */

	ppDatas[0] = (void*) analyzedTracksCreateFromFile(ppsFiles);

/* ********************************************************************* */
/* Chargements des préférences...                                        */
/* ********************************************************************* */

	ppDatas[1] = (void*) preferencesCreateFromFile(ppsFiles);

	filesClose(&ppsFiles);

/* ********************************************************************* */
/* Initialisation de l'interface...                                      */
/* ********************************************************************* */

	ppDatas[2] = (void*) guiDataCreateWithData(GUI_WIN,
										GUI_STELLARIUM,
										GUI_PREFERENCES,
										GUI_LIBRARY,
										GUI_ABOUT);
	guiDataConnectSignals((GuiData*) ppDatas[2],
						ppDatas);

/* ********************************************************************* */
/* Initialisation du lecteur...                                          */
/* ********************************************************************* */

	ppDatas[3] = (void*) playerDataCreate();

/* ********************************************************************* */
/* Initialisation de l'affichage 3D...                                   */
/* ********************************************************************* */

	ppDatas[4] = malloc(sizeof(OpenGLData));
	drawingGLStellariumInit(ppDatas[4]);

/* ********************************************************************* */
/* Remise en analyse des morceaux non finis.                             */
/* ********************************************************************* */

	iNbTracks = analyzedTracksGetNbTracks(ppDatas[0]);
	for (i = 0; i < iNbTracks; i++)
	{
		psTrackToAnalyzed = analyzedTracksGetTrackInArray(ppDatas[0],
															i);
		analyzedTracksCheckForAnalyze(psTrackToAnalyzed, ppDatas);
	}

/* ********************************************************************* */
/* Ouverture de la fenêtre...                                            */
/* ********************************************************************* */

	pMainBuilder= guiDataGetBuilder((GuiData*) ppDatas[2],
											MAIN);

	pMainWindow = GTK_WIDGET(
				gtk_builder_get_object(pMainBuilder, GUI_MAIN_WIN));
	gtk_window_set_icon_from_file (GTK_WINDOW(pMainWindow),
							"data/images/icone.png", NULL);

	gtk_widget_show_all(pMainWindow);

/* ********************************************************************* */
/* Fin                                                                   */
/* ********************************************************************* */

	return EXIT_SUCCESS;
}

int on_Quit_Action_activate(GtkWidget* psWidget, gpointer* pData)
{
	AnalyzedTracks* psTracks = pData[0];
	Preferences* psPreferences = pData[1];
	GuiData* psGuiData = pData[2];
	PlayerData* psPlayerData = pData[3];
	OpenGLData* psGLData = pData[4];

	GKeyFile** ppsFiles = NULL;

	on_Stop_Action_activate(psWidget, pData);
	ppsFiles = filesOpen();
	filesCloseAndSave(&ppsFiles, psPreferences, psTracks);

	analyzedTracksDestroy(&psTracks);
	preferencesDestroy(&psPreferences);
	guiDataDestroy(&psGuiData);
	playerDataDestroy(&psPlayerData);
	drawingGLFree(psGLData); free(psGLData); psGLData = NULL;

	gtk_main_quit();

	return EXIT_SUCCESS;
}


int on_Play_Action_activate (GtkWidget* psWidget, gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	AnalyzedTracks* psTracks = pData[0];
	Preferences* psPreferences = pData[1];
	GuiData* psGuiData = pData[2];
	PlayerData* psPlayerData = pData[3];
	OpenGLData* psGLData = pData[4];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	FMOD_CHANNEL* psPlayingChannel = NULL;
	FMOD_BOOL bIsPlaying = FALSE;


	AnalyzedTrack* psTrackToPlay = NULL;
	GtkBuilder* psMainBuilder = NULL;
	GtkTreeModel* psModel = NULL;
	GtkTreeRowReference* psRowRef = NULL;
	GtkTreePath* psPath = NULL;
	GtkTreeIter sIter;
	int iTrackIndex = -1;
	int iIncrementTimerID = 0;

/* ********************************************************************* */
/* Si un morceau est en lecture, on le met en pause et inversement       */
/* ********************************************************************* */

	psPlayingChannel = playerDataGetPlayingChannel(psPlayerData);
	bIsPlaying = playerIsPlaying(psPlayingChannel);
	if (bIsPlaying == TRUE)
	{
		FMOD_BOOL bIsPaused = FALSE;

		bIsPaused = playerIsPaused(psPlayingChannel);
		playerSetPaused(psPlayingChannel, !bIsPaused);
	}

/* ********************************************************************* */
/* Sinon, on regarde dans la liste de lecture.                           */
/* ********************************************************************* */

	else
	{
		psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);
		psModel = GTK_TREE_MODEL(
				gtk_builder_get_object(psMainBuilder, "liststore1"));

		if (gtk_tree_model_get_iter_first(psModel, &sIter) == FALSE) /* Si la liste est vide */
		{
			/* On prends un morceau aléatoirement */
			/** @todo Selection aléatoire d'un morceau **/
			printf("Aucun morceau trouvé dans la playlist.\n");
		}
		else
		{
			FMOD_SYSTEM* psFmodContext = NULL;
			int iGoToStar = -1;

			/* Sinon, on lit le morceau actif de la liste */
			psRowRef = guiDataGetTreeRowReference(psGuiData);
			psPath = gtk_tree_row_reference_get_path(psRowRef);
			assert(gtk_tree_model_get_iter(psModel, &sIter, psPath)
					== TRUE);
			gtk_tree_model_get(psModel, &sIter, 1, &iTrackIndex, -1);

			psTrackToPlay =
				analyzedTracksGetTrack(psTracks, iTrackIndex);

			psFmodContext = playerDataGetSystem(psPlayerData);
			iGoToStar = preferencesGetMoveCam(psPreferences);

			guiPlayTrack(psTrackToPlay, psMainBuilder, psFmodContext,
							&psPlayingChannel, iGoToStar, psGLData);
			playerDataSetPlayingChannel(psPlayerData, psPlayingChannel);

			iIncrementTimerID =
			g_timeout_add_seconds(1,
						(GSourceFunc) guiTrackScaleIncrement,
						pData);
			guiDataSetIncrementTimerID(psGuiData, iIncrementTimerID);
		}

	}

	return EXIT_SUCCESS;
}

int on_Stop_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	GuiData* psGuiData = pData[2];
	PlayerData* psPlayerData = pData[3];
	OpenGLData* psGLData = pData[4];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	FMOD_CHANNEL* psPlayingChannel = NULL;
	GtkBuilder* psMainBuilder = NULL;
	int iTimeOutID = 0;

/* ********************************************************************* */
/* Execution                                                             */
/* ********************************************************************* */

	psPlayingChannel = playerDataGetPlayingChannel(psPlayerData);

	if (playerIsPlaying(psPlayingChannel))
	{
		psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);
		iTimeOutID = guiDataGetIncrementTimerID(psGuiData);

		guiStopTrack(psMainBuilder,
					psPlayingChannel,
					psGuiData,
					psGLData,
					iTimeOutID);
	}

	return EXIT_SUCCESS;
}

int on_Pause_Action_activate (GtkWidget* psWidget, gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	PlayerData* psPlayerData = pData[3];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	FMOD_CHANNEL* psPlayingChannel = NULL;
	FMOD_BOOL bIsPaused;

/* ********************************************************************* */
/* ********************************************************************* */

	psPlayingChannel = playerDataGetPlayingChannel(psPlayerData);

	if (psPlayingChannel != NULL)
	{
		bIsPaused = playerIsPaused(psPlayingChannel);
		playerSetPaused(psPlayingChannel, !bIsPaused);
	}

	return EXIT_SUCCESS;
}

int on_AddTrack_Action_activate (GtkWidget* psWidget, gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	GuiData* psGuiData = pData[2];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GtkBuilder* psMainBuilder = NULL;
	GtkWidget* pDialog = NULL;
	GtkWidget* pParent = NULL;
	int iDialogAnswer = 0;

/* ********************************************************************* */
/* ********************************************************************* */

	psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);

	/* Affichage de la fenêtre de sélection de fichier. */
	pParent = GTK_WIDGET(gtk_builder_get_object(psMainBuilder,
										GUI_MAIN_WIN));
	pDialog = gtk_file_chooser_dialog_new ("Ajouter un morceau...",
				      GTK_WINDOW(pParent),
				      GTK_FILE_CHOOSER_ACTION_OPEN,
				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
				      NULL);
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(pDialog),
										"$HOME");
	gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(pDialog),
										TRUE);
	iDialogAnswer = gtk_dialog_run (GTK_DIALOG (pDialog));
	gtk_widget_hide_all (pDialog);


	/* Si l'utilisateur a ouvert un fichier */
	if (iDialogAnswer == GTK_RESPONSE_ACCEPT)
	{
		GSList* psListOfFiles = NULL;
		psListOfFiles = gtk_file_chooser_get_filenames(
										GTK_FILE_CHOOSER(pDialog));

		g_slist_foreach(psListOfFiles,
						(GFunc) analysisAddTracksToAnalyzed,
						pData);

		g_slist_free(psListOfFiles);
	}

	gtk_widget_destroy (pDialog);

	return EXIT_SUCCESS;
}

int on_AddDir_Action_activate (GtkWidget* psWidget, gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	GuiData* psGuiData = pData[2];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GtkBuilder* psMainBuilder = NULL;
	GtkWidget* pDialog;
	GtkWidget* pParent;
	int iDialogAnswer;

/* ********************************************************************* */
/* ********************************************************************* */

	psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);

	pParent = GTK_WIDGET(gtk_builder_get_object(psMainBuilder,
										GUI_MAIN_WIN));


/* ********************************************************************* */
/* Si on est sous Windows, le systeme de dossier n'est pas pris en       */
/* compte.                                                               */
/* ********************************************************************* */

#if defined(_WIN32)

	/* Affiche un message d'errreur. */
	pDialog = gtk_message_dialog_new(
									GTK_WINDOW(pParent),
									GTK_DIALOG_MODAL,
									GTK_MESSAGE_WARNING,
									GTK_BUTTONS_CLOSE,
					"Fonctionnalité non prise en charge sous Windows.\n\
Utilisez un systeme <u>UNIX</u> :p !");

/* ********************************************************************* */
/* Sinon, on ajoute tous les morceaux du dossier.                        */
/* ********************************************************************* */

#else

	/* Affichage de la fenêtre de sélection de fichier. */
	pDialog = gtk_file_chooser_dialog_new ("Ajouter un dossier...",
				      GTK_WINDOW(pParent),
				      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
				      NULL);
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(pDialog),
										"$HOME");
	iDialogAnswer = gtk_dialog_run (GTK_DIALOG (pDialog));
	gtk_widget_hide_all (pDialog);


	/* Si l'utilisateur a ouvert un dossier */
	if (iDialogAnswer == GTK_RESPONSE_ACCEPT)
	{
		char* strFolderName;
		int iPathAdded = 0;


		strFolderName = gtk_file_chooser_get_filename(
											GTK_FILE_CHOOSER (pDialog));
		/*iPathAdded = preferencesAddFilesPath((Preferences*)
											pData[PREFERENCES],
											strFolderName);*/

		if (iPathAdded == EXIT_SUCCESS)
		{
			guiAddFolderToAnalyze(strFolderName, pData);
		}
		else
		{
			printf ("Dossier déjà existant...\n");
		}

		g_free (strFolderName);
	}

#endif

	gtk_widget_destroy (pDialog);

	return EXIT_SUCCESS;
}

int on_Stellarium_Action_activate (GtkWidget* psWidget, gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	GuiData* psGuiData = pData[2];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GtkBuilder* psStellariumBuilder = NULL;
	GtkBuilder* psMainBuilder = NULL;
	GtkWidget* psStellarium = NULL;
	GtkWidget* psContainer = NULL;

	GdkGLConfig* pConfig = NULL;
	GtkWidget* pDrawingArea = NULL;

/* ********************************************************************* */
/* On détache le widget attaché.                                         */
/* ********************************************************************* */

	guiUnparent(psGuiData);

	psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);
	psStellariumBuilder = guiDataGetBuilder(psGuiData, STELLARIUM);

	/* Active la capacité OpenGL au Stellarium */
	pDrawingArea = GTK_WIDGET(gtk_builder_get_object(psStellariumBuilder,
										"Stellarium_DrawingArea"));

	pConfig = gdk_gl_config_new_by_mode(GDK_GL_MODE_RGBA |
										GDK_GL_MODE_DEPTH |
										GDK_GL_MODE_DOUBLE);

	gtk_widget_set_gl_capability(pDrawingArea,
									pConfig,
									NULL,
									TRUE,
									GDK_GL_RGBA_TYPE);

	psStellarium = GTK_WIDGET(gtk_builder_get_object(
											psStellariumBuilder,
											GUI_STELLARIUM_WIN));
	psContainer = GTK_WIDGET(
					gtk_builder_get_object(psMainBuilder,
										GUI_MAIN_CONTAINER));
	gtk_box_pack_start((GtkBox*)psContainer,
							psStellarium, TRUE, TRUE, 0);
	gtk_widget_show_all(psStellarium);

	return EXIT_SUCCESS;
}

int on_Preferences_Action_activate (GtkWidget* psWidget, gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	Preferences* psPreferences = pData[1];
	GuiData* psGuiData = pData[2];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GtkBuilder* psPreferencesBuilder = NULL;
	GtkBuilder* psMainBuilder = NULL;
	GtkWidget* psPrefWin = NULL;
	GtkWidget* psContainer = NULL;
	GtkWidget* psScale = NULL;
	GtkWidget* psCheckBut = NULL;
	int iCheck = 0;
	gboolean bCheck = FALSE;

/* ********************************************************************* */
/* ********************************************************************* */

	guiUnparent(psGuiData);

	psPreferencesBuilder = guiDataGetBuilder(psGuiData, PREFERENCE);
	psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);

	psPrefWin = GTK_WIDGET(gtk_builder_get_object(
												psPreferencesBuilder,
												GUI_PREFERENCES_WIN));
	psScale = GTK_WIDGET(gtk_builder_get_object(
										psPreferencesBuilder,
										"AnalysisRate_Scale"));
	gtk_range_set_value((GtkRange*)psScale,
					(double) preferencesGetAnalysisRate(
												psPreferences));

	psScale = GTK_WIDGET(gtk_builder_get_object(
										psPreferencesBuilder,
										"3DQuality_Scale"));
	gtk_range_set_value((GtkRange*)psScale,
					(double) preferencesGet3DQuality(
												psPreferences));

	psCheckBut = GTK_WIDGET(gtk_builder_get_object(
										psPreferencesBuilder,
										"MoveCamera_Check"));

	iCheck = preferencesGetMoveCam(psPreferences);
	bCheck = (gboolean) preferencesGetMoveCam(psPreferences);
	gtk_toggle_button_set_active(
					GTK_TOGGLE_BUTTON(psCheckBut),
					bCheck);

	psContainer = GTK_WIDGET(
						gtk_builder_get_object(psMainBuilder,
											GUI_MAIN_CONTAINER));
	gtk_box_pack_start((GtkBox*)psContainer,
							psPrefWin, TRUE, TRUE, 0);

	gtk_widget_show_all(psPrefWin);

	return EXIT_SUCCESS;
}

int on_Library_Action_activate (GtkWidget* psWidget, gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	AnalyzedTracks* psTracks = pData[0];
	GuiData* psGuiData = pData[2];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GtkBuilder* psLibraryBuilder = NULL;
	GtkBuilder* psMainBuilder = NULL;
	GtkWidget* psLibraryWin = NULL;
	GtkWidget* psContainer = NULL;

	GtkListStore* psListStore = NULL;
	GtkTreeIter sIter;

	AnalyzedTrack* psTrack = NULL;
	int iTID = 0;
	char* strPath = NULL;
	char* strFileName = NULL;
	float* pfCoord = NULL;

	int iNbTracks = 0;
	int i = 0;

/* ********************************************************************* */
/* ********************************************************************* */

	guiUnparent(psGuiData);

	psLibraryBuilder = guiDataGetBuilder(psGuiData, LIBRARY);
	psMainBuilder = guiDataGetBuilder (psGuiData, MAIN);

	psLibraryWin = GTK_WIDGET(gtk_builder_get_object(
										psLibraryBuilder,
										GUI_LIBRARY_WIN));
	psContainer = GTK_WIDGET(gtk_builder_get_object(
										psMainBuilder,
										GUI_MAIN_CONTAINER));

	psListStore = GTK_LIST_STORE(gtk_builder_get_object(psLibraryBuilder,
													"Library_ListStore"));

	if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(psListStore),
									&sIter) == FALSE)
	{
		iNbTracks = analyzedTracksGetNbTracks(psTracks);
		for (i = 0; i < iNbTracks; i++)
		{
			psTrack = analyzedTracksGetTrackInArray(psTracks, i);
			iTID = analyzedTrackGetTID(psTrack);
			strPath = analyzedTrackGetPath(psTrack);
			strFileName = strrchr(strPath, '/');
			pfCoord = analyzedTrackGetCoord(psTrack);

			gtk_list_store_append(GTK_LIST_STORE(psListStore), &sIter);
			gtk_list_store_set(GTK_LIST_STORE(psListStore), &sIter,
								0, iTID,
								1, &(strFileName[1]),
								2, pfCoord[0],
								3, pfCoord[1],
								4, pfCoord[2], -1);
		}
	}


	gtk_box_pack_start((GtkBox*)psContainer,
							psLibraryWin, TRUE, TRUE, 0);
	gtk_widget_show_all(psLibraryWin);

	return EXIT_SUCCESS;
}

int on_About_Action_activate (GtkWidget* psWidget, gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	GuiData* psGuiData = pData[2];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GtkBuilder* psMainBuilder = NULL;
	GtkBuilder* psAboutBuilder = NULL;
	GtkWidget* psAbout = NULL;
	GtkWidget* psContainer = NULL;

/* ********************************************************************* */
/* ********************************************************************* */

	guiUnparent(psGuiData);

	psAboutBuilder = guiDataGetBuilder(psGuiData, ABOUT);
	psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);

	psAbout = GTK_WIDGET(gtk_builder_get_object(
											psAboutBuilder,
											GUI_ABOUT_WIN));
	psContainer = GTK_WIDGET(
					gtk_builder_get_object(psMainBuilder,
										GUI_MAIN_CONTAINER));
	gtk_box_pack_start((GtkBox*)psContainer,
							psAbout, TRUE, TRUE, 0);
	gtk_widget_show_all(psAbout);

	return EXIT_SUCCESS;
}

int on_Next_Action_activate (GtkWidget* psWidget, gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	AnalyzedTracks* psTracks = pData[0];
	Preferences* psPreferences = pData[1];
	GuiData* psGuiData = pData[2];
	PlayerData* psPlayerData = pData[3];
	OpenGLData* psGLData = pData[4];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	FMOD_CHANNEL* psPlayingChannel = NULL;
	AnalyzedTrack* psTrackToPlay = NULL;
	GtkBuilder* psMainBuilder = NULL;
	GtkTreeModel* psModel = NULL;
	GtkTreeRowReference* psRowRef = NULL;
	GtkTreePath* psPath = NULL;

	GtkTreeIter sIter;
	int iTrackIndex = -1;

/* ********************************************************************* */
/* ********************************************************************* */

	psPlayingChannel = playerDataGetPlayingChannel(psPlayerData);
	psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);
	psModel = GTK_TREE_MODEL(
			gtk_builder_get_object(psMainBuilder, "liststore1"));

	/* Si la liste est vide */
	if (gtk_tree_model_get_iter_first(psModel, &sIter) == FALSE)
	{
		/* On affiche un message à l'utilisateur */
		/** @todo Affichage d'un msg. **/
		printf("La playlist est vide...\n");
	}
	else
	{
		FMOD_SYSTEM* psFmodContext = NULL;
		int iGoToStar = -1;
		int iIncrementTimerID = 0;

		/* Sinon, on lit le morceau suivant de la liste */
		psRowRef = guiDataGetTreeRowReference(psGuiData);
		psPath = gtk_tree_row_reference_get_path(psRowRef);
		gtk_tree_model_get_iter(psModel, &sIter, psPath);

		/* Si le morceau suivant est valide/existe */
		if (gtk_tree_model_iter_next(psModel, &sIter) == TRUE)
		{
			/* On met à jour le morceau actif */
			gtk_tree_path_next(psPath);
			psRowRef = gtk_tree_row_reference_new(psModel, psPath);
			guiDataSetTreeRowReference(psGuiData, psRowRef);
			gtk_tree_row_reference_free(psRowRef);

			/* On arrête la lecture en cours */
			on_Stop_Action_activate(psWidget, pData);

			gtk_tree_model_get(psModel, &sIter, 1, &iTrackIndex, -1);

			psTrackToPlay =
				analyzedTracksGetTrack(psTracks, iTrackIndex);

			psFmodContext = playerDataGetSystem(psPlayerData);
			iGoToStar = preferencesGetMoveCam(psPreferences);

			guiPlayTrack(psTrackToPlay, psMainBuilder, psFmodContext,
							&psPlayingChannel, iGoToStar, psGLData);
			playerDataSetPlayingChannel(psPlayerData, psPlayingChannel);

			iIncrementTimerID =
			g_timeout_add_seconds(1,
						(GSourceFunc) guiTrackScaleIncrement,
						pData);
			guiDataSetIncrementTimerID(psGuiData, iIncrementTimerID);
		}
		else /* Si le morceau suivant n'est pas valide (fin de la liste...)*/
		{
			printf("Fin de la playlist.\n");
		}

	}


	return EXIT_SUCCESS;
}

int on_Previous_Action_activate (GtkWidget* psWidget, gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	AnalyzedTracks* psTracks = pData[0];
	Preferences* psPreferences = pData[1];
	GuiData* psGuiData = pData[2];
	PlayerData* psPlayerData = pData[3];
	OpenGLData* psGLData = pData[4];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	FMOD_CHANNEL* psPlayingChannel = NULL;
	AnalyzedTrack* psTrackToPlay = NULL;
	GtkBuilder* psMainBuilder = NULL;
	GtkTreeModel* psModel = NULL;
	GtkTreeRowReference* psRowRef = NULL;
	GtkTreePath* psPath = NULL;

	GtkTreeIter sIter;
	int iTrackIndex = -1;

/* ********************************************************************* */
/* ********************************************************************* */

	psPlayingChannel = playerDataGetPlayingChannel(psPlayerData);
	psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);
	psModel = GTK_TREE_MODEL(
			gtk_builder_get_object(psMainBuilder, "liststore1"));

	/* Si la liste est vide */
	if (gtk_tree_model_get_iter_first(psModel, &sIter) == FALSE)
	{
		/* On affiche un message à l'utilisateur */
		/** @todo Affichage d'un msg. **/
		printf("La playlist est vide...\n");
	}
	else
	{
		FMOD_SYSTEM* psFmodContext = NULL;
		int iGoToStar = -1;
		int iIncrementTimerID = 0;

		/* Sinon, on lit le morceau suivant de la liste */
		psRowRef = guiDataGetTreeRowReference(psGuiData);
		psPath = gtk_tree_row_reference_get_path(psRowRef);

		/* Si le morceau précédent est valide/existe */
		if (gtk_tree_path_prev(psPath) == TRUE)
		{
			/* On met à jour le morceau actif */
			psRowRef = gtk_tree_row_reference_new(psModel, psPath);
			guiDataSetTreeRowReference(psGuiData, psRowRef);
			gtk_tree_row_reference_free(psRowRef);

			/* On arrête la lecture en cours */
			on_Stop_Action_activate(psWidget, pData);

			gtk_tree_model_get_iter(psModel, &sIter, psPath);
			gtk_tree_model_get(psModel, &sIter, 1, &iTrackIndex, -1);

			psTrackToPlay =
				analyzedTracksGetTrack(psTracks, iTrackIndex);

			psFmodContext = playerDataGetSystem(psPlayerData);
			iGoToStar = preferencesGetMoveCam(psPreferences);

			guiPlayTrack(psTrackToPlay, psMainBuilder, psFmodContext,
							&psPlayingChannel, iGoToStar, psGLData);
			playerDataSetPlayingChannel(psPlayerData, psPlayingChannel);

			iIncrementTimerID =
			g_timeout_add_seconds(1,
						(GSourceFunc) guiTrackScaleIncrement,
						pData);
			guiDataSetIncrementTimerID(psGuiData, iIncrementTimerID);
		}
		else /* Si le morceau suivant n'est pas valide (fin de la liste...)*/
		{
			printf("Debut de la playlist.\n");
		}

	}


	return EXIT_SUCCESS;
}

int on_Track_Scale_value_changed (GtkWidget* psWidget,
								GdkEventButton* pEvent,
								 gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	PlayerData* psPlayerData = pData[3];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	FMOD_CHANNEL* psPlayingChannel = NULL;

	double dNewTrackPosition = 0;

/* ********************************************************************* */
/* ********************************************************************* */

	psPlayingChannel = playerDataGetPlayingChannel(psPlayerData);

	if (psPlayingChannel != NULL)
	{
		dNewTrackPosition = gtk_range_get_value(GTK_RANGE(psWidget));
		FMOD_Channel_SetPosition(psPlayingChannel,
									dNewTrackPosition*1000,
									FMOD_TIMEUNIT_MS);
	}

	return EXIT_SUCCESS;
}

int on_PlayList_TreeView_row_activated (GtkTreeView* psTreeView,
										GtkTreePath* psSelectedTrackPath,
										GtkTreeViewColumn* psColumn,
										gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	GuiData* psGuiData = pData[2];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GtkTreeModel* psModel = NULL;
	GtkTreeRowReference* psRowRef = NULL;

/* ********************************************************************* */
/* ********************************************************************* */

	psModel = gtk_tree_view_get_model(psTreeView);

	/* On met a jour le chemin du morceau sélectionné. */
	psRowRef = gtk_tree_row_reference_new(psModel, psSelectedTrackPath);
	guiDataSetTreeRowReference(psGuiData, psRowRef);
	gtk_tree_row_reference_free(psRowRef);

	/* On arrête la lecture courante */
	on_Stop_Action_activate(NULL, pData);

	/* On lance la lecture, qui se chargera de lire le morceau */
	/* sélectionné                                             */
	on_Play_Action_activate(NULL, pData);

	return EXIT_SUCCESS;
}


int on_PlayList_TreeView_key_release_event (GtkWidget* psWidget,
											GdkEvent* psEvent,
											gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	GuiData* psGuiData = pData[2];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	int iStop = 0;
	GtkBuilder* psMainBuilder = NULL;
	GtkTreeView* psTreeView = NULL;
	GtkTreeModel* psModel = NULL;
	GtkTreePath* psPointedPath = NULL;
	GtkTreePath* psCurrentPath = NULL;
	GtkTreeRowReference* psRowRef = NULL;
	GtkTreeIter sIter;

/* ********************************************************************* */
/* ********************************************************************* */

	/* Si on appuie sur la touche 'suppr' */
	if (psEvent->key.keyval == 0xffff)
	{
		/* On récupère la ligne active de la playlist */
		psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);
		psModel = GTK_TREE_MODEL(gtk_builder_get_object(psMainBuilder,
													"liststore1"));
		psTreeView = GTK_TREE_VIEW(gtk_builder_get_object(psMainBuilder,
													"PlayList_TreeView"));
		gtk_tree_view_get_cursor(psTreeView, &psPointedPath, NULL);

		/* S'il y en a une */
		if (psPointedPath != NULL)
		{

			psRowRef = guiDataGetTreeRowReference(psGuiData);
			psCurrentPath = gtk_tree_row_reference_get_path(psRowRef);

			/* On regarde si c'est celle en lecture */
			if (gtk_tree_path_compare(psCurrentPath, psPointedPath) == 0)
			{ /* Si oui */
				iStop = 1;
			}

			/* On supprime la ligne */
			gtk_tree_model_get_iter(psModel, &sIter, psPointedPath);
			if (gtk_list_store_remove(GTK_LIST_STORE(psModel),
									&sIter) == FALSE)
			{
				gtk_tree_model_get_iter_first(psModel, &sIter);
			}

			/* Si la ligne supprimée est celle qui était en lecture */
			if (iStop == 1)
			{
				GtkTreePath* psNewPath = NULL;

				/* On stoppe la lecture */
				on_Stop_Action_activate(psWidget, pData);

				/* Et on réinitialise le morceau en lecture sur le
				morceau suivant */
				psNewPath = gtk_tree_model_get_path(psModel, &sIter);
				psRowRef = gtk_tree_row_reference_new(psModel, psNewPath);
				guiDataSetTreeRowReference(psGuiData, psRowRef);
				gtk_tree_path_free(psNewPath);
				gtk_tree_row_reference_free(psRowRef);

			}

			gtk_tree_path_free(psCurrentPath);
		}

		gtk_tree_path_free(psPointedPath);

	}

	return EXIT_SUCCESS;
}


/* ********************************************************************* */
/*                            FONCTIONS TIMEOUT                          */
/* ********************************************************************* */


gboolean guiTimeoutAnalyze (gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	Preferences* psPreferences = pData[1];
	GuiData* psGuiData = pData[2];
	PlayerData* psPlayerData = pData[3];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	FMOD_CHANNEL* psAnalyzingChannel = NULL;
	gboolean bIsPlaying = FALSE;
	LinkedList* psAnalyzingList = NULL;
	LinkedList* psFirstCell = NULL;
	AnalyzedTrack* psTrack = NULL;

/* ********************************************************************* */
/* ********************************************************************* */

	/* On cherche à savoir si l'analyse est finie ou non */
	psAnalyzingChannel = playerDataGetAnalyzingChannel(psPlayerData);
	bIsPlaying = playerIsPlaying(psAnalyzingChannel);


	psAnalyzingList = playerDataGetAnalyzingList(psPlayerData);
	/* On récupère le morceau en cours d'analyse */
	assert (linkedListGetFirst(psAnalyzingList,
									&psFirstCell) == LIST_OK);
	psTrack = (AnalyzedTrack*) psFirstCell->pData;

	/* Si l'analyse est toujours en cours */
	if (bIsPlaying == TRUE)
	{
		float fAnalysisRate = 0;
		int iAnalyzingCounter = 0;

		/* On récupère le taux d'analyse et le nb de boucles d'analyse */

		fAnalysisRate = (float) preferencesGetAnalysisRate(psPreferences);
		iAnalyzingCounter = playerDataGetAnalyzingLoopCounter(psPlayerData);

		/* Au maximum (100%) on analysera toutes les 25ms le morceau en
		vitesse normale.
		Au quart (25%) on analysera toutes les 25ms le morceau en vitesse
		x4. */

		fAnalysisRate = 25/(fAnalysisRate/100.0);

		analysisAnalyze (psAnalyzingChannel,
						fAnalysisRate,
						psTrack,
						iAnalyzingCounter);

		iAnalyzingCounter++;
		playerDataSetAnalyzingLoopCounter(psPlayerData, iAnalyzingCounter);

	}
	else /* Si l'analyse est terminée */
	{
		const char* strConstFileName = NULL;
		char* strFileName = NULL;
		char* strStatusBarMessage = NULL;

		GtkBuilder* psMainBuilder = NULL;
		GtkWidget* psStatusBar = NULL;

/* ********************************************************************* */
/* ********************************************************************* */

		/* On change le status du morceau (analysé) */
		analyzedTrackSetAnalyzed(psTrack, 1);

/* ********************************************************************* */
/* ********************************************************************* */

		/* On le supprime de la liste d'analyse */
		linkedListRemoveFirst(psAnalyzingList, NULL);

/* ********************************************************************* */
/* ********************************************************************* */

		/* On raffraichit la barre d'état */

		strConstFileName = analyzedTrackGetPath(psTrack);
		strFileName = strrchr(strConstFileName, '/');

		strStatusBarMessage = (char*) malloc((
									strlen("Le fichier '")+
									strlen(&(strFileName[1]))+
									strlen("' vient d'être analysé.")+1)
											*sizeof(char));
		sprintf(strStatusBarMessage,
				"Le fichier '%s' vient d'être analysé.", &(strFileName[1]));

		psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);
		psStatusBar = GTK_WIDGET(gtk_builder_get_object(
									psMainBuilder,
									"Stellody_StatusBar"));

		gtk_statusbar_pop(GTK_STATUSBAR(psStatusBar), 1);
		gtk_statusbar_push(GTK_STATUSBAR(psStatusBar), 1,
						 strStatusBarMessage);
		free(strStatusBarMessage);
		strStatusBarMessage = NULL;

	}

	return bIsPlaying;
}



gboolean guiTimeoutCheckForAnalyze (gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	GuiData* psGuiData = pData[2];
	PlayerData* psPlayerData = pData[3];


/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	LinkedList* psAnalyzeList = NULL;
	int iListLength = 0;

	FMOD_CHANNEL* psAnalyzingChannel = NULL;

/* ********************************************************************* */
/* ********************************************************************* */

	psAnalyzeList = playerDataGetAnalyzingList(psPlayerData);
	linkedListGetLength(psAnalyzeList, &iListLength);
	psAnalyzingChannel = playerDataGetAnalyzingChannel(psPlayerData);

/* Si la liste d'analyse est vide et aucun morceau en train d'être analysé */
	if (iListLength == 0 &&
		playerIsPlaying(psAnalyzingChannel) == FALSE)
	{
		GtkBuilder* psMainBuilder = NULL;
		GtkWidget* psStatusBar = NULL;

		/* On efface le message de la barre d'état */
		psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);
		psStatusBar = GTK_WIDGET(gtk_builder_get_object(
									psMainBuilder,
									"Stellody_StatusBar"));
		gtk_statusbar_pop(GTK_STATUSBAR(psStatusBar), 1);

		/* C'est fonction n'a plus lieu d'être appelée */

		playerDataSetCheckForAnalyze(psPlayerData, 0);

		return FALSE;
	}
	else
	{

		/* Si le canal d'analyse est déja en train d'analysé, */
		if (playerIsPlaying(psAnalyzingChannel) == TRUE)
		{
			/* on attend */
			return TRUE;
		}
		else /* Sinon, on lance une nouvelle analyse */
		{
			LinkedList* psFirstCell = NULL;
			FMOD_SYSTEM* psFmodContext = NULL;
			unsigned int uiTrackLength = 0;
			char* strSinger = NULL; /* Not Use */
			char* strTitle = NULL; /* Not Use */
			char* strConstFileName = NULL;
			char* strFileName = NULL;
			char* strStatusBarMessage = NULL;
			AnalyzedTrack* psTrack = NULL;
			GtkBuilder* psMainBuilder = NULL;
			GtkWidget* psStatusBar = NULL;

/* ********************************************************************* */
/* ********************************************************************* */

			/* On charge le morceau dans le canal d'analyse */
			assert (linkedListGetFirst(psAnalyzeList, &psFirstCell) ==
						LIST_OK);
			psTrack = psFirstCell->pData;

			psFmodContext = playerDataGetSystem(psPlayerData);

			playerPlayTrack(analyzedTrackGetPath(psTrack),
							psFmodContext,
							&psAnalyzingChannel,
							0,
							0,
							&uiTrackLength,
							&strSinger,
							&strTitle);
			playerDataSetAnalyzingChannel(psPlayerData,
										psAnalyzingChannel);
			/* On coupe le son, le morceau doit être joué mais pas entendu */
			playerSetVolume(psAnalyzingChannel,
							0.0);

/* ********************************************************************* */
/* ********************************************************************* */

			/* On indique le début de l'analyse */

			strConstFileName = analyzedTrackGetPath(psTrack);
			strFileName = strrchr(strConstFileName, '/');

			strStatusBarMessage = (char*) malloc((
									strlen("Analyse de '")+
									strlen(&(strFileName[1]))+
									strlen("' en cours...")+1)
											*sizeof(char));
			sprintf(strStatusBarMessage,
				"Analyse de '%s' en cours...", &(strFileName[1]));

			psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);
			psStatusBar = GTK_WIDGET(gtk_builder_get_object(
									psMainBuilder,
									"Stellody_StatusBar"));
			gtk_statusbar_pop(GTK_STATUSBAR(psStatusBar), 1);
			gtk_statusbar_push(GTK_STATUSBAR(psStatusBar), 1,
								strStatusBarMessage);
			free(strStatusBarMessage);
			strStatusBarMessage = NULL;

/* ********************************************************************* */
/* ********************************************************************* */

			/* On sauvegarde la durée du morceau */
			analyzedTrackSetLength(psTrack, uiTrackLength);

			/* On crée un time_out sur la fonction qui analysera et stockera
			les données d'analyse */

			g_idle_add((GSourceFunc) guiTimeoutAnalyze,
						pData);

/* ********************************************************************* */
/* ********************************************************************* */

			return TRUE;
		}
	}

	return TRUE;
}




/* ********************************************************************* */
/*                           FENÊTRE PREFERENCES                         */
/* ********************************************************************* */



int on_PrefOKBut_Action_activate (GtkWidget* psWidget, gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	Preferences* psPreferences = pData[1];
	GuiData* psGuiData = pData[2];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GtkBuilder* psPreferencesBuilder = NULL;
	GtkWidget* psScale = NULL;
	GtkWidget* psCheckBut = NULL;

/* ********************************************************************* */
/* ********************************************************************* */

	psPreferencesBuilder = guiDataGetBuilder(psGuiData, PREFERENCE);

	psScale = GTK_WIDGET(gtk_builder_get_object(
										psPreferencesBuilder,
										"AnalysisRate_Scale"));
	preferencesSetAnalysisRate(psPreferences,
						(int) gtk_range_get_value((GtkRange*)psScale));

	psScale = GTK_WIDGET(gtk_builder_get_object(
										psPreferencesBuilder,
										"3DQuality_Scale"));
	preferencesSet3DQuality(psPreferences,
						(int) gtk_range_get_value((GtkRange*)psScale));

	psCheckBut = GTK_WIDGET(gtk_builder_get_object(
										psPreferencesBuilder,
										"MoveCamera_Check"));
	preferencesSetMoveCam(psPreferences,
						(int) gtk_toggle_button_get_active(
										GTK_TOGGLE_BUTTON(psCheckBut)));

	on_Stellarium_Action_activate(psWidget, pData);

	return EXIT_SUCCESS;
}

int on_FlushData_Action_activate (GtkWidget* psWidget, gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	AnalyzedTracks* psTracks = pData[0];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GtkWidget* pDialog = NULL;

/* ********************************************************************* */
/* ********************************************************************* */

	analyzedTracksRelease(psTracks);

	pDialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
									GTK_MESSAGE_WARNING,
									GTK_BUTTONS_CLOSE,
	"Stellody va se fermer pour prendre en compte ce changement...");

	gtk_dialog_run(GTK_DIALOG(pDialog));
	gtk_widget_hide_all(pDialog);

	on_Quit_Action_activate(NULL, pData);

	return EXIT_SUCCESS;
}



/* ********************************************************************* */
/*                           FENÊTRE STELLARIUM                          */
/* ********************************************************************* */



int on_Stellarium_DrawingArea_realize(
								GtkWidget* psWidget,
								gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	OpenGLData* psGLData = pData[4];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GdkGLContext * psContext = NULL;
	GdkGLDrawable * psSurface = NULL;
	gboolean bActivate = FALSE;

/* ********************************************************************* */
/* ********************************************************************* */

	psContext = gtk_widget_get_gl_context(psWidget);
	psSurface = gtk_widget_get_gl_drawable(psWidget);

	bActivate = gdk_gl_drawable_gl_begin(psSurface,psContext);
	if (bActivate == TRUE)
	{
		drawingGLInit(psGLData);

		gdk_gl_drawable_gl_end(psSurface); /* désactivation du contexte */

		g_timeout_add(40,
					(GSourceFunc) gtk_widget_queue_draw,
					psWidget);
	}

	return EXIT_SUCCESS;
}



int on_Stellarium_DrawingArea_configure_event(
								GtkWidget* psWidget,
								GdkEventConfigure* psEvent,
								gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	OpenGLData* psGLData = pData[4];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GdkGLContext * psContext = NULL;
	GdkGLDrawable * psSurface = NULL;
	gboolean bActivate = FALSE;

/* ********************************************************************* */
/* ********************************************************************* */

	psContext = gtk_widget_get_gl_context(psWidget) ;
	psSurface = gtk_widget_get_gl_drawable(psWidget) ;

	bActivate = gdk_gl_drawable_gl_begin(psSurface,psContext);
	if (bActivate == TRUE)
	{
		drawingGLResize(psGLData, psEvent->width, psEvent->height);
		gdk_gl_drawable_gl_end(psSurface); /* désactivation du contexte */
	}
	return EXIT_SUCCESS;
}


int on_Stellarium_DrawingArea_expose_event (
							GtkWidget* psWidget,
							GdkEventExpose* psEvent,
							gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	AnalyzedTracks* psTracks = pData[0];
	Preferences* psPreferences = pData[1];
	OpenGLData* psGLData = pData[4];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GdkGLContext * psContext = NULL;
	GdkGLDrawable * psSurface = NULL;
	gboolean bActivate = FALSE;

/* ********************************************************************* */
/* ********************************************************************* */

	psContext = gtk_widget_get_gl_context(psWidget);
	psSurface = gtk_widget_get_gl_drawable(psWidget);

	bActivate = gdk_gl_drawable_gl_begin(psSurface,psContext);

	if (bActivate == TRUE)
	{
		drawingGLDraw(psGLData, psTracks,
					  preferencesGet3DQuality(
								psPreferences));
		gdk_gl_drawable_swap_buffers(psSurface); /* permutation des tampons */
		gdk_gl_drawable_gl_end(psSurface); /* désactivation du contexte */
	}

	return EXIT_SUCCESS;
}


int on_Stellarium_DrawingArea_button_release_event (
								GtkWidget * psWidget,
								GdkEventAny * psEvent,
								gpointer * pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	AnalyzedTracks* psTracks = pData[0];
	GuiData* psGuiData = pData[2];
	OpenGLData* psGLData = pData[4];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GtkAllocation sAllocation;
	int iXMousePosition = 0;
	int iYMousePosition = 0;

/* ********************************************************************* */
/* ********************************************************************* */

	guiDataGetMousePosition(psGuiData, &iXMousePosition, &iYMousePosition);

	switch (psEvent->type)
	{
		case GDK_SCROLL: /* Si on scroll avec la molette */
			switch (((GdkEventScroll*)psEvent)->direction)
			{
				case GDK_SCROLL_UP:
					drawingGLZoom(psGLData,
								  (float) iXMousePosition,
								  (float) iYMousePosition,
								  0.1);
					break;
				case GDK_SCROLL_DOWN:
					gtk_widget_get_allocation(psWidget,
												&sAllocation);
					drawingGLZoom(psGLData,
								  sAllocation.width - (float) iXMousePosition,
								  sAllocation.height - (float) iYMousePosition,
								  -0.1);
					break;
				default:
					break;
			}
			break;
		case GDK_BUTTON_RELEASE: /* Si on relache un bouton */
			if (((GdkEventButton*)psEvent)->button == 1)
			{
				int iTID = 0;
				GdkGLContext * psContext = NULL;
				GdkGLDrawable * psSurface = NULL;
				gboolean bActivate = FALSE;
				AnalyzedTrack* pTrack = NULL;

				psContext = gtk_widget_get_gl_context(psWidget);
				psSurface = gtk_widget_get_gl_drawable(psWidget);

				bActivate = gdk_gl_drawable_gl_begin(psSurface,psContext);

				if (bActivate == TRUE)
				{
					iTID = drawingGLSelect(
						psGLData,
						psTracks,
						((GdkEventButton*)psEvent)->x,
						((GdkEventButton*)psEvent)->y);

					gdk_gl_drawable_swap_buffers(psSurface); /* permutation des tampons */
					gdk_gl_drawable_gl_end(psSurface); /* désactivation du contexte */
				}

				if (iTID >= 0)
				{
					GtkBuilder* psMainBuilder = NULL;


					pTrack = analyzedTracksGetTrack(psTracks, iTID);
					psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);

					guiAddTrackToPlaylist(psMainBuilder,
											pTrack,
											pData);
				}
			}
			break;
		default:
			break;
	}

	return EXIT_SUCCESS;
}


int on_Stellarium_DrawingArea_motion_notify_event (GtkWidget* psWidget,
												GdkEventMotion* psEvent,
												gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	AnalyzedTracks* psTracks = pData[0];
	GuiData* psGuiData = pData[2];
	OpenGLData* psGLData = pData[4];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	float fStepX = 0;
	float fStepY = 0;
	int iXMousePosition = 0;
	int iYMousePosition = 0;

/* ********************************************************************* */
/* ********************************************************************* */

	guiDataGetMousePosition(psGuiData, &iXMousePosition, &iYMousePosition);

	if (((psEvent->state)>>9)%2 == 1 || ((psEvent->state)>>10)%2 == 1)
	/* Si le déplacement est fait avec le boutton central ou droit appuyé */
	{
		fStepX = (float) iXMousePosition - (float) psEvent->x;
		fStepY = (float) iYMousePosition - (float) psEvent->y;

		drawingGLRotate(psGLData, -fStepX/20, fStepY/20);
	}
	else if (((psEvent->state)>>8)%2 == 1)
	/* Si c'est avec le bouton gauche */
	{
		fStepX = (float) iXMousePosition - (float) psEvent->x;
		fStepY = (float) iYMousePosition - (float) psEvent->y;

		drawingGLTranslate(psGLData, -fStepX/500, fStepY/500, 0);
	}
	else /* Si on appuie pas sur un bouton de la souris */
	{
		int iTID = -1;
		GdkGLContext * psContext = NULL;
		GdkGLDrawable * psSurface = NULL;
		gboolean bActivate = FALSE;
		GtkBuilder* psStellariumBuilder = NULL;
		GtkWidget* psStellariumLabel = NULL;

		psContext = gtk_widget_get_gl_context(psWidget);
		psSurface = gtk_widget_get_gl_drawable(psWidget);

		bActivate = gdk_gl_drawable_gl_begin(psSurface,psContext);

		if (bActivate == TRUE)
		{
			iTID = drawingGLSelect(
				psGLData,
				psTracks,
				((GdkEventButton*)psEvent)->x,
				((GdkEventButton*)psEvent)->y);

			gdk_gl_drawable_swap_buffers(psSurface); /* permutation des tampons */
			gdk_gl_drawable_gl_end(psSurface); /* désactivation du contexte */
		}

		psStellariumBuilder = guiDataGetBuilder(psGuiData,
												STELLARIUM);
		psStellariumLabel = GTK_WIDGET(gtk_builder_get_object(
												psStellariumBuilder,
												"Stellarium_Label"));

		if (iTID >= 0)
		{
			AnalyzedTrack* pTrack = NULL;
			char* strPath = NULL;
			char* strFileName = NULL;

			pTrack = analyzedTracksGetTrack(psTracks, iTID);
			strPath = analyzedTrackGetPath(pTrack);
			strFileName = strrchr(strPath, '/');

			gtk_label_set(GTK_LABEL(psStellariumLabel),
							&(strFileName[1]));

		}
		else
		{
			gtk_label_set(GTK_LABEL(psStellariumLabel),
							"");
		}
	}

	guiDataSetMousePosition(psGuiData, psEvent->x, psEvent->y);

	return EXIT_SUCCESS;
}


/* ********************************************************************* */
/*                           FENETRE LIBRAIRIE                           */
/* ********************************************************************* */


int on_Library_TreeView_row_activated (GtkTreeView* psTreeView,
										GtkTreePath* psSelectedTrackPath,
										GtkTreeViewColumn* psColumn,
										gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	AnalyzedTracks* psTracks = pData[0];
	GuiData* psGuiData = pData[2];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GtkTreeModel* psModel = NULL;
	GtkTreeIter sIter;

	int iTID = 0;
	AnalyzedTrack* psTrack = NULL;
	GtkBuilder* psMainBuilder = NULL;

/* ********************************************************************* */
/* ********************************************************************* */

	psModel = gtk_tree_view_get_model(psTreeView);

	gtk_tree_model_get_iter(psModel, &sIter, psSelectedTrackPath);
	gtk_tree_model_get(psModel, &sIter, 0, &iTID, -1);

	psTrack = analyzedTracksGetTrack(psTracks, iTID);

	psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);
	guiAddTrackToPlaylist(psMainBuilder, psTrack, pData);


	return EXIT_SUCCESS;
}


int on_Library_Columns_clicked (GtkTreeViewColumn* psColumn,
								gpointer* pData)
{

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GtkTreeViewColumn* psTempColumn = NULL;
	GtkTreeView* psTreeView = NULL;
	GtkListStore* psListStore = NULL;
	GtkSortType iSortOrder;

	const gchar* strTitle = NULL;
	int i = 0;

/* ********************************************************************* */
/* ********************************************************************* */

	psTreeView = GTK_TREE_VIEW(
						gtk_tree_view_column_get_tree_view(psColumn));
	psListStore = GTK_LIST_STORE(gtk_tree_view_get_model(psTreeView));

	for (i = 0; i < 5; i++)
	{
		psTempColumn = gtk_tree_view_get_column(psTreeView, i);
		gtk_tree_view_column_set_sort_indicator(psTempColumn, FALSE);
	}

	strTitle = gtk_tree_view_column_get_title(psColumn);
	iSortOrder = gtk_tree_view_column_get_sort_order(psColumn);
	gtk_tree_view_column_set_sort_indicator(psColumn, TRUE);

	if (strcmp(strTitle, "ID") == 0)
	{
		gtk_tree_view_column_set_sort_order(psColumn, !iSortOrder);
		gtk_tree_sortable_set_sort_column_id(
									GTK_TREE_SORTABLE(psListStore),
									0,
									!iSortOrder);
	}
	else if (strcmp(strTitle, "Fichier") == 0)
	{
		gtk_tree_view_column_set_sort_order(psColumn, !iSortOrder);
		gtk_tree_sortable_set_sort_column_id(
									GTK_TREE_SORTABLE(psListStore),
									1,
									!iSortOrder);
	}
	else if (strcmp(strTitle, "CoordX") == 0)
	{
		gtk_tree_view_column_set_sort_order(psColumn, !iSortOrder);
		gtk_tree_sortable_set_sort_column_id(
									GTK_TREE_SORTABLE(psListStore),
									2,
									!iSortOrder);
	}
	else if (strcmp(strTitle, "CoordY") == 0)
	{
		gtk_tree_view_column_set_sort_order(psColumn, !iSortOrder);
		gtk_tree_sortable_set_sort_column_id(
									GTK_TREE_SORTABLE(psListStore),
									3,
									!iSortOrder);
	}
	else if (strcmp(strTitle, "CoordZ") == 0)
	{
		gtk_tree_view_column_set_sort_order(psColumn, !iSortOrder);
		gtk_tree_sortable_set_sort_column_id(
									GTK_TREE_SORTABLE(psListStore),
									4,
									!iSortOrder);
	}
	else
	{
		/* On ne fait rien */
	}

	return EXIT_SUCCESS;
}


int on_Library_TreeView_key_release_event (GtkWidget* psWidget,
											GdkEvent* psEvent,
											gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	AnalyzedTracks* psTracks = pData[0];
	GuiData* psGuiData = pData[2];
	OpenGLData* psGLData = pData[4];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

	GtkBuilder* psLibraryBuilder = NULL;
	GtkBuilder* psMainBuilder = NULL;
	GtkTreeView* psTreeView = NULL;
	GtkTreeModel* psModel = NULL;
	GtkTreeView* psPlayListTreeView = NULL;
	GtkTreeModel* psPlayListModel = NULL;
	GtkTreePath* psPointedPath = NULL;
	GtkTreeIter sIter;
	GtkTreeIter sPlayListIter;

	GtkTreePath* psPlayListPath = NULL;
	int iTIDInPlayList = -1;

	int iTID = 0;

/* ********************************************************************* */
/* ********************************************************************* */

	/* Si on appuie sur la touche 'suppr' */
	if (psEvent->key.keyval == 0xffff)
	{
		/* On récupère la ligne active de la bibliothèque */
		psLibraryBuilder = guiDataGetBuilder(psGuiData, LIBRARY);
		psModel = GTK_TREE_MODEL(gtk_builder_get_object(psLibraryBuilder,
													"Library_ListStore"));
		psTreeView = GTK_TREE_VIEW(gtk_builder_get_object(psLibraryBuilder,
													"Library_TreeView"));
		gtk_tree_view_get_cursor(psTreeView, &psPointedPath, NULL);

		/* S'il y en a une */
		if (psPointedPath != NULL)
		{
			/* On récupère le TID du morceau sélectionné */
			gtk_tree_model_get_iter(psModel, &sIter, psPointedPath);
			gtk_tree_model_get(psModel, &sIter, 0, &iTID, -1);


			/* Il va falloir supprimer les morceau de la playlist qui
			font références à ce morceau */
			psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);
			psPlayListModel = GTK_TREE_MODEL(gtk_builder_get_object(
														psMainBuilder,
														"liststore1"));
			psPlayListTreeView = GTK_TREE_VIEW(gtk_builder_get_object(
														psMainBuilder,
													"PlayList_TreeView"));
			if(gtk_tree_model_get_iter_first(psPlayListModel,
										&sPlayListIter) == TRUE)
			{
				/* Tant qu'on est pas à la fin de la playlist */
				do
				{
					gtk_tree_model_get(psPlayListModel, &sPlayListIter,
										1, &iTIDInPlayList, -1);
					/* Si le TID du morceau de la playlist est le même */
					if (iTID == iTIDInPlayList)
					{
						psPlayListPath = gtk_tree_model_get_path(
													psPlayListModel,
													&sPlayListIter);
						/* On active cette ligne */
						gtk_tree_view_set_cursor(psPlayListTreeView,
													psPlayListPath,
													NULL, FALSE);
						/* Et la supprimons */
						on_PlayList_TreeView_key_release_event(psWidget,
																psEvent,
																pData);
					}
				} while (gtk_tree_model_iter_next(psPlayListModel,
											&sPlayListIter) == TRUE);

			}

			/* On supprime la ligne */
			gtk_list_store_remove(GTK_LIST_STORE(psModel), &sIter);

			/* le morceau */
			analyzedTracksRemoveTrack(psTracks, iTID);

			/* Et l'étoile */
			drawingGLRemoveStar(psGLData, iTID);
		}
	}

	return EXIT_SUCCESS;
}



































