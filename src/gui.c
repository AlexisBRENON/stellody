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

#define GUI_MAIN_WIN "Stellody_Window" /**< Nom du widget de niveau
supérieur. */
#define GUI_MAIN_CONTAINER "Variable_Box" /**< Nom du widget contenant les
différentes fenêtres (variables). */
#define GUI_STELLARIUM_WIN "Stellarium_VBox" /**< Nom du conteneur du
stellarium. */
#define GUI_PREFERENCES_WIN "Preferences_VBox" /**< Nom du conteneur des
preferences. */
#define GUI_ABOUT_WIN "About_VBox" /**< Nom du conteneur de A Propos; */
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

static gboolean guiTrackScaleIncrement (GtkBuilder* psMainBuilder,
										FMOD_CHANNEL* psPlayingChannel,
										gpointer* pData);

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


/* ********************************************************************* */
/* Implémentation                                                        */
/* ********************************************************************* */

static int guiUnparent (GuiData* psData)
{
	GtkBuilder* pMainBuilder = NULL;
	GtkBuilder* pPreferencesBuilder = NULL;
	GtkBuilder* pAboutBuilder = NULL;
	GtkBuilder* pStellariumBuilder = NULL;

	GtkWidget* psStellarium = NULL;
	GtkWidget* psPreference = NULL;
	GtkWidget* psAbout = NULL;
	GtkWidget* psContainer = NULL;


	pMainBuilder = guiDataGetBuilder(psData, MAIN);
	pPreferencesBuilder = guiDataGetBuilder(psData, PREFERENCE);
	pAboutBuilder = guiDataGetBuilder(psData, ABOUT);
	pStellariumBuilder = guiDataGetBuilder(psData, STELLARIUM);


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

	gtk_widget_hide_all(psPreference);
	gtk_widget_hide_all(psAbout);
	gtk_widget_hide_all(psStellarium);


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

	return EXIT_SUCCESS;
}


static gboolean guiTrackScaleIncrement (GtkBuilder* psMainBuilder,
										FMOD_CHANNEL* psPlayingChannel,
										gpointer* pData)
{
	GtkObject* psAdjustment = NULL;

	assert (psMainBuilder != NULL);
	assert (pData != NULL);

	psAdjustment = GTK_OBJECT(gtk_builder_get_object(psMainBuilder,
										"Track_Adjustment"));

	if (playerIsPlaying(psPlayingChannel) == 0)	/* Si la lecture est stoppée, on réinitialise la barre de progression */
	{
		gtk_adjustment_set_value(GTK_ADJUSTMENT(psAdjustment), 0);
		gtk_adjustment_set_upper(GTK_ADJUSTMENT(psAdjustment), 100);

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

	GtkCellRenderer* psCellRender = NULL;
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

	if (gtk_tree_model_get_iter_first(psListStore, &Iter) == FALSE)
	{
		on_Stop_Action_activate(NULL, pData);
		guiPlayTrack(pTrack,
					psMainBuilder,
					psFmodContext,
					&psPlayingChannel,
					iMoveCam,
					psGLData);

		iIncrementTimerID =
			g_timeout_add_seconds(1,
						(GSourceFunc) guiTrackScaleIncrement,
						pData);
		guiDataSetIncrementTimerID(psGuiData, iIncrementTimerID);
	}

	strTrackPath = analyzedTrackGetPath(pTrack);
	strTrackName = strrchr(strTrackPath, '/');
	iTID = analyzedTrackGetTID(pTrack);

	psTreeView = (GtkTreeView*) gtk_builder_get_object(psMainBuilder,
														"PlayList_TreeView");

	psCellRender = gtk_cell_renderer_text_new ();


	gtk_list_store_append(GTK_LIST_STORE(psListStore), &Iter);
	gtk_list_store_set(GTK_LIST_STORE(psListStore), &Iter,
						0, &(strTrackName[1]), 1, iTID, -1);



	gtk_widget_show_all(GTK_WIDGET(psTreeView));

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
	GtkBuilder* psLoadingBuilder = NULL;
	GtkWidget* psLoadingWindow = NULL;
	GtkWidget* psLoadingLabel = NULL;

	GKeyFile** ppsFiles = NULL;

	GtkBuilder* pMainBuilder = NULL;
	GtkWidget* pMainWindow = NULL;

/* ********************************************************************* */
/* Ouverture de la fenêtre de chargement...                              */
/* ********************************************************************* */

	psLoadingBuilder = gtk_builder_new();
	gtk_builder_add_from_file(psLoadingBuilder,
							"data/windows/Loading.glade",
							NULL);

	psLoadingWindow = GTK_WIDGET(
					gtk_builder_get_object(
										psLoadingBuilder,
										"Loading_Window"));
	psLoadingLabel = GTK_WIDGET(
					gtk_builder_get_object(
										psLoadingBuilder,
										"Loading_Label"));

	gtk_widget_show_all(psLoadingWindow);


/* ********************************************************************* */
/*                                                                       */
/*                             CHARGEMENTS                               */
/*                                                                       */
/* ********************************************************************* */


/* ********************************************************************* */
/* Ouverture des fichiers...                                             */
/* ********************************************************************* */

	gtk_label_set_text(GTK_LABEL(psLoadingLabel),
						"Ouverture des fichiers...");

	ppsFiles = filesOpen();
	g_usleep(100000);

/* ********************************************************************* */
/* Chargements des morceaux...                                           */
/* ********************************************************************* */

	gtk_label_set_text(GTK_LABEL(psLoadingLabel),
						"Chargement des morceaux...");

	ppDatas[0] = (void*) analyzedTracksCreateFromFile(ppsFiles);
	g_usleep(100000);

/* ********************************************************************* */
/* Chargements des préférences...                                        */
/* ********************************************************************* */

	gtk_label_set_text(GTK_LABEL(psLoadingLabel),
						"Chargement des préférences...");

	ppDatas[1] = (void*) preferencesCreateFromFile(ppsFiles);
	g_usleep(100000);

/* ********************************************************************* */
/* Initialisation de l'interface...                                      */
/* ********************************************************************* */

	gtk_label_set_text(GTK_LABEL(psLoadingLabel),
						"Initialisation de l'interface...");

	ppDatas[2] = (void*) guiDataCreateWithData(GUI_WIN,
										GUI_STELLARIUM,
										GUI_PREFERENCES,
										GUI_ABOUT);

	guiDataConnectSignals((GuiData*) ppDatas[2],
						ppDatas);
	g_usleep(100000);

/* ********************************************************************* */
/* Initialisation du lecteur...                                          */
/* ********************************************************************* */

	gtk_label_set_text(GTK_LABEL(psLoadingLabel),
						"Initialisation du lecteur...");

	ppDatas[3] = (void*) playerDataCreate();
	g_usleep(1000000);

/* ********************************************************************* */
/* Initialisation de l'affichage 3D...                                   */
/* ********************************************************************* */

	gtk_label_set_text(GTK_LABEL(psLoadingLabel),
						"Initialisation de l'affichage 3D...");

	ppDatas[4] = malloc(sizeof(OpenGLData));
	drawingGLInit((OpenGLData*) ppDatas[4]);

/* ********************************************************************* */
/* Ouverture de la fenêtre...                                            */
/* ********************************************************************* */

	pMainBuilder = guiDataGetBuilder((GuiData*) ppDatas[2], MAIN);
	pMainWindow = GTK_WIDGET(
				gtk_builder_get_object(pMainBuilder, GUI_MAIN_WIN));

	gtk_widget_show_all(pMainWindow);

	gtk_widget_destroy(psLoadingWindow);
	g_object_unref(G_OBJECT(psLoadingBuilder));

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

	on_Stop_Action_activate(psWidget, pData);

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
	GtkTreePath* psPath = NULL;
	GtkTreeIter sIter;
	int iTrackIndex = -1;

/* ********************************************************************* */
/* Si un morceau est en lecture, on le met en pause et inversement       */
/* ********************************************************************* */

	psPlayingChannel = playerDataGetPlayingChannel(psPlayerData);
	bIsPlaying = playerIsPlaying(psPlayingChannel);
	if (bIsPlaying == TRUE)
	{
		playerSetPaused(psPlayingChannel, !bIsPlaying);
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
		}
		else
		{
			FMOD_SYSTEM* psFmodContext = NULL;
			int iGoToStar = -1;

			/* Sinon, on lit le morceau actif de la liste */
			psPath = guiDataGetTreePath(psGuiData);
			assert(gtk_tree_model_get_iter(psModel, &sIter, psPath)
					== TRUE);
			gtk_tree_model_get(psModel, &sIter, 1, &iTrackIndex, -1);

			psTrackToPlay =
				analyzedTracksGetTrack(psTracks, iTrackIndex);

			psFmodContext = playerDataGetSystem(psPlayerData);
			iGoToStar = preferencesGetMoveCam(psPreferences);

			guiPlayTrack(psTrackToPlay, psMainBuilder, psFmodContext,
							&psPlayingChannel, iGoToStar, psGLData);
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

	bIsPaused = playerIsPaused(psPlayingChannel);
	playerSetPaused(psPlayingChannel, !bIsPaused);

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
	iDialogAnswer = gtk_dialog_run (GTK_DIALOG (pDialog));
	gtk_widget_hide_all (pDialog);


	/* Si l'utilisateur a ouvert un fichier */
	if (iDialogAnswer == GTK_RESPONSE_ACCEPT)
	{
		char* strFilename;
		char* strExtension;
		int iLength;
		int i;

		strFilename = gtk_file_chooser_get_filename(
											GTK_FILE_CHOOSER (pDialog));
		strExtension = strrchr(strFilename, '.');
		iLength = strlen(strExtension);

		for (i = 0; i<iLength; i++)
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
			analysisTrack(strFilename, pData);
		}
		else
		{
			GtkWidget* pErrorDialog;

			pErrorDialog = gtk_message_dialog_new_with_markup(
										GTK_WINDOW(pParent),
										GTK_DIALOG_MODAL,
										GTK_MESSAGE_WARNING,
										GTK_BUTTONS_CLOSE,
								"<b><big><u>Erreur :</u></big></b>\n\n\
Le fichier:\n \t%s\n n'est pas un fichier comptatible.",
										strFilename);
			gtk_dialog_run(GTK_DIALOG(pErrorDialog));

			gtk_widget_destroy(pErrorDialog);
		}

		g_free (strFilename);
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
		char* strFileName;
		char* strExtension;
		DIR *dirp;
		struct dirent *dp;
		int iLength = 0;
		int i = 0;
		int iPathAdded = 0;


		strFolderName = gtk_file_chooser_get_filename(
											GTK_FILE_CHOOSER (pDialog));
		/*iPathAdded = preferencesAddFilesPath((Preferences*)
											pData[PREFERENCES],
											strFolderName);*/

		if (iPathAdded == EXIT_SUCCESS)
		{
			/* Ajoute tous les fichiers du dossier. */
			dirp = opendir(strFolderName);
			assert(dirp);
			while ((dp = readdir(dirp)) != NULL)
			{
				strFileName = dp->d_name;
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

			closedir(dirp);

			g_free (strFolderName);
		}
		else
		{
			printf ("Dossier déjà existant...\n");
		}
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
	gtk_toggle_button_set_active(
					GTK_TOGGLE_BUTTON(psCheckBut),
					(gboolean) preferencesGetMoveCam(psPreferences));

	psContainer = GTK_WIDGET(
						gtk_builder_get_object(psMainBuilder,
											GUI_MAIN_CONTAINER));
	gtk_box_pack_start((GtkBox*)psContainer,
							psPrefWin, TRUE, TRUE, 0);

	gtk_widget_show_all(psPrefWin);

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
	}
	else
	{
		FMOD_SYSTEM* psFmodContext = NULL;
		int iGoToStar = -1;

		/* On arrête la lecture en cours */
		on_Stop_Action_activate(psWidget, pData);

		/* Sinon, on lit le morceau actif de la liste */
		psPath = guiDataGetTreePath(psGuiData);
		gtk_tree_path_next(psPath);
		assert(gtk_tree_model_get_iter(psModel, &sIter, psPath)
				== TRUE);
		gtk_tree_model_get(psModel, &sIter, 1, &iTrackIndex, -1);

		psTrackToPlay =
			analyzedTracksGetTrack(psTracks, iTrackIndex);

		psFmodContext = playerDataGetSystem(psPlayerData);
		iGoToStar = preferencesGetMoveCam(psPreferences);

		guiPlayTrack(psTrackToPlay, psMainBuilder, psFmodContext,
						&psPlayingChannel, iGoToStar, psGLData);
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

	GtkTreePath* psCurrentPath = NULL;

/* ********************************************************************* */
/* ********************************************************************* */

	/* On met a jour le chemin du morceau sélectionné. */
	psCurrentPath = guiDataGetTreePath(psGuiData);
	gtk_tree_path_free(psCurrentPath);
	psCurrentPath = gtk_tree_path_copy(psSelectedTrackPath);

	/* On arrête la lecture courante */
	on_Stop_Action_activate(NULL, pData);

	/* On lance la lecture, qui se chargera de lire le morceau */
	/* sélectionné                                             */
	on_Play_Action_activate(NULL, pData);

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

	if (iListLength == 0 &&
		!playerIsPlaying(psAnalyzingChannel)) /* Si la liste d'analyse est vide */
	{
		GtkBuilder* psMainBuilder = NULL;
		GtkWidget* psStatusBar = NULL;

		/* On efface le message de la barre d'état */
		psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);
		psStatusBar = GTK_WIDGET(gtk_builder_get_object(
									psMainBuilder,
									"Stellody_StatusBar"));
		gtk_statusbar_pop(GTK_STATUSBAR(psStatusBar), 1);

/* ********************************************************************* */
/* ********************************************************************* */

		/* C'est fonction n'a plus lieu d'être appelée */

		playerDataSetCheckForAnalyze(psPlayerData, 0);

		return FALSE;
	}
	else
	{
		if (playerIsPlaying(psAnalyzingChannel))
		{
			/* Si le canal d'analyse est déja en train d'analysé,
			on attend */

			return TRUE;
		}
		else /* On lance une nouvelle analyse */
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

	guiDataSetMousePosition(psGuiData, psEvent->x, psEvent->y);

	return EXIT_SUCCESS;
}

