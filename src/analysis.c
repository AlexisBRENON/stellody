/**
  * @file analysis.c
  * @brief Fichier d'implémentation du module \em Analyse.
  *
  * @author Alexis BRENON in STELLODY TEAM
  */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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


AnalyzedTrack* analysisTrack (const char* strPath, gpointer* pData)
{
	AnalyzedTrack* psTrack = NULL;
	FMOD_SOUND* psSound = NULL;
	FMOD_BOOL bIsPlaying = FALSE;
	int iAnalysisRate = 0;

	if (pData[ANALYZING_CHANNEL] != NULL)
	{
		FMOD_Channel_IsPlaying((FMOD_CHANNEL*)pData[ANALYZING_CHANNEL],
								&bIsPlaying);
	}

	if (bIsPlaying == FALSE)
	{
		FMOD_System_CreateSound((FMOD_SYSTEM*) pData[FMOD_CONTEXT],
								strPath,
								FMOD_CREATESTREAM | FMOD_SOFTWARE,
								NULL,
								&psSound);

		iAnalysisRate = preferencesGetAnalysisRate(
									(Preferences*) pData[PREFERENCES]);

		/* Au maximum (100%) on analysera toutes les 25ms */
		iAnalysisRate = (25*iAnalysisRate)/100;


	}

}
































