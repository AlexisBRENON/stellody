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
#include "preferences.h"
#include "analyzed_tracks.h"
#include "analyzed_track.h"
#include "analysis.h"
#include "opengl_drawing.h"
#include "player.h"
#include "stellarium.h"


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

static int guiUnparent (GtkBuilder* pMainBuilder,
						GtkBuilder* pPreferencesBuilder,
						GtkBuilder* pAboutBuilder,
						GtkBuilder* pStellariumBuilder);

static gboolean guiTrackScaleIncrement (gpointer* pData);

static int guiPlayTrack (AnalyzedTrack* pTrack,
						GtkBuilder* pMainBuilder,
						FMOD_SYSTEM* pFmodContext,
						FMOD_CHANNEL** ppPlayingChannel,
						int iGoToStar,
						OpenGLData* pGLData);

static int guiAddTrackToPlaylist (GtkBuilder* pMainBuilder,
								AnalyzedTrack* pTrack,
								gpointer* pData);


static int guiUnparent (GtkBuilder* pMainBuilder,
						GtkBuilder* pPreferencesBuilder,
						GtkBuilder* pAboutBuilder,
						GtkBuilder* pStellariumBuilder)
{
	GtkWidget* psStellarium = NULL;
	GtkWidget* psPreference = NULL;
	GtkWidget* psAbout = NULL;
	GtkWidget* psContainer = NULL;

	psContainer = GTK_WIDGET(gtk_builder_get_object(
										pMainBuilder,
										GUI_MAIN_CONTAINER));

	/* Récupération des potentiels widgets fils et obturation du widget */
	if (pPreferencesBuilder != NULL)
	{
		psPreference = GTK_WIDGET(gtk_builder_get_object(
										pPreferencesBuilder,
										GUI_PREFERENCES_WIN));
		gtk_widget_hide_all(psPreference);

		if (gtk_widget_get_parent(psPreference) != NULL)
		{
			gtk_container_remove ((GtkContainer*)psContainer, psPreference);
		}
	}

	if (pAboutBuilder != NULL)
	{
		psAbout = GTK_WIDGET(gtk_builder_get_object(
												pAboutBuilder,
												GUI_ABOUT_WIN));
		gtk_widget_hide_all(psAbout);

		if (gtk_widget_get_parent(psAbout) != NULL)
		{
			gtk_container_remove ((GtkContainer*)psContainer, psAbout);
		}
	}

	if (pStellariumBuilder != NULL)
	{
		psStellarium = GTK_WIDGET(gtk_builder_get_object(
											pStellariumBuilder,
											GUI_STELLARIUM_WIN));
		gtk_widget_hide_all(psStellarium);

		if (gtk_widget_get_parent(psStellarium) != NULL)
		{
			gtk_container_remove ((GtkContainer*)psContainer, psStellarium);
		}
	}

	return EXIT_SUCCESS;
}


static gboolean guiTrackScaleIncrement (gpointer* pData)
{
	GtkWidget* pScale = NULL;
	GtkObject* pAdjustment = NULL;
	double dTrackPosition = 0;
	double dLength = 0;
	gboolean bIsPlaying = FALSE;
	gboolean bIsPaused = FALSE;

	bIsPlaying = playerIsPlaying((FMOD_CHANNEL*)pData[PLAYING_CHANNEL]);
	bIsPaused = playerIsPaused((FMOD_CHANNEL*)pData[PLAYING_CHANNEL]);

	if (bIsPlaying == TRUE && bIsPaused == FALSE)
	{
		pScale = GTK_WIDGET(gtk_builder_get_object(pData[MAIN_BUILDER],
														"Track_Scale"));
		dTrackPosition = gtk_range_get_value(GTK_RANGE(pScale));
		gtk_range_set_value(GTK_RANGE(pScale), dTrackPosition+1);
	}


	pAdjustment = GTK_OBJECT(gtk_builder_get_object(pData[MAIN_BUILDER],
												"Track_Adjustment"));
	dLength = gtk_adjustment_get_upper(GTK_ADJUSTMENT(pAdjustment));

	if (dTrackPosition+1 >= dLength)
	{
		on_Stop_Action_activate(NULL, pData);

		if(pData[PLAYLIST] != NULL)
		{
			AnalyzedTrack* pTrack = NULL;
			GList* pList = NULL;

			*((int*)pData[PLAYLIST_INDEX]) =
					(*((int*) pData[PLAYLIST_INDEX]))+1;
			pList = g_list_nth(
						(GList*)pData[PLAYLIST],
						*((int*) pData[PLAYLIST_INDEX]));

			if (pList != NULL)
			{
				pTrack = (AnalyzedTrack*) pList->data;
			}

			if (pTrack != NULL)
			{
				guiPlayTrack(pTrack,
						pData[MAIN_BUILDER],
						pData[FMOD_CONTEXT],
						(FMOD_CHANNEL**) &pData[PLAYING_CHANNEL],
						((Preferences*)pData[PREFERENCES])->iMoveCam,
						pData[OPENGLDATA]);
				*((int*)pData[INCREMENT_TIMER]) =
					g_timeout_add_seconds(1,
							(GSourceFunc) guiTrackScaleIncrement,
							pData);
			}
		}
	}

	if (bIsPlaying == FALSE &&
		*((int*)pData[INCREMENT_TIMER]) != 0)
	{
		on_Stop_Action_activate(NULL, pData);
	}

	if (*((int*)pData[INCREMENT_TIMER]) == 0)
	{
		return FALSE;
	}


	bIsPlaying = playerIsPlaying((FMOD_CHANNEL*)pData[PLAYING_CHANNEL]);


	return bIsPlaying;
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


static int guiAddTrackToPlaylist (GtkBuilder* pMainBuilder,
								AnalyzedTrack* pTrack,
								gpointer* pData)
{
	GtkWidget* psPlayListBox = NULL;
	GtkWidget* psTrackLabel = NULL;
	char* strTrackPath = NULL;
	char* strTrackName = NULL;
	int* piTID = NULL;

	if (pData[PLAYLIST] == NULL)
	{
		on_Stop_Action_activate(NULL, pData);
		guiPlayTrack(pTrack,
					GTK_BUILDER(pData[MAIN_BUILDER]),
					(FMOD_SYSTEM*) pData[FMOD_CONTEXT],
					(FMOD_CHANNEL**) &(pData[PLAYING_CHANNEL]),
					((Preferences*)pData[PREFERENCES])->iMoveCam,
					(OpenGLData*) pData[OPENGLDATA]);

		*((int*)pData[INCREMENT_TIMER]) =
			g_timeout_add_seconds(1,
						(GSourceFunc) guiTrackScaleIncrement,
						pData);
	}

	pData[PLAYLIST] = g_list_append((GList*) pData[PLAYLIST], pTrack);

	strTrackPath = analyzedTrackGetPath(pTrack);
	strTrackName = strrchr(strTrackPath, '/');
	psTrackLabel = gtk_label_new(&(strTrackName[1]));

	gtk_label_set_selectable(GTK_LABEL(psTrackLabel), TRUE);

	piTID = (int*) malloc(sizeof(int));
	*piTID = analyzedTrackGetTID(pTrack);
	g_object_set_data(G_OBJECT(psTrackLabel),
					"TID", piTID);

	psPlayListBox = GTK_WIDGET(gtk_builder_get_object(pMainBuilder,
													"PlayList_Box"));

	gtk_box_pack_start(GTK_BOX(psPlayListBox), psTrackLabel,
						FALSE, FALSE, 0);

	g_signal_connect(psTrackLabel, "button-release-event",
					G_CALLBACK(on_PlayList_button_release_event), pData);

	gtk_widget_show_all(psPlayListBox);

	return EXIT_SUCCESS;
}

/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions relatives à la fenêtre                    */
/*                                                                       */
/* ********************************************************************* */

/* ********************************************************************* */
/*                           FENÊTRE PRINCIPALE                          */
/* ********************************************************************* */





int guiLoad (gpointer* pData)
{
	GtkBuilder* psBuilder = NULL;
	GtkWidget* psWin = NULL;

	psBuilder = gtk_builder_new();
	gtk_builder_add_from_file(psBuilder, GUI_WIN, NULL);
	gtk_builder_connect_signals(psBuilder, pData);
	psWin = GTK_WIDGET(gtk_builder_get_object(psBuilder,
											GUI_MAIN_WIN));
	gtk_widget_show_all(psWin);

	pData[MAIN_BUILDER] = psBuilder;

	return EXIT_SUCCESS;
}

int on_Quit_Action_activate(GtkWidget* psWidget, gpointer* pData)
{
	on_Stop_Action_activate(psWidget, pData);

	gtk_widget_destroy(GTK_WIDGET(
							gtk_builder_get_object(pData[MAIN_BUILDER],
													GUI_MAIN_WIN)));

	if (pData[PREFERENCES_BUILDER] != NULL)
	{
		gtk_widget_destroy(GTK_WIDGET(
					gtk_builder_get_object(pData[PREFERENCES_BUILDER],
											GUI_PREFERENCES_WIN)));
	}

	if (pData[ABOUT_BUILDER] != NULL)
	{
		gtk_widget_destroy(GTK_WIDGET(
					gtk_builder_get_object(pData[ABOUT_BUILDER],
											GUI_ABOUT_WIN)));
	}

	if (pData[STELLARIUM_BUILDER] != NULL)
	{
		gtk_widget_destroy(GTK_WIDGET(
					gtk_builder_get_object(pData[STELLARIUM_BUILDER],
											GUI_STELLARIUM_WIN)));
	}

	gtk_main_quit();

	return EXIT_SUCCESS;
}


int on_Play_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	FMOD_BOOL bIsPlaying = FALSE;
	char* strPath;
	AnalyzedTrack* psTrack = NULL;
	int i = 1;


	if (pData[PLAYING_CHANNEL] != NULL)
	{
		FMOD_Channel_IsPlaying((FMOD_CHANNEL*)pData[PLAYING_CHANNEL],
								&bIsPlaying);
	}
	if (bIsPlaying == FALSE)
	{
		/* Si aucun morceau n'est dans la playlist... */
		if (pData[PLAYLIST] == NULL)
		{
			int iTIDMax = 0;

			iTIDMax = preferencesGetMaxTID((Preferences*)
											pData[PREFERENCES]);

			/* On charge un morceau aléatoirement */
			i = rand() % (iTIDMax+1);
			/*i = 0;*/
			do
			{
				psTrack = analyzedTracksGetTrack(pData[ANALYZED_TRACKS],
													i);
				i++;
			} while ((psTrack == NULL && i <= iTIDMax));

			if (psTrack == NULL)
			{
				GtkWidget* psStatusBar;

				psStatusBar = GTK_WIDGET(gtk_builder_get_object(
										(GtkBuilder*) pData[MAIN_BUILDER],
										"Stellody_StatusBar"));
				gtk_statusbar_pop(GTK_STATUSBAR(psStatusBar), 1);
				gtk_statusbar_push(GTK_STATUSBAR(psStatusBar), 1,
									"Aucun morceau trouvé...");
				return EXIT_FAILURE;
			}

			strPath = analyzedTrackGetPath(psTrack);

		}
		else
		{
			/* Sinon, on charge le premier morceau de la playlist*/
			psTrack = (AnalyzedTrack*)
						(g_list_nth((GList*) pData[PLAYLIST],
								*((int*) pData[PLAYLIST_INDEX])))->data;
			strPath = analyzedTrackGetPath(psTrack);
		}

		on_Stop_Action_activate(psWidget, pData);
		guiPlayTrack(psTrack,
					(GtkBuilder*) pData[MAIN_BUILDER],
					(FMOD_SYSTEM*) pData[FMOD_CONTEXT],
					(FMOD_CHANNEL**) &pData[PLAYING_CHANNEL],
					((Preferences*)pData[PREFERENCES])->iMoveCam,
					(OpenGLData*) pData[OPENGLDATA]);

		/* On crée le timer chargé de faire progresser la barre de
		lecture */
		*((int*)pData[INCREMENT_TIMER]) =
			g_timeout_add_seconds(1,
						(GSourceFunc) guiTrackScaleIncrement,
						pData);
	}

	return EXIT_SUCCESS;
}

int on_Stop_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	GtkWidget* pRange = NULL;
	GtkWidget* pLabel = NULL;
	GtkObject* pAdjustment = NULL;

	FMOD_Channel_Stop((FMOD_CHANNEL*) pData[PLAYING_CHANNEL]);
	pData[PLAYING_CHANNEL] = NULL;

	*((int*)pData[INCREMENT_TIMER]) = 0;
	guiTrackScaleIncrement(pData);

	drawingGLSetPlayedTrack(pData[OPENGLDATA], NULL);

	pAdjustment = GTK_OBJECT(gtk_builder_get_object(
											pData[MAIN_BUILDER],
											"Track_Adjustment"));
	pRange = GTK_WIDGET(gtk_builder_get_object(pData[MAIN_BUILDER],
												"Track_Scale"));

	gtk_adjustment_set_upper(GTK_ADJUSTMENT(pAdjustment), 100);
	gtk_range_set_value(GTK_RANGE(pRange), 0);

	pLabel = GTK_WIDGET(gtk_builder_get_object(pData[MAIN_BUILDER],
													"TrackInfo_Label"));
	gtk_label_set_text (GTK_LABEL(pLabel), "");

	return EXIT_SUCCESS;
}

int on_Pause_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	FMOD_BOOL bIsPaused;

	FMOD_Channel_GetPaused((FMOD_CHANNEL*) pData[PLAYING_CHANNEL],
							&bIsPaused);
	FMOD_Channel_SetPaused(pData[PLAYING_CHANNEL],
							!bIsPaused);

	return EXIT_SUCCESS;
}

int on_AddTrack_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	GtkWidget* pDialog;
	GtkWidget* pParent;
	int iDialogAnswer;


	/* Affichage de la fenêtre de sélection de fichier. */
	pParent = GTK_WIDGET(gtk_builder_get_object(pData[MAIN_BUILDER],
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
	GtkWidget* pDialog;
	GtkWidget* pParent;
	int iDialogAnswer;

	pParent = GTK_WIDGET(gtk_builder_get_object(pData[MAIN_BUILDER],
										GUI_MAIN_WIN));

#if defined(_WIN32)
	/* Affiche un message d'errreur. */
	pDialog = gtk_message_dialog_new(
									GTK_WINDOW(pParent),
									GTK_DIALOG_MODAL,
									GTK_MESSAGE_WARNING,
									GTK_BUTTONS_CLOSE,
					"Fonctionnalité non prise en charge sous Windows.\n\
Utilisez un systeme <u>UNIX</u> :p !");
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


	/* Si l'utilisateur a ouvert un fichier */
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
	GtkBuilder* psBuilder = NULL;
	GtkWidget* psStellarium = NULL;
	GtkWidget* psContainer = NULL;

	guiUnparent(pData[MAIN_BUILDER],
				pData[PREFERENCES_BUILDER],
				pData[ABOUT_BUILDER],
				pData[STELLARIUM_BUILDER]);

	if (pData[STELLARIUM_BUILDER] == NULL)
	{
		GdkGLConfig* pConfig = NULL;
		GtkWidget* pDrawingArea = NULL;

		/* Crée le Stellarium à partir du fichier glade. */
		psBuilder = gtk_builder_new();
		gtk_builder_add_from_file(psBuilder, GUI_STELLARIUM, NULL);
		gtk_builder_connect_signals(psBuilder, pData);

		pData[STELLARIUM_BUILDER] = psBuilder;

		/* Active la capacité OpenGL au Stellarium */
		pDrawingArea = GTK_WIDGET(gtk_builder_get_object(psBuilder,
											"Stellarium_DrawingArea"));

		pConfig = gdk_gl_config_new_by_mode(GDK_GL_MODE_RGBA |
											GDK_GL_MODE_DEPTH |
											GDK_GL_MODE_DOUBLE);

		gtk_widget_set_gl_capability(pDrawingArea,
										pConfig,
										NULL,
										TRUE,
										GDK_GL_RGBA_TYPE);

	}

	psStellarium = GTK_WIDGET(gtk_builder_get_object(
											pData[STELLARIUM_BUILDER],
											GUI_STELLARIUM_WIN));
	psContainer = GTK_WIDGET(
					gtk_builder_get_object(pData[MAIN_BUILDER],
										GUI_MAIN_CONTAINER));
	gtk_box_pack_start((GtkBox*)psContainer,
							psStellarium, TRUE, TRUE, 0);
	gtk_widget_show_all(psStellarium);

	return EXIT_SUCCESS;
}

int on_Preferences_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	GtkBuilder* psBuilder = NULL;
	GtkWidget* psPrefWin = NULL;
	GtkWidget* psContainer = NULL;
	GtkWidget* psScale = NULL;
	GtkWidget* psCheckBut = NULL;

	guiUnparent(pData[MAIN_BUILDER],
				pData[PREFERENCES_BUILDER],
				pData[ABOUT_BUILDER],
				pData[STELLARIUM_BUILDER]);

	/* Si la fenêtre n'a jamais été créée. */
	if (pData[PREFERENCES_BUILDER] == NULL)
	{
		psBuilder = gtk_builder_new();
		gtk_builder_add_from_file(psBuilder, GUI_PREFERENCES, NULL);
		gtk_builder_connect_signals(psBuilder, pData);

		pData[PREFERENCES_BUILDER] = psBuilder;
	}

	psPrefWin = GTK_WIDGET(gtk_builder_get_object(
												pData[PREFERENCES_BUILDER],
												GUI_PREFERENCES_WIN));
	psScale = GTK_WIDGET(gtk_builder_get_object(
										pData[PREFERENCES_BUILDER],
										"AnalysisRate_Scale"));
	gtk_range_set_value((GtkRange*)psScale,
					(double) preferencesGetAnalysisRate(
												pData[PREFERENCES]));

	psScale = GTK_WIDGET(gtk_builder_get_object(
										pData[PREFERENCES_BUILDER],
										"3DQuality_Scale"));
	gtk_range_set_value((GtkRange*)psScale,
					(double) preferencesGet3DQuality(
												pData[PREFERENCES]));

	psCheckBut = GTK_WIDGET(gtk_builder_get_object(
										pData[PREFERENCES_BUILDER],
										"MoveCamera_Check"));
	gtk_toggle_button_set_active(
					GTK_TOGGLE_BUTTON(psCheckBut),
					(gboolean) preferencesGetMoveCam(pData[PREFERENCES]));

	psContainer = GTK_WIDGET(
						gtk_builder_get_object(pData[MAIN_BUILDER],
											GUI_MAIN_CONTAINER));
	gtk_box_pack_start((GtkBox*)psContainer,
							psPrefWin, TRUE, TRUE, 0);

	gtk_widget_show_all(psPrefWin);

	return EXIT_SUCCESS;
}

int on_About_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	GtkBuilder* psBuilder = NULL;
	GtkWidget* psAbout = NULL;
	GtkWidget* psContainer = NULL;

	guiUnparent(pData[MAIN_BUILDER],
				pData[PREFERENCES_BUILDER],
				pData[ABOUT_BUILDER],
				pData[STELLARIUM_BUILDER]);

	if (pData[ABOUT_BUILDER] == NULL)
	{
		psBuilder = gtk_builder_new();
		gtk_builder_add_from_file(psBuilder, GUI_ABOUT, NULL);
		gtk_builder_connect_signals(psBuilder, pData);

		pData[ABOUT_BUILDER] = psBuilder;
	}

	psAbout = GTK_WIDGET(gtk_builder_get_object(
											pData[ABOUT_BUILDER],
											GUI_ABOUT_WIN));
	psContainer = GTK_WIDGET(
					gtk_builder_get_object(pData[MAIN_BUILDER],
										GUI_MAIN_CONTAINER));
	gtk_box_pack_start((GtkBox*)psContainer,
							psAbout, TRUE, TRUE, 0);
	gtk_widget_show_all(psAbout);

	return EXIT_SUCCESS;
}

int on_Next_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	AnalyzedTrack* pTrack = NULL;
	FMOD_BOOL bIsPlaying = FALSE;
	int i;

	on_Stop_Action_activate(psWidget, pData);

	if (pData[PLAYLIST] != NULL)
	{
		GList* pList = NULL;

		*((int*)pData[PLAYLIST_INDEX]) = (*((int*) pData[PLAYLIST_INDEX]))+1;
		pList = g_list_nth(
					(GList*)pData[PLAYLIST],
					*((int*) pData[PLAYLIST_INDEX]));

		if (pList != NULL)
		{
			pTrack = (AnalyzedTrack*) pList->data;
		}
	}
	else
	{
		if (pData[PLAYING_CHANNEL] != NULL)
		{
			FMOD_Channel_IsPlaying((FMOD_CHANNEL*)pData[PLAYING_CHANNEL],
									&bIsPlaying);
		}

		if (bIsPlaying == TRUE)
		{
			int iTIDMax = 0;

			iTIDMax = preferencesGetMaxTID((Preferences*)
											pData[PREFERENCES]);

			/* On charge un morceau aléatoirement */
			i = rand() % (iTIDMax+1);
			do
			{
				pTrack = analyzedTracksGetTrack(pData[ANALYZED_TRACKS],
													i);
				i++;
			} while ((pTrack == NULL && i <= iTIDMax));

			if (pTrack == NULL)
			{
				for (i = 0; pTrack == NULL && i <= iTIDMax; i++)
				{
					pTrack = analyzedTracksGetTrack(pData[ANALYZED_TRACKS],
													i);
				}
			}
		}
	}

	if (pTrack != NULL)
	{
		guiPlayTrack(pTrack,
					pData[MAIN_BUILDER],
					pData[FMOD_CONTEXT],
					(FMOD_CHANNEL**) &pData[PLAYING_CHANNEL],
					((Preferences*)pData[PREFERENCES])->iMoveCam,
					pData[OPENGLDATA]);
		*((int*)pData[INCREMENT_TIMER]) =
			g_timeout_add_seconds(1,
						(GSourceFunc) guiTrackScaleIncrement,
						pData);
	}

	return EXIT_SUCCESS;
}

int on_Track_Scale_value_changed (GtkWidget* psWidget,
								GdkEventButton* pEvent,
								 gpointer* pData)
{
	gdouble dNewTrackPosition;

	if (pData[PLAYING_CHANNEL] != NULL)
	{
		dNewTrackPosition = gtk_range_get_value(GTK_RANGE(psWidget));
		FMOD_Channel_SetPosition((FMOD_CHANNEL*) pData[PLAYING_CHANNEL],
									dNewTrackPosition*1000,
									FMOD_TIMEUNIT_MS);
	}

	return EXIT_SUCCESS;
}

int on_PlayList_button_release_event(GtkWidget* psWidget,
									GdkEventButton* psEvent,
									gpointer* pData)
{
	int* piTID = NULL;
	AnalyzedTrack* pTrack = NULL;

	if (psEvent->button != 1)
	{
		return EXIT_FAILURE;
	}

	piTID = (int*) g_object_get_data(G_OBJECT(psWidget), "TID");
	pTrack = g_tree_lookup((GTree*) pData[ANALYZED_TRACKS],
							piTID);

	if (pTrack == NULL)
	{
		GtkContainer* psPlayListBox = NULL;

		psPlayListBox = GTK_CONTAINER(gtk_widget_get_parent(psWidget));
		gtk_container_remove(psPlayListBox, psWidget);
		gtk_widget_destroy(psWidget);
		free(piTID);

		return EXIT_FAILURE;
	}

	on_Stop_Action_activate(psWidget, pData);
	guiPlayTrack(pTrack,
				GTK_BUILDER(pData[MAIN_BUILDER]),
				(FMOD_SYSTEM*) pData[FMOD_CONTEXT],
				(FMOD_CHANNEL**) &(pData[PLAYING_CHANNEL]),
				((Preferences*)pData[PREFERENCES])->iMoveCam,
				(OpenGLData*) pData[OPENGLDATA]);

	*((int*)pData[INCREMENT_TIMER]) =
			g_timeout_add_seconds(1,
						(GSourceFunc) guiTrackScaleIncrement,
						pData);

	*((int*) pData[PLAYLIST_INDEX]) = g_list_index(
											(GList*) pData[PLAYLIST],
											pTrack);

	return EXIT_SUCCESS;
}



/* ********************************************************************* */
/*                            FONCTIONS TIMEOUT                          */
/* ********************************************************************* */




gboolean guiTimeoutAnalyze (gpointer* pData)
{
	gboolean bIsPlaying = FALSE;
	AnalyzedTrack* psTrack = NULL;

	/* On cherche à savoir si l'analyse est finie ou non */
	bIsPlaying = (gboolean) playerIsPlaying(
							(FMOD_CHANNEL*)pData[ANALYZING_CHANNEL]);

	/* On récupère le morceau en cours d'analyse */
	psTrack = (AnalyzedTrack*) (g_list_first(
								(GList*) pData[ANALYZELIST]))->data;

	if (bIsPlaying == TRUE)
	{
		float fAnalysisRate = 0;

		/* On récupère le taux d'analyse */

		fAnalysisRate = (float) preferencesGetAnalysisRate(
								(Preferences*) pData[PREFERENCES]);

		/* Au maximum (100%) on analysera toutes les 25ms le morceau en
		vitesse normale.
		Au quart (25%) on analysera toutes les 25ms le morceau en vitesse
		x4. */

		fAnalysisRate = 25/(fAnalysisRate/100.0);

		analysisAnalyze ((FMOD_CHANNEL*) pData[ANALYZING_CHANNEL],
						fAnalysisRate,
						psTrack,
						(int*) pData[ANALYZING_COUNTER]);

		(*((int*) pData[ANALYZING_COUNTER]))++;

	}
	else
	{
		int iTIDMax = 0;
		int iTIDMin = 0;
		int iTID = 0;
		char* strConstFileName = NULL;
		char* strFileName = NULL;
		char* strStatusBarMessage = NULL;
		GtkWidget* psStatusBar = NULL;


/* ********************************************************************* */
/* ********************************************************************* */

		/* On récupère le TID actuel pour enlever le morceau de l'arbre */

		iTID = analyzedTrackGetTID(psTrack);
		g_tree_steal((GTree*) pData[ANALYZED_TRACKS],
					&iTID);

/* ********************************************************************* */
/* ********************************************************************* */

		/* On calcul le nouveau TID, et changeons le status du morceau
		(analysé) */

		analyzedTrackSetTID(psTrack,
							0);
		analyzedTrackSetAnalyzed(psTrack, 1);

/* ********************************************************************* */
/* ********************************************************************* */

		/* On ajoute le morceau à l'arbre avec son nouveau TID */

		analyzedTracksInsertTrack((AnalyzedTracks*) pData[ANALYZED_TRACKS],
									psTrack);

		/* On le supprime de la liste d'analyse */

		pData[ANALYZELIST] = g_list_remove((GList*) pData[ANALYZELIST],
											psTrack);

/* ********************************************************************* */
/* ********************************************************************* */

		/* Il se peut que le TID ai changé à l'insertion, on le récupère */

		iTID = analyzedTrackGetTID(psTrack);
		iTIDMax = preferencesGetMaxTID((Preferences*) pData[PREFERENCES]);
		iTIDMin = preferencesGetMinTID((Preferences*) pData[PREFERENCES]);

		/* On vérifie que les bornes sont tjs correctes */

		if (iTID > iTIDMax)
		{
			preferencesSetMaxTID((Preferences*) pData[PREFERENCES], iTID);
		}
		else if (iTID < iTIDMin)
		{
			preferencesSetMinTID((Preferences*) pData[PREFERENCES], iTID);
		}

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
		psStatusBar = GTK_WIDGET(gtk_builder_get_object(
									(GtkBuilder*) pData[MAIN_BUILDER],
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
	if (pData[ANALYZELIST] == NULL &&
		!playerIsPlaying((FMOD_CHANNEL*) pData[ANALYZING_CHANNEL]))
	{
		/* Si la liste d'analyse est vide */

		GtkWidget* psStatusBar = NULL;

/* ********************************************************************* */
/* ********************************************************************* */

		/* On efface le message de la barre d'état */

		psStatusBar = GTK_WIDGET(gtk_builder_get_object(
									(GtkBuilder*) pData[MAIN_BUILDER],
									"Stellody_StatusBar"));
		gtk_statusbar_pop(GTK_STATUSBAR(psStatusBar), 1);

/* ********************************************************************* */
/* ********************************************************************* */

		/* C'est fonction n'a plus lieu d'être appelée */

		*((int*) pData[CHECKANALYZE]) = 0;

		return FALSE;
	}
	else
	{
		if (playerIsPlaying((FMOD_CHANNEL*) pData[ANALYZING_CHANNEL]))
		{
			/* Si le canal d'analyse est déja en train d'analysé,
			on attend */

			return TRUE;
		}
		else
		{
			unsigned int uiTrackLength = 0;
			char* strSinger = NULL; /* Not Use */
			char* strTitle = NULL; /* Not Use */
			char* strConstFileName = NULL;
			char* strFileName = NULL;
			char* strStatusBarMessage = NULL;
			AnalyzedTrack* psTrack = NULL;
			GtkWidget* psStatusBar = NULL;

/* ********************************************************************* */
/* ********************************************************************* */

			/* On charge le morceau dans le canal d'analyse */

			psTrack = (g_list_first((GList*) pData[ANALYZELIST]))->data;

			playerPlayTrack(analyzedTrackGetPath(psTrack),
							(FMOD_SYSTEM*) pData[FMOD_CONTEXT],
							(FMOD_CHANNEL**) &pData[ANALYZING_CHANNEL],
							0,
							0,
							&uiTrackLength,
							&strSinger,
							&strTitle);
			/* On coupe le son, le morceau doit être joué mais pas entendu */
			playerSetVolume((FMOD_CHANNEL*) pData[ANALYZING_CHANNEL],
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
			psStatusBar = GTK_WIDGET(gtk_builder_get_object(
										(GtkBuilder*) pData[MAIN_BUILDER],
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
	GtkWidget* psScale = NULL;
	GtkWidget* psCheckBut = NULL;

	psScale = GTK_WIDGET(gtk_builder_get_object(
										pData[PREFERENCES_BUILDER],
										"AnalysisRate_Scale"));
	preferencesSetAnalysisRate(pData[PREFERENCES],
						(int) gtk_range_get_value((GtkRange*)psScale));

	psScale = GTK_WIDGET(gtk_builder_get_object(
										pData[PREFERENCES_BUILDER],
										"3DQuality_Scale"));
	preferencesSet3DQuality(pData[PREFERENCES],
						(int) gtk_range_get_value((GtkRange*)psScale));

	psCheckBut = GTK_WIDGET(gtk_builder_get_object(
										pData[PREFERENCES_BUILDER],
										"MoveCamera_Check"));
	preferencesSetMoveCam(pData[PREFERENCES],
						(int) gtk_toggle_button_get_active(
										GTK_TOGGLE_BUTTON(psCheckBut)));

	on_Stellarium_Action_activate(psWidget, pData);

	return EXIT_SUCCESS;
}

int on_FlushData_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	GtkWidget* pDialog = NULL;

	analyzedTracksRelease((AnalyzedTracks*) pData[ANALYZED_TRACKS]);

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
	GdkGLContext * psContext = NULL;
	GdkGLDrawable * psSurface = NULL;
	gboolean bActivate = FALSE;

	psContext = gtk_widget_get_gl_context(psWidget);
	psSurface = gtk_widget_get_gl_drawable(psWidget);

	bActivate = gdk_gl_drawable_gl_begin(psSurface,psContext);
	if (bActivate == TRUE)
	{
		drawingGLInit((OpenGLData*)pData[OPENGLDATA]);
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
	GdkGLContext * psContext = NULL;
	GdkGLDrawable * psSurface = NULL;
	gboolean bActivate = FALSE;

	psContext = gtk_widget_get_gl_context(psWidget) ;
	psSurface = gtk_widget_get_gl_drawable(psWidget) ;

	bActivate = gdk_gl_drawable_gl_begin(psSurface,psContext);
	if (bActivate == TRUE)
	{
		drawingGLResize(pData[OPENGLDATA], psEvent->width, psEvent->height);
		gdk_gl_drawable_gl_end(psSurface); /* désactivation du contexte */
	}
	return EXIT_SUCCESS;
}


int on_Stellarium_DrawingArea_expose_event (
							GtkWidget* psWidget,
							GdkEventExpose* psEvent,
							gpointer* pData)
{
	GdkGLContext * psContext = NULL;
	GdkGLDrawable * psSurface = NULL;
	gboolean bActivate = FALSE;

	psContext = gtk_widget_get_gl_context(psWidget);
	psSurface = gtk_widget_get_gl_drawable(psWidget);

	bActivate = gdk_gl_drawable_gl_begin(psSurface,psContext);

	if (bActivate == TRUE)
	{
		drawingGLDraw(pData[OPENGLDATA], pData[ANALYZED_TRACKS],
					  preferencesGet3DQuality(
								(Preferences*) pData[PREFERENCES]));
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
	GtkAllocation sAllocation;

	switch (psEvent->type)
	{
		case GDK_SCROLL: /* Si on scroll avec la molette */
			switch (((GdkEventScroll*)psEvent)->direction)
			{
				case GDK_SCROLL_UP:
					drawingGLZoom(pData[OPENGLDATA],
								  (*(float*) pData[MOUSEPOSITION_X]),
								  (*(float*) pData[MOUSEPOSITION_Y]),
								  0.1);
					break;
				case GDK_SCROLL_DOWN:
					gtk_widget_get_allocation(psWidget,
												&sAllocation);
					drawingGLZoom(pData[OPENGLDATA],
								  sAllocation.width - (*(float*) pData[MOUSEPOSITION_X]),
								  sAllocation.height - (*(float*) pData[MOUSEPOSITION_Y]),
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
						(OpenGLData*) pData[OPENGLDATA],
						(AnalyzedTracks*) pData[ANALYZED_TRACKS],
						((GdkEventButton*)psEvent)->x,
						((GdkEventButton*)psEvent)->y);

					gdk_gl_drawable_swap_buffers(psSurface); /* permutation des tampons */
					gdk_gl_drawable_gl_end(psSurface); /* désactivation du contexte */
				}

				if (iTID >= 0)
				{
					pTrack = g_tree_lookup(pData[ANALYZED_TRACKS], &iTID);

					guiAddTrackToPlaylist(GTK_BUILDER(pData[MAIN_BUILDER]),
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
	float iStepX = 0;
	float iStepY = 0;

	if (((psEvent->state)>>9)%2 == 1 || ((psEvent->state)>>10)%2 == 1)
	{
		iStepX = (*(float*) pData[MOUSEPOSITION_X]) - (float) psEvent->x;
		iStepY = (*(float*) pData[MOUSEPOSITION_Y]) - (float) psEvent->y;

		drawingGLRotate(pData[OPENGLDATA], -iStepX/20, iStepY/20);
	}
	else if (((psEvent->state)>>8)%2 == 1)
	{
		iStepX = (*(float*) pData[MOUSEPOSITION_X]) - (float) psEvent->x;
		iStepY = (*(float*) pData[MOUSEPOSITION_Y]) - (float) psEvent->y;

		drawingGLTranslate(pData[OPENGLDATA], -iStepX/500, iStepY/500, 0);
	}

	*((float*) pData[MOUSEPOSITION_X]) = (float) psEvent->x;
	*((float*) pData[MOUSEPOSITION_Y]) = (float) psEvent->y;

	return EXIT_SUCCESS;
}

