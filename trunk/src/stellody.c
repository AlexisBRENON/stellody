/**
  * @brief Fichier contenant le \em main.
  *
  * @author STELLODY TEAM
  * @file stellody.c
  * @todo
		Ajouter récurcivement les dossiers de musique. \n
		Refaire l'algo d'analyse (+ tests) \n
  */



/* ********************************************************************* */
/*                                                                       */
/*                     INCLUSIONS DU PREPROCESSEUR                       */
/*                                                                       */
/* ********************************************************************* */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <gtk/gtk.h>
#include <gtk/gtkgl.h>

#if defined(__linux)
#include <fmodex/fmod.h>
#endif
#if defined (__APPLE__) || defined (_WIN32)
#include <fmod.h>
#endif

#include "dynamic_array.h"
#include "linked_list.h"
#include "analyzed_track.h"
#include "analyzed_tracks.h"
#include "preferences.h"
#include "opengl_drawing.h"
#include "files.h"
#include "gui.h"


/* ********************************************************************* */
/*                                                                       */
/*                              PROTOTYPES                               */
/*                                                                       */
/* ********************************************************************* */


int regressionTests ();
int stellody (int argc, char* argv[]);


/* ********************************************************************* */
/*                                                                       */
/*                            IMPLEMENTATION                             */
/*                                                                       */
/* ********************************************************************* */


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
	printf ("\t6 : dynamicArrayRegressionTest\n");
	printf ("\t7 : linkedListRegressionTest\n");

	while (iAnswer < 0 || iAnswer > 7)
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
		case 6:
			printf("Returned value : %d\n",
					dynamicArrayRegressionTest());
			break;
		case 7:
			printf("Returned value : %d\n",
					linkedListRegressionTest());
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
	void* ppDatas[5] = {NULL, NULL, NULL, NULL, NULL};

	gtk_init(&argc, &argv);
	assert (guiLoad(ppDatas) == EXIT_SUCCESS);

	gtk_main();

	return EXIT_SUCCESS;
}
