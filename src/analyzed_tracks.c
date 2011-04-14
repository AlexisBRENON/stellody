/**
  * @brief Implémentation des morceaux analysés
  *
  * Fichier d'implémentation des morceaux analysés.
  * @author Alexis BRENON in STELLODY TEAM
  * @file analyzed_tracks.c
  * @todo Faire chaque fonction !!
  */

#include <stdlib.h>
#include <gtk/gtk.h>
#include <assert.h>

#include "analyzed_tracks.h"
#include "analyzed_track.h"

/* ********************************************************************* */
/*                                                                       */
/*         Fonctions relatives à la structure AnalyzedTracks             */
/*                                                                       */
/* ********************************************************************* */

int analyzedTracksInit(AnalyzedTracks* psTracks)
{
	assert(psTracks != NULL);

	return EXIT_SUCCESS;
}
int analyzedTracksInitFromFile (AnalyzedTracks* psTracks,
								const GKeyFile* ppsContext[])
{
	/** @todo Implémenter la fonction analyzedTracksInitFromFile() avec
	les fonctions du module files.*/
	return EXIT_SUCCESS;
}
int analyzedTracksRelease(AnalyzedTracks* psTracks)
{
	assert(psTracks != NULL);

	g_tree_foreach(psTracks, (GTraverseFunc) analyzedTrackReleaseFromTree,
					NULL);

	return EXIT_SUCCESS;
}


AnalyzedTracks* analyzedTracksCreateFromFile (const GKeyFile* ppsContext[])
{
	/** @todo Implémenter la fonction analyzedTracksCreateFromFile() avec
	les fonctions du module files.*/

	return EXIT_SUCCESS;
}


/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */

