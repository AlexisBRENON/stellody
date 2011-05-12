/**
 * @brief Fichier d'implémentation du module \em OpenGLDrawing
 *
 * @author Matthieu VIDAL in STELLODY TEAM
 * @file opengl_drawing.c
 */


/* ********************************************************************* */
/*                                                                       */
/*                     Librairies OpenGL MAC OS X                        */
/*                                                                       */
/* ********************************************************************* */


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <gtk/gtk.h>

#include <gtk/gtkgl.h>

#include "opengl_drawing.h"

#ifndef M_PI
	#define M_PI 3.14159 /**< Défini la constante PI si elle ne l'est pas.*/
#endif


int drawingGlInit (GtkWidget* psWidget, gpointer* pData)
{
	GdkGLContext * contexte = NULL;
	GdkGLDrawable * surface = NULL;

	contexte = gtk_widget_get_gl_context(psWidget);
	surface = gtk_widget_get_gl_drawable(psWidget);

	if(gdk_gl_drawable_gl_begin(surface,contexte))
	{
		/* appels OpenGL */
		printf("Fonction d'initialisation Open_GL\n");

		glClearColor(0,0,0.2,1);
		gdk_gl_drawable_swap_buffers(surface); /* permutation des tampons */
		gdk_gl_drawable_gl_end(surface); /* désactivation du contexte */
	}

	return EXIT_SUCCESS;

	return EXIT_SUCCESS;
}

int drawingGlDraw (GtkWidget* psWidget,
				GdkEventExpose* psEvent,
				gpointer* pData)
{

	GdkGLContext * contexte = NULL;
	GdkGLDrawable * surface = NULL;

	contexte = gtk_widget_get_gl_context(psWidget);
	surface = gtk_widget_get_gl_drawable(psWidget);

	if(gdk_gl_drawable_gl_begin(surface,contexte))
	{
		/* appels OpenGL */
		printf("Fonction de dessin Open_GL\n");

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gdk_gl_drawable_swap_buffers(surface); /* permutation des tampons */
		gdk_gl_drawable_gl_end(surface); /* désactivation du contexte */
	}

	return EXIT_SUCCESS;
}

int drawingGlResize (GtkWidget* psWidget,
					GdkEventConfigure* psEvent,
					gpointer* pData)
{
	printf("Fonction de redimenssionnement\n");

	return EXIT_SUCCESS;
}


/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */

int OpenGLDrawingRegressionTest(int * argc, char * argv[])
{
	return EXIT_SUCCESS;
}
