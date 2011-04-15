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
#include "files.h"

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
								GKeyFile* ppsContext[])
{
	gsize ulNbTracks;
	gchar** strGroups;
	int iTID;
	char* strPath;
	float fAverage, fMedian;
	AnalyzedTrack* psTrack;

	int i;

	assert(psTracks != NULL);
	assert(ppsContext != NULL);

	psTrack = analyzedTrackCreate();

	/* Récupère l'ensemble des noms des groupes */
	strGroups = g_key_file_get_groups(ppsContext[DATA], &ulNbTracks);

	/* Chaque groupe correspond à un morceau. On les charge tous. */
	for (i = 0; strGroups[i] != NULL; i++)
	{
		/* On récupère toutes les données relatives au morceau */
		iTID = g_key_file_get_integer(ppsContext[DATA],
										strGroups[i], "iTID", NULL);
		strPath = g_key_file_get_string(ppsContext[DATA],
										strGroups[i], "strPath", NULL);
		fAverage = (float) g_key_file_get_double(ppsContext[DATA],
										strGroups[i], "fAverage", NULL);
		fMedian = (float) g_key_file_get_double(ppsContext[DATA],
										strGroups[i], "fMedian", NULL);
		/* On initialise le morceau avec les bonnes valeurs */
		analyzedTrackInitWithData(psTrack, iTID, strPath,
									fAverage, fMedian);
		/* On le stocke dans l'arbre */
		analyzedTracksInsertTrack(psTracks, psTrack);

		/* On libère ce qui est contenu dans le morceau (pour laisser la
		place au prochain chargement */
		analyzedTrackRelease(psTrack);
		free(strPath);
	}

	analyzedTrackDestroy(&psTrack);

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
AnalyzedTracks* analyzedTracksCreateFromFile (GKeyFile* ppsContext[])
{
	AnalyzedTracks* psTracks;

	assert(ppsContext != NULL);

	psTracks = analyzedTracksCreate();
	analyzedTracksInitFromFile(psTracks, ppsContext);

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

int analyzedTracksRemoveTrack(AnalyzedTracks* psTracks,
							const AnalyzedTrack* psTrack)
{
	assert(psTracks != NULL);
	assert(psTrack != NULL);

	g_tree_remove(psTracks, &(psTrack->iTID));

	return EXIT_SUCCESS;
}

const AnalyzedTrack* analyzedTracksGetConstTrack(
									AnalyzedTracks* psTracks,
									int iKey)
{
	assert(psTracks != NULL);

	return g_tree_search(psTracks, (GCompareFunc) analyzedTrackCompare,
						&iKey);
}

AnalyzedTrack* analyzedTracksGetTrack(AnalyzedTracks* psTracks,
									int iKey)
{
	assert (psTracks != NULL);

	return g_tree_search(psTracks, (GCompareFunc) analyzedTrackCompare,
						&iKey);
}


/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */



