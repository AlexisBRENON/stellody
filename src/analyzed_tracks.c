/**
  * @brief Implémentation des morceaux analysés
  *
  * Fichier d'implémentation des morceaux analysés.
  * @author Alexis BRENON in STELLODY TEAM
  * @file analyzed_tracks.c
  */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <gtk/gtk.h>

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
	return g_tree_new_full((GCompareDataFunc) analyzedTrackDataCompare, NULL,
							(GDestroyNotify) free,
							NULL);
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

	printf("Libération\n");
	g_tree_destroy(*ppsTracks);
	*ppsTracks = NULL;
	printf("FAIT !\n");

	return EXIT_SUCCESS;
}


int analyzedTracksInsertTrack(AnalyzedTracks* psTracks,
							AnalyzedTrack* psTrack)
{
	int iTID = 0, iNewTID = 0;
	AnalyzedTrack* psTrackInTree = NULL;
	int *piKey = NULL;

	assert(psTracks != NULL);
	assert(psTrack != NULL);

	piKey = (int*) malloc(sizeof(int));

	/* On vérifie que la clé qu'on s'apprette à entrer n'existe pas déja */
	iTID = analyzedTrackGetTID(psTrack);
	iNewTID = iTID;
	psTrackInTree = g_tree_search(psTracks,
								(GCompareFunc) analyzedTrackCompare,
								&iTID);

	while (psTrackInTree != NULL)
	{
		/* Si elle existe déja, on incrémente l'identification et on refait
		le test */
		iNewTID++;
		psTrackInTree = g_tree_search(psTracks,
									(GCompareFunc) analyzedTrackCompare,
									&iNewTID);
	}

	/* Lorsque qu'on a trouvé une clé inutilisée, on la sauvegarde et on
	stocke le morceau dans l'arbre */
	if (iTID != iNewTID)
	{
		analyzedTrackSetTID(psTrack, iNewTID);
	}

	*piKey = iNewTID;
	g_tree_insert(psTracks, piKey, psTrack);

	return EXIT_SUCCESS;
}

AnalyzedTrack* analyzedTracksRemoveTrack(AnalyzedTracks* psTracks,
										int iKey)
{
	AnalyzedTrack* psTrack = NULL;

	assert(psTracks != NULL);
	assert(iKey > -1);

	psTrack = analyzedTracksGetTrack(psTracks, iKey);

	if (psTrack != NULL)
	{
		g_tree_remove(psTracks, &(psTrack->iTID));
	}

	return psTrack;
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


int analyzedTracksRegressionTest(void)
{
	AnalyzedTrack *psTrack1 = NULL, *psTrack2 = NULL,
					*psTrackSet = NULL;
	const AnalyzedTrack* psTrackGet;
	AnalyzedTracks *psTracks = NULL;

	printf("\n\t -- MODULE ANALYZED_TRACKS --\n");

	printf("Création de l'arbre...\n");
	psTracks = analyzedTracksCreate();
	assert(psTracks != NULL);
	printf("\tFAIT !!\n");

	printf("Ajout de morceaux à l'arbre...\n");
	psTrack1 = analyzedTrackCreateWithData(1, "test1", 0.0, 0.0);
	psTrack2 = analyzedTrackCreateWithData(1, "test2", 0.0, 0.0);
	printf("\t\tLes deux identifiants sont à 1...\n");
	assert(psTrack1 != NULL &&
			psTrack2 != NULL);
	analyzedTracksInsertTrack(psTracks, psTrack1);
	analyzedTracksInsertTrack(psTracks, psTrack2);
	printf("\t\tL'identifiant du second morceau vaut maintenant : %d\n",
			analyzedTrackGetTID(psTrack2));
	assert(g_tree_nnodes(psTracks) == 2);
	assert(analyzedTrackGetTID(psTrack2) == 2);
	printf("\tFAIT !!\n");

	printf("Suppression d'un morceau...\n");
	analyzedTracksRemoveTrack(psTracks, psTrack1->iTID);
	assert(g_tree_nnodes(psTracks) == 1);
	assert(psTrack1 != NULL);
	printf("\tFAIT !!\n");

	printf("Modification d'une valeur d'un morceau...\n");
	psTrackSet = analyzedTracksGetTrack(psTracks, 2);
	assert(psTrackSet != NULL);
	analyzedTrackSetPath(psTrackSet, "ça marche");
	assert(strcmp(analyzedTrackGetPath(psTrackSet), "ça marche") == 0);
	printf("\tFAIT !!\n");

	printf("Lecture d'une valeur d'un morceau...\n");
	psTrackGet = analyzedTracksGetConstTrack(psTracks, 2);
	assert(psTrackGet != NULL);
	assert(analyzedTrackGetTID(psTrackGet) == 2 &&
			analyzedTrackGetFrequenciesAverage(psTrackGet) == 0.0 &&
			analyzedTrackGetFrequenciesMedian(psTrackGet) == 0.0 &&
			strcmp(analyzedTrackGetPath(psTrackSet), "ça marche") == 0);
	printf("\tFAIT !!\n");

	printf("Destruction de l'arbre...\n");
	analyzedTracksDestroy(&psTracks);
	assert(psTracks == NULL);
	assert(psTrack1 != NULL);
	assert(psTrack2 != NULL);
	analyzedTrackDestroy(&psTrack1);
	analyzedTrackDestroy(&psTrack2);
	assert(psTrack1 == NULL);
	assert(psTrack2 == NULL);
	printf("\tFAIT !!\n");

	printf("\n\t-- Fin des tests --\n");

	return EXIT_SUCCESS;
}


