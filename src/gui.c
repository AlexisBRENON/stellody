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

#if defined(__linux) || defined (__APPLE__)
	#include <dirent.h>
#endif

#include <gtk/gtk.h>
/*#include <gtk/gtkgl.h>*/

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
#define GUI_MAIN_CONTAINER "Display_HBox" /**< Nom du widget contenant les
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


/**
  * @fn static int guiUnparent (gpointer* pData)
  * @brief Déparente toutes les fenêtres variables.
  *
  * @param[in,out] pData Le tableau de données général
  * @return EXIT_SUCCESS si tout est OK
  */
static int guiUnparent (gpointer* pData)
{
	GtkWidget* psStellarium = NULL;
	GtkWidget* psPreference = NULL;
	GtkWidget* psAbout = NULL;
	GtkWidget* psContainer = NULL;

	psContainer = GTK_WIDGET(gtk_builder_get_object(pData[MAIN_BUILDER],
										GUI_MAIN_CONTAINER));

	/* Récupération des potentiels widgets fils et obturation du widget */
	if (pData[PREFERENCES_BUILDER] != NULL)
	{
		psPreference = GTK_WIDGET(gtk_builder_get_object(
										pData[PREFERENCES_BUILDER],
										GUI_PREFERENCES_WIN));
		gtk_widget_hide_all(psPreference);

		if (gtk_widget_get_parent(psPreference) != NULL)
		{
			gtk_container_remove ((GtkContainer*)psContainer, psPreference);
		}
	}

	if (pData[ABOUT_BUILDER] != NULL)
	{
		psAbout = GTK_WIDGET(gtk_builder_get_object(pData[ABOUT_BUILDER],
												GUI_ABOUT_WIN));
		gtk_widget_hide_all(psAbout);

		if (gtk_widget_get_parent(psAbout) != NULL)
		{
			gtk_container_remove ((GtkContainer*)psContainer, psAbout);
		}
	}

	if (pData[STELLARIUM_BUILDER] != NULL)
	{
		psStellarium = GTK_WIDGET(gtk_builder_get_object(
											pData[STELLARIUM_BUILDER],
											GUI_STELLARIUM_WIN));
		gtk_widget_hide_all(psStellarium);

		if (gtk_widget_get_parent(psStellarium) != NULL)
		{
			gtk_container_remove ((GtkContainer*)psContainer, psStellarium);
		}
	}

	return EXIT_SUCCESS;
}

/**
  * @fn static gboolean guiTrackScaleIncrement (gpointer* pData)
  * @brief Déplace le curseur durant la lecture d'un morceau.
  *
  * @param pData Le tableau de données.
  * @return TRUE si le morceau est encore en lecture, FALSE sinon
  */
static gboolean guiTrackScaleIncrement (gpointer* pData)
{
	GtkWidget* pScale = NULL;
	GtkObject* pAdjustment = NULL;
	double dTrackPosition = 0;
	double dLength = 0;
	gboolean bIsPlaying = FALSE;
	gboolean bIsPaused = FALSE;

	FMOD_Channel_IsPlaying((FMOD_CHANNEL*)pData[PLAYING_CHANNEL],
								&bIsPlaying);
	FMOD_Channel_GetPaused((FMOD_CHANNEL*)pData[PLAYING_CHANNEL],
								&bIsPaused);

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

	if (dTrackPosition+1 >= dLength || bIsPlaying == FALSE)
	{
		on_Stop_Action_activate(NULL, pData);
	}

	return bIsPlaying;
}



/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions relatives à la fenêtre                    */
/*                                                                       */
/* ********************************************************************* */

/* ********************************************************************* */
/*                           FENÊTRE PRINCIPALE                          */
/* ********************************************************************* */


GtkBuilder* guiLoad (const gpointer pData)
{
	GtkBuilder* psBuilder = NULL;
	GtkWidget* psWin = NULL;

	psBuilder = gtk_builder_new();
	gtk_builder_add_from_file(psBuilder, GUI_WIN, NULL);
	gtk_builder_connect_signals(psBuilder, pData);
	psWin = GTK_WIDGET(gtk_builder_get_object(psBuilder,
											GUI_MAIN_WIN));
	gtk_widget_show_all(psWin);

	return psBuilder;
}

int on_Quit_Action_activate(GtkWidget* psWidget, gpointer* pData)
{
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
	FMOD_SOUND* pSound = NULL;
	FMOD_BOOL bIsPlaying = FALSE;
	const char* strPath;
	AnalyzedTrack* psTrack = NULL;
	GtkWidget* pScale = NULL;
	GtkWidget* pLabel = NULL;
	GtkObject* pAdjustment = NULL;
	int i = 1;
	unsigned int uiTrackPosition = 0;
	unsigned int uiTrackLength = 0;
	FMOD_TAG Tag;
	char* strTitle;
	char* strSinger;
	char* strTrackInfo;


	if (pData[PLAYING_CHANNEL] != NULL)
	{
		FMOD_Channel_IsPlaying((FMOD_CHANNEL*)pData[PLAYING_CHANNEL],
								&bIsPlaying);
	}
	if (bIsPlaying == FALSE)
	{
		/* On récupère à partir de quelle partie commencer le morceau */
		pScale = GTK_WIDGET(gtk_builder_get_object(pData[MAIN_BUILDER],
													"Track_Scale"));
		uiTrackPosition = (unsigned int)
						gtk_range_get_value(GTK_RANGE(pScale));

		pAdjustment = GTK_OBJECT(gtk_builder_get_object(
												pData[MAIN_BUILDER],
												"Track_Adjustment"));

		/* Si aucun morceau n'est dans la playlist... */
		if (pData[PLAYLIST] == NULL)
		{
			/* On charge la premier morceau qu'on trouve */
			do
			{
				psTrack = analyzedTracksGetTrack(pData[ANALYZED_TRACKS], i);
				i++;
			} while (psTrack == NULL && i < 100);

			strPath = analyzedTrackGetPath(psTrack);

		}
		else
		{
			/* Sinon, on charge le premier morceau de la playlist et on
			le supprime. */
			AnalyzedTrack* psTrackInPlaylist;
			psTrackInPlaylist = ((GSList*)pData[PLAYLIST])->data;
			strPath = analyzedTrackGetPath(psTrackInPlaylist);
			pData[PLAYLIST] = g_slist_remove((GSList*)pData[PLAYLIST],
												psTrackInPlaylist);
		}

		FMOD_System_CreateSound((FMOD_SYSTEM*) pData[FMOD_CONTEXT],
									strPath,
									FMOD_CREATESTREAM | FMOD_SOFTWARE,
									NULL,
									&pSound);

		/* On met à l'échelle la barre de lecture */
		FMOD_Sound_GetLength(pSound, &uiTrackLength, FMOD_TIMEUNIT_MS);
		uiTrackPosition = (uiTrackPosition*uiTrackLength)/100;

		gtk_adjustment_set_upper(GTK_ADJUSTMENT(pAdjustment),
								(double) uiTrackLength/1000);
		gtk_range_set_value(GTK_RANGE(pScale),
							(double) uiTrackPosition/1000);

		/* On récupère les tags */
		FMOD_Sound_GetTag(pSound, "TIT2", 0, &Tag);
		strTitle = (char*)Tag.data;
		FMOD_Sound_GetTag(pSound, "TPE1", 0, &Tag);
		strSinger = (char*) Tag.data;

		/* On les concatène */
		strTrackInfo = (char*) malloc((strlen(strTitle)+
										3+
										strlen(strSinger)
										+1)*sizeof(char));
		strcpy(strTrackInfo, strSinger);
		strcat(strTrackInfo, " - ");
		strcat(strTrackInfo, strTitle);

		/* On les affiche dans le label */
		pLabel = GTK_WIDGET(gtk_builder_get_object(pData[MAIN_BUILDER],
													"TrackInfo_Label"));
		gtk_label_set_text(GTK_LABEL(pLabel), strTrackInfo);

		free(strTrackInfo);

		/* On joue le morceau. */
		g_timeout_add_seconds(1,
							(GSourceFunc) guiTrackScaleIncrement,
							pData);

		FMOD_System_PlaySound((FMOD_SYSTEM*) pData[FMOD_CONTEXT],
							FMOD_CHANNEL_FREE, pSound, TRUE,
							(FMOD_CHANNEL**)&(pData[PLAYING_CHANNEL]));
		/* A partir du moment voulu */
		FMOD_Channel_SetPosition((FMOD_CHANNEL*)pData[PLAYING_CHANNEL],
						uiTrackPosition, FMOD_TIMEUNIT_MS);
		FMOD_Channel_SetPaused((FMOD_CHANNEL*)pData[PLAYING_CHANNEL],
								FALSE);
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

	pAdjustment = GTK_OBJECT(gtk_builder_get_object(
											pData[MAIN_BUILDER],
											"Track_Adjustment"));
	pRange = GTK_WIDGET(gtk_builder_get_object(pData[MAIN_BUILDER],
												"Track_Scale"));

	gtk_adjustment_set_upper(GTK_ADJUSTMENT(pAdjustment), 0);
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
			strcmp(strExtension, ".wma") == 0 ||
			strcmp(strExtension, ".mid") == 0 ||
			strcmp(strExtension, ".m3u") == 0 ||
			strcmp(strExtension, ".mp2") == 0 ||
			strcmp(strExtension, ".ogg") == 0 ||
			strcmp(strExtension, ".raw") == 0 ||
			strcmp(strExtension, ".wav") == 0)
		{
			analysisTrack(strFilename, pData);
			/* Modification du TIDMAx */
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
		iPathAdded = preferencesAddFilesPath((Preferences*)
											pData[PREFERENCES],
											strFolderName);

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
					strcmp(strExtension, ".wma") == 0 ||
					strcmp(strExtension, ".mid") == 0 ||
					strcmp(strExtension, ".m3u") == 0 ||
					strcmp(strExtension, ".mp2") == 0 ||
					strcmp(strExtension, ".ogg") == 0 ||
					strcmp(strExtension, ".raw") == 0 ||
					strcmp(strExtension, ".wav") == 0)
				{
					printf ("Nouveau morceau ajouté :\n");
					printf ("\t%s\n", strFileName);
					/* Modification du TIDMax */
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

	guiUnparent(pData);

	if (pData[STELLARIUM_BUILDER] == NULL)
	{
		/*GdkGLConfig* pConfig = NULL;*/
		GtkWidget* pDrawingArea = NULL;

		/* Crée le Stellarium à partir du fichier glade. */
		psBuilder = gtk_builder_new();
		gtk_builder_add_from_file(psBuilder, GUI_STELLARIUM, NULL);

		pData[STELLARIUM_BUILDER] = psBuilder;

		/* Active la capacité OpenGL au Stellarium */
		pDrawingArea = GTK_WIDGET(gtk_builder_get_object(psBuilder,
											"Stellarium_DrawingArea"));

		/*pConfig = gdk_gl_config_new_by_mode(GDK_GL_MODE_RGBA |
											GDK_GL_MODE_DEPTH |
											GDK_GL_MODE_DOUBLE);
		gtk_widget_set_gl_capability(pDrawingArea,
										pConfig,
										NULL,
										TRUE,
										GDK_GL_RGBA_TYPE);*/

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

	guiUnparent(pData);

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
	preferencesGetAnalysisRate(pData[PREFERENCES]);
	gtk_range_set_value((GtkRange*)psScale,
					(double) preferencesGetAnalysisRate(
												pData[PREFERENCES]));


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

	guiUnparent(pData);

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
/* ********************************************************************* */
/*                           FENÊTRE PREFERENCES                         */
/* ********************************************************************* */

int on_PrefOKBut_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	GtkWidget* psScale = NULL;

	psScale = GTK_WIDGET(gtk_builder_get_object(
										pData[PREFERENCES_BUILDER],
										"AnalysisRate_Scale"));

	preferencesSetAnalysisRate(pData[PREFERENCES],
						(int) gtk_range_get_value((GtkRange*)psScale));

	on_Stellarium_Action_activate(psWidget, pData);

	return EXIT_SUCCESS;
}






























