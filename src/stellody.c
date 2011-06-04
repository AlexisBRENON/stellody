/**
  * @brief Fichier contenant le \em main.
  *
  * @author STELLODY TEAM
  * @file stellody.c
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gtk/gtkgl.h>

#if defined(__linux)
#include <fmodex/fmod.h>
#endif
#if defined (__APPLE__) || defined (_WIN32)
#include <fmod.h>
#endif

#include "analyzed_track.h"
#include "analyzed_tracks.h"
#include "preferences.h"
#include "opengl_drawing.h"
#include "files.h"
#include "gui.h"


int regressionTests ();
int stellody (int argc, char* argv[]);

/**
  * @fn int main (int argc, char* argv[])
  * @brief Fonction exécutée au lancement du programme.
  *
  * @param[in] argc Nombre d'argument donnés au programme (>0)
  * @param[in,out] argv Tableau de chaine de caractères contenant les
  arguments.
  * @return 0 si tout est OK.
  */
int main (int argc, char* argv[])
{
	if (argc == 1)
	{
		stellody(argc, argv);
	}

	if (argc == 2 && strcmp(argv[1], "--tests") == 0)
	{
		regressionTests();
	}

    return 0;
}

/**
  * @fn int regressionTests()
  * @brief Fonction regroupant les tests de regressions dans un menu.
  *
  * @return 0 si tout est OK.
  */
int regressionTests()
{
	int iAnswer = -1;

	printf ("Que voulez-vous faire ?\n\n");
	printf ("\t1 : preferencesRegressionTest\n");
	printf ("\t2 : analyzedTracksRegressionTest\n");
	printf ("\t3 : analyzedTrackRegressionTest\n");
	printf ("\t4 : filesRegressionTest\n");
	printf ("\t5 : openglRegressionTest\n");

	while (iAnswer < 0 || iAnswer > 5)
    {
    	printf("\n Choix : ");
    	scanf("%d", &iAnswer);
    }

    switch (iAnswer)
    {
		case 1:
			printf ("\nReturned value : %d\n",
					preferencesRegressionTest());
			break;
		case 2:
			printf ("\nReturned value : %d\n",
					analyzedTracksRegressionTest());
			break;
		case 3:
			printf ("\nReturned value : %d\n",
					analyzedTrackRegressionTest());
			break;
		case 4:
			printf ("\nReturned value : %d\n",
					filesRegressionTest());
			break;
		case 5:
			printf("Returned value : %d\n",
				   drawingGLRegressionTest());
			break;
		default:
			break;
    }

	printf("Fin du MAIN !");

    return EXIT_SUCCESS;
}

/**
  * @fn int stellody(int argc, char* argv[])
  * @brief Lance le programme principal (programme final).
  *
  * @param[in] argc Nombre d'argument donnés au programme (>0)
  * @param[in,out] argv Tableau de chaine de caractères contenant les
  arguments.
  * @return 0 si tout est OK.
  */
int stellody(int argc, char* argv[])
{
	/* Déclaration des variables. */
	gpointer pDatas[NB_DATA];
	GKeyFile** pFileContext;

	int i;

	/* Initialisation des variables */

	srand(time(NULL));

	for (i = 0; i < NB_DATA; i++)
	{
		pDatas[i] = NULL;
	}

	pDatas[ANALYZING_COUNTER] = (int*) malloc(sizeof(int));
	*((int*) pDatas[ANALYZING_COUNTER]) = 0;
	pDatas[CHECKANALYZE] = (int*) malloc(sizeof(int));
	*((int*) pDatas[CHECKANALYZE]) = 0;
	pDatas[INCREMENT_TIMER] = (int*) malloc(sizeof(int));
	*((int*)pDatas[INCREMENT_TIMER]) = 0;
	pDatas[PLAYLIST_INDEX] = (int*) malloc(sizeof(int));
	*((int*) pDatas[PLAYLIST_INDEX]) = 0;
	pDatas[MOUSEPOSITION_X] = (float*) malloc(sizeof(float));
	*((float*) pDatas[MOUSEPOSITION_X]) = 0;
	pDatas[MOUSEPOSITION_Y] = (float*) malloc(sizeof(float));
	*((float*) pDatas[MOUSEPOSITION_Y]) = 0;

	FMOD_System_Create((FMOD_SYSTEM**) &(pDatas[FMOD_CONTEXT]));
	FMOD_System_Init((FMOD_SYSTEM*) pDatas[FMOD_CONTEXT],
					2, FMOD_INIT_NORMAL, NULL);
	pDatas[OPENGLDATA] = (OpenGLData *) malloc (sizeof(OpenGLData)) ;

	/* Initialisation des bibliothèques tierces. */

	gtk_init(&argc, &argv);
	gtk_gl_init(&argc, &argv);

	/* Création de l'interface et chargement des données */

	guiLoad(pDatas);

	pFileContext = filesOpen();
	pDatas[PREFERENCES] = preferencesCreateFromFile(pFileContext);
	pDatas[ANALYZED_TRACKS] = analyzedTracksCreateFromFile(pFileContext);

	g_tree_foreach((GTree*) pDatas[ANALYZED_TRACKS],
					(GTraverseFunc) analyzedTracksCheckForAnalyze,
					pDatas);

	/* Lancement de la boucle d'évenements */
	on_Stellarium_Action_activate(NULL, pDatas);
	gtk_main();

	/* Libération de la mémoire */

	filesCloseAndSave(&pFileContext,
					pDatas[PREFERENCES],
					pDatas[ANALYZED_TRACKS]);

	preferencesDestroy((Preferences**) &(pDatas[PREFERENCES]));

	analyzedTracksDestroy((AnalyzedTracks**)&(pDatas[ANALYZED_TRACKS]));

	FMOD_System_Release((FMOD_SYSTEM*) pDatas[FMOD_CONTEXT]);

	free(pDatas[ANALYZING_COUNTER]); pDatas[ANALYZING_COUNTER] = NULL;
	free(pDatas[CHECKANALYZE]); pDatas[CHECKANALYZE] = NULL;
	free(pDatas[INCREMENT_TIMER]); pDatas[INCREMENT_TIMER] = NULL;
	free(pDatas[PLAYLIST_INDEX]); pDatas[PLAYLIST_INDEX] = NULL;
	free(pDatas[MOUSEPOSITION_X]); pDatas[MOUSEPOSITION_X] = NULL;
	free(pDatas[MOUSEPOSITION_Y]); pDatas[MOUSEPOSITION_Y] = NULL;


	free(pDatas[OPENGLDATA]); pDatas[OPENGLDATA] = NULL;

	/* All is alright ! ;p */

	return EXIT_SUCCESS;
}
