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

/* ********************************************************************* */
/*                                                                       */
/*                     Fonctions relatives à l'analyse                   */
/*                                                                       */
/* ********************************************************************* */


int analysisAnalyze (FMOD_CHANNEL* pChannel,
					float fAnalysisRate,
					AnalyzedTrack* pTrack,
					int* piAnalyzingCounter)
{
	int i = 0;
	unsigned int uiPosition = 0;
	float pfSpectrumValueLeft[iNUMVALUES];
	float pfSpectrumValueRight[iNUMVALUES];
	float fAverage = 0;
	float fOldAverage = 0;
	float fMedian = 0;
	float fOldMedian = 0;

/* ********************************************************************* */
/* ********************************************************************* */

	/* On se déplace d'un certains temps dépendant du taux d'analyse */

	FMOD_Channel_GetPosition(pChannel,
							&uiPosition,
							FMOD_TIMEUNIT_MS);
	FMOD_Channel_SetPosition(pChannel,
							uiPosition+fAnalysisRate,
							FMOD_TIMEUNIT_MS);

/* ********************************************************************* */
/* ********************************************************************* */

	/* On récupère le spectre d'amplitude du temps passé sur chaque canal
	et on en fait la moyenne pour la stéréo.*/

	FMOD_Channel_GetSpectrum(pChannel,
							pfSpectrumValueLeft, iNUMVALUES,
							1, FMOD_DSP_FFT_WINDOW_RECT);
	FMOD_Channel_GetSpectrum(pChannel,
							pfSpectrumValueLeft, iNUMVALUES,
							0, FMOD_DSP_FFT_WINDOW_RECT);
	for (i = 0; i < iNUMVALUES; i++)
	{
		pfSpectrumValueLeft[i] =
			(pfSpectrumValueLeft[i] + pfSpectrumValueRight[i])/2;
	}

/* ********************************************************************* */
/* ********************************************************************* */

	/* Calcul de la moyenne immédiate */

	for (i = 0; i < iNUMVALUES; i++)
	{
		fAverage = fAverage+pfSpectrumValueLeft[i];
	}
	fAverage = fAverage/iNUMVALUES;

	/* Calcul de la moyenne totale */

	fOldAverage = analyzedTrackGetFrequenciesAverage(pTrack);
	analyzedTrackSetFrequenciesAverage(pTrack,
									(fOldAverage*(*piAnalyzingCounter)+
									fAverage)/((*piAnalyzingCounter)+1));

/* ********************************************************************* */
/* ********************************************************************* */

	/* Calcul de la médiane immédiate */

	fMedian = pfSpectrumValueLeft[iNUMVALUES/2];

	/* Calcul de la médiane totale */

	fOldMedian = analyzedTrackGetFrequenciesMedian(pTrack);
	analyzedTrackSetFrequenciesMedian(pTrack,
									(fOldMedian*(*piAnalyzingCounter)+
									fMedian)/((*piAnalyzingCounter)+1));

/* ********************************************************************* */
/* ********************************************************************* */

	/* Stocke l'ensemble du tableau après les premières secondes d'analyse*/
	if ((*piAnalyzingCounter) == 10)
	{
		analyzedTrackSetFrequenciesValues(pTrack,
										pfSpectrumValueLeft);
	}

/* ********************************************************************* */
/* ********************************************************************* */

	return EXIT_SUCCESS;
}

int analysisTrack (const char* strPath, gpointer* pData)
{
	AnalyzedTrack* psTrack = NULL;

	/* On ajoute le morceau à la liste d'analyse (et à l'arbre en cas de
	sauvegarde). */

	psTrack = analyzedTrackCreate();
	analyzedTrackSetPath(psTrack, strPath);
	pData[ANALYZELIST] = g_list_append((GList*) pData[ANALYZELIST],
										psTrack);
	analyzedTracksInsertTrack((AnalyzedTracks*) pData[ANALYZED_TRACKS],
								psTrack);

	/* On crée le timer sur la vérification d'analyse s'il n'existe pas */

	if (*((int*) pData[CHECKANALYZE]) == 0)
	{
		*((int*) pData[CHECKANALYZE]) =
			g_timeout_add_seconds(2,
							(GSourceFunc) guiTimeoutCheckForAnalyze,
							pData);
	}

	return EXIT_SUCCESS;
}


