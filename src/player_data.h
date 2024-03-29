/**
  * @brief Fichier d'entête du module @em PLAYER_DATA.
  *
  *
  * @author Alexis BRENON in STELLODY TEAM
  * @file player_data.h
  */

#ifndef __PLAYER_DATA_H__
#define __PLAYER_DATA_H__

#if defined(__linux)
#include <fmodex/fmod.h>
#endif
#if defined (__APPLE__) || defined (_WIN32)
#include <fmod.h>
#endif

#include "linked_list.h"


/* ********************************************************************* */
/*                                                                       */
/*                        Définitions de types                           */
/*                                                                       */
/* ********************************************************************* */


/**
  * @struct sPlayerData
  * @brief Structure contenant les différentes données nescessaire à la
  lecture et à l'analyse.
  */
struct sPlayerData
{
	FMOD_SYSTEM* pFmodSystem; /**< Contexte FMod */
	FMOD_CHANNEL* pFmodPlayingChannel; /**< Canal de lecture */
	FMOD_CHANNEL* pFmodAnalyzingChannel; /**< Canal d'analyse */

	int iAnalyzingLoopCounter; /**< Compteur de boucle d'analyse (permet de calculer la moyenne) */

	LinkedList* pAnalyzeList; /**< Liste des morceaux à analyser */
	int iCheckForAnalyze; /**< ID du timer de vérification d'analyse */
};
/**
  * @typedef PlayerData
  * @brief Redéfinifiton de la structure.
  */
typedef struct sPlayerData PlayerData;



/* ********************************************************************* */
/*                                                                       */
/*                               Fonctions                               */
/*                                                                       */
/* ********************************************************************* */

/**
  * @fn PlayerData* playerDataCreate (void)
  * @brief Création et initialisation de la structure.
  *
  * @return Structure nouvellement allouée.
  */
PlayerData* playerDataCreate (void);

/**
  * @fn int playerDataDestroy (PlayerData** ppData)
  * @brief Détruit une structure.
  *
  * @param[in,out] ppData Pointeur sur pointeur de structure
  * @return EXIT_SUCCESS
  */
int playerDataDestroy (PlayerData** ppData);

/**
  * @fn int playerDataInit (PlayerData* pData)
  * @brief Initialise la strucutre.
  *
  * @param[in,out] pData Pointeur sur structure à initialiser
  * @return EXIT_SUCCESS
  */
int playerDataInit (PlayerData* pData);
/**
  * @fn int playerDataRelease (PlayerData* pData)
  * @brief Libère la structure.
  *
  * @param[in,out] pData Adresse de la structure
  * @return EXIT_SUCCESS
  */
int playerDataRelease (PlayerData* pData);

/**
  * @fn FMOD_SYSTEM* playerDataGetSystem (const PlayerData* pData)
  * @brief Accesseur
  *
  * @param[in] pData Adresse de la structure
  * @return Le systeme FMod
  */
FMOD_SYSTEM* playerDataGetSystem (const PlayerData* pData);

/**
  * @fn FMOD_CHANNEL* playerDataGetPlayingChannel (PlayerData* pData);
  * @brief Permet de récupérer le cannal de lecture.
  *
  * @param[in,out] pData Adresse de la structure
  * @return Pointeur sur le canal de lecture
  */
FMOD_CHANNEL* playerDataGetPlayingChannel (PlayerData* pData);

/**
  * @fn int playerDataSetPlayingChannel (PlayerData* pData,
								FMOD_CHANNEL* psPlayingChannel);
  * @brief Mutateur
  *
  * @param[in,out] pData Données du player
  * @param[in,out] psPlayingChannel Nouveau canal de lecture
  * @return EXIT_SUCCESS
  */
int playerDataSetPlayingChannel (PlayerData* pData,
								FMOD_CHANNEL* psPlayingChannel);
/**
  * @fn FMOD_CHANNEL* playerDataGetAnalyzingChannel (PlayerData* pData);
  * @brief Permet de récupérer le cannal d'analyse.
  *
  * @param[in,out] pData Adresse de la structure
  * @return Pointeur sur le canal d'analyse
  */
FMOD_CHANNEL* playerDataGetAnalyzingChannel (PlayerData* pData);

/**
  * @fn int playerDataSetAnalyzingChannel (PlayerData* pData,
								FMOD_CHANNEL* psAnalyzingChannel);
  * @brief Mutateur
  *
  * @param[in,out] pData Données du player
  * @param[in,out] psAnalyzingChannel Nouveau canal de lecture
  * @return EXIT_SUCCESS
  */
int playerDataSetAnalyzingChannel (PlayerData* pData,
								FMOD_CHANNEL* psAnalyzingChannel);

/**
  * @fn int playerDataGetAnalyzingLoopCounter (const PlayerData* pData)
  * @brief Accesseur sur le compteur de boucle d'analyse.
  *
  * @param[in] pData Adresse de la structure
  * @return La valeur du compteur de boucle.
  */
int playerDataGetAnalyzingLoopCounter (const PlayerData* pData);
/**
  * @fn int playerDataSetAnalyzingLoopCounter (PlayerData* pData,
									int iValue)
  * @brief Mutateur sur le compteur de boucle d'analyse.
  *
  * @param[in,out] pData Adresse de la structure
  * @param[in] iValue Valeur à affecter
  * @return EXIT_SUCCESS
  */
int playerDataSetAnalyzingLoopCounter (PlayerData* pData,
									int iValue);

/**
  * @fn int playerDataGetCheckForAnalyze (const PlayerData* pData)
  * @brief Accesseur sur le vérificateur d'analyse.
  *
  * @param[in] pData Adresse de la structure
  * @return La valeur du booléen
  */
int playerDataGetCheckForAnalyze (const PlayerData* pData);
/**
  * @fn int playerDataSetCheckForAnalyze (PlayerData* pData,
								int iValue)
  * @brief Mutateur sur le vérificateur d'analyse.
  *
  * @param[in,out] pData Adresse de la structure.
  * @param[in] iValue Valeur à affecter
  * @return EXIT_SUCCESS
  */
int playerDataSetCheckForAnalyze (PlayerData* pData,
								int iValue);

/**
  * @fn LinkedList* playerDataGetAnalyzingList (const PlayerData* pData)
  * @brief Accesseur
  *
  * @param[in] pData Adresse de la structure.
  * @return La liste d'analyse.
  */
LinkedList* playerDataGetAnalyzingList (const PlayerData* pData);





















#endif /* __PLAYER_DATA_H__ */
