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


/**
  * @fn FMOD_BOOL playerIsPlaying (FMOD_CHANNEL* pChannel)
  * @brief Permet de savoir si le canal est en train de jouer.
  *
  * @param[in,out] pChannel Canal étudié
  * @return 1/TRUE si le canal est utilisé, 0/FALSE sinon.
  */
FMOD_BOOL playerIsPlaying (FMOD_CHANNEL* pChannel);

/**
  * @fn FMOD_BOOL playerIsPaused (FMOD_CHANNEL* pChannel)
  * @brief Permet de savoir si le canal est en pause.
  *
  * @param[in,out] pChannel Canal étudié
  * @return 1/TRUE si le canal est en pause, 0/FALSE sinon.
  */
FMOD_BOOL playerIsPaused (FMOD_CHANNEL* pChannel);

/**
  * @fn int playerSetVolume (FMOD_CHANNEL* pChannel, float fValue)
  * @brief Permet de modifier le volume du canal.
  *
  * @param[in,out] pChannel Canal étudié
  * @param[in] fValue Valeur à affecter (0.0 muted, 1.0 max).
  * @return EXIT_SUCCESS si tout est OK.
  */
int playerSetVolume (FMOD_CHANNEL* pChannel, float fValue);

/**
  * @fn int playerPlayTrack (char* strPath,
					FMOD_SYSTEM* pFmodContext,
					FMOD_CHANNEL** ppPlayingChannel,
					FMOD_BOOL bPaused,
					unsigned int uiStartingPerCent,
					unsigned int* puiTrackLength,
					char** strSinger, char** strTitle);
  * @brief Lance la lecture du morceau pointé pat strPath.
  *
  * @param[in,out] strPath Chemin du morceau à lire
  * @param[in,out] pFmodContext Contexte FMOD
  * @param[in,out] ppPlayingChannel Canal sur lequel joué
  * @param[in] bPaused 1,TRUE pour démarrer en pause, 0/FALSE sinon
  * @param[in] uiStartingPerCent Pourcentage du morceau pour démarrer
  * @param[out] puiTrackLength Longueur du morceau en millisecondes
  * @param[out] strSinger Nom du chanteur/groupe
  * @param[out] strTitle Titre du morceau
  * @return EXIT_SUCCESS si tout est OK
  */
int playerPlayTrack (char* strPath,
					FMOD_SYSTEM* pFmodContext,
					FMOD_CHANNEL** ppPlayingChannel,
					FMOD_BOOL bPaused,
					unsigned int uiStartingPerCent,
					unsigned int* puiTrackLength,
					char** strSinger, char** strTitle);

/**
  * @fn int playerStopTrack (FMOD_CHANNEL* psPlayingChannel);
  * @brief Stoppe la lecture du morceau.
  *
  * @param[in] psPlayingChannel Canal de lecture.
  * @return EXIT_SUCCESS
  */
int playerStopTrack (FMOD_CHANNEL* psPlayingChannel);

/**
  * @fn int playerSetPaused (FMOD_CHANNEL* psChannel,
					FMOD_BOOL bPause);
  * @brief Modifie l'état de pause d'un canal.
  *
  * @param[in,out] psChannel Canal à modifier
  * @param[in] bPause L'état à assigner
  * @return EXIT_SUCCESS
  */
int playerSetPaused (FMOD_CHANNEL* psChannel,
					FMOD_BOOL bPause);

#endif /* __PLAYER_H__ */
