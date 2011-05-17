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
#include "analysis.h"



#define iNUMVALUES 512


/* ********************************************************************* */
/*                                                                       */
/*                           Fonctions privées                           */
/*                                                                       */
/* ********************************************************************* */

static gboolean analysisSetValues (gpointer pData[])
{
	gboolean bIsPlaying = FALSE;
	AnalyzedTrack* psTrack = NULL;
	float pfSpectrumValue[iNUMVALUES];
	int i;
	float fAnalysisRate;
	unsigned int uiPosition;
	float fOldAverage = 0;
	float fOldMedian = 0;
	float fAverage = 0;
	float fMedian = 0;
	GtkWidget* psStatusBar = NULL;
	const char* strConstFileName = NULL;
	char* strFileName = NULL;
	char* strStatusBarMessage = NULL;

	FMOD_Channel_IsPlaying((FMOD_CHANNEL*)pData[ANALYZING_CHANNEL],
								&bIsPlaying);

	psTrack = (AnalyzedTrack*) (g_list_first(
								(GList*) pData[ANALYZELIST]))->data;

	/* On récupère le taux d'analyse */
	fAnalysisRate = (float) preferencesGetAnalysisRate(
										(Preferences*) pData[PREFERENCES]);
	/* Au maximum (100%) on analysera toutes les 25ms le morceau en
	vitesse normale.
	Au quart (25%) on analysera toutes les 25ms le morceau en vitesse
	x4. */
	fAnalysisRate = 25/(fAnalysisRate/100.0);


	/* On informe l'utilisateur que l'analyse est finie */
	strConstFileName = analyzedTrackGetPath(psTrack);
	strFileName = strrchr(strConstFileName, '/');

	/* On récupère la barre d'état */
	psStatusBar = GTK_WIDGET(gtk_builder_get_object(
										(GtkBuilder*) pData[MAIN_BUILDER],
										"Stellody_StatusBar"));
	/* On efface le message présent */
	gtk_statusbar_pop(GTK_STATUSBAR(psStatusBar), 1);


	if (bIsPlaying == TRUE)
	{
		strStatusBarMessage = (char*) malloc((
									strlen("Analyse du fichier ")+
									strlen(strFileName)+
									strlen(" en cours..."))
											*sizeof(char));
		sprintf(strStatusBarMessage,
				"Analyse du fichier %s en cours...", &(strFileName[1]));
		gtk_statusbar_push(GTK_STATUSBAR(psStatusBar), 1,
							strStatusBarMessage);

		FMOD_Channel_GetPosition((FMOD_CHANNEL*)pData[ANALYZING_CHANNEL],
								&uiPosition,
								FMOD_TIMEUNIT_MS);
		FMOD_Channel_SetPosition((FMOD_CHANNEL*)pData[ANALYZING_CHANNEL],
								uiPosition+fAnalysisRate,
								FMOD_TIMEUNIT_MS);

		FMOD_Channel_GetSpectrum((FMOD_CHANNEL*)pData[ANALYZING_CHANNEL],
								pfSpectrumValue, iNUMVALUES,
								1, FMOD_DSP_FFT_WINDOW_RECT);

		/* On regarde les valeurs retournées (on ignore les bords) */
		for (i = 1; i < 13; i++)
		{
			/* On uniformise les valeurs en passant aux décibels */
			pfSpectrumValue[i] = (float) log10(pfSpectrumValue[i]);
			pfSpectrumValue[i] = 10.0*pfSpectrumValue[i]*2.0;

			printf("[%d] : %f\n", i, pfSpectrumValue[i]);

			fAverage = fAverage + pfSpectrumValue[i];
		}
		fAverage = fAverage/12;
		fMedian = pfSpectrumValue[6];

		fOldAverage = analyzedTrackGetFrequenciesAverage(psTrack);
		fOldMedian = analyzedTrackGetFrequenciesMedian(psTrack);

		analyzedTrackSetFrequenciesAverage(psTrack,
											fOldAverage+fAverage);
		analyzedTrackSetFrequenciesMedian(psTrack,
											fOldMedian+fMedian);
	}
	else /* Si ça ne joue plus, c'est que l'analyse est finie. */
	{
		int iTIDMax = 0;
		int iTIDMin = 0;
		int iTID = 0;

		iTID = analyzedTrackGetTID(psTrack);

/* ********************************************************************* */

		/* On assigne les nouvelles valeur au morceau */
		fOldAverage = analyzedTrackGetFrequenciesAverage(psTrack);
		fOldMedian = analyzedTrackGetFrequenciesMedian(psTrack);
		fAverage = fOldAverage/fAnalysisRate;
		fMedian = fOldMedian/fAnalysisRate,

		analyzedTrackSetFrequenciesAverage(psTrack,
											fAverage);
		analyzedTrackSetFrequenciesMedian(psTrack,
											fMedian);
		analyzedTrackSetTID(psTrack, (fAverage+fMedian)*(-100));
		analyzedTrackSetAnalyzed(psTrack, 1);

/* ********************************************************************* */

		/* On supprime le morceau de la liste d'analyse */
		g_tree_steal((GTree*) pData[ANALYZED_TRACKS],
					&iTID);
		analyzedTracksInsertTrack((AnalyzedTracks*) pData[ANALYZED_TRACKS],
									psTrack);
		pData[ANALYZELIST] = g_list_remove((GList*) pData[ANALYZELIST],
											psTrack);

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

		strStatusBarMessage = (char*) malloc((
									strlen("Le fichier ")+
									strlen(strFileName)+
									strlen(" vient d'être analysé."))
											*sizeof(char));
		sprintf(strStatusBarMessage,
				"Le fichier %s vient d'être analysé.", &(strFileName[1]));

		gtk_statusbar_push(GTK_STATUSBAR(psStatusBar), 1,
						 strStatusBarMessage);

	}

	free(strStatusBarMessage);

	return bIsPlaying;
}


/* ********************************************************************* */
/*                                                                       */
/*                     Fonctions relatives à l'analyse                   */
/*                                                                       */
/* ********************************************************************* */


int analysisTrack (const char* strPath, gpointer* pData)
{
	AnalyzedTrack* psTrack = NULL;
	FMOD_BOOL bIsPlaying = FALSE;


	psTrack = analyzedTrackCreate();
	analyzedTrackSetPath(psTrack, strPath);
	pData[ANALYZELIST] = g_list_append((GList*) pData[ANALYZELIST],
										psTrack);
	analyzedTracksInsertTrack((AnalyzedTracks*) pData[ANALYZED_TRACKS],
								psTrack);

	if (pData[ANALYZING_CHANNEL] != NULL) /* Si le canal à déjà été crée */
	{
		/* On regarde s'il est en train de jouer. */
		FMOD_Channel_IsPlaying((FMOD_CHANNEL*)pData[ANALYZING_CHANNEL],
								&bIsPlaying);
	}

	if (bIsPlaying == FALSE) /* S'il n'est pas en train de jouer */
	{
		FMOD_SOUND* psSound = NULL;

		/* On crée le son à analyser */
		FMOD_System_CreateSound((FMOD_SYSTEM*) pData[FMOD_CONTEXT],
								strPath,
								FMOD_CREATESTREAM | FMOD_SOFTWARE,
								NULL,
								&psSound);

		/* On lance le son (en pause) */
		FMOD_System_PlaySound((FMOD_SYSTEM*) pData[FMOD_CONTEXT],
								FMOD_CHANNEL_FREE,
								psSound,
								FALSE,
								(FMOD_CHANNEL**) &pData[ANALYZING_CHANNEL]);
		/* On coupe le son, le morceau doit être joué mais pas entendu */
		FMOD_Channel_SetVolume((FMOD_CHANNEL*) pData[ANALYZING_CHANNEL],
								0.0);
		/* On crée un time_out sur la fonction qui analysera et stockera
		les données d'analyse */
		g_idle_add((GSourceFunc) analysisSetValues,
					pData);
	}
	else /* Si un morceau est déjà en cours d'analyse */
	{
		if (*((int*) pData[CHECKANALYZE]) == 0)
		{
			*((int*) pData[CHECKANALYZE]) = g_timeout_add_seconds(2,
									(GSourceFunc) analysisCheckNewAnalyze,
									pData);
		}
	}

	return EXIT_SUCCESS;

}


gboolean analysisCheckNewAnalyze (gpointer pData[])
{
	FMOD_BOOL bIsPlaying;

	/* Si la liste d'analyse est vide */
	if (pData[ANALYZELIST] == NULL)
	{
		GtkWidget* psStatusBar = NULL;

		/* On récupère la barre d'état */
		psStatusBar = GTK_WIDGET(gtk_builder_get_object(
									(GtkBuilder*) pData[MAIN_BUILDER],
									"Stellody_StatusBar"));
		/* On efface le message présent */
		gtk_statusbar_pop(GTK_STATUSBAR(psStatusBar), 1);


		/* C'est fonction n'a plus lieu d'être appelée */
		return FALSE;
	}

	FMOD_Channel_IsPlaying((FMOD_CHANNEL*) pData[ANALYZING_CHANNEL],
							&bIsPlaying);
	/* Si le canal d'analyse est déja en train de jouer */
	if (bIsPlaying == TRUE)
	{
		/* On attend */
		return TRUE;
	}
	else /* Sinon */
	{
		FMOD_SOUND* psSound = NULL;
		AnalyzedTrack* psTrack = NULL;

		psTrack = (g_list_first((GList*) pData[ANALYZELIST]))->data;

		/* On crée le son à analyser */
		FMOD_System_CreateSound((FMOD_SYSTEM*) pData[FMOD_CONTEXT],
								analyzedTrackGetPath(psTrack),
								FMOD_CREATESTREAM | FMOD_SOFTWARE,
								NULL,
								&psSound);

		/* On lance le son (en pause) */
		FMOD_System_PlaySound((FMOD_SYSTEM*) pData[FMOD_CONTEXT],
								FMOD_CHANNEL_FREE,
								psSound,
								FALSE,
								(FMOD_CHANNEL**) &pData[ANALYZING_CHANNEL]);
		/* On coupe le son, le morceau doit être joué mais pas entendu */
		FMOD_Channel_SetVolume((FMOD_CHANNEL*) pData[ANALYZING_CHANNEL],
								0.0);
		FMOD_Channel_SetPaused((FMOD_CHANNEL*) pData[ANALYZING_CHANNEL],
								FALSE);

		/* On crée un time_out sur la fonction qui analysera et stockera
		les données d'analyse */
		g_idle_add((GSourceFunc) analysisSetValues,
					pData);
		return TRUE;
	}

}


