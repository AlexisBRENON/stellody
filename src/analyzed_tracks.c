/**
  * @brief Fichier d'implémentation du module \em AnalyzedTracks.
  *
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
#include "gui.h"
#include "analysis.h"

/* ********************************************************************* */
/*                                                                       */
/*         Fonctions relatives à la structure AnalyzedTracks             */
/*                                                                       */
/* ********************************************************************* */

gboolean analyzedTracksSaveTracks (int* pKey,
								AnalyzedTrack* pValue,
								GKeyFile* pData)
{
	int iTID = 0;
	char bAnalyzed = 0;
	const char* strPath = NULL;
	char* strGroupName = NULL;
	char* pcToChange = NULL;
	unsigned int uiLength = 0;
	float fAverage = 0;
	float* fRate = NULL;
	float* fCoord = NULL;

	iTID = analyzedTrackGetTID(pValue);
	bAnalyzed = analyzedTrackGetAnalyzed(pValue);
	strPath = analyzedTrackGetPath(pValue);
	uiLength = analyzedTrackGetLength(pValue);
	fAverage = analyzedTrackGetAverage(pValue);
	fRate = analyzedTrackGetRate(pValue);
	fCoord = analyzedTrackGetCoord(pValue);

	/* Il faut vérifier que le chemin (qui sera utilisé comme identifiant)
	ne contiennent pas de "[]". On remplace les crochets par des
	parenthèses. */

	strGroupName = (char*) malloc((strlen(strPath)+1)*sizeof(char));
	strcpy(strGroupName, strPath);
	pcToChange = strchr(strGroupName, '[');
	while (pcToChange != NULL)
	{
		*pcToChange = '(';
		pcToChange = strchr(strGroupName, '[');
	}
	pcToChange = strchr(strGroupName, ']');
	while (pcToChange != NULL)
	{
		*pcToChange = ')';
		pcToChange = strchr(strGroupName, ']');
	}

	/* On sauvegarde les données */


	g_key_file_set_string(pData,
						strGroupName,
						"strPath",
						strPath);
	g_key_file_set_integer(pData,
							strGroupName,
							"iTID",
							iTID);
	g_key_file_set_integer(pData,
							strGroupName,
							"bAnalyzed",
							(int) bAnalyzed);
	g_key_file_set_integer(pData,
						strGroupName,
						"uiLength",
						uiLength);
	g_key_file_set_double(pData,
						strGroupName,
						"fAverage",
						fAverage);
	g_key_file_set_double(pData,
						strGroupName,
						"fBass",
						(double) fRate[0]);
	g_key_file_set_double(pData,
						strGroupName,
						"fMedium",
						(double) fRate[1]);
	g_key_file_set_double(pData,
						strGroupName,
						"fTreble",
						(double) fRate[2]);
	g_key_file_set_double(pData,
						strGroupName,
						"fCoordX",
						(double) fCoord[0]);
	g_key_file_set_double(pData,
						strGroupName,
						"fCoordY",
						(double) fCoord[1]);
	g_key_file_set_double(pData,
						strGroupName,
						"fCoordZ",
						(double) fCoord[2]);


	free(strGroupName); strGroupName = NULL;

	return FALSE;
}



int analyzedTracksInit(AnalyzedTracks* psTracks)
{
	assert (psTracks != NULL);

	return EXIT_SUCCESS;
}
int analyzedTracksInitFromFile (AnalyzedTracks* psTracks,
								GKeyFile* ppsContext[])
{
	gsize ulNbTracks;
	gchar** strGroups = NULL;
	int iTID = 0;
	char bAnalyzed = 0;
	char* strPath = NULL;
	unsigned int uiLength = 0;
	float fAverage = 0;
	float pfRate[3] = {0};
	float pfCoord[3] = {0};
	AnalyzedTrack* psTrack = NULL;

	int i;

	assert (psTracks != NULL);
	assert (ppsContext != NULL);

	/* Récupère l'ensemble des noms des groupes */
	strGroups = g_key_file_get_groups(ppsContext[DATA], &ulNbTracks);

	/* Chaque groupe correspond à un morceau. On les charge tous. */
	for (i = 0; strGroups[i] != NULL; i++)
	{
		psTrack = analyzedTrackCreate();

		/* On récupère toutes les données relatives au morceau */
		iTID = g_key_file_get_integer(ppsContext[DATA],
										strGroups[i], "iTID", NULL);
		bAnalyzed = (char) g_key_file_get_integer(ppsContext[DATA],
										strGroups[i], "bAnalyzed", NULL);
		strPath = g_key_file_get_string(ppsContext[DATA],
										strGroups[i], "strPath", NULL);
		uiLength = g_key_file_get_integer(ppsContext[DATA],
										strGroups[i], "uiLength", NULL);
		fAverage = (float) g_key_file_get_double(ppsContext[DATA],
										strGroups[i], "fAverage", NULL);
		pfRate[0] = (float) g_key_file_get_double(ppsContext[DATA],
										strGroups[i], "fBass", NULL);
		pfRate[1] = (float) g_key_file_get_double(ppsContext[DATA],
										strGroups[i], "fMedium", NULL);
		pfRate[2] = (float) g_key_file_get_double(ppsContext[DATA],
										strGroups[i], "fTreble", NULL);
		pfCoord[0] = (float) g_key_file_get_double(ppsContext[DATA],
										strGroups[i], "fCoordX", NULL);
		pfCoord[1] = (float) g_key_file_get_double(ppsContext[DATA],
										strGroups[i], "fCoordY", NULL);
		pfCoord[2] = (float) g_key_file_get_double(ppsContext[DATA],
										strGroups[i], "fCoordZ", NULL);

		/* On initialise le morceau avec les bonnes valeurs */
		analyzedTrackInitWithData(psTrack, iTID, strPath, uiLength,
									fAverage, pfRate, pfCoord);
		analyzedTrackSetAnalyzed(psTrack, bAnalyzed);

		/* On le stocke dans l'arbre */
		analyzedTracksInsertTrack(psTracks, psTrack);

		free(strPath); strPath = NULL;
	}

	g_strfreev(strGroups);
	
	return EXIT_SUCCESS;
}
int analyzedTracksRelease(AnalyzedTracks* psTracks)
{
	assert (psTracks != NULL);

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

	assert (ppsContext != NULL);

	psTracks = analyzedTracksCreate();
	analyzedTracksInitFromFile(psTracks, ppsContext);

	return psTracks;
}
int analyzedTracksDestroy(AnalyzedTracks** ppsTracks)
{
	assert (ppsTracks != NULL);
	assert (*ppsTracks != NULL);

	analyzedTracksRelease(*ppsTracks);

	g_tree_destroy(*ppsTracks);
	*ppsTracks = NULL;

	return EXIT_SUCCESS;
}


int analyzedTracksInsertTrack(AnalyzedTracks* psTracks,
							AnalyzedTrack* psTrack)
{
	int iTID = 0, iNewTID = 0;
	AnalyzedTrack* psTrackInTree = NULL;
	int *piKey = NULL;

	assert (psTracks != NULL);
	assert (psTrack != NULL);

	piKey = (int*) malloc(sizeof(int));

	/* On vérifie que la clé qu'on s'apprette à entrer n'existe pas déja */
	iTID = analyzedTrackGetTID(psTrack);
	iNewTID = iTID;
	psTrackInTree = g_tree_lookup(psTracks,
								&iTID);

	while (psTrackInTree != NULL)
	{
		/* Si elle existe déja, on incrémente l'identification et on refait
		le test */
		iNewTID++;
		psTrackInTree = g_tree_lookup(psTracks,
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

	assert (psTracks != NULL);
	assert (iKey > -1);

	psTrack = analyzedTracksGetTrack(psTracks, iKey);

	if (psTrack != NULL)
	{
		g_tree_remove(psTracks, &(psTrack->iTID));
		analyzedTrackDestroy(&psTrack);
	}

	return psTrack;
}

const AnalyzedTrack* analyzedTracksGetConstTrack(
									AnalyzedTracks* psTracks,
									int iKey)
{
	assert (psTracks != NULL);

	return g_tree_lookup(psTracks, &iKey);
}

AnalyzedTrack* analyzedTracksGetTrack(AnalyzedTracks* psTracks,
									int iKey)
{
	AnalyzedTrack* psTrack = NULL;

	assert (psTracks != NULL);

	psTrack = g_tree_lookup(psTracks, &iKey);

	return psTrack;
}


gboolean analyzedTracksCheckForAnalyze(int* piKey,
										AnalyzedTrack* psTrack,
										gpointer* pData)
{
	char bAnalyzed = 0;

	bAnalyzed = analyzedTrackGetAnalyzed(psTrack);

	if (bAnalyzed == 0)
	{
		analyzedTrackSetAverage(psTrack, 0);
		analyzedTrackSetRate(psTrack, NULL);
		analyzedTrackSetCoord(psTrack, NULL);

		pData[ANALYZELIST] = g_list_append((GList*) pData[ANALYZELIST],
										psTrack);
		/* On crée le timer sur la vérification d'analyse s'il n'existe pas */

		if (*((int*) pData[CHECKANALYZE]) == 0)
		{
			*((int*) pData[CHECKANALYZE]) =
				g_timeout_add_seconds(2,
								(GSourceFunc) guiTimeoutCheckForAnalyze,
								pData);
		}
	}

	return FALSE;
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
	assert (psTracks != NULL);
	printf("\tFAIT !!\n");

	printf("Ajout de morceaux à l'arbre...\n");
	psTrack1 = analyzedTrackCreateWithData(1, "test1", 0, 0.0, NULL, NULL);
	psTrack2 = analyzedTrackCreateWithData(1, "test2", 0, 0.0, NULL, NULL);
	printf("\t\tLes deux identifiants sont à 1...\n");
	assert (psTrack1 != NULL &&
			psTrack2 != NULL);
	analyzedTracksInsertTrack(psTracks, psTrack1);
	analyzedTracksInsertTrack(psTracks, psTrack2);
	printf("\t\tL'identifiant du second morceau vaut maintenant : %d\n",
			analyzedTrackGetTID(psTrack2));
	assert (g_tree_nnodes(psTracks) == 2);
	assert (analyzedTrackGetTID(psTrack2) == 2);
	printf("\tFAIT !!\n");

	printf("Suppression d'un morceau...\n");
	psTrack1 = analyzedTracksRemoveTrack(psTracks, psTrack1->iTID);
	assert (g_tree_nnodes(psTracks) == 1);
	assert (psTrack1 == NULL);
	printf("\tFAIT !!\n");

	printf("Modification d'une valeur d'un morceau...\n");
	psTrackSet = analyzedTracksGetTrack(psTracks, 2);
	assert (psTrackSet != NULL);
	analyzedTrackSetPath(psTrackSet, "ça marche");
	assert (strcmp(analyzedTrackGetPath(psTrackSet), "ça marche") == 0);
	printf("\tFAIT !!\n");

	printf("Lecture d'une valeur d'un morceau...\n");
	psTrackGet = analyzedTracksGetConstTrack(psTracks, 2);
	assert (psTrackGet != NULL);
	assert (analyzedTrackGetTID(psTrackGet) == 2 &&
			analyzedTrackGetAverage(psTrackGet) == 0.0 &&
			strcmp(analyzedTrackGetPath(psTrackSet), "ça marche") == 0);
	printf("\tFAIT !!\n");

	printf("Destruction de l'arbre...\n");
	analyzedTracksDestroy(&psTracks);
	assert (psTracks == NULL);
	analyzedTrackDestroy(&psTrack2);
	printf("\tFAIT !!\n");

	printf("\n\t-- Fin des tests --\n");

	return EXIT_SUCCESS;
}


