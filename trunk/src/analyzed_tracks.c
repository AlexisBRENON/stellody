/**
  * @brief Implémentation des morceaux analysés
  *
  * Fichier d'implémentation des morceaux analysés.
  * @author Alexis BRENON in STELLODY TEAM
  * @file analyzed_tracks.c
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


AnalyzedTracks* analyzedTracksCreate(void)
{
	return g_tree_new_full((GCompareDataFunc) analyzedTrackCompare, NULL,
							(GDestroyNotify) free,
							(GDestroyNotify) analyzedTrackDestroy);
}
AnalyzedTracks* analyzedTracksCreateFromFile (const GKeyFile* ppsContext[])
{
	/** @todo Implémenter la fonction analyzedTracksCreateFromFile() avec
	les fonctions du module files.*/

	return EXIT_SUCCESS;
}
int analyzedTracksDestroy(AnalyzedTracks** ppsTracks)
{
	assert(ppsTracks != NULL &&
			*ppsTracks != NULL);

	g_tree_destroy(*ppsTracks);
	*ppsTracks = NULL;

	return EXIT_SUCCESS;
}


int analyzedTracksInsertTrack(AnalyzedTracks* psTracks,
							AnalyzedTrack* psTrack)
{
	int iKey;
	AnalyzedTrack* psTrackInTree;

	assert(psTracks != NULL);
	assert(psTrack != NULL);

	/* On vérifie que la clé qu'on s'apprette à entrer n'existe pas déja */
	iKey = analyzedTrackGetTID(psTrack);
	psTrackInTree = g_tree_search(psTracks,
								(GCompareFunc) analyzedTrackCompare,
								&iKey);

	while (psTrackInTree != NULL)
	{
		/* Si elle existe déja, on incrémente l'identification et on refait
		le test */
		iKey++;
		psTrackInTree = g_tree_search(psTracks,
									(GCompareFunc) analyzedTrackCompare,
									&iKey);
	}

	/* Lorsque qu'on a trouvé une clé inutilisée, on la sauvegarde et on
	stocke le morceau dans l'arbre */
	analyzedTrackSetTID(psTrack, iKey);
	g_tree_insert(psTracks, &iKey, psTrack);

	return EXIT_SUCCESS;
}

/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */

