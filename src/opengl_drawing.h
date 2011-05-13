/**
 * @brief Fichier d'entête du module \em OpenGLDrawing.
 *
 * @author Matthieu VIDAL in STELLODY TEAM
 * @file opengl_drawing.h
 */


#ifndef _OPENGL_DRAWING_H_
#define _OPENGL_DRAWING_H_


/* ********************************************************************* */
/*                                                                       */
/*                          Librairies OpenGL                            */
/*                                                                       */
/* ********************************************************************* */

#include <gtk/gtk.h>

#if defined(__linux)
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif
#if defined(__APPLE__)
	#include <openGL/gl.h>
	#include <openGL/glu.h>
#endif

#include <gtk/gtk.h>


/* ********************************************************************* */
/*                                                                       */
/*                        Définitions de types                           */
/*                                                                       */
/* ********************************************************************* */


/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions relatives à OpenGL                        */
/*                                                                       */
/* ********************************************************************* */

int GLInit(GtkWidget * Widget, gpointer * pData) ;

int GLDraw(GtkWidget * Widget, GdkEventExpose * Event, gpointer * pData) ;

int drawingGlInit (GtkWidget* psWidget, gpointer* pData);

int drawingGlDraw (GtkWidget* psWidget,
				GdkEventExpose* psEvent,
				gpointer* pData);

int drawingGlResize (GtkWidget* psWidget,
					GdkEventConfigure* psEvent,
					gpointer* pData);

/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */


/**
 * @fn OpenGLDrawingRegressionTest(int * argc, char * argv[])
 * @brief Teste toutes les fonctions du module opengl_drawing.h
 *
 * @param [in] argc pointeur vers argc
 * @param [in] argv pointeur vers argv[]
 *
 * @return EXIT_SUCCESS si tout est OK
 */
int OpenGLDrawingRegressionTest(int * argc, char * argv[]) ;


#endif
