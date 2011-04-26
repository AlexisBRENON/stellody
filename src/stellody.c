/**
  * @brief Fichier contenant le \em main.
  *
  * @author STELLODY TEAM
  * @file stellody.c
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(__linux)
#include <fmodex/fmod.h>
#endif
#if defined (__APPLE__)
#include <fmod.h>
#endif

#include "analyzed_track.h"
#include "analyzed_tracks.h"
#include "preferences.h"
#include "opengl_drawing.h"
#include "files.h"
#include "gui.h"


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
	gpointer pDatas[NB_DATA];
	GKeyFile** ppsFilesContext;
    int i, iAnswer = -1;

    for (i = 0; i < NB_DATA; i++)
    {
    	pDatas[i] = NULL;
    }

	printf ("Que voulez-vous faire ?\n\n");
	printf ("\t1 : preferencesRegressionTest\n");
	printf ("\t2 : analyzedTracksRegressionTest\n");
	printf ("\t3 : analyzedTrackRegressionTest\n");
	printf ("\t4 : filesRegressionTest\n");
	printf ("\t5 : openglRegressionTest\n");
	printf ("\t9 : Test intégral\n");
	printf ("\n\t0 : Quitter\n");

    while (iAnswer < 0 || iAnswer > 9)
    {
    	printf("\n Choix : ");
    	scanf("%d", &iAnswer);
    }

    gtk_init(&argc, &argv);

    switch (iAnswer)
    {
    	case 0:
			break;
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
		case 9:
			ppsFilesContext = filesOpen();
			pDatas[PREFERENCES] = preferencesCreateFromFile(
													ppsFilesContext);
			pDatas[MAIN_BUILDER] = guiLoad(pDatas);
			gtk_main();
			filesCloseAndSave(&ppsFilesContext,
							(Preferences*) pDatas[PREFERENCES],
							(AnalyzedTracks*) pDatas[ANALYZED_TRACKS]);
			preferencesDestroy((Preferences**) &pDatas[PREFERENCES]);
			break;
		default:
			printf ("Choix incorrect...");
			break;
    }


    return 0;
}

