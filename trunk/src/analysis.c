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


static gboolean analysisSetValues (gpointer pData[])
{
	gboolean bIsPlaying = FALSE;
	AnalyzedTrack* psTrack = NULL;
	float pfSpectrumValue[64];
	int i;
	float fAnalysisRate;
	unsigned int uiPosition;

	FMOD_Channel_IsPlaying((FMOD_CHANNEL*)pData[ANALYZING_CHANNEL],
								&bIsPlaying);

	if (bIsPlaying == TRUE)
	{
		/* On récupère le taux d'analyse */
		fAnalysisRate = (float) preferencesGetAnalysisRate(
										(Preferences*) pData[PREFERENCES]);

		/* Au maximum (100%) on analysera toutes les 25ms le morceau en
		vitesse normale.
		Au quart (25%) on analysera toutes les 25ms le morceau en vitesse
		x4. */
		fAnalysisRate = 25/(fAnalysisRate/100.0);

		FMOD_Channel_GetPosition((FMOD_CHANNEL*)pData[ANALYZING_CHANNEL],
								&uiPosition,
								FMOD_TIMEUNIT_MS);
		FMOD_Channel_SetPosition((FMOD_CHANNEL*)pData[ANALYZING_CHANNEL],
								uiPosition+fAnalysisRate,
								FMOD_TIMEUNIT_MS);

		FMOD_Channel_GetSpectrum((FMOD_CHANNEL*)pData[ANALYZING_CHANNEL],
								pfSpectrumValue, 64,
								1, FMOD_DSP_FFT_WINDOW_RECT);

		/* Test... */
		for (i = 0; i < 64; i = i+2)
		{
			pfSpectrumValue[i] = (float) log10(pfSpectrumValue[i]);
			printf("décibel %d : %.2f\n", i,
							10.0*pfSpectrumValue[i]*2.0);
		}
		printf("\n\n");
	}
	else /* Si ça ne joue plus, c'est que l'analyse est finie. */
	{
		psTrack = (AnalyzedTrack*) (g_list_first(
									(GList*) pData[ANALYZELIST]))->data;
		analyzedTrackSetAnalyzed(psTrack, 1);
		/* Set TID */
		analyzedTracksInsertTrack(pData[ANALYZED_TRACKS],
									psTrack);
		pData[ANALYZELIST] = g_list_remove((GList*) pData[ANALYZELIST],
											psTrack);

		printf("Analyse Terminée\n");
	}

	return bIsPlaying;
}


int analysisTrack (const char* strPath, gpointer* pData)
{
	AnalyzedTrack* psTrack = NULL;
	FMOD_SOUND* psSound = NULL;
	FMOD_BOOL bIsPlaying = FALSE;


	psTrack = analyzedTrackCreate();
	analyzedTrackSetPath(psTrack, strPath);
	pData[ANALYZELIST] = g_list_append((GList*) pData[ANALYZELIST],
										psTrack);

	if (pData[ANALYZING_CHANNEL] != NULL) /* Si le canal à déjà été crée */
	{
		/* On regarde s'il est en train de jouer. */
		FMOD_Channel_IsPlaying((FMOD_CHANNEL*)pData[ANALYZING_CHANNEL],
								&bIsPlaying);
	}

	if (bIsPlaying == FALSE) /* S'il n'est pas en train de jouer */
	{
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
		/*FMOD_Channel_SetVolume((FMOD_CHANNEL*) pData[ANALYZING_CHANNEL],
								0.0);*/
		FMOD_Channel_SetPaused((FMOD_CHANNEL*) pData[ANALYZING_CHANNEL],
								FALSE);

		g_idle_add((GSourceFunc) analysisSetValues,
					pData);
	}
	else /* Si un morceau est déjà en cours d'analyse */
	{
		/* On ne fait rien */
	}

	return EXIT_SUCCESS;

}
































