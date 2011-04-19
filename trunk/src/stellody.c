/**
  * @brief Fichier contenant le \em main.
  *
  * @author STELLODY TEAM
  * @file stellody.c
  */

#include <stdio.h>
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
	GtkBuilder* builder = NULL;
	GtkWidget* win = NULL;

	if (argc > 1 && strcmp(argv[1], "-x") == 0)
	{
		gtk_init(&argc, &argv);

		gtk_builder_add_from_file(builder, "data/windows/Window.glade",
									NULL);
		gtk_builder_connect_signals(builder, NULL);

		win = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
		gtk_widget_show_all(win);
		gtk_main();
	}
	else
	{
		printf("Returned value : %d\n", filesRegressionTest());
	}

	printf("Returned value : %d\n", OpenGLDrawingRegressionTest(&argc, argv));

	return 0;
}
