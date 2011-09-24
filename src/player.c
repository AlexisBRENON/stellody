/**
  * @brief Fichier d'implémentation du module \em Player
  *
  *
  * @author Alexis BRENON in STELLODY TEAM
  * @file player.c
  */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#if defined(__linux)
#include <fmodex/fmod.h>
#endif
#if defined (__APPLE__) || defined (_WIN32)
#include <fmod.h>
#endif

#include "player.h"


FMOD_BOOL playerIsPlaying(FMOD_CHANNEL* pChannel)
{
	FMOD_BOOL bIsPlaying = 0;

	if (pChannel != NULL)
	{
		FMOD_Channel_IsPlaying(pChannel, &bIsPlaying);
	}

	return bIsPlaying;
}

FMOD_BOOL playerIsPaused(FMOD_CHANNEL* pChannel)
{
	FMOD_BOOL bIsPaused = 0;

	if (playerIsPlaying(pChannel))
	{
		FMOD_Channel_GetPaused(pChannel, &bIsPaused);
	}

	return bIsPaused;
}

int playerSetVolume (FMOD_CHANNEL* pChannel,
					float fValue)
{
	assert(pChannel != NULL);
	assert (fValue >= 0.0 && fValue <= 1.0);

	FMOD_Channel_SetVolume(pChannel,
							0.0);

	return EXIT_SUCCESS;
}

int playerPlayTrack (char* strPath,
					FMOD_SYSTEM* pFmodContext,
					FMOD_CHANNEL** ppPlayingChannel,
					FMOD_BOOL bPaused,
					unsigned int uiStartingPerCent,
					unsigned int* puiTrackLength,
					char** strSinger, char** strTitle)
{
	FMOD_SOUND* pSound = NULL;
	FMOD_TAG Tag;

	assert(FMOD_System_CreateSound(pFmodContext,
							strPath,
							FMOD_CREATESTREAM | FMOD_SOFTWARE,
							NULL,
							&pSound) == FMOD_OK);

	FMOD_Sound_GetLength(pSound, puiTrackLength, FMOD_TIMEUNIT_MS);

	FMOD_Sound_GetTag(pSound, "TITLE", 0, &Tag);
	*strTitle = (char*)Tag.data;
	FMOD_Sound_GetTag(pSound, "ARTIST", 0, &Tag);
	*strSinger = (char*) Tag.data;

	/* On lance la lecture à partir du moment choisi */
	assert(FMOD_System_PlaySound(pFmodContext,
						FMOD_CHANNEL_FREE, pSound, 1 /* TRUE */,
						ppPlayingChannel) == FMOD_OK);
	FMOD_Channel_SetPosition(*ppPlayingChannel,
					*puiTrackLength*(uiStartingPerCent)/100,
					FMOD_TIMEUNIT_MS);
	FMOD_Channel_SetPaused(*ppPlayingChannel,
							0 /* FALSE */);

	return EXIT_SUCCESS;
}

int playerStopTrack (FMOD_CHANNEL* psPlayingChannel)
{
	FMOD_Channel_Stop(psPlayingChannel);
	psPlayingChannel = NULL;

	return EXIT_SUCCESS;
}


int playerSetPaused (FMOD_CHANNEL* psChannel,
					FMOD_BOOL bPause)
{
	assert (psChannel != NULL);

	FMOD_Channel_SetPaused(psChannel, bPause);

	return EXIT_SUCCESS;
}


