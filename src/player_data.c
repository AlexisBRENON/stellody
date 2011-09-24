/**
  * @brief Fichier d'implémentation du module @em PLAYER_DATA.
  *
  *
  * @author Alexis BRENON in STELLODY TEAM
  * @file player_data.c
  */

#include <stdlib.h>
#include <assert.h>

#if defined(__linux)
	#include <fmodex/fmod.h>
#endif
#if defined (__APPLE__) || defined (_WIN32)
	#include <fmod.h>
#endif

#include "player_data.h"



PlayerData* playerDataCreate()
{
	PlayerData* pData = NULL;

	pData = (PlayerData*) malloc(sizeof(PlayerData));

	playerDataInit(pData);

	return pData;
}

int playerDataDestroy(PlayerData** ppData)
{
	assert (ppData != NULL && *ppData != NULL);

	playerDataRelease(*ppData);
	free(*ppData); *ppData = NULL;

	return EXIT_SUCCESS;
}


int playerDataInit (PlayerData* pData)
{
	assert (pData != NULL);

	FMOD_System_Create(&(pData->pFmodSystem));
	FMOD_System_Init(pData->pFmodSystem, 1, FMOD_INIT_NORMAL, NULL);

	pData->pFmodAnalyzingChannel = NULL;
	pData->pFmodPlayingChannel = NULL;

	pData->iAnalyzingLoopCounter = 0;
	pData->iCheckForAnalyze = 0;

	pData->pAnalyzeList = linkedListCreate();

	return EXIT_SUCCESS;
}

int playerDataRelease (PlayerData* pData)
{
	int i = 0;

	assert (pData != NULL);

	FMOD_System_Release(pData->pFmodSystem);

	pData->iAnalyzingLoopCounter = 0;
	pData->iCheckForAnalyze = 0;

	while (linkedListRemoveIeme(pData->pAnalyzeList,
								i,
								NULL) == LIST_OK)
	{
		i++;
	}

	linkedListDestroy(&(pData->pAnalyzeList));

	return EXIT_SUCCESS;
}


FMOD_SYSTEM* playerDataGetSystem (const PlayerData* pData)
{
	assert (pData != NULL);

	return pData->pFmodSystem;
}

FMOD_CHANNEL* playerDataGetPlayingChannel (PlayerData* pData)
{
	assert (pData != NULL);

	return pData->pFmodPlayingChannel;
}


FMOD_CHANNEL* playerDataGetAnalyzingChannel (PlayerData* pData)
{
	assert (pData != NULL);

	return pData->pFmodAnalyzingChannel;
}


int playerDataGetAnalyzingLoopCounter (const PlayerData* pData)
{
	assert (pData != NULL);

	return pData->iAnalyzingLoopCounter;
}

int playerDataSetAnalyzingLoopCounter (PlayerData* pData,
									int iValue)
{
	assert (pData != NULL);
	assert (iValue >= 0);

	pData->iAnalyzingLoopCounter = iValue;

	return EXIT_SUCCESS;
}

int playerDataGetCheckForAnalyze(const PlayerData* pData)
{
	assert (pData != NULL);

	return pData->iCheckForAnalyze;
}

int playerDataSetCheckForAnalyze (PlayerData* pData,
								int iValue)
{
	assert (pData != NULL);
	assert (iValue >= 0);

	pData->iCheckForAnalyze = iValue;

	return EXIT_SUCCESS;
}


LinkedList* playerDataGetAnalyzingList (const PlayerData* pData)
{
	assert (pData != NULL);

	return pData->pAnalyzeList;
}
