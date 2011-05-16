/**
 * @brief Fichier d'implémentation du module \em OpenGLDrawing
 *
 * @author Matthieu VIDAL in STELLODY TEAM
 * @file opengl_drawing.c
 */


/* ********************************************************************* */
/*                                                                       */
/*                             Librairies                                */
/*                                                                       */
/* ********************************************************************* */

#if defined(__linux)
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#if defined(__APPLE__)
#include <openGL/gl.h>
#include <openGL/glu.h>
#include <glut/glut.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <gtk/gtk.h>

#include <gtk/gtkgl.h>

#include "star.h"
#include "opengl_drawing.h"

#ifndef M_PI
	#define M_PI 3.14159 /**< Défini la constante PI si elle ne l'est pas.*/
#endif


/* ********************************************************************* */
/*                                                                       */
/*                    Fonctions générales OpenGL                         */
/*                                                                       */
/* ********************************************************************* */


static void drawSphere(void)
{
	float alpha = -90 ;
	float beta = 0 ;
	float alphaS = -90 ;
	float betaS = 0 ;
	float x1 = 0 ;
	float y1 = 0 ;
	float z1 = 0 ;
	float x2 = 0 ;
	float y2 = 0 ;
	float z2 = 0 ;
	int i = 0 ;
	int j = 0 ;
	int n = 64 ;
	double step ;
	step = 2*M_PI / (n) ;
	
	
	for (i = 0 ; i < n/2 ; i++)
	{
		
		alpha = -M_PI/2 + i * step ;
		
		glBegin(GL_TRIANGLE_STRIP) ;
		
		for (j = 0 ; j <= n ; j++)
		{
			beta = j * step ;
			
			x1 = cos (alpha) * cos (beta) ;
			y1 = sin (alpha) ;
			z1 = cos (alpha) * sin (beta) ;
			
			alphaS = alpha + step ;
			betaS = beta + step ;
			
			/*
			 if (alphaS >= 2 * M_PI)
			 {
			 alphaS = alphaS - 2 * M_PI ;
			 }
			 
			 if (betaS >= 2 * M_PI)
			 {
			 betaS = betaS - 2 * M_PI ;
			 } */
			
			x2 = cos (alphaS) * cos (betaS) ;
			y2 = sin (alphaS) ;
			z2 = cos (alphaS) * sin (betaS) ;
						
			glVertex3f(x1, y1, z1) ;
			glVertex3f(x2, y2, z2) ;
			
		}
		
		glEnd() ;
		
	}
}

static void sceneDraw(void)
{
	glPushMatrix() ;
	glTranslatef(0, 0, -1) ;
	glScalef(0.5, 0.5, 0.5) ;
	glColor3f(1, 0, 0) ;
	drawSphere() ;
	glPopMatrix() ;

	glPushMatrix() ;
	glTranslatef(1, 0, -1) ;
	glScalef(0.5, 0.5, 0.5) ;
	glColor3f(0, 1, 0) ;
	drawSphere() ;
	glPopMatrix() ;

	glPushMatrix() ;
	glTranslatef(-1, 0, -1) ;
	glScalef(0.5, 0.5, 0.5) ;
	glColor3f(0, 0, 1) ;
	drawSphere() ;
	glPopMatrix() ;
}


/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions d'affichage OpenGL                        */
/*                                                                       */
/* ********************************************************************* */


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
		
		
		/* Début de l'initialisation. */
		
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f) ;
		glClearDepth(1.0) ;
		glDepthFunc(GL_LESS) ;
		glEnable(GL_DEPTH_TEST) ;
		glShadeModel(GL_SMOOTH) ;

		glEnable(GL_NORMALIZE);

		/* Fin de l'initialisation. */
		
		gdk_gl_drawable_swap_buffers(surface) ;	/* permutation des tampons */
		gdk_gl_drawable_gl_end(surface) ;		/* désactivation du contexte */
	}

	return EXIT_SUCCESS;
}

int drawingGlResize (GtkWidget* psWidget,
					 GdkEventConfigure* psEvent,
					 gpointer* pData)
{
	GdkGLContext * contexte = NULL;
	GdkGLDrawable * surface = NULL;
	
	contexte = gtk_widget_get_gl_context(psWidget);
	surface = gtk_widget_get_gl_drawable(psWidget);
	
	if(gdk_gl_drawable_gl_begin(surface,contexte))
	{
		
		printf("Fonction de redimenssionnement\n");
		
		glViewport(0, 0, psEvent->width, psEvent->height);
				
		glMatrixMode(GL_PROJECTION) ;
		glLoadIdentity() ;
		gluPerspective(45, (GLfloat) psEvent->width / (GLfloat) psEvent->height , 0, 10000);
		glMatrixMode(GL_MODELVIEW) ;
		
		gdk_gl_drawable_gl_end(surface); /* désactivation du contexte */
	}
	
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
		
		/* Début des dessins. */
		
		glPointSize(1.0) ;
		glEnable(GL_DEPTH_TEST) ;
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		gluLookAt(0, 0, 0, 0, 0, 1, 0, 1, 0);
		
		sceneDraw() ;
		
		
		/* Fin des dessins. */
		
		
		gdk_gl_drawable_swap_buffers(surface); /* permutation des tampons */
		gdk_gl_drawable_gl_end(surface); /* désactivation du contexte */
	}

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
