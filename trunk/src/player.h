/**
  * @brief Fichier d'entête du module \em Player
  *
  *
  * @author Alexis BRENON in STELLODY TEAM
  * @file player.h
  */

#ifndef __PLAYER_H__
#define __PLAYER_H__

#if defined(__linux)
#include <fmodex/fmod.h>
#endif
#if defined (__APPLE__) || defined (_WIN32)
#include <fmod.h>
#endif


FMOD_BOOL playerIsPlaying (FMOD_CHANNEL* pChannel);

FMOD_BOOL playerIsPaused (FMOD_CHANNEL* pChannel);

int playerSetVolume (FMOD_CHANNEL* pChannel, float fValue);

int playerPlayTrack (char* strPath,
					FMOD_SYSTEM* pFmodContext,
					FMOD_CHANNEL** ppPlayingChannel,
					FMOD_BOOL bPaused,
					unsigned int uiStartingPerCent,
					unsigned int* puiTrackLength,
					char** strSinger, char** strTitle);
#endif /* __PLAYER_H__ */
