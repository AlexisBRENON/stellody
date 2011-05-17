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
#endif
#if defined(__APPLE__)
#include <openGL/gl.h>
#include <openGL/glu.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtkgl.h>

#include "star.h"
#include "opengl_drawing.h"
#include "gui.h"

#ifndef M_PI
	#define M_PI 3.14159 /**< Défini la constante PI si elle ne l'est pas.*/
#endif


/* ********************************************************************* */
/*                                                                       */
/*                    Fonctions de dessin OpenGL                         */
/*                                                                       */
/* ********************************************************************* */

static void DrawCube()
{
	int i = 0 ;
	int j = 0 ;
	int Colour = 1 ;

		/* 0 à 2 : coordonnées ; 3 à 5 : normale. */
	float ppfVertexCube[8][6] =
	{
		{-0.5, -0.5, -0.5, 1, 1, 1},
		{-0.5, -0.5, 0.5, 1, 1, 1},
		{-0.5, 0.5, -0.5, 1, 0, 0},
		{-0.5, 0.5, 0.5, 1, 0, 0},
		{0.5, -0.5, -0.5, 0, 1, 0},
		{0.5, -0.5, 0.5, 0, 1, 0},
		{0.5, 0.5, -0.5, 0, 0, 1},
		{0.5, 0.5, 0.5, 0, 0, 1}
	} ;

	int piFacesCube[6][4] =
	{
		{0, 1, 3, 2},
		{4, 5, 7, 6},
		{0, 1, 5, 4},
		{2, 3, 7, 6},
		{0, 2, 6, 4},
		{1, 3, 7, 5}
	} ;

	int piNormalesCube[6][3] =
	{
		{-1, 0, 0},
		{1, 0, 0},
		{0, -1, 0},
		{0, 1, 0},
		{0, 0, -1},
		{0, 0, 1}
	} ;

	glBegin(GL_QUADS) ;
	for(i = 0 ; i < 6 ; i++)
	{
		glNormal3f(piNormalesCube[i][0],piNormalesCube[i][1],piNormalesCube[i][2]) ;
		for(j = 0 ; j < 4 ; j ++)
		{
			switch (Colour)
			{
				case 0 :
					glColor3f(ppfVertexCube[piFacesCube[i][j]][3], ppfVertexCube[piFacesCube[i][j]][4], ppfVertexCube[piFacesCube[i][j]][5]) ;
					break ;
				default :
					break ;
			}

			/* glNormal3f(2*ppfVertexCube[piFacesCube[i][j]][0], 2*ppfVertexCube[piFacesCube[i][j]][1], 2*ppfVertexCube[piFacesCube[i][j]][2]) ; */
			glVertex3f(ppfVertexCube[piFacesCube[i][j]][0], ppfVertexCube[piFacesCube[i][j]][1], ppfVertexCube[piFacesCube[i][j]][2]) ;
		}
	}
	glEnd() ;
}

static void DrawTriangle(void)
{
	int i = 0 ;
	int j = 0 ;

	float ppfVertexTriangle[6][6] =
	{
		{-0.5, -0.5, -0.5, 1, 1, 1},
		{-0.5, -0.5, 0.5, 1, 1, 1},
		{-0.5, 0.5, -0.5, 1, 0, 0},
		{-0.5, 0.5, 0.5, 1, 0, 0},
		{0.5, -0.5, -0.5, 0, 1, 0},
		{0.5, -0.5, 0.5, 0, 1, 0}
	} ;

	int piFacesTriangle[3][4] =
	{
		{0, 1, 3, 2},
		{2, 3, 5, 4},
		{0, 1, 5, 4}
	} ;

	int piNormalesTriangle[3][3] =
	{
		{-1, 0, 0},
		{1, 1, 0},
		{0, -1, 0}
	} ;

	glBegin(GL_QUADS) ;
	for(i = 0 ; i < 3 ; i++)
	{
		glNormal3f(piNormalesTriangle[i][0],piNormalesTriangle[i][1],piNormalesTriangle[i][2]) ;
		for(j = 0 ; j < 4 ; j ++)
		{
			glVertex3f(ppfVertexTriangle[piFacesTriangle[i][j]][0], ppfVertexTriangle[piFacesTriangle[i][j]][1], ppfVertexTriangle[piFacesTriangle[i][j]][2]) ;
		}
	}
	glEnd() ;

	glNormal3f(0, 0, 1) ;
	glBegin(GL_TRIANGLES) ;
	for(i = 0 ; i < 3 ; i++)
	{
		glVertex3f(ppfVertexTriangle[2*i+1][0], ppfVertexTriangle[2*i+1][1], ppfVertexTriangle[2*i+1][2]) ;
	}
	glEnd() ;


	glNormal3f(0, 0, -1) ;
	glBegin(GL_TRIANGLES) ;
	for(i = 0 ; i < 3 ; i++)
	{
		glVertex3f(ppfVertexTriangle[2*i][0], ppfVertexTriangle[2*i][1], ppfVertexTriangle[2*i][2]) ;
	}
	glEnd() ;
}

static void DrawSphere()
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
	double step = 2*M_PI / (n) ;


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

			x2 = cos (alphaS) * cos (betaS) ;
			y2 = sin (alphaS) ;
			z2 = cos (alphaS) * sin (betaS) ;

			glNormal3f(x1, y1, z1) ;
			glVertex3f(x1, y1, z1) ;

			glNormal3f(x2, y2, z2) ;
			glVertex3f(x2, y2, z2) ;
		}

		glEnd() ;
	}
}

static void DrawCutCone(float Longueur)
{
	float beta = 0 ;
	float betaS = 0 ;
	float x1 = 0 ;
	float y1 = 0 ;
	float z1 = 0 ;
	float x2 = 0 ;
	float y2 = 0 ;
	float z2 = 0 ;
	int i = 0 ;
	int n = 64 ;
	double step = 2*M_PI / (n) ;

	glBegin(GL_TRIANGLE_FAN) ;
	x1 = 0 ;
	y1 = 0.5 ;
	z1 = 0 ;
	glNormal3f(x1, y1, z1) ;
	glVertex3f(x1, y1, z1) ;
	for (i = 0 ; i <= n ; i++)
	{
		beta = i * step ;
		x1 = (1 - Longueur)*cos(beta) ;
		z1 = -(1 - Longueur)*sin(beta) ;
		glVertex3f(x1, y1, z1) ;
	}
	glEnd() ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for (i = 0 ; i <= n ; i++)
	{
		beta = i * step ;

		x1 = (1 - Longueur)*cos (beta) ;
		y1 = 0.5 ;
		z1 = -(1 - Longueur)*sin(beta) ;

		betaS = beta + step ;

		x2 = cos (beta) ;
		y2 = -0.5 ;
		z2 = -sin(beta) ;


		glNormal3f(x1, y1, z1) ;
		glVertex3f(x1, y1, z1) ;

		glNormal3f(x2, y2, z2) ;
		glVertex3f(x2, y2, z2) ;

	}
	glEnd() ;

	glBegin(GL_TRIANGLE_FAN) ;
	x1 = 0 ;
	y1 = -0.5 ;
	z1 = 0 ;
	glNormal3f(x1, y1, z1) ;
	glVertex3f(x1, y1, z1) ;
	for (i = 0 ; i <= n ; i++)
	{
		beta = i * step ;
		x1 = cos(beta) ;
		z1 = -sin(beta) ;
		glVertex3f(x1, y1, z1) ;
	}
	glEnd() ;
}

static void DrawCylinder()
{
	float beta = 0 ;
	float betaS = 0 ;
	float x1 = 0 ;
	float y1 = 0 ;
	float z1 = 0 ;
	float x2 = 0 ;
	float y2 = 0 ;
	float z2 = 0 ;
	int i = 0 ;
	int n = 64 ;
	double step = 2*M_PI / (n) ;

	glBegin(GL_TRIANGLE_FAN) ;
	x1 = 0 ;
	y1 = -0.5 ;
	z1 = 0 ;
	glNormal3f(x1, y1, z1) ;
	glVertex3f(x1, y1, z1) ;
	for (i = 0 ; i <= n ; i++)
	{
		beta = i * step ;
		x1 = cos(beta) ;
		z1 = -sin(beta) ;
		glVertex3f(x1, y1, z1) ;
	}
	glEnd() ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for (i = 0 ; i <= n ; i++)
	{
		beta = i * step ;

		x1 = cos (beta) ;
		y1 = -0.5 ;
		z1 = -sin(beta) ;

		betaS = beta + step ;

		x2 = cos (beta) ;
		y2 = 0.5 ;
		z2 = -sin(beta) ;


		glNormal3f(x1, y1, z1) ;
		glVertex3f(x1, y1, z1) ;

		glNormal3f(x2, y2, z2) ;
		glVertex3f(x2, y2, z2) ;

	}
	glEnd() ;

	glBegin(GL_TRIANGLE_FAN) ;
	x1 = 0 ;
	y1 = 0.5 ;
	z1 = 0 ;
	glNormal3f(x1, y1, z1) ;
	glVertex3f(x1, y1, z1) ;
	for (i = 0 ; i <= n ; i++)
	{
		beta = i * step ;
		x1 = cos(beta) ;
		z1 = -sin(beta) ;
		glVertex3f(x1, y1, z1) ;
	}
	glEnd() ;
}

/* Angle compris entre 1 et 64 ; 64 pour un cylindre complet. */
static void DrawPartialCylinder(int Angle)
{
	float beta = 0 ;
	float betaS = 0 ;
	float x1 = 0 ;
	float y1 = 0 ;
	float z1 = 0 ;
	float x2 = 0 ;
	float y2 = 0 ;
	float z2 = 0 ;
	int i = 0 ;
	int n = 64 ;
	double step = 2*M_PI / (n) ;

	glBegin(GL_TRIANGLE_FAN) ;
	x1 = 0 ;
	y1 = -0.5 ;
	z1 = 0 ;
	glNormal3f(x1, y1, z1) ;
	glVertex3f(x1, y1, z1) ;
	for (i = 0 ; i <= Angle ; i++)
	{
		beta = i * step ;
		x1 = cos(beta) ;
		z1 = -sin(beta) ;
		glVertex3f(x1, y1, z1) ;
	}
	glEnd() ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for (i = 0 ; i <= Angle ; i++)
	{
		beta = i * step ;

		x1 = cos (beta) ;
		y1 = -0.5 ;
		z1 = -sin(beta) ;

		betaS = beta + step ;

		x2 = cos (beta) ;
		y2 = 0.5 ;
		z2 = -sin(beta) ;


		glNormal3f(x1, y1, z1) ;
		glVertex3f(x1, y1, z1) ;

		glNormal3f(x2, y2, z2) ;
		glVertex3f(x2, y2, z2) ;

	}
	glEnd() ;

	glBegin(GL_TRIANGLE_FAN) ;
	x1 = 0 ;
	y1 = 0.5 ;
	z1 = 0 ;
	glNormal3f(x1, y1, z1) ;
	glVertex3f(x1, y1, z1) ;
	for (i = 0 ; i <= Angle ; i++)
	{
		beta = i * step ;
		x1 = cos(beta) ;
		z1 = -sin(beta) ;
		glVertex3f(x1, y1, z1) ;
	}
	glEnd() ;

	glNormal3f(0, 0, 1) ;
	glBegin(GL_QUADS) ;
	glVertex3f(1, 0.5, 0) ;
	glVertex3f(1, -0.5, 0) ;
	glVertex3f(0, -0.5, 0) ;
	glVertex3f(0, 0.5, 0) ;
	glEnd() ;

	glNormal3f(-cos(Angle*step), 0, -sin(Angle*step)) ;
	glBegin(GL_QUADS) ;
	glVertex3f(cos(Angle*step), 0.5, -sin(Angle*step)) ;
	glVertex3f(cos(Angle*step), -0.5, -sin(Angle*step)) ;
	glVertex3f(0, -0.5, 0) ;
	glVertex3f(0, 0.5, 0) ;
	glEnd() ;
}

static void DrawCone()
{
	float beta = 0 ;
	float x = 0 ;
	float y = 0 ;
	float z = 0 ;
	int i = 0 ;
	int n = 64 ;
	double step = 2*M_PI / (n) ;

	glBegin(GL_TRIANGLE_FAN) ;
	x = 0 ;
	y = -0.5 ;
	z = 0 ;
	glNormal3f(0, -1, 0) ;
	glVertex3f(x, y, z) ;
	for (i = 0 ; i <= n ; i++)
	{
		beta = i * step ;
		x = cos(beta) ;
		z = -sin(beta) ;
		glNormal3f(x, 0, z) ;
		glVertex3f(x, y, z) ;
	}
	glEnd() ;

	glBegin(GL_TRIANGLE_FAN) ;
	x = 0 ;
	y = 0.5 ;
	z = 0 ;
	glNormal3f(0, 1, 0) ;
	glVertex3f(x, y, z) ;
	for (i = 0 ; i <= n ; i++)
	{
		beta = i * step ;
		x = cos(beta) ;
		z = -sin(beta) ;
		glNormal3f(x, 0, z) ;
		glVertex3f(x, -y, z) ;
	}
	glEnd() ;

}

static void DrawRing(void)
{
	float beta = 0 ;
	float betaS = 0 ;
	float x1 = 0 ;
	float y1 = 0 ;
	float z1 = 0 ;
	float x2 = 0 ;
	float y2 = 0 ;
	float z2 = 0 ;
	int i = 0 ;
	int n = 64 ;
	double step = 2*M_PI / (n) ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for (i = 0 ; i <= n ; i++)
	{
		beta = i * step ;

		x1 = 0.9*cos (beta) ;
		y1 = -0.5 ;
		z1 = -0.9*sin(beta) ;

		betaS = beta + step ;

		x2 = 0.9*cos (beta) ;
		y2 = 0.5 ;
		z2 = -0.9*sin(beta) ;


		glNormal3f(x1, y1, z1) ;
		glVertex3f(x1, y1, z1) ;

		glNormal3f(x2, y2, z2) ;
		glVertex3f(x2, y2, z2) ;

	}
	glEnd() ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for (i = 0 ; i <= n ; i++)
	{
		beta = i * step ;

		x1 = cos (beta) ;
		y1 = -0.5 ;
		z1 = -sin(beta) ;

		betaS = beta + step ;

		x2 = cos (beta) ;
		y2 = 0.5 ;
		z2 = -sin(beta) ;


		glNormal3f(x1, y1, z1) ;
		glVertex3f(x1, y1, z1) ;

		glNormal3f(x2, y2, z2) ;
		glVertex3f(x2, y2, z2) ;

	}
	glEnd() ;

	glNormal3f(0, -1, 0) ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for (i = 0 ; i <= n ; i++)
	{
		beta = i * step ;

		x1 = 0.9*cos (beta) ;
		y1 = -0.5 ;
		z1 = -0.9*sin(beta) ;

		betaS = beta + step ;

		x2 = cos (beta) ;
		y2 = -0.5 ;
		z2 = -sin(beta) ;

		glVertex3f(x1, y1, z1) ;
		glVertex3f(x2, y2, z2) ;

	}
	glEnd() ;

	glNormal3f(0, 1, 0) ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for (i = 0 ; i <= n ; i++)
	{
		beta = i * step ;

		x1 = 0.9*cos (beta) ;
		y1 = 0.5 ;
		z1 = -0.9*sin(beta) ;

		betaS = beta + step ;

		x2 = cos (beta) ;
		y2 = 0.5 ;
		z2 = -sin(beta) ;

		glVertex3f(x1, y1, z1) ;
		glVertex3f(x2, y2, z2) ;

	}
	glEnd() ;

}

static void DrawHalfRing(void)
{
	float beta = 0 ;
	float betaS = 0 ;
	float x1 = 0 ;
	float y1 = 0 ;
	float z1 = 0 ;
	float x2 = 0 ;
	float y2 = 0 ;
	float z2 = 0 ;
	int i = 0 ;
	int n = 64 ;
	double step = 2*M_PI / (n) ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for (i = 0 ; i <= n/2 ; i++)
	{
		beta = i * step ;

		x1 = 0.9*cos (beta) ;
		y1 = -0.5 ;
		z1 = -0.9*sin(beta) ;

		betaS = beta + step ;

		x2 = 0.9*cos (beta) ;
		y2 = 0.5 ;
		z2 = -0.9*sin(beta) ;


		glNormal3f(x1, y1, z1) ;
		glVertex3f(x1, y1, z1) ;

		glNormal3f(x2, y2, z2) ;
		glVertex3f(x2, y2, z2) ;

	}
	glEnd() ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for (i = 0 ; i <= n/2 ; i++)
	{
		beta = i * step ;

		x1 = cos (beta) ;
		y1 = -0.5 ;
		z1 = -sin(beta) ;

		betaS = beta + step ;

		x2 = cos (beta) ;
		y2 = 0.5 ;
		z2 = -sin(beta) ;


		glNormal3f(x1, y1, z1) ;
		glVertex3f(x1, y1, z1) ;

		glNormal3f(x2, y2, z2) ;
		glVertex3f(x2, y2, z2) ;

	}
	glEnd() ;

	glNormal3f(0, -1, 0) ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for (i = 0 ; i <= n/2 ; i++)
	{
		beta = i * step ;

		x1 = 0.9*cos (beta) ;
		y1 = -0.5 ;
		z1 = -0.9*sin(beta) ;

		betaS = beta + step ;

		x2 = cos (beta) ;
		y2 = -0.5 ;
		z2 = -sin(beta) ;

		glVertex3f(x1, y1, z1) ;
		glVertex3f(x2, y2, z2) ;

	}
	glEnd() ;

	glNormal3f(0, 1, 0) ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for (i = 0 ; i <= n/2 ; i++)
	{
		beta = i * step ;

		x1 = 0.9*cos (beta) ;
		y1 = 0.5 ;
		z1 = -0.9*sin(beta) ;

		betaS = beta + step ;

		x2 = cos (beta) ;
		y2 = 0.5 ;
		z2 = -sin(beta) ;

		glVertex3f(x1, y1, z1) ;
		glVertex3f(x2, y2, z2) ;

	}
	glEnd() ;

	glNormal3f(0, 0, 1) ;
	glBegin(GL_QUADS) ;
	glVertex3f(1, 0.5, 0) ;
	glVertex3f(1, -0.5, 0) ;
	glVertex3f(0.9, -0.5, 0) ;
	glVertex3f(0.9, 0.5, 0) ;
	glEnd() ;

	glNormal3f(0, 0, 1) ;
	glBegin(GL_QUADS) ;
	glVertex3f(-1, 0.5, 0) ;
	glVertex3f(-1, -0.5, 0) ;
	glVertex3f(-0.9, -0.5, 0) ;
	glVertex3f(-0.9, 0.5, 0) ;
	glEnd() ;
}

/* Angle compris entre 1 et 64 ; 64 pour un anneau complet. */
static void DrawPartialRing(int Angle)
{
	float beta = 0 ;
	float betaS = 0 ;
	float x1 = 0 ;
	float y1 = 0 ;
	float z1 = 0 ;
	float x2 = 0 ;
	float y2 = 0 ;
	float z2 = 0 ;
	int i = 0 ;
	int n = 64 ;
	double step = 2*M_PI / (n) ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for (i = 0 ; i <= Angle ; i++)
	{
		beta = i * step ;

		x1 = 0.9*cos (beta) ;
		y1 = -0.5 ;
		z1 = -0.9*sin(beta) ;

		betaS = beta + step ;

		x2 = 0.9*cos (beta) ;
		y2 = 0.5 ;
		z2 = -0.9*sin(beta) ;


		glNormal3f(x1, y1, z1) ;
		glVertex3f(x1, y1, z1) ;

		glNormal3f(x2, y2, z2) ;
		glVertex3f(x2, y2, z2) ;

	}
	glEnd() ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for (i = 0 ; i <= Angle ; i++)
	{
		beta = i * step ;

		x1 = cos (beta) ;
		y1 = -0.5 ;
		z1 = -sin(beta) ;

		betaS = beta + step ;

		x2 = cos (beta) ;
		y2 = 0.5 ;
		z2 = -sin(beta) ;


		glNormal3f(x1, y1, z1) ;
		glVertex3f(x1, y1, z1) ;

		glNormal3f(x2, y2, z2) ;
		glVertex3f(x2, y2, z2) ;

	}
	glEnd() ;

	glNormal3f(0, -1, 0) ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for (i = 0 ; i <= Angle ; i++)
	{
		beta = i * step ;

		x1 = 0.9*cos (beta) ;
		y1 = -0.5 ;
		z1 = -0.9*sin(beta) ;

		betaS = beta + step ;

		x2 = cos (beta) ;
		y2 = -0.5 ;
		z2 = -sin(beta) ;

		glVertex3f(x1, y1, z1) ;
		glVertex3f(x2, y2, z2) ;

	}
	glEnd() ;

	glNormal3f(0, 1, 0) ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for (i = 0 ; i <= Angle ; i++)
	{
		beta = i * step ;

		x1 = 0.9*cos (beta) ;
		y1 = 0.5 ;
		z1 = -0.9*sin(beta) ;

		betaS = beta + step ;

		x2 = cos (beta) ;
		y2 = 0.5 ;
		z2 = -sin(beta) ;

		glVertex3f(x1, y1, z1) ;
		glVertex3f(x2, y2, z2) ;

	}
	glEnd() ;

	glNormal3f(0, 0, 1) ;
	glBegin(GL_QUADS) ;
	glVertex3f(1, 0.5, 0) ;
	glVertex3f(1, -0.5, 0) ;
	glVertex3f(0.9, -0.5, 0) ;
	glVertex3f(0.9, 0.5, 0) ;
	glEnd() ;

	glNormal3f(-cos(Angle*step), 0, -sin(Angle*step)) ;
	glBegin(GL_QUADS) ;
	glVertex3f(cos(Angle*step), 0.5, -sin(Angle*step)) ;
	glVertex3f(cos(Angle*step), -0.5, -sin(Angle*step)) ;
	glVertex3f(cos(Angle*step)*0.9, -0.5, -sin(Angle*step)*0.9) ;
	glVertex3f(cos(Angle*step)*0.9, 0.5, -sin(Angle*step)*0.9) ;
	glEnd() ;
}

static void DrawAWingMiddle()
{
	float z1 = 0.75 ;
	float z2 = 0.01 ;

		/* Début. */
	glPushMatrix() ;

		/* Avant. */

	glPushMatrix() ;
	glScalef(1, 0.75, z1) ;
	glTranslatef(0, 0, z2) ;

		/* Bout de l'avant. */
	glPushMatrix() ;
	glTranslatef(0, 0, -4.74) ;
	glScalef(0.2, 0.276, 1) ;
	glRotatef(45, 1, 0, 0) ;
	glColor3f(0.6, 0.20, 0.20) ;
	DrawCube() ;
	glPopMatrix() ;

		/* Fuselage haut. */
	glPushMatrix() ;
	glTranslatef(0, 0.39, -2.95) ;
	glRotatef(-7, 1, 0, 0) ;
	glScalef(0.2, 0.05, 3.6) ;
	glColor3f(0.6, 0.20, 0.20) ;
	DrawCube() ;
	glPopMatrix() ;

		/* Fuselage bas. */
	glPushMatrix() ;
	glTranslatef(0, -0.39, -2.95) ;
	glRotatef(7, 1, 0, 0) ;
	glTranslatef(0, 0, 0.6) ;
	glScalef(0.2, 0.05, 4.8) ;
	glColor3f(0.6, 0.20, 0.20) ;
	DrawCube() ;
	glPopMatrix() ;

	glPopMatrix() ;

		/* Cockpit. */

	glPushMatrix() ;
	glTranslatef(0,0.7*0.75,0) ;

	glPushMatrix() ;
	glScalef(1, 1, z1) ;
	glTranslatef(0, 0, z2) ;

		/* Bout. */
	glPushMatrix() ;
	glTranslatef(0, -0.1, -1.15) ;
	glScalef(0.2, 0.33, 0.2*2) ;
	glColor3f(0, 0, 0) ;
	DrawSphere() ;
	glPopMatrix() ;

		/* Habitacle amovible. */
	glPushMatrix();
	glScalef(0.5, 0.5, 1.5) ;
	glColor3f(0.75, 0.85, 1) ;
	DrawSphere() ;
	glPopMatrix() ;

		/* Anneau horizontal. */
	glPushMatrix() ;
	glScalef(1, 0.75, 1) ;
	glTranslatef(0, 0.035, 0) ;
	glRotatef(-7, 1, 0, 0) ;
	glScalef(0.52, 0.2, 1.5) ;
	glColor3f(0, 0, 0) ;
	DrawHalfRing() ;
	glPopMatrix() ;

	glPopMatrix() ;

		/* Anneau. */
	glPushMatrix() ;
	glScalef(0.52, 0.52, 0.1) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0, 0, 0) ;
	DrawRing() ;
	glPopMatrix() ;

		/* Habitacle. */
	glPushMatrix() ;
	glTranslatef(0, 0, 0.3) ;
	glScalef(0.51, 0.51, 0.6) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.75, 0.85, 1) ;
	DrawCylinder();
	glPopMatrix() ;

		/* Anneau horizontal. */
	glPushMatrix() ;
	glTranslatef(0, 0, 0.3) ;
	glScalef(1.04, 0.2, 0.6) ;
	glColor3f(0, 0, 0) ;
	DrawCube() ;
	glPopMatrix() ;

		/* Anneau. */
	glPushMatrix() ;
	glTranslatef(0, 0, 0.6) ;
	glScalef(0.53, 0.53, 0.1) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0, 0, 0) ;
	DrawRing() ;
	glPopMatrix() ;

		/* Structure. */
	glPushMatrix() ;
	glTranslatef(0, -0.08, 1.3) ;
	glRotatef(5, 1, 0, 0) ;
	glScalef(0.5, 0.5, 1.4) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.6, 0.20, 0.20) ;
	DrawCylinder() ;
	glPopMatrix() ;

		/* Remplissage. */
	glPushMatrix() ;
	glTranslatef(0, -0.08, 1.3) ;
	glRotatef(5, 1, 0, 0) ;
	glScalef(0.47, 0.47, 1.5) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.1, 0.1, 0.1) ;
	DrawCylinder() ;
	glPopMatrix() ;

		/* Bout. */
	glPushMatrix() ;
	glTranslatef(0, -0.08, 2.1) ;
	glRotatef(5, 1, 0, 0) ;
	glScalef(0.5, 0.5, 0.2) ;
	glRotatef(90, 1, 0, 0) ;
	glTranslatef(0, 0, 0.14) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawRing() ;
	glPopMatrix() ;

	glPopMatrix() ;

		/* Réacteur. */
	glPushMatrix() ;
	glScalef(0.791, 0.791, 0.8) ;
	glTranslatef(0, 0, 2.5) ;

		/* Réacteur Haut. */
	glPushMatrix() ;
	glScalef(0.7, 0.7, 1.4) ;
	glRotatef(80, 1, 0, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawHalfRing() ;
	glPopMatrix () ;

		/* Réacteur Bas. */
	glPushMatrix() ;
	glRotatef(180, 0, 0, 1) ;
	glScalef(0.7, 0.7, 1.4) ;
	glRotatef(80, 1, 0, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawHalfRing() ;
	glPopMatrix () ;

		/* Remplissage. */
	glPushMatrix() ;
	glScalef(0.65, 0.7, 0.6) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.40, 0.80, 1.00) ;					/* Couleur réacteur central de l'A-Wing. */
	DrawCylinder() ;
	glPopMatrix () ;

		/* Séparation. */
	glPushMatrix() ;
	glScalef(1.41, 0.12, 1.385) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawCube() ;
	glPopMatrix() ;

	glPopMatrix() ;

		/* Plaque basse. */
	glPushMatrix() ;
	glScalef(0.8, 1, 0.8) ;
	glTranslatef(0, -0.56+0.0085/2, 1.81915) ;
	glScalef(1.41, 0.0585, 2.0167) ;
	glColor3f(0.60, 0.20, 0.20) ;
	DrawCube() ;
	glPopMatrix() ;

		/* Fin. */
	glPopMatrix() ;

}

static void DrawAWingRight(int Armes)
{
	float x = -0.025 ;
	float z1 = 0.75 ;
	float z2 = 0.01 ;

		/* Début. */
	glPushMatrix() ;
	glTranslatef(1, 0, 0) ;

		/* Aile centrale. */

	glPushMatrix() ;
	glScalef(1, 0.75, 1) ;

	glPushMatrix() ;
	glScalef(1, 1, z1) ;
	glTranslatef(0, 0, z2) ;

		/* 0AB : Lien avant. */
	glPushMatrix() ;
	glTranslatef(-0.5, 0, -5.93075) ;
	glScalef(0.8, 0.0695, 0.2) ;
	glRotatef(45, 1, 0, 0) ;
	glColor3f(0.60, 0.20, 0.20) ;
	DrawCube() ;
	glPopMatrix() ;

		/* 0ABbis : Remplissage avant. */
	glPushMatrix() ;
	glTranslatef(-0.5, 0, -4.5) ;
	glScalef(0.8, 0.26, 2.2) ;
	glRotatef(45, 1, 0, 0) ;
	glColor3f(0.6, 0.20, 0.20) ;
	DrawCube() ;
	glPopMatrix() ;

		/* 1A : Structure rouge descendante. */
	glPushMatrix() ;
	glTranslatef(-0.5, 0.39, -2.95) ;
	glRotatef(-7, 1, 0, 0) ;

	glTranslatef(0, 0, -0.00155) ;
	glScalef(0.8, 0.05, 6-(0.00155*2)) ;

	glColor3f(0.6, 0.20, 0.20) ;
	DrawCube() ;
	glPopMatrix() ;

		/* 1B : Structure rouge montante. */
	glPushMatrix() ;
	glTranslatef(-0.5, -0.39, -2.95) ;
	glRotatef(7, 1, 0, 0) ;
	glScalef(0.8, 0.05, 6) ;
	glColor3f(0.6, 0.20, 0.20) ;
	DrawCube() ;
	glPopMatrix() ;

	glPopMatrix() ;

		/* 2 : Triangle rouge haut. */
	glPushMatrix() ;
	glTranslatef(-0.2868, 0.7549, 0.336) ;
	glScalef(0.3736, 0.05, 0.625) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.6, 0.20, 0.20) ;
	DrawTriangle() ;
	glPopMatrix() ;

		/* 3 : Triangle blanc haut. */
	glPushMatrix() ;
	glTranslatef(-0.2868, 0.7549, 0.336) ;
	glScalef(0.3736, 0.05, 0.625) ;
	glRotatef(-90, 1, 0, 0) ;
	glRotatef(180, 0, 1, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawTriangle() ;
	glPopMatrix() ;

		/* 3bis : Complément rouge haut. */
	glPushMatrix() ;
	glTranslatef(-0.4768, 0.7549, 0.336) ;
	glScalef(0.0064, 0.05, 0.625) ;
	glColor3f(0.60, 0.20, 0.20) ;
	DrawCube() ;
	glPopMatrix() ;

		/* 4 : Triangle rouge bas. */
	glPushMatrix() ;
	glTranslatef(-0.2673, -0.7549, 0.336) ;
	glScalef(0.3346, 0.05, 0.625) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.6, 0.20, 0.20) ;
	DrawTriangle() ;
	glPopMatrix() ;

		/* 4bis : Triangle blanc bas. */
	glPushMatrix() ;
	glTranslatef(-0.2673, -0.7549, 0.336) ;
	glScalef(0.3346, 0.05, 0.625) ;
	glRotatef(-90, 1, 0, 0) ;
	glRotatef(180, 0, 1, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawTriangle() ;
	glPopMatrix() ;

		/* 4ter : Complément rouge bas. */
	glPushMatrix() ;
	glTranslatef(-0.74, -0.7549, 0.336) ;
	glScalef(0.52+0.0454*2, 0.05, 0.625) ;
	glColor3f(0.60, 0.20, 0.20) ;
	DrawCube() ;
	glPopMatrix() ;

	glPopMatrix () ;


		/* Aile extérieure. */

	glPushMatrix() ;
	glScalef(0.8, 0.75, 1) ;

	glPushMatrix() ;
	glScalef(1, 1, z1) ;
	glTranslatef(x, 0, z2) ;

		/* 1 : Pointe avant. */
	glPushMatrix() ;
	glTranslatef(-0.1001, 0, -6.0014) ;
	glScalef(0.04916*2, 0.04916, 0.1414) ;
	glRotatef(-90, 1, 0, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawCone() ;
	glPopMatrix() ;

		/* 2 : Aile avant. */
	glPushMatrix() ;
	glTranslatef(-0.1001, 0, -2.9546) ;
	glScalef(0.78*2, 0.78, 5.9523) ;
	glRotatef(-90, 1, 0, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawCutCone(0.937) ;
	glPopMatrix() ;

	glPopMatrix() ;

		/* 3 : Aile arrière. */

	glPushMatrix() ;
	glTranslatef(x, 0, 0) ;

	if(Armes == 1)
	{
		glPushMatrix() ;
		glTranslatef(-0.1001, 0, 0.336) ;
		glScalef(0.78*2, 0.78, 0.625) ;
		glRotatef(-90, 1, 0, 0) ;
		glRotatef(270, 0, 1, 0) ;
		glColor3f(0.90, 0.90, 0.90) ;
		DrawPartialRing(15) ;
		glPopMatrix() ;

		glPushMatrix() ;
		glScalef(1, -1, 1) ;
		glTranslatef(-0.1001, 0, 0.336) ;
		glScalef(0.78*2, 0.78, 0.625) ;
		glRotatef(-90, 1, 0, 0) ;
		glRotatef(270, 0, 1, 0) ;
		glColor3f(0.90, 0.90, 0.90) ;
		DrawPartialRing(15) ;
		glPopMatrix() ;
	}
	else
	{
		glPushMatrix() ;
		glTranslatef(-0.1001, 0, 0.336) ;
		glScalef(0.78*2, 0.78, 0.625) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.90, 0.90, 0.90) ;
		DrawRing() ;
		glPopMatrix() ;
	}

		/* 3bis : Remplissage aile arrière. */
	glPushMatrix() ;
	glTranslatef(-0.1001, 0, 0.3) ;
	glScalef(0.77*2, 0.77, 0.5) ;
	glRotatef(-90, 1, 0, 0) ;
	glColor3f(0.1, 0.1, 0.1) ;
	DrawCylinder() ;
	glPopMatrix() ;

	glPopMatrix() ;

	glPopMatrix() ;

		/* Lasers. */
	if (Armes == 1)
	{
		glPushMatrix() ;
		glTranslatef(1.3, 0, 0.336) ;

			/* 1 : Cylindre de raccord. */
		glPushMatrix() ;
		glTranslatef(-0.3, 0, 0) ;
		glScalef(0.6, 0.05, 0.05) ;
		glRotatef(-90, 0, 0, 1) ;
		glColor3f(0.10, 0.10, 0.10) ;
		DrawCylinder() ;
		glPopMatrix() ;

			/* 2 - 1 : Laser principal - tube arrière. */
		glPushMatrix() ;
		glTranslatef(0, 0, 0) ;
		glScalef(0.06, 0.06, 0.60) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.10, 0.10, 0.10) ;
		DrawCylinder() ;
		glPopMatrix() ;

			/* 2 - 2 : Laser principal - tube principal. */
		glPushMatrix() ;
		glTranslatef(0, 0, -0.1) ;
		glScalef(0.05, 0.05, 1) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.10, 0.10, 0.10) ;
		DrawCylinder() ;
		glPopMatrix() ;

			/* 2 - 3 : Laser principal - tube blanc. */
		glPushMatrix() ;
		glTranslatef(0, 0, -0.9) ;
		glScalef(0.04, 0.04, 0.80) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.90, 0.90, 0.90) ;
		DrawCylinder() ;
		glPopMatrix() ;

			/* 2 - 4 : Laser principal - pointe rouge. */
		glPushMatrix() ;
		glTranslatef(0, 0, -1.35) ;
		glScalef(0.05, 0.05, 0.1) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.60, 0.20, 0.20) ;
		DrawCylinder() ;
		glPopMatrix() ;

			/* 2 - 5 : Laser principal - fin de la pointe. */
		glPushMatrix() ;
		glTranslatef(0, 0, -1.4) ;
		glScalef(0.05, 0.05, 0.05) ;
		glColor3f(0.60, 0.20, 0.20) ;
		DrawSphere() ;
		glPopMatrix() ;

			/* 3 - 1 : Cylindre de raccord entre les deux lasers 1. */
		glPushMatrix() ;
		glTranslatef(0, 0.075, -0.15) ;
		glScalef(0.01, 0.05, 0.01) ;
		glColor3f(0.10, 0.10, 0.10) ;
		DrawCylinder() ;
		glPopMatrix() ;

			/* 3 - 2 : Cylindre de raccord entre les deux lasers 2. */
		glPushMatrix() ;
		glTranslatef(0, 0.075, 0.15) ;
		glScalef(0.01, 0.05, 0.01) ;
		glColor3f(0.10, 0.10, 0.10) ;
		DrawCylinder() ;
		glPopMatrix() ;

			/* 4 - 1 : Laser secondaire - tube arrière. */
		glPushMatrix() ;
		glTranslatef(0, 0.1, 0.6) ;
		glScalef(0.03, 0.03, 0.05) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.10, 0.10, 0.10) ;
		DrawCylinder() ;
		glPopMatrix() ;

			/* 4 - 2 : Laser secondaire - tube principal. */
		glPushMatrix() ;
		glTranslatef(0, 0.1, 0.05) ;
		glScalef(0.02, 0.02, 1.15) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.10, 0.10, 0.10) ;
		DrawCylinder() ;
		glPopMatrix() ;

			/* 4 - 2bis : Laser secondaire - tube principal. */
		glPushMatrix() ;
		glTranslatef(0, 0.1, -0.575) ;
		glScalef(0.02, 0.02, 0.1) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.10, 0.10, 0.10) ;
		DrawCone() ;
		glPopMatrix() ;

			/* 4 - 3 : Laser secondaire - tube fin. */
		glPushMatrix() ;
		glTranslatef(0, 0.1, -0.69) ;
		glScalef(0.015, 0.015, 0.35) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.10, 0.10, 0.10) ;
		DrawCylinder() ;
		glPopMatrix() ;

			/* 4 - 4 : Laser secondaire - lien. */
		glPushMatrix() ;
		glTranslatef(0, 0.1, -0.8375) ;
		glScalef(0.02, 0.02, 0.05) ;
		glRotatef(90, 1, 0, 0) ;
		glColor3f(0.10, 0.10, 0.10) ;
		DrawCone() ;
		glPopMatrix() ;

			/* 4 - 5 : Laser secondaire - pointe. */
		glPushMatrix() ;
		glTranslatef(0, 0.1, -0.8875) ;
		glScalef(0.02, 0.02, 0.05) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.60, 0.20, 0.20) ;
		DrawCone() ;
		glPopMatrix() ;

		glPopMatrix() ;
	}

		/* Corps arrière. */

	glPushMatrix() ;

		/* 1 : Rectangle blanc haut. */
	glPushMatrix() ;
	glTranslatef(-0.55, 0.56, 1.45532) ;
	glScalef(0.90, 0.05, 1.61336) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawCube() ;
	glPopMatrix() ;

		/* 1bis : Rectangle blanc bas. */
	glPushMatrix() ;
	glTranslatef(-0.268, -0.56+0.0085/2, 1.45532) ;
	glScalef(0.336, 0.0585, 1.61336) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawCube() ;
	glPopMatrix() ;

		/* 1ter : Remplissage arrière. */
	glPushMatrix() ;
	glTranslatef(-0.55, 0, 1.41) ;
	glScalef(0.90, 1.1, 1.61336) ;
	glColor3f(0.1, 0.1, 0.1) ;
	DrawCube() ;
	glPopMatrix() ;

		/* 2 : Structure basse complémentaire. */
	glPushMatrix() ;
	glTranslatef(-0.1001, 0, 1.45532) ;
	glScalef(0.78*2*0.75, 0.78*0.75, 1.61336) ;
	glRotatef(-90, 1, 0, 0) ;
	glRotatef(90, 0, 1, 0) ;
	glScalef(1, 1, -1) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawPartialRing(2) ;
	glPopMatrix() ;

		/* 2bis : Remplissage. */
	glPushMatrix() ;
	glTranslatef(-0.1001, 0, 1.41) ;
	glScalef(0.77*2*0.75, 0.77*0.75, 1.61336) ;
	glRotatef(-90, 1, 0, 0) ;
	glRotatef(90, 0, 1, 0) ;
	glScalef(1, 1, -1) ;
	glColor3f(0.10, 0.10, 0.10) ;
	DrawPartialCylinder(2) ;
	glPopMatrix() ;

	glPopMatrix() ;

		/* Réacteur. */

	glPushMatrix() ;

		/* 1 : Tour réacteur. */
	glPushMatrix() ;
	glTranslatef(0, 0, 1.5) ;
	glScalef(0.4, 0.4, 2) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.60, 0.20, 0.20) ;
	DrawRing() ;
	glPopMatrix() ;

		/* 2 : Lien avec le réacteur central. */
	glPushMatrix() ;
	glTranslatef(-0.5, 0, 2.51) ;
	glScalef(0.2, 0.01, 0.01) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawCube() ;
	glPopMatrix() ;

		/* 3 : Anneau réacteur. */
	glPushMatrix() ;
	glTranslatef(0, 0, 2.6) ;
	glScalef(0.4, 0.4, 0.2) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawRing() ;
	glPopMatrix() ;

		/* 4 : Réacteur. */
	glPushMatrix() ;
	glTranslatef(0, 0, 1.5) ;
	glScalef(0.4, 0.4, 2) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(1.00, 1.00, 0.70) ;					/* Couleur réacteurs secondaires de l'A-Wing. */
	DrawCylinder() ;
	glPopMatrix() ;

		/* 5 : Anneau fin. */
	glPushMatrix() ;
	glTranslatef(0, 0, 3.2) ;
	glScalef(0.4, 0.4, 0.1) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawRing() ;
	glPopMatrix() ;

	glPopMatrix () ;

		/* Aileron haut. */

	glPushMatrix() ;
	glRotatef(2*360/64, 0, 0, 1) ;

		/* 1 : Rectangle bas. */
	glPushMatrix() ;
	glTranslatef(0, 0.48, 1.95);
	glScalef(0.05, 0.24, 2.6);
	glColor3f(0.90, 0.90, 0.90);
	DrawCube() ;
	glPopMatrix() ;

		/* 2 : Cylindre partiel avant. */
	glPushMatrix() ;
	glTranslatef(0, 0.6, 0.75) ;
	glScalef(0.05, 0.1, 0.1);
	glRotatef(90, 0, 0, 1) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawPartialCylinder(16) ;
	glPopMatrix() ;

		/* 3 : Rectangle milieu bas. */
	glPushMatrix() ;
	glTranslatef(0, 0.65, 2);
	glScalef(0.05, 0.1, 2.5);
	glColor3f(0.90, 0.90, 0.90);
	DrawCube() ;
	glPopMatrix() ;

		/* 4 : Triangle milieu. */
	glPushMatrix() ;
	glTranslatef(0, 1, 1.5);
	glScalef(0.05, 0.6, 0.2);
	glRotatef(90, 0, 1, 0) ;
	glColor3f(0.90, 0.90, 0.90);
	DrawTriangle() ;
	glPopMatrix() ;

		/* 5 : Cylindre partiel milieu. */
	glPushMatrix() ;
	glTranslatef(0, 1.271, 1.696) ;
	glScalef(0.05, 0.1, 0.1);
	glRotatef(90, 0, 0, 1) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawPartialCylinder(13) ;
	glPopMatrix() ;

		/* 6 : Rectangle milieu. */
	glPushMatrix() ;
	glTranslatef(0, 1, 2.325);
	glScalef(0.05, 0.6, 1.45);
	glColor3f(0.90, 0.90, 0.90);
	DrawCube() ;
	glPopMatrix() ;

		/* 7 : Rectangle milieu haut. */
	glPushMatrix() ;
	glTranslatef(0, 1.335, 2.373);
	glScalef(0.05, 0.07, 1.354);
	glColor3f(0.90, 0.90, 0.90);
	DrawCube() ;
	glPopMatrix() ;

		/* 8 : Cylindre partiel haut. */
	glPushMatrix() ;
	glTranslatef(0, 1.17, 3.05) ;
	glScalef(0.05, 0.2, 0.2);
	glRotatef(90, 0, 0, 1) ;
	glRotatef(-90, 0, 1, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawPartialCylinder(16) ;
	glPopMatrix() ;

		/* 9 : Rectangle arrière. */
	glPushMatrix() ;
	glTranslatef(0, 0.935, 3.15);
	glScalef(0.05, 0.47, 0.2);
	glColor3f(0.90, 0.90, 0.90);
	DrawCube() ;
	glPopMatrix() ;

	glPopMatrix() ;

		/* Aileron bas. */

	glPushMatrix() ;
	glRotatef(2*360/64, 0, 0, 1) ;
	glScalef(1, -1, 1) ;

		/* 1 : Rectangle bas. */
	glPushMatrix() ;
	glTranslatef(0, 0.48, 1.95);
	glScalef(0.05, 0.24, 2.6);
	glColor3f(0.90, 0.90, 0.90);
	DrawCube() ;
	glPopMatrix() ;

		/* 2 : Cylindre partiel avant. */
	glPushMatrix() ;
	glTranslatef(0, 0.6, 0.75) ;
	glScalef(0.05, 0.1, 0.1);
	glRotatef(90, 0, 0, 1) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawPartialCylinder(16) ;
	glPopMatrix() ;

		/* 3 : Rectangle milieu bas. */
	glPushMatrix() ;
	glTranslatef(0, 0.65, 2);
	glScalef(0.05, 0.1, 2.5);
	glColor3f(0.90, 0.90, 0.90);
	DrawCube() ;
	glPopMatrix() ;

		/* 4 : Triangle milieu. */
	glPushMatrix() ;
	glTranslatef(0, 1, 1.5);
	glScalef(0.05, 0.6, 0.2);
	glRotatef(90, 0, 1, 0) ;
	glColor3f(0.90, 0.90, 0.90);
	DrawTriangle() ;
	glPopMatrix() ;

		/* 5 : Cylindre partiel milieu. */
	glPushMatrix() ;
	glTranslatef(0, 1.271, 1.696) ;
	glScalef(0.05, 0.1, 0.1);
	glRotatef(90, 0, 0, 1) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawPartialCylinder(13) ;
	glPopMatrix() ;

		/* 6 : Rectangle milieu. */
	glPushMatrix() ;
	glTranslatef(0, 1, 2.325);
	glScalef(0.05, 0.6, 1.45);
	glColor3f(0.90, 0.90, 0.90);
	DrawCube() ;
	glPopMatrix() ;

		/* 7 : Rectangle milieu haut. */
	glPushMatrix() ;
	glTranslatef(0, 1.335, 2.373);
	glScalef(0.05, 0.07, 1.354);
	glColor3f(0.90, 0.90, 0.90);
	DrawCube() ;
	glPopMatrix() ;

		/* 8 : Cylindre partiel haut. */
	glPushMatrix() ;
	glTranslatef(0, 1.17, 3.05) ;
	glScalef(0.05, 0.2, 0.2);
	glRotatef(90, 0, 0, 1) ;
	glRotatef(-90, 0, 1, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	DrawPartialCylinder(16) ;
	glPopMatrix() ;

		/* 9 : Rectangle arrière. */
	glPushMatrix() ;
	glTranslatef(0, 0.935, 3.15);
	glScalef(0.05, 0.47, 0.2);
	glColor3f(0.90, 0.90, 0.90);
	DrawCube() ;
	glPopMatrix() ;

	glPopMatrix () ;

		/* Fin. */
	glPopMatrix() ;
}

static void DrawAWing(int Armes)
{
		/* Initialisation. */
	glPushMatrix() ;
	glDisable(GL_TEXTURE_2D) ;

	DrawAWingRight(Armes) ;
	DrawAWingMiddle() ;
	glScalef(-1, 1, 1) ;
	DrawAWingRight(Armes) ;

		/* Fin. */
	glPopMatrix() ;
}


/* ********************************************************************* */
/*                                                                       */
/*                    Fonctions générales OpenGL                         */
/*                                                                       */
/* ********************************************************************* */

static void printSelectedStar(Star * pStar)
{
	int iRotations = 4 ;
	float fRotationAngle = 360/iRotations ;
	float fTime = ((float) (clock())) / CLOCKS_PER_SEC ;

	while (fTime >= iRotations) fTime = fTime - iRotations ;

	glPushMatrix() ;

	glTranslatef(starGetX(pStar), starGetY(pStar), starGetZ(pStar)) ;
	glRotatef(-15, 0, 0, 1) ;
	glRotatef(fTime*fRotationAngle, 0, 1, 0) ;
	glTranslatef(0.75, 0, 1) ;
	glRotatef(-90, 0, 0, 1) ;
	glScalef(0.05, 0.05, 0.05) ;
	DrawAWing(1) ;

	glPopMatrix() ;
}

static int DrawStar(Star * psStar)
{
	assert(psStar != NULL) ;

	glPushMatrix() ;
	glTranslatef(starGetX(psStar), starGetY(psStar), starGetZ(psStar)) ;
	glScalef(starGetSize(psStar), starGetSize(psStar), starGetSize(psStar)) ;
	glColor3f(starGetColourR(psStar), starGetColourG(psStar), starGetColourB(psStar)) ;
	DrawSphere() ;
	glPopMatrix() ;

	return 0 ;
}

static void sceneDraw(gpointer* pData)
{
	Star sStar ;
	AnalyzedTrack *psTrack = NULL ;
	psTrack = analyzedTrackCreateWithData(100, NULL, 0, 0) ;

	starCreate(& sStar, psTrack) ;

	/* printSelectedStar(& sStar) ; */

	glPushMatrix() ;
	DrawStar(& sStar) ;
	glPopMatrix() ;
}


/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions d'affichage OpenGL                        */
/*                                                                       */
/* ********************************************************************* */

gboolean drawingButtonMouse (GtkWidget * psWidget,
							 GdkEventButton * psEvent,
							 gpointer * pData)
{
	switch (psEvent->button)
	{
		case 4 :		/* Pifomètre haut */
			printf("Touche appuyée : gauche\n") ;
			((OpenGLData*)pData[OPENGLDATA])->fEyeZ = ((OpenGLData*)pData[OPENGLDATA])->fEyeZ + 1 ;
			break ;
		case 5 :		/* Pifomètre bas */
			printf("Touche appuyée : droite\n") ;
			((OpenGLData*)pData[OPENGLDATA])->fEyeZ = ((OpenGLData*)pData[OPENGLDATA])->fEyeZ - 1 ;
			break ;

		default:
			printf("Touche appuyée : %x\n", psEvent->button) ;
			break;
	}
	
	return TRUE ;
}

gboolean drawingMotionMouse (GtkWidget * psWidget,
							 GdkDragContext * psDragContext,
							 gint iPostionX,
							 gint iPostionY,
							 guint iTime,
							 gpointer * pData)
{
	return TRUE ;
}

gboolean drawingKeyboard (GtkWidget * psWidget,
						  GdkEventKey * psEvent,
						  gpointer * pData)
{
	switch (psEvent->keyval)
	{
		case 0xff51 :		/* Flèche gauche */
			printf("Touche appuyée : gauche\n") ;
			((OpenGLData*)pData[OPENGLDATA])->fCenterX = ((OpenGLData *)pData[OPENGLDATA])->fCenterX + 1 ;
			((OpenGLData*)pData[OPENGLDATA])->fEyeX = ((OpenGLData*)pData[OPENGLDATA])->fEyeX + 1 ;
			break ;
		case 0xff53 :		/* Flèche droite */
			printf("Touche appuyée : droite\n") ;
			((OpenGLData*)pData[OPENGLDATA])->fCenterX = ((OpenGLData*)pData[OPENGLDATA])->fCenterX - 1 ;
			((OpenGLData*)pData[OPENGLDATA])->fEyeX = ((OpenGLData*)pData[OPENGLDATA])->fEyeX - 1 ;
			break ;
		case 0xff54 :		/* Flèche basse */
			printf("Touche appuyée : bas\n") ;
			((OpenGLData*)pData[OPENGLDATA])->fCenterY = ((OpenGLData*)pData[OPENGLDATA])->fCenterY - 1 ;
			((OpenGLData*)pData[OPENGLDATA])->fEyeY = ((OpenGLData*)pData[OPENGLDATA])->fEyeY - 1 ;
			break ;
		case 0xff52 :		/* Flèche haute */
			printf("Touche appuyée : haut\n") ;
			((OpenGLData*)pData[OPENGLDATA])->fCenterY = ((OpenGLData*)pData[OPENGLDATA])->fCenterY + 1 ;
			((OpenGLData*)pData[OPENGLDATA])->fEyeY = ((OpenGLData*)pData[OPENGLDATA])->fEyeY + 1 ;
			break ;
		default:
			printf("Touche appuyée : %x\n", psEvent->keyval) ;
			break;
	}
	
	gtk_widget_queue_draw(psWidget) ;
	
	return TRUE ;
}

int drawingGlInit (GtkWidget* psWidget, gpointer* pData)
{
	GdkGLContext * contexte = NULL;
	GdkGLDrawable * surface = NULL;

	printf("Fonction d'initialisation Open_GL\n");

	contexte = gtk_widget_get_gl_context(psWidget);
	surface = gtk_widget_get_gl_drawable(psWidget);
	
	pData[OPENGLDATA] = (OpenGLData *) malloc (sizeof(OpenGLData)) ;

	printf("\tContexte et surface récupérés.\n");

	if(gdk_gl_drawable_gl_begin(surface,contexte))
	{
		/* appels OpenGL */

		printf("\tOuverture de la surface OK\n");

		/* Début de l'initialisation. */

		glClearColor(0.0f, 0.0f, 0.1f, 1.0f) ;
		glClearDepth(1.0) ;
		glDepthFunc(GL_LESS) ;
		glEnable(GL_DEPTH_TEST) ;
		glShadeModel(GL_SMOOTH) ;

		glEnable(GL_NORMALIZE);

		((OpenGLData*)pData[OPENGLDATA])->fCenterX = 0 ;
		((OpenGLData*)pData[OPENGLDATA])->fCenterY = 0 ;
		((OpenGLData*)pData[OPENGLDATA])->fCenterZ = 0 ;
		((OpenGLData*)pData[OPENGLDATA])->fEyeX = 0 ;
		((OpenGLData*)pData[OPENGLDATA])->fEyeY = 0 ;
		((OpenGLData*)pData[OPENGLDATA])->fEyeZ = 100 ;

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

	printf("Fonction de redimenssionnement\n");

	contexte = gtk_widget_get_gl_context(psWidget);
	surface = gtk_widget_get_gl_drawable(psWidget);

	if(gdk_gl_drawable_gl_begin(surface,contexte))
	{
		if (psEvent->width >=psEvent->width)
		{
			glViewport(0, 0, psEvent->width, psEvent->width);
		}
		else
		{
			glViewport(0, 0, psEvent->height, psEvent->height);
		}

		glMatrixMode(GL_PROJECTION) ;
		glLoadIdentity() ;
		gluPerspective(90, (GLfloat) psEvent->width / (GLfloat) psEvent->height , 0, 10000);
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

	printf("Fonction de dessin Open_GL\n");

	contexte = gtk_widget_get_gl_context(psWidget);
	surface = gtk_widget_get_gl_drawable(psWidget);

	if(gdk_gl_drawable_gl_begin(surface,contexte))
	{
		/* appels OpenGL */

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Début des dessins. */

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		gluLookAt(((OpenGLData*)pData[OPENGLDATA])->fCenterX,
				  ((OpenGLData*)pData[OPENGLDATA])->fCenterY,
				  ((OpenGLData*)pData[OPENGLDATA])->fCenterZ,
				  ((OpenGLData*)pData[OPENGLDATA])->fEyeX,
				  ((OpenGLData*)pData[OPENGLDATA])->fEyeY,
				  ((OpenGLData*)pData[OPENGLDATA])->fEyeZ,
				  0, 1, 0) ;

		sceneDraw(pData) ;


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
