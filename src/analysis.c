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
	int i = 0, j = 0;
	float fMax = 0;
	int iIndex = 0;
	unsigned int uiPosition = 0;
	unsigned int uiNewPosition = 0;
	float fAverage = 0;
	float fOldAverage = 0;
	float pfSpectrumValueLeft[iNUMVALUES] = {0};
	float pfSpectrumValueRight[iNUMVALUES] = {0};
	float pfRate[3] = {0};
	float* pfOldRate = NULL;
	float* pfSpectrumValues = NULL;
	float pfSum[3] = {0};
	float fTotalSum = 0;
	FMOD_RESULT iSpectrumLeftResult = 0;
	FMOD_RESULT iSpectrumRightResult = 0;

/* ********************************************************************* */
/* ********************************************************************* */

	/* On se déplace d'un certains temps dépendant du taux d'analyse */

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

/* ********************************************************************* */
/* ********************************************************************* */

	/* On récupère le spectre d'amplitude du temps passé sur chaque canal
	et on en fait la moyenne pour la stéréo.*/

	iSpectrumLeftResult = FMOD_Channel_GetSpectrum(pChannel,
							pfSpectrumValueLeft, iNUMVALUES,
							1, FMOD_DSP_FFT_WINDOW_RECT);
	iSpectrumRightResult = FMOD_Channel_GetSpectrum(pChannel,
							pfSpectrumValueRight, iNUMVALUES,
							0, FMOD_DSP_FFT_WINDOW_HAMMING);

	/* Si le son était stéréo, on fait la moyenne des deux canaux */

	if (iSpectrumLeftResult == FMOD_OK && iSpectrumRightResult == FMOD_OK)
	{
		pfSpectrumValues = (float*) malloc(iNUMVALUES*sizeof(float));

		for (i = 0; i < iNUMVALUES; i++)
		{
			pfSpectrumValues[i] =
				(pfSpectrumValueLeft[i] + pfSpectrumValueRight[i])/2;
		}
	}
	else if (iSpectrumLeftResult != FMOD_OK)
	{
		pfSpectrumValues = pfSpectrumValueRight;
	}
	else if (iSpectrumRightResult != FMOD_OK)
	{
		pfSpectrumValues = pfSpectrumValueLeft;
	}
	else
	{
		return EXIT_FAILURE;
	}

/* ********************************************************************* */
/* ********************************************************************* */

	/* Calcul de la valeur maximale et son indice */

	for (i = 0; i < iNUMVALUES; i++)
	{
		if (pfSpectrumValues[i] > fMax)
		{
			fMax = pfSpectrumValues[i];
			iIndex = i;
		}
	}

	/* On calcule la fréquence maximale moyenne */

	fOldAverage = analyzedTrackGetAverage(pTrack);
	fAverage = ((fOldAverage*iAnalyzingCounter)+
							(iIndex*fHERTZSTEP))/
							(iAnalyzingCounter+1);
	analyzedTrackSetAverage(pTrack,
							fAverage);

/* ********************************************************************* */
/* ********************************************************************* */

	pfOldRate = analyzedTrackGetRate(pTrack);

	for (i = 0; i < 3; i++)
	{
		for (j = i*(48/3); j < (i+1)*(48/3); j++)
		{
			pfSum[i] = pfSum[i]+pfSpectrumValues[j];
		}
		fTotalSum = fTotalSum+pfSum[i];
	}
	for (i = 0; i < 3; i++)
	{
		pfRate[i] = (pfOldRate[i]*iAnalyzingCounter+
							(pfSum[i]/fTotalSum))/(iAnalyzingCounter+1);
	}

	analyzedTrackSetRate(pTrack, pfRate);

/* ********************************************************************* */
/* ********************************************************************* */

	return EXIT_SUCCESS;
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


	/* On ajoute le morceau à la liste d'analyse (et la liste complète
	en cas de sauvegarde. */
	psAnalyzeList = playerDataGetAnalyzingList(psPlayerData);
	psTrack = analyzedTrackCreate();
	analyzedTrackSetPath(psTrack, strPath);
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
	}

	return EXIT_SUCCESS;
}


