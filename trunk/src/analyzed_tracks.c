/**
  * @brief Implémentation des morceaux analysés
  *
  * Fichier d'implémentation des structures de morceaux analysés.
  * @author Alexis BRENON in STELLODY TEAM
  * @file analyzed_tracks.h
  */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "analyzed_tracks.h"


int analyzedTrackInit(AnalyzedTrack* psTrack, const char* strPath,
					float fAverage, float fMedian)
{
	assert (psTrack != NULL);
	assert (fAverage >= 0.0 && fAverage <= 1.0);
	assert (fMedian >= 0.0 && fMedian <= 1.0);

	/* Si on lui passe une chaine de caractère (même vide), on la copie.*/
	if (strPath != NULL)
	{
		psTrack->strPath = (char*) malloc(strlen(strPath)*sizeof(char));
		strcpy(psTrack->strPath, strPath);
	}
	else
	{
		psTrack->strPath = NULL;
	}
	psTrack->fFrequenciesAverage = fAverage;
	psTrack->fFrequenciesMedian = fMedian;

	return EXIT_SUCCESS;
}
int analyzedTrackRelease(AnalyzedTrack* psTrack)
{
	assert (psTrack != NULL);

	psTrack->fFrequenciesAverage = 0;
	psTrack->fFrequenciesMedian = 0;
	if (psTrack->strPath != NULL)
	{
		free(psTrack->strPath);
		psTrack->strPath = NULL;
	}

	return EXIT_SUCCESS;
}

AnalyzedTrack* analyzedTrackCreate(const char* strPath, float fAverage,
								float fMedian)
{
	AnalyzedTrack* psTrack = NULL;

	psTrack = (AnalyzedTrack*) malloc (sizeof(AnalyzedTrack));
	analyzedTrackInit(psTrack, strPath, fAverage, fMedian);

	return psTrack;
}
int analyzedTrackDestroy(AnalyzedTrack** ppsTrack)
{
	assert(ppsTrack != NULL);
	assert(*ppsTrack != NULL);

	analyzedTrackRelease(*ppsTrack);
	*ppsTrack = NULL;

	return EXIT_SUCCESS;
}

char* analyzedTrackGetPath (const AnalyzedTrack* psTrack)
{
	assert(psTrack != NULL);

	return psTrack->strPath;
}
int analyzedTrackSetPath (AnalyzedTrack* psTrack, const char* strNewPath)
{
	assert(psTrack != NULL);
	assert(strNewPath != NULL);

	if (psTrack->strPath != NULL)
	{
		free(psTrack->strPath);
	}
	psTrack->strPath = malloc(strlen(strNewPath)*sizeof(char));
	strcpy(psTrack->strPath, strNewPath);

	return EXIT_SUCCESS;
}

float analyzedTrackGetFrequenciesAverage (const AnalyzedTrack* psTrack)
{
	assert(psTrack != NULL);

	return psTrack->fFrequenciesAverage;
}
int analyzedTrackSetFrequenciesAverage (AnalyzedTrack* psTrack,
										float fvalue)
{
	assert(psTrack != NULL);
	assert(fvalue >= 0.0 && fvalue <= 1.0);

	psTrack->fFrequenciesAverage = fvalue;

	return EXIT_SUCCESS;
}

float analyzedTrackGetFrequenciesMedian (const AnalyzedTrack* psTrack)
{
	assert(psTrack != NULL);

	return psTrack->fFrequenciesMedian;
}
int analyzedTrackSetFrequenciesMedian (AnalyzedTrack *psTrack, float fvalue)
{
	assert(psTrack != NULL);
	assert(fvalue >= 0.0 && fvalue <= 1.0);

	psTrack->fFrequenciesMedian = fvalue;

	return EXIT_SUCCESS;
}
