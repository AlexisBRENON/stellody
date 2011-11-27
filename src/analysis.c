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
/*                                                                       */
/*                     Fonctions relatives à l'analyse                   */
/*                                                                       */
/* ********************************************************************* */


int analysisAnalyze (FMOD_CHANNEL* pChannel,
                     float fAnalysisRate,
                     AnalyzedTrack* pTrack,
                     int iAnalyzingCounter)
{
   float farrWave[1024] = {0};
   FMOD_RESULT iWave = 0;

/* ********************************************************************* */
/* ********************************************************************* */

	iWave = FMOD_Channel_GetWaveData(
						pChannel,
						farrWave,
						1024,
						0
						);
	if (iWave != 0)
	{
		iWave = FMOD_Channel_GetWaveData(
							pChannel,
							farrWave,
							1024,
							1
							);

	}



    return EXIT_SUCCESS;
}

void analysisAddTracksToAnalyzed (char* strFilename, gpointer* pData)
{
	GuiData* psGuiData = pData[2];

    char* strExtension;
    int iLength;
    int i;

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
    	GtkBuilder* psMainBuilder = NULL;
    	GtkWidget* pErrorDialog;
		GtkWidget* pParent = NULL;

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


int analysisTrack (const char* strPath, gpointer* pData)
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


