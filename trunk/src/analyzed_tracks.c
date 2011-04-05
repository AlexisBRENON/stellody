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

char* analyzedTrackGetPlace (const AnalyzedTrack* psTrack)
{
	assert(psTrack != NULL);

	return psTrack->strPlace;
}
int analyzedTrackSetPlace (AnalyzedTrack* psTrack, const char* strPath)
{
	assert(psTrack != NULL);
	assert(strPath != NULL);

	if (psTrack->strPlace != NULL)
	{
		free(psTrack->strPlace);
	}
	psTrack->strPlace = malloc(strlen(strPath)*sizeof(char));
	strcpy(psTrack->strPlace, strPath);

	return EXIT_SUCCESS;
}
