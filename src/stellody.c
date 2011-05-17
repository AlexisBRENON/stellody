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


int regressionTests (int argc, char* argv[]);
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
		regressionTests(argc, argv);
	}

    return 0;
}

/**
  * @fn int regressionTests(int argc, char* argv[])
  * @brief Fonction regroupant les tests de regressions dans un menu.
  *
  * @param[in] argc Nombre d'argument donnés au programme (>0)
  * @param[in,out] argv Tableau de chaine de caractères contenant les
  arguments.
  * @return 0 si tout est OK.
  */
int regressionTests(int argc, char* argv[])
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
               OpenGLDrawingRegressionTest(&argc, argv));
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
	gpointer pDatas[NB_DATA];
	GKeyFile** pFileContext;
	int i;

	srand(time(NULL));

	for (i = 0; i < NB_DATA; i++)
	{
		pDatas[i] = NULL;
	}
	pDatas[CHECKANALYZE] = (int*) malloc(sizeof(int));
	*((int*) pDatas[CHECKANALYZE]) = 0;

	FMOD_System_Create((FMOD_SYSTEM**) &(pDatas[FMOD_CONTEXT]));
	FMOD_System_Init((FMOD_SYSTEM*) pDatas[FMOD_CONTEXT],
					2, FMOD_INIT_NORMAL, NULL);
	pDatas[OPENGLDATA] = (OpenGLData *) malloc (sizeof(OpenGLData)) ;
	gtk_init(&argc, &argv);
	gtk_gl_init(&argc, &argv);

	pDatas[MAIN_BUILDER] = guiLoad(pDatas);

	pFileContext = filesOpen();
	pDatas[PREFERENCES] = preferencesCreateFromFile(pFileContext);
	pDatas[ANALYZED_TRACKS] = analyzedTracksCreateFromFile(pFileContext);

	g_tree_foreach((GTree*) pDatas[ANALYZED_TRACKS],
					(GTraverseFunc) analyzedTracksRemoveForAnalyze,
					pDatas);

	gtk_main();

	filesCloseAndSave(&pFileContext,
					pDatas[PREFERENCES],
					pDatas[ANALYZED_TRACKS]);
	preferencesDestroy((Preferences**) &(pDatas[PREFERENCES]));
	analyzedTracksDestroy((AnalyzedTracks**)&(pDatas[ANALYZED_TRACKS]));

	FMOD_System_Release((FMOD_SYSTEM*) pDatas[FMOD_CONTEXT]);

	free(pDatas[CHECKANALYZE]);
	pDatas[CHECKANALYZE] = NULL;

	free(pDatas[OPENGLDATA]);
	pDatas[OPENGLDATA] = NULL;

	return EXIT_SUCCESS;
}














