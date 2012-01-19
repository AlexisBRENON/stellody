/**
  * @file analysis.c
  * @brief Fichier d'implémentation du module \em Analyse.
  *
  * @author Alexis BRENON in STELLODY TEAM
  */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include <gtk/gtk.h>

#if defined(__linux)
#include <fmodex/fmod.h>
#endif
#if defined (__APPLE__) || defined (_WIN32)
#include <fmod.h>
#endif

#include "gui.h"
#include "analyzed_track.h"
#include "analyzed_tracks.h"
#include "preferences.h"
#include "player.h"
#include "analysis.h"
#include "gui_data.h"
#include "player_data.h"
#include "opengl_drawing.h"


#define fHERTZSTEP 43.06640625 /**< Ecart entre 2 raies du spectre d'amplitude */


/* ********************************************************************* */
/*                            Fonctions privées                          */
/* ********************************************************************* */

static int analysisAdd (const char* strPath, gpointer* pData);

/* ********************************************************************* */
/*                                                                       */
/*                     Fonctions relatives à l'analyse                   */
/*                                                                       */
/* ********************************************************************* */


int analysisAnalyze (FMOD_CHANNEL* pChannel,
                     float fAnalysisRate,
                     AnalyzedTrack* pTrack,
                     int iAnalyzingCounter)
{
	unsigned int uiPosition;
	unsigned int uiNewPosition;

/* ********************************************************************* */
/* ********************************************************************* */

	fprintf(stdout, "\tAnalyse n°%d...\n", iAnalyzingCounter);

	FMOD_Channel_GetPosition(pChannel,
							&uiPosition,
							FMOD_TIMEUNIT_MS);
	FMOD_Channel_SetPosition(pChannel,
							uiPosition+fAnalysisRate,
							FMOD_TIMEUNIT_MS);
	FMOD_Channel_GetPosition(pChannel,
							&uiNewPosition,
							FMOD_TIMEUNIT_MS);


	/* Si on ne peux plus incrémenter, on arrête */


	if (uiNewPosition == uiPosition)
	{
		FMOD_Channel_Stop(pChannel);

		return EXIT_SUCCESS;
	}

    return EXIT_SUCCESS;
}

void analysisAddTrackToAnalyze (char* strFilename, gpointer* pData)
{
/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

	GuiData* psGuiData = pData[2];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

    char* strExtension;
    int iLength;
    int i;

/* ********************************************************************* */
/* ********************************************************************* */

	fprintf(stdout, "Ajout d'un fichier à la liste d'analyse.\n");

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
        analysisAdd(strFilename, pData);
    }
    else
    {
    	GtkBuilder* psMainBuilder = NULL;
    	GtkWidget* pErrorDialog;
		GtkWidget* pParent = NULL;

		fprintf(stderr, "Ajout impossible du fichier %s.\n\
Fichier non pris en charge (extension invalide).\n\n", strFilename);

    	psMainBuilder = guiDataGetBuilder(psGuiData, MAIN);
    	pParent = GTK_WIDGET(gtk_builder_get_object(psMainBuilder,
										"Stellody_Window"));

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

    g_free(strFilename);
}


static int analysisAdd (const char* strPath, gpointer* pData)
{

/* ********************************************************************* */
/* Données habituelles                                                   */
/* ********************************************************************* */

    AnalyzedTracks* psTracks = pData[0];
    PlayerData* psPlayerData = pData[3];

/* ********************************************************************* */
/* Données annexes                                                       */
/* ********************************************************************* */

    LinkedList* psAnalyzeList = NULL;
    AnalyzedTrack* psTrack = NULL;

    int iCheckAnalyze = 0;

/* ********************************************************************* */
/* ********************************************************************* */

	fprintf(stdout,
			"Ajout d'un morceau \"%s\" à la liste d'analyse.\n", strPath);

    psAnalyzeList = playerDataGetAnalyzingList(psPlayerData);
    psTrack = analyzedTrackCreate();
    analyzedTrackSetPath(psTrack, strPath);

    /* On ajoute le morceau à la liste d'analyse (et la liste complète
    en cas de sauvegarde). */
    linkedListAppend(psAnalyzeList, psTrack);
    analyzedTracksInsertTrack(psTracks,
                              psTrack);

    /* On crée le timer sur la vérification d'analyse s'il n'existe pas */
    iCheckAnalyze = playerDataGetCheckForAnalyze(psPlayerData);
    if (iCheckAnalyze == 0)
    {
        iCheckAnalyze =
            g_timeout_add_seconds(2,
                                  (GSourceFunc) guiTimeoutCheckForAnalyze,
                                  pData);
        playerDataSetCheckForAnalyze(psPlayerData, iCheckAnalyze);

        /* On appelle la fonction immédiatement */
        guiTimeoutCheckForAnalyze(pData);
    }

    return EXIT_SUCCESS;
}


