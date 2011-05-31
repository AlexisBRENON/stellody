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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <gtk/gtkgl.h>
#include <gdk/gdkkeysyms.h>

#include "gui.h"
#include "star.h"
#include "image.h"
#include "analyzed_track.h"
#include "opengl_drawing.h"

#ifndef M_PI
	#define M_PI 3.14159 /**< Défini la constante PI si elle ne l'est pas.*/
#endif


/* ********************************************************************* */
/*                                                                       */
/*                    Fonctions de dessin OpenGL                         */
/*                                                                       */
/* ********************************************************************* */

static void drawCube()
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
		glNormal3f(piNormalesCube[i][0],
				   piNormalesCube[i][1],
				   piNormalesCube[i][2]) ;

		for(j = 0 ; j < 4 ; j ++)
		{
			switch (Colour)
			{
				case 0 :
					glColor3f(ppfVertexCube[piFacesCube[i][j]][3],
							  ppfVertexCube[piFacesCube[i][j]][4],
							  ppfVertexCube[piFacesCube[i][j]][5]) ;
					break ;
				default :
					break ;
			}
			glVertex3f(ppfVertexCube[piFacesCube[i][j]][0],
					   ppfVertexCube[piFacesCube[i][j]][1],
					   ppfVertexCube[piFacesCube[i][j]][2]) ;
		}
	}
	glEnd() ;
}

static void drawTriangle(void)
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
		glNormal3f(piNormalesTriangle[i][0],
				   piNormalesTriangle[i][1],
				   piNormalesTriangle[i][2]) ;

		for(j = 0 ; j < 4 ; j ++)
		{
			glVertex3f(ppfVertexTriangle[piFacesTriangle[i][j]][0],
					   ppfVertexTriangle[piFacesTriangle[i][j]][1],
					   ppfVertexTriangle[piFacesTriangle[i][j]][2]) ;
		}
	}
	glEnd() ;

	glNormal3f(0, 0, 1) ;
	glBegin(GL_TRIANGLES) ;
	for(i = 0 ; i < 3 ; i++)
	{
		glVertex3f(ppfVertexTriangle[2*i+1][0],
				   ppfVertexTriangle[2*i+1][1],
				   ppfVertexTriangle[2*i+1][2]) ;
	}
	glEnd() ;


	glNormal3f(0, 0, -1) ;
	glBegin(GL_TRIANGLES) ;
	for(i = 0 ; i < 3 ; i++)
	{
		glVertex3f(ppfVertexTriangle[2*i][0],
				   ppfVertexTriangle[2*i][1],
				   ppfVertexTriangle[2*i][2]) ;
	}
	glEnd() ;
}

static void drawSphere(int iPrecision)
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
	int n = iPrecision ;
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

static void drawCutCone(float Longueur, int iPrecision)
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
	int n = iPrecision ;
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

static void drawCylinder(int iPrecision)
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
	int n = iPrecision ;
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
static void drawPartialCylinder(int Angle, int iPrecision)
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
	int n = iPrecision ;
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

static void drawCone(int iPrecision)
{
	float beta = 0 ;
	float x = 0 ;
	float y = 0 ;
	float z = 0 ;
	int i = 0 ;
	int n = iPrecision ;
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

static void drawRing(int iPrecision)
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
	int n = iPrecision ;
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

static void drawHalfRing(int iPrecision)
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
	int n = iPrecision ;
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
static void drawPartialRing(int Angle, int iPrecision)
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
	int n = iPrecision ;
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

static void drawAWingMiddle()
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
	drawCube() ;
	glPopMatrix() ;

		/* Fuselage haut. */
	glPushMatrix() ;
	glTranslatef(0, 0.39, -2.95) ;
	glRotatef(-7, 1, 0, 0) ;
	glScalef(0.2, 0.05, 3.6) ;
	glColor3f(0.6, 0.20, 0.20) ;
	drawCube() ;
	glPopMatrix() ;

		/* Fuselage bas. */
	glPushMatrix() ;
	glTranslatef(0, -0.39, -2.95) ;
	glRotatef(7, 1, 0, 0) ;
	glTranslatef(0, 0, 0.6) ;
	glScalef(0.2, 0.05, 4.8) ;
	glColor3f(0.6, 0.20, 0.20) ;
	drawCube() ;
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
	drawSphere(64) ;
	glPopMatrix() ;

		/* Habitacle amovible. */
	glPushMatrix();
	glScalef(0.5, 0.5, 1.5) ;
	glColor3f(0.75, 0.85, 1) ;
	drawSphere(64) ;
	glPopMatrix() ;

		/* Anneau horizontal. */
	glPushMatrix() ;
	glScalef(1, 0.75, 1) ;
	glTranslatef(0, 0.035, 0) ;
	glRotatef(-7, 1, 0, 0) ;
	glScalef(0.52, 0.2, 1.5) ;
	glColor3f(0, 0, 0) ;
	drawHalfRing(64) ;
	glPopMatrix() ;

	glPopMatrix() ;

		/* Anneau. */
	glPushMatrix() ;
	glScalef(0.52, 0.52, 0.1) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0, 0, 0) ;
	drawRing(64) ;
	glPopMatrix() ;

		/* Habitacle. */
	glPushMatrix() ;
	glTranslatef(0, 0, 0.3) ;
	glScalef(0.51, 0.51, 0.6) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.75, 0.85, 1) ;
	drawCylinder(64);
	glPopMatrix() ;

		/* Anneau horizontal. */
	glPushMatrix() ;
	glTranslatef(0, 0, 0.3) ;
	glScalef(1.04, 0.2, 0.6) ;
	glColor3f(0, 0, 0) ;
	drawCube() ;
	glPopMatrix() ;

		/* Anneau. */
	glPushMatrix() ;
	glTranslatef(0, 0, 0.6) ;
	glScalef(0.53, 0.53, 0.1) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0, 0, 0) ;
	drawRing(64) ;
	glPopMatrix() ;

		/* Structure. */
	glPushMatrix() ;
	glTranslatef(0, -0.08, 1.3) ;
	glRotatef(5, 1, 0, 0) ;
	glScalef(0.5, 0.5, 1.4) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.6, 0.20, 0.20) ;
	drawCylinder(64) ;
	glPopMatrix() ;

		/* Remplissage. */
	glPushMatrix() ;
	glTranslatef(0, -0.08, 1.3) ;
	glRotatef(5, 1, 0, 0) ;
	glScalef(0.47, 0.47, 1.5) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.1, 0.1, 0.1) ;
	drawCylinder(64) ;
	glPopMatrix() ;

		/* Bout. */
	glPushMatrix() ;
	glTranslatef(0, -0.08, 2.1) ;
	glRotatef(5, 1, 0, 0) ;
	glScalef(0.5, 0.5, 0.2) ;
	glRotatef(90, 1, 0, 0) ;
	glTranslatef(0, 0, 0.14) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawRing(64) ;
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
	drawHalfRing(64) ;
	glPopMatrix () ;

		/* Réacteur Bas. */
	glPushMatrix() ;
	glRotatef(180, 0, 0, 1) ;
	glScalef(0.7, 0.7, 1.4) ;
	glRotatef(80, 1, 0, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawHalfRing(64) ;
	glPopMatrix () ;

		/* Remplissage. */
	glPushMatrix() ;
	glScalef(0.65, 0.7, 0.66) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.40, 0.80, 1.00) ;
	drawCylinder(64) ;
	glPopMatrix () ;

		/* Séparation. */
	glPushMatrix() ;
	glScalef(1.41, 0.12, 1.385) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawCube() ;
	glPopMatrix() ;

	glPopMatrix() ;

		/* Plaque basse. */
	glPushMatrix() ;
	glScalef(0.8, 1, 0.8) ;
	glTranslatef(0, -0.56+0.0085/2, 1.81915) ;
	glScalef(1.41, 0.0585, 2.0167) ;
	glColor3f(0.60, 0.20, 0.20) ;
	drawCube() ;
	glPopMatrix() ;

		/* Fin. */
	glPopMatrix() ;

}

static void drawAWingRight(int Armes)
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
	drawCube() ;
	glPopMatrix() ;

		/* 0ABbis : Remplissage avant. */
	glPushMatrix() ;
	glTranslatef(-0.5, 0, -4.5) ;
	glScalef(0.8, 0.26, 2.2) ;
	glRotatef(45, 1, 0, 0) ;
	glColor3f(0.6, 0.20, 0.20) ;
	drawCube() ;
	glPopMatrix() ;

		/* 1A : Structure rouge descendante. */
	glPushMatrix() ;
	glTranslatef(-0.5, 0.39, -2.95) ;
	glRotatef(-7, 1, 0, 0) ;

	glTranslatef(0, 0, -0.00155) ;
	glScalef(0.8, 0.05, 6-(0.00155*2)) ;

	glColor3f(0.6, 0.20, 0.20) ;
	drawCube() ;
	glPopMatrix() ;

		/* 1B : Structure rouge montante. */
	glPushMatrix() ;
	glTranslatef(-0.5, -0.39, -2.95) ;
	glRotatef(7, 1, 0, 0) ;
	glScalef(0.8, 0.05, 6) ;
	glColor3f(0.6, 0.20, 0.20) ;
	drawCube() ;
	glPopMatrix() ;

	glPopMatrix() ;

		/* 2 : Triangle rouge haut. */
	glPushMatrix() ;
	glTranslatef(-0.2868, 0.7549, 0.336) ;
	glScalef(0.3736, 0.05, 0.625) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.6, 0.20, 0.20) ;
	drawTriangle() ;
	glPopMatrix() ;

		/* 3 : Triangle blanc haut. */
	glPushMatrix() ;
	glTranslatef(-0.2868, 0.7549, 0.336) ;
	glScalef(0.3736, 0.05, 0.625) ;
	glRotatef(-90, 1, 0, 0) ;
	glRotatef(180, 0, 1, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawTriangle() ;
	glPopMatrix() ;

		/* 3bis : Complément rouge haut. */
	glPushMatrix() ;
	glTranslatef(-0.4768, 0.7549, 0.336) ;
	glScalef(0.0064, 0.05, 0.625) ;
	glColor3f(0.60, 0.20, 0.20) ;
	drawCube() ;
	glPopMatrix() ;

		/* 4 : Triangle rouge bas. */
	glPushMatrix() ;
	glTranslatef(-0.2673, -0.7549, 0.336) ;
	glScalef(0.3346, 0.05, 0.625) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.6, 0.20, 0.20) ;
	drawTriangle() ;
	glPopMatrix() ;

		/* 4bis : Triangle blanc bas. */
	glPushMatrix() ;
	glTranslatef(-0.2673, -0.7549, 0.336) ;
	glScalef(0.3346, 0.05, 0.625) ;
	glRotatef(-90, 1, 0, 0) ;
	glRotatef(180, 0, 1, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawTriangle() ;
	glPopMatrix() ;

		/* 4ter : Complément rouge bas. */
	glPushMatrix() ;
	glTranslatef(-0.74, -0.7549, 0.336) ;
	glScalef(0.52+0.0454*2, 0.05, 0.625) ;
	glColor3f(0.60, 0.20, 0.20) ;
	drawCube() ;
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
	drawCone(64) ;
	glPopMatrix() ;

		/* 2 : Aile avant. */
	glPushMatrix() ;
	glTranslatef(-0.1001, 0, -2.9546) ;
	glScalef(0.78*2, 0.78, 5.9523) ;
	glRotatef(-90, 1, 0, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawCutCone(0.937, 64) ;
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
		drawPartialRing(15, 64) ;
		glPopMatrix() ;

		glPushMatrix() ;
		glScalef(1, -1, 1) ;
		glTranslatef(-0.1001, 0, 0.336) ;
		glScalef(0.78*2, 0.78, 0.625) ;
		glRotatef(-90, 1, 0, 0) ;
		glRotatef(270, 0, 1, 0) ;
		glColor3f(0.90, 0.90, 0.90) ;
		drawPartialRing(15, 64) ;
		glPopMatrix() ;
	}
	else
	{
		glPushMatrix() ;
		glTranslatef(-0.1001, 0, 0.336) ;
		glScalef(0.78*2, 0.78, 0.625) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.90, 0.90, 0.90) ;
		drawRing(64) ;
		glPopMatrix() ;
	}

		/* 3bis : Remplissage aile arrière. */
	glPushMatrix() ;
	glTranslatef(-0.1001, 0, 0.3) ;
	glScalef(0.77*2, 0.77, 0.5) ;
	glRotatef(-90, 1, 0, 0) ;
	glColor3f(0.1, 0.1, 0.1) ;
	drawCylinder(64) ;
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
		drawCylinder(64) ;
		glPopMatrix() ;

			/* 2 - 1 : Laser principal - tube arrière. */
		glPushMatrix() ;
		glTranslatef(0, 0, 0) ;
		glScalef(0.06, 0.06, 0.60) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.10, 0.10, 0.10) ;
		drawCylinder(64) ;
		glPopMatrix() ;

			/* 2 - 2 : Laser principal - tube principal. */
		glPushMatrix() ;
		glTranslatef(0, 0, -0.1) ;
		glScalef(0.05, 0.05, 1) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.10, 0.10, 0.10) ;
		drawCylinder(64) ;
		glPopMatrix() ;

			/* 2 - 3 : Laser principal - tube blanc. */
		glPushMatrix() ;
		glTranslatef(0, 0, -0.9) ;
		glScalef(0.04, 0.04, 0.80) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.90, 0.90, 0.90) ;
		drawCylinder(64) ;
		glPopMatrix() ;

			/* 2 - 4 : Laser principal - pointe rouge. */
		glPushMatrix() ;
		glTranslatef(0, 0, -1.35) ;
		glScalef(0.05, 0.05, 0.1) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.60, 0.20, 0.20) ;
		drawCylinder(64) ;
		glPopMatrix() ;

			/* 2 - 5 : Laser principal - fin de la pointe. */
		glPushMatrix() ;
		glTranslatef(0, 0, -1.4) ;
		glScalef(0.05, 0.05, 0.05) ;
		glColor3f(0.60, 0.20, 0.20) ;
		drawSphere(64) ;
		glPopMatrix() ;

			/* 3 - 1 : Cylindre de raccord entre les deux lasers 1. */
		glPushMatrix() ;
		glTranslatef(0, 0.075, -0.15) ;
		glScalef(0.01, 0.05, 0.01) ;
		glColor3f(0.10, 0.10, 0.10) ;
		drawCylinder(64) ;
		glPopMatrix() ;

			/* 3 - 2 : Cylindre de raccord entre les deux lasers 2. */
		glPushMatrix() ;
		glTranslatef(0, 0.075, 0.15) ;
		glScalef(0.01, 0.05, 0.01) ;
		glColor3f(0.10, 0.10, 0.10) ;
		drawCylinder(64) ;
		glPopMatrix() ;

			/* 4 - 1 : Laser secondaire - tube arrière. */
		glPushMatrix() ;
		glTranslatef(0, 0.1, 0.6) ;
		glScalef(0.03, 0.03, 0.05) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.10, 0.10, 0.10) ;
		drawCylinder(64) ;
		glPopMatrix() ;

			/* 4 - 2 : Laser secondaire - tube principal. */
		glPushMatrix() ;
		glTranslatef(0, 0.1, 0.05) ;
		glScalef(0.02, 0.02, 1.15) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.10, 0.10, 0.10) ;
		drawCylinder(64) ;
		glPopMatrix() ;

			/* 4 - 2bis : Laser secondaire - tube principal. */
		glPushMatrix() ;
		glTranslatef(0, 0.1, -0.575) ;
		glScalef(0.02, 0.02, 0.1) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.10, 0.10, 0.10) ;
		drawCone(64) ;
		glPopMatrix() ;

			/* 4 - 3 : Laser secondaire - tube fin. */
		glPushMatrix() ;
		glTranslatef(0, 0.1, -0.69) ;
		glScalef(0.015, 0.015, 0.35) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.10, 0.10, 0.10) ;
		drawCylinder(64) ;
		glPopMatrix() ;

			/* 4 - 4 : Laser secondaire - lien. */
		glPushMatrix() ;
		glTranslatef(0, 0.1, -0.8375) ;
		glScalef(0.02, 0.02, 0.05) ;
		glRotatef(90, 1, 0, 0) ;
		glColor3f(0.10, 0.10, 0.10) ;
		drawCone(64) ;
		glPopMatrix() ;

			/* 4 - 5 : Laser secondaire - pointe. */
		glPushMatrix() ;
		glTranslatef(0, 0.1, -0.8875) ;
		glScalef(0.02, 0.02, 0.05) ;
		glRotatef(-90, 1, 0, 0) ;
		glColor3f(0.60, 0.20, 0.20) ;
		drawCone(64) ;
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
	drawCube() ;
	glPopMatrix() ;

		/* 1bis : Rectangle blanc bas. */
	glPushMatrix() ;
	glTranslatef(-0.268, -0.56+0.0085/2, 1.45532) ;
	glScalef(0.336, 0.0585, 1.61336) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawCube() ;
	glPopMatrix() ;

		/* 1ter : Remplissage arrière. */
	glPushMatrix() ;
	glTranslatef(-0.55, 0, 1.41) ;
	glScalef(0.90, 1.1, 1.61336) ;
	glColor3f(0.1, 0.1, 0.1) ;
	drawCube() ;
	glPopMatrix() ;

		/* 2 : Structure basse complémentaire. */
	glPushMatrix() ;
	glTranslatef(-0.1001, 0, 1.45532) ;
	glScalef(0.78*2*0.75, 0.78*0.75, 1.61336) ;
	glRotatef(-90, 1, 0, 0) ;
	glRotatef(90, 0, 1, 0) ;
	glScalef(1, 1, -1) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawPartialRing(2, 64) ;
	glPopMatrix() ;

		/* 2bis : Remplissage. */
	glPushMatrix() ;
	glTranslatef(-0.1001, 0, 1.41) ;
	glScalef(0.77*2*0.75, 0.77*0.75, 1.61336) ;
	glRotatef(-90, 1, 0, 0) ;
	glRotatef(90, 0, 1, 0) ;
	glScalef(1, 1, -1) ;
	glColor3f(0.10, 0.10, 0.10) ;
	drawPartialCylinder(2, 64) ;
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
	drawRing(64) ;
	glPopMatrix() ;

		/* 2 : Lien avec le réacteur central. */
	glPushMatrix() ;
	glTranslatef(-0.5, 0, 2.51) ;
	glScalef(0.2, 0.01, 0.01) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawCube() ;
	glPopMatrix() ;

		/* 3 : Anneau réacteur. */
	glPushMatrix() ;
	glTranslatef(0, 0, 2.6) ;
	glScalef(0.4, 0.4, 0.2) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawRing(64) ;
	glPopMatrix() ;

		/* 4 : Réacteur. */
	glPushMatrix() ;
	glTranslatef(0, 0, 1.5) ;
	glScalef(0.4, 0.4, 2) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(1.00, 1.00, 0.70) ;
	drawCylinder(64) ;
	glPopMatrix() ;

		/* 5 : Anneau fin. */
	glPushMatrix() ;
	glTranslatef(0, 0, 3.2) ;
	glScalef(0.4, 0.4, 0.1) ;
	glRotatef(90, 1, 0, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawRing(64) ;
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
	drawCube() ;
	glPopMatrix() ;

		/* 2 : Cylindre partiel avant. */
	glPushMatrix() ;
	glTranslatef(0, 0.6, 0.75) ;
	glScalef(0.05, 0.1, 0.1);
	glRotatef(90, 0, 0, 1) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawPartialCylinder(16, 64) ;
	glPopMatrix() ;

		/* 3 : Rectangle milieu bas. */
	glPushMatrix() ;
	glTranslatef(0, 0.65, 2);
	glScalef(0.05, 0.1, 2.5);
	glColor3f(0.90, 0.90, 0.90);
	drawCube() ;
	glPopMatrix() ;

		/* 4 : Triangle milieu. */
	glPushMatrix() ;
	glTranslatef(0, 1, 1.5);
	glScalef(0.05, 0.6, 0.2);
	glRotatef(90, 0, 1, 0) ;
	glColor3f(0.90, 0.90, 0.90);
	drawTriangle() ;
	glPopMatrix() ;

		/* 5 : Cylindre partiel milieu. */
	glPushMatrix() ;
	glTranslatef(0, 1.271, 1.696) ;
	glScalef(0.05, 0.1, 0.1);
	glRotatef(90, 0, 0, 1) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawPartialCylinder(13, 64) ;
	glPopMatrix() ;

		/* 6 : Rectangle milieu. */
	glPushMatrix() ;
	glTranslatef(0, 1, 2.325);
	glScalef(0.05, 0.6, 1.45);
	glColor3f(0.90, 0.90, 0.90);
	drawCube() ;
	glPopMatrix() ;

		/* 7 : Rectangle milieu haut. */
	glPushMatrix() ;
	glTranslatef(0, 1.335, 2.373);
	glScalef(0.05, 0.07, 1.354);
	glColor3f(0.90, 0.90, 0.90);
	drawCube() ;
	glPopMatrix() ;

		/* 8 : Cylindre partiel haut. */
	glPushMatrix() ;
	glTranslatef(0, 1.17, 3.05) ;
	glScalef(0.05, 0.2, 0.2);
	glRotatef(90, 0, 0, 1) ;
	glRotatef(-90, 0, 1, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawPartialCylinder(16, 64) ;
	glPopMatrix() ;

		/* 9 : Rectangle arrière. */
	glPushMatrix() ;
	glTranslatef(0, 0.935, 3.15);
	glScalef(0.05, 0.47, 0.2);
	glColor3f(0.90, 0.90, 0.90);
	drawCube() ;
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
	drawCube() ;
	glPopMatrix() ;

		/* 2 : Cylindre partiel avant. */
	glPushMatrix() ;
	glTranslatef(0, 0.6, 0.75) ;
	glScalef(0.05, 0.1, 0.1);
	glRotatef(90, 0, 0, 1) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawPartialCylinder(16, 64) ;
	glPopMatrix() ;

		/* 3 : Rectangle milieu bas. */
	glPushMatrix() ;
	glTranslatef(0, 0.65, 2);
	glScalef(0.05, 0.1, 2.5);
	glColor3f(0.90, 0.90, 0.90);
	drawCube() ;
	glPopMatrix() ;

		/* 4 : Triangle milieu. */
	glPushMatrix() ;
	glTranslatef(0, 1, 1.5);
	glScalef(0.05, 0.6, 0.2);
	glRotatef(90, 0, 1, 0) ;
	glColor3f(0.90, 0.90, 0.90);
	drawTriangle() ;
	glPopMatrix() ;

		/* 5 : Cylindre partiel milieu. */
	glPushMatrix() ;
	glTranslatef(0, 1.271, 1.696) ;
	glScalef(0.05, 0.1, 0.1);
	glRotatef(90, 0, 0, 1) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawPartialCylinder(13, 64) ;
	glPopMatrix() ;

		/* 6 : Rectangle milieu. */
	glPushMatrix() ;
	glTranslatef(0, 1, 2.325);
	glScalef(0.05, 0.6, 1.45);
	glColor3f(0.90, 0.90, 0.90);
	drawCube() ;
	glPopMatrix() ;

		/* 7 : Rectangle milieu haut. */
	glPushMatrix() ;
	glTranslatef(0, 1.335, 2.373);
	glScalef(0.05, 0.07, 1.354);
	glColor3f(0.90, 0.90, 0.90);
	drawCube() ;
	glPopMatrix() ;

		/* 8 : Cylindre partiel haut. */
	glPushMatrix() ;
	glTranslatef(0, 1.17, 3.05) ;
	glScalef(0.05, 0.2, 0.2);
	glRotatef(90, 0, 0, 1) ;
	glRotatef(-90, 0, 1, 0) ;
	glColor3f(0.90, 0.90, 0.90) ;
	drawPartialCylinder(16, 64) ;
	glPopMatrix() ;

		/* 9 : Rectangle arrière. */
	glPushMatrix() ;
	glTranslatef(0, 0.935, 3.15);
	glScalef(0.05, 0.47, 0.2);
	glColor3f(0.90, 0.90, 0.90);
	drawCube() ;
	glPopMatrix() ;

	glPopMatrix () ;

		/* Fin. */
	glPopMatrix() ;
}

static void drawAWing(int Armes)
{
		/* Initialisation. */
	glPushMatrix() ;
	glDisable(GL_TEXTURE_2D) ;

	drawAWingRight(Armes) ;
	drawAWingMiddle() ;
	glScalef(-1, 1, 1) ;
	drawAWingRight(Armes) ;

		/* Fin. */
	glPopMatrix() ;
}

static void drawGrid()
{
    int i = 0 ;
    glLineWidth(1) ;

    glPushMatrix() ;
    glTranslatef( -10, 0, -10) ;
	glColor3f(1, 0, 0) ;
    glBegin(GL_LINES) ;

    for (i = 0 ; i <= 20 ; i+=2)
    {
        glVertex3f(i, 0, 0) ;
        glVertex3f(i, 0, 20) ;
    }

    for (i = 0 ; i <= 20 ; i+=2)
    {
        glVertex3f(0, 0, i) ;
        glVertex3f(20, 0, i) ;
    }

    glEnd() ;
    glPopMatrix() ;

    glPushMatrix() ;
	glRotatef(90, 1, 0, 0) ;
    glTranslatef(-10, 0, -10) ;
	glColor3f(0, 1, 0) ;
    glBegin(GL_LINES) ;

    for (i = 0 ; i <= 20 ; i+=2)
    {
        glVertex3f(i, 0, 0) ;
        glVertex3f(i, 0, 20) ;
    }

    for (i = 0 ; i <= 20 ; i+=2)
    {
        glVertex3f(0, 0, i) ;
        glVertex3f(20, 0, i) ;
    }

    glEnd() ;
    glPopMatrix() ;

    glPushMatrix() ;
	glRotatef(90, 0, 0, 1) ;
    glTranslatef(-10, 0, -10) ;
	glColor3f(0, 0, 1) ;
    glBegin(GL_LINES) ;

    for (i = 0 ; i <= 20 ; i+=2)
    {
        glVertex3f(i, 0, 0) ;
        glVertex3f(i, 0, 20) ;
    }

    for (i = 0 ; i <= 20 ; i+=2)
    {
        glVertex3f(0, 0, i) ;
        glVertex3f(20, 0, i) ;
    }

    glEnd() ;
    glPopMatrix() ;
}

/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions d'affichage OpenGL                        */
/*                                                                       */
/* ********************************************************************* */

static void drawSelectedStar(Star * pStar)
{
	int iRotations = 4 ;
	float fRotationAngle = 360/iRotations ;
	float fTime = ((float) (clock())) / CLOCKS_PER_SEC ;

	while (fTime >= iRotations) fTime = fTime - iRotations ;

	glPushMatrix() ;

	glTranslatef(starGetX(pStar), starGetY(pStar), starGetZ(pStar)) ;
	glRotatef(15, 0, 0, 1) ;
	glRotatef(fTime*fRotationAngle, 0, 1, 0) ;
	glTranslatef(pStar->fSize * 1.2, 0, 0) ;
	glRotatef(-90, 0, 0, 1) ;
	glScalef(pStar->fSize*0.05, pStar->fSize*0.05, pStar->fSize*0.05) ;
	drawAWing(1) ;

	glPopMatrix() ;
}

static int drawStar(Star * psStar, int iPrecision)
{
	assert(psStar != NULL) ;

	glPushMatrix() ;
	glTranslatef(starGetX(psStar), starGetY(psStar), starGetZ(psStar)) ;
	glScalef(starGetSize(psStar), starGetSize(psStar), starGetSize(psStar)) ;
	glColor3f(starGetColourR(psStar),
			  starGetColourG(psStar),
			  starGetColourB(psStar)) ;
	drawSphere(iPrecision) ;
	glPopMatrix() ;

	return 0 ;
}

static void drawCubeMap(OpenGLData * pData)
{
	int i = 0 ;

	float ppfVertexCube[8][9] =
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

	glEnable(GL_TEXTURE_2D) ;
	glPushMatrix() ;
	/*
	glTranslatef(fPosX, fPosY, fPosZ) ;
	*/
	glScalef(5000, 5000, 5000) ;
	glBindTexture(GL_TEXTURE_2D, pData->uiTexture) ;
	glColor3f(1, 1, 1) ;

	for(i = 0 ; i < 6 ; i++)
	{
		glBegin(GL_QUADS) ;
		glTexCoord2f(0, 1) ;
		glVertex3f(ppfVertexCube[piFacesCube[i][0]][0],
				   ppfVertexCube[piFacesCube[i][0]][1],
				   ppfVertexCube[piFacesCube[i][0]][2]) ;
		glTexCoord2f(1, 1) ;
		glVertex3f(ppfVertexCube[piFacesCube[i][1]][0],
				   ppfVertexCube[piFacesCube[i][1]][1],
				   ppfVertexCube[piFacesCube[i][1]][2]) ;
		glTexCoord2f(1, 0) ;
		glVertex3f(ppfVertexCube[piFacesCube[i][2]][0],
				   ppfVertexCube[piFacesCube[i][2]][1],
				   ppfVertexCube[piFacesCube[i][2]][2]) ;
		glTexCoord2f(0, 0) ;
		glVertex3f(ppfVertexCube[piFacesCube[i][3]][0],
				   ppfVertexCube[piFacesCube[i][3]][1],
				   ppfVertexCube[piFacesCube[i][3]][2]) ;
		glEnd() ;
	}

	glPopMatrix() ;
	glDisable(GL_TEXTURE_2D) ;
}

static gboolean drawStellarium(int * piKey,
							   AnalyzedTrack * pTrack,
							   OpenGLData * pData)
{
	int iAnalyzed = 0 ;
	Star sStar ;
	/* Vérifie que l'analyse a bien été faite. */
	iAnalyzed = (int) analyzedTrackGetAnalyzed(pTrack) ;

	if (iAnalyzed == 1)
	{
		starCreate(& sStar, pTrack, pData->psExistingStars) ;

		glPushMatrix() ;
		if (pData->bPicking == 1)
		{
			glLoadName(analyzedTrackGetTID(pTrack)) ;
		}
		drawStar(& sStar, pData->iPrecision) ;
		glPopMatrix() ;
	}

	return FALSE ;
}

static void drawScene(AnalyzedTracks * pTracks, OpenGLData * pData)
{
	Star sSelectedStar ;
	pData->psExistingStars = g_ptr_array_new_with_free_func(free) ;

	if (pData->bPicking == 1)
	{
		(void) glRenderMode (GL_SELECT) ;
		glInitNames() ;
	}

	if (pData->pPlayedTrack != NULL)
	{
		starCreate(& sSelectedStar,
				   pData->pPlayedTrack,
				   pData->psExistingStars) ;
		drawSelectedStar(& sSelectedStar) ;
	}

	glDisable(GL_LIGHTING) ;

	drawGrid() ;
	drawCubeMap(pData) ;

	glPushMatrix() ;
	glScalef(2, 2, 2) ;
	glColor3f(1, 1, 1) ;
	drawSphere(pData->iPrecision) ;
	glPopMatrix() ;

	glEnable(GL_LIGHTING) ;

	g_tree_foreach(pTracks, (GTraverseFunc) drawStellarium, pData) ;

	g_ptr_array_free(pData->psExistingStars, TRUE) ;
	pData->psExistingStars = NULL ;
}


/* ********************************************************************* */
/*                                                                       */
/*                    Fonctions générales OpenGL                         */
/*                                                                       */
/* ********************************************************************* */

static unsigned int drawingGLLoadTexture(const char * pcFileName)
{
	/* Fonction fournie par nos professeurs d'informatique. */

    Image iImage ;
    unsigned int iTexture ;

	imInitPPM(& iImage, pcFileName) ;

	/* Creation de la texture */
    glGenTextures(1, & iTexture) ;

	/* Texture mipmappé */
    glBindTexture(GL_TEXTURE_2D, iTexture) ;
    glTexParameteri(GL_TEXTURE_2D,
					GL_TEXTURE_MAG_FILTER, GL_LINEAR) ;
    glTexParameteri(GL_TEXTURE_2D,
					GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST) ;

    switch (imGetDimC(& iImage))
    {
		case 1:
			gluBuild2DMipmaps(GL_TEXTURE_2D,
							  1,
							  imGetDimX(& iImage),
							  imGetDimY(& iImage),
							  GL_LUMINANCE,
							  GL_UNSIGNED_BYTE,
							  imGetData(& iImage)) ;
			break;
		case 3:
			gluBuild2DMipmaps(GL_TEXTURE_2D,
							  3,
							  imGetDimX(& iImage),
							  imGetDimY(& iImage),
							  GL_RGB,
							  GL_UNSIGNED_BYTE,
							  imGetData(& iImage)) ;
			break;
		case 4:
			gluBuild2DMipmaps(GL_TEXTURE_2D,
							  4,
							  imGetDimX(& iImage),
							  imGetDimY(& iImage),
							  GL_RGBA,
							  GL_UNSIGNED_BYTE,
							  imGetData(& iImage)) ;
			break;
		default:
			printf("drawingGLLoadTexture : impossible de charger l'image,\
				   nombre de couleur non géré!\n");
			break;
    }

    imFree(& iImage);

    return iTexture;
}

static int drawingGLUpdateTransfertMatrix (OpenGLData* pData)
{
	int i = 0 ;
	float fTemp = 0 ;
	float pfVectorX[3] = {0, 0, 0} ;
	float pfVectorY[3] = {0, 0, 0} ;
	float pfVectorZ[3] = {0, 0, 0} ;
	float fPosX = pData->fRadius * cos(pData->fAlpha) * cos(pData->fBeta)
					+ pData->fTranslateX ;
	float fPosY = pData->fRadius * sin(pData->fAlpha) + pData->fTranslateY ;
	float fPosZ = pData->fRadius * cos(pData->fAlpha) * -1*sin(pData->fBeta)
					+ pData->fTranslateZ ;

	/* Calcul des vecteurs unitaires du repère de la caméra. */

	/* Calcul du Vecteur Z = Destination - Origine. */
	pfVectorZ[0] = (fPosX) - pData->fCenterX ;
	pfVectorZ[1] = (fPosY) - pData->fCenterY ;
	pfVectorZ[2] = (fPosZ) - pData->fCenterZ ;
	/* Normalisation de Z. */
	fTemp = sqrt(pfVectorZ[0]*pfVectorZ[0] +
				 pfVectorZ[1]*pfVectorZ[1] +
				 pfVectorZ[2]*pfVectorZ[2]) ;
	pfVectorZ[0] = pfVectorZ[0] / fTemp ;
	pfVectorZ[1] = pfVectorZ[1] / fTemp ;
	pfVectorZ[2] = pfVectorZ[2] / fTemp ;

	/* pfVectorX = Y(Monde) (produit vectoriel) pfVectorZ */
	pfVectorX[0] = 1*pfVectorZ[2] - 0*pfVectorZ[1] ;
	pfVectorX[1] = 0*pfVectorZ[0] - 0*pfVectorZ[2] ;
	pfVectorX[2] = 0*pfVectorZ[1] - 1*pfVectorZ[0] ;
	/* Normalisation de X. */
	fTemp = sqrt(pfVectorX[0]*pfVectorX[0] +
				 pfVectorX[1]*pfVectorX[1] +
				 pfVectorX[2]*pfVectorX[2]) ;
	pfVectorX[0] = pfVectorX[0] / fTemp ;
	pfVectorX[1] = pfVectorX[1] / fTemp ;
	pfVectorX[2] = pfVectorX[2] / fTemp ;

	/* pfVectorY = pfVectorZ (produit vectoriel) pfVectorX */
	pfVectorY[0] = pfVectorZ[1]*pfVectorX[2] - pfVectorZ[2]*pfVectorX[1] ;
	pfVectorY[1] = pfVectorZ[2]*pfVectorX[0] - pfVectorZ[0]*pfVectorX[2] ;
	pfVectorY[2] = pfVectorZ[0]*pfVectorX[1] - pfVectorZ[1]*pfVectorX[0] ;
	/* Normalisation de Y. */
	fTemp = sqrt(pfVectorY[0]*pfVectorY[0] +
				 pfVectorY[1]*pfVectorY[1] +
				 pfVectorY[2]*pfVectorY[2]) ;
	pfVectorY[0] = pfVectorY[0] / fTemp ;
	pfVectorY[1] = pfVectorY[1] / fTemp ;
	pfVectorY[2] = pfVectorY[2] / fTemp ;

	/* Remplissage de la matrice de passage. */

	for (i = 0 ; i < 3 ; i++)
	{
		pData->pfTransfertMatrix[3 * i] = pfVectorX[i] ;
		pData->pfTransfertMatrix[3 * i + 1] = pfVectorY[i] ;
		pData->pfTransfertMatrix[3 * i + 2] = pfVectorZ[i] ;
	}

	return EXIT_SUCCESS ;
}

int drawingGLTranslate (OpenGLData* pData,
					  float fTranslateX,
					  float fTranslateY,
					  float fTranslateZ)
{
	int i = 0 ;
	float fTranslation = 0 ;
	float pfVectorX[3] = {0, 0, 0} ;
	float pfVectorY[3] = {0, 0, 0} ;
	float pfVectorZ[3] = {0, 0, 0} ;

	/* Récupération des vecteurs unitaires du repère caméra. */

	for (i = 0 ; i < 3 ; i++)
	{
		pfVectorX[i] = pData->pfTransfertMatrix[3 * i] ;
		pfVectorY[i] = pData->pfTransfertMatrix[3 * i + 1] ;
		pfVectorZ[i] = pData->pfTransfertMatrix[3 * i + 2] ;
	}


	fTranslation = -1 * (pData->fRadius * (fTranslateX * pfVectorX[0] +
										   fTranslateY * pfVectorY[0] +
										   fTranslateZ * pfVectorZ[0])) ;
	if ((pData->fCenterX + fTranslation) < 45 &&
		(pData->fCenterX + fTranslation) > -45)
	{
		pData->fCenterX = pData->fCenterX + fTranslation ;
		pData->fTranslateX = pData->fTranslateX + fTranslation ;
	}

	fTranslation = -1 * (pData->fRadius * (fTranslateX * pfVectorX[1] +
										   fTranslateY * pfVectorY[1] +
										   fTranslateZ * pfVectorZ[1])) ;
	if ((pData->fCenterY + fTranslation) < 45 &&
		(pData->fCenterY + fTranslation) > -45)
	{
		pData->fCenterY = pData->fCenterY + fTranslation ;
		pData->fTranslateY = pData->fTranslateY + fTranslation ;
	}

	fTranslation = -1 * (pData->fRadius * (fTranslateX * pfVectorX[2] +
										   fTranslateY * pfVectorY[2] +
										   fTranslateZ * pfVectorZ[2])) ;
	if ((pData->fCenterZ + fTranslation) < 45 &&
		(pData->fCenterZ + fTranslation) > -45)
	{
		pData->fCenterZ = pData->fCenterZ + fTranslation ;
		pData->fTranslateZ = pData->fTranslateZ + fTranslation ;
	}

	drawingGLUpdateTransfertMatrix(pData) ;

	return EXIT_SUCCESS ;
}

int drawingGLRotate (OpenGLData* pData,
				   float fTranslateX,
				   float fTranslateY,
				   float fMovedRadius)
{
	pData->fAlpha = pData->fAlpha - fTranslateY ;
	if (pData->fAlpha >= M_PI/2) pData->fAlpha = M_PI/2 - 0.001 ;
	if (pData->fAlpha <= -M_PI/2) pData->fAlpha = -M_PI/2 + 0.001 ;

	pData->fBeta = pData->fBeta - fTranslateX ;

	pData->fRadius = pData->fRadius - (fMovedRadius * pData->fRadius) ;

	if (pData->fRadius < 0.01) pData->fRadius = 0.01 ;
	if (pData->fRadius > 45) pData->fRadius = 45 ;

	drawingGLUpdateTransfertMatrix(pData) ;

	return EXIT_SUCCESS ;
}

int drawingGLZoom (OpenGLData* pData,
				 float fPositionX,
				 float fPositionY,
				 float fMovedRadius)
{
	int i = 0 ;
	float fTranslation = 0 ;
	float pfVectorX[3] = {0, 0, 0} ;
	float pfVectorY[3] = {0, 0, 0} ;
	float pfVectorZ[3] = {0, 0, 0} ;
	float fTranslateX = 0 ;
	float fTranslateY = 0 ;
	float fTranslateZ = 0 ;

	pData->fRadius = pData->fRadius - (fMovedRadius * pData->fRadius) ;

	if (pData->fRadius < 0.01)
	{
		pData->fRadius = 0.01 ;
	}
	else if (pData->fRadius > 45)
	{
		pData->fRadius = 45 ;
	}
	else
	{
		/* Réglage de la translation de la caméra lors du zoom. */

		fPositionX = -fPositionX + (float) pData->iWidth / 2 ;
		fPositionY = fPositionY - (float) pData->iHeight / 2 ;

		fTranslateX = 0.002 * fPositionX * fMovedRadius ;
		fTranslateY = 0.002 * fPositionY * fMovedRadius ;

		/* Récupération des vecteurs unitaires du repère caméra. */

		for (i = 0 ; i < 3 ; i++)
		{
			pfVectorX[i] = pData->pfTransfertMatrix[3 * i] ;
			pfVectorY[i] = pData->pfTransfertMatrix[3 * i + 1] ;
			pfVectorZ[i] = pData->pfTransfertMatrix[3 * i + 2] ;
		}

		fTranslation = -1 * (pData->fRadius * (fTranslateX * pfVectorX[0] +
											   fTranslateY * pfVectorY[0] +
											   fTranslateZ * pfVectorZ[0])) ;
		if ((pData->fCenterX + fTranslation) < 45 &&
			(pData->fCenterX + fTranslation) > -45)
		{
			pData->fCenterX = pData->fCenterX + fTranslation ;
			pData->fTranslateX = pData->fTranslateX + fTranslation ;
		}

		fTranslation = -1 * (pData->fRadius * (fTranslateX * pfVectorX[1] +
											   fTranslateY * pfVectorY[1] +
											   fTranslateZ * pfVectorZ[1])) ;
		if ((pData->fCenterY + fTranslation) < 45 &&
			(pData->fCenterY + fTranslation) > -45)
		{
			pData->fCenterY = pData->fCenterY + fTranslation ;
			pData->fTranslateY = pData->fTranslateY + fTranslation ;
		}

		fTranslation = -1 * (pData->fRadius * (fTranslateX * pfVectorX[2] +
											   fTranslateY * pfVectorY[2] +
											   fTranslateZ * pfVectorZ[2])) ;
		if ((pData->fCenterZ + fTranslation) < 45 &&
			(pData->fCenterZ + fTranslation) > -45)
		{
			pData->fCenterZ = pData->fCenterZ + fTranslation ;
			pData->fTranslateZ = pData->fTranslateZ + fTranslation ;
		}

		drawingGLUpdateTransfertMatrix(pData) ;
	}

	return EXIT_SUCCESS ;
}

int drawingGLResize (OpenGLData* pData, int iWidth, int iHeight)
{
	glViewport(0, 0, iWidth, iHeight) ;

	glMatrixMode(GL_PROJECTION) ;
	glLoadIdentity() ;
	gluPerspective(45,
				   (GLfloat) iWidth / (GLfloat) iHeight ,
				   0.001, 100000);
	glMatrixMode(GL_MODELVIEW) ;

	pData->iWidth = iWidth ;
	pData->iHeight = iHeight ;

	return EXIT_SUCCESS ;
}

int drawingGLInit (OpenGLData* pData)
{
	/* Début de l'initialisation. */

	int i = 0 ;
	float fAmbiantLight = 0 ;
	float fDiffuseLight = 0 ;
	float fSpecularLight = 0 ;
	float pfAmbiantLight[4] = {0, 0, 0, 0} ;
	float pfDiffuseLight[4] = {0, 0, 0, 0} ;
	float pfSpecularLight[4] = {0, 0, 0, 0} ;
	float pfPositionLight[4] = {0, 0, 0, 0} ;

	pData->fRadius = 20 ;
	pData->fAlpha = 0 ;
	pData->fBeta = 3*M_PI/2 ;
	pData->fCenterX = 0 ;
	pData->fCenterY = 0 ;
	pData->fCenterZ = 0 ;
	pData->fTranslateX = 0 ;
	pData->fTranslateY = 0 ;
	pData->fTranslateZ = 0 ;
	pData->iDirectionX = 0 ;
	pData->iDirectionY = 0 ;
	pData->iDirectionZ = 0 ;
	pData->fMoveX = 0 ;
	pData->fMoveY = 0 ;
	pData->fMoveZ = 0 ;
	pData->bPicking = 0 ;

	pData->pPlayedTrack = NULL ;
	pData->iPrecision = 0 ;
	pData->uiTexture = drawingGLLoadTexture("data/images/cubemap1.ppm") ;
	glDisable(GL_TEXTURE_2D) ;

	drawingGLUpdateTransfertMatrix(pData) ;

	glClearColor(0.0f, 0.0f, 0.1f, 1.0f) ;
	glClearDepth(1.0) ;
	glDepthFunc(GL_LESS) ;
	glEnable(GL_DEPTH_TEST) ;
	glShadeModel(GL_SMOOTH) ;
	glEnable(GL_LIGHTING) ;

	/* Fiat lux. */

	fAmbiantLight = 0.02 ;
	fDiffuseLight = 0.98 ;
	fSpecularLight = 0.5 ;

	for (i = 0 ; i < 3 ; i ++)
	{
		pfAmbiantLight[i] = fAmbiantLight ;
		pfDiffuseLight[i] =  fDiffuseLight ;
		pfSpecularLight[i] = fSpecularLight ;
		pfPositionLight[i] = 0 ;
	}
	pfAmbiantLight[3] = 1 ;
	pfDiffuseLight[3] = 1 ;
	pfSpecularLight[3] = 1 ;
	pfPositionLight[3] = 1 ;

	glLightfv(GL_LIGHT1, GL_AMBIENT, pfAmbiantLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, pfDiffuseLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, pfSpecularLight);
    glLightfv(GL_LIGHT1, GL_POSITION, pfPositionLight);
    glEnable(GL_LIGHT1);

	/* Lux fit. */

	glEnable(GL_NORMALIZE) ;

	glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable( GL_COLOR_MATERIAL );

	/* Fin de l'initialisation. */

	return EXIT_SUCCESS;
}

static int drawingGLMoveDirection(OpenGLData * pData)
{
	if (pData->fMoveX != 0)
	{
		if (pData->fCenterX < pData->iDirectionX)
		{
			if (pData->fCenterX + pData->fMoveX >= pData->iDirectionX)
			{
				pData->fCenterX = pData->iDirectionX ;
				pData->iDirectionX = 0 ;
				pData->fMoveX = 0 ;
			}
		}
		else if (pData->fCenterX > pData->iDirectionX)
		{
			if (pData->fCenterX + pData->fMoveX <= pData->iDirectionX)
			{
				pData->fCenterX = pData->iDirectionX ;
				pData->iDirectionX = 0 ;
				pData->fMoveX = 0 ;
			}
		}
		pData->fCenterX = pData->fCenterX + pData->fMoveX ;
		pData->fTranslateX = pData->fTranslateX + pData->fMoveX ;
	}

	if (pData->fMoveY != 0)
	{
		if (pData->fCenterY < pData->iDirectionY)
		{
			if (pData->fCenterY + pData->fMoveY >= pData->iDirectionY)
			{
				pData->fCenterY = pData->iDirectionY ;
				pData->iDirectionY = 0 ;
				pData->fMoveY = 0 ;
			}
		}
		else if (pData->fCenterY > pData->iDirectionY)
		{
			if (pData->fCenterY + pData->fMoveY <= pData->iDirectionY)
			{
				pData->fCenterY = pData->iDirectionY ;
				pData->iDirectionY = 0 ;
				pData->fMoveY = 0 ;
			}
		}
		pData->fCenterY = pData->fCenterY + pData->fMoveY ;
		pData->fTranslateY = pData->fTranslateY + pData->fMoveY ;
	}

	if (pData->fMoveZ != 0)
	{
		if (pData->fCenterZ < pData->iDirectionZ)
		{
			if (pData->fCenterZ + pData->fMoveZ >= pData->iDirectionZ)
			{
				pData->fCenterZ = pData->iDirectionZ ;
				pData->iDirectionZ = 0 ;
				pData->fMoveZ = 0 ;
			}
		}
		else if (pData->fCenterZ > pData->iDirectionZ)
		{
			if (pData->fCenterZ + pData->fMoveZ <= pData->iDirectionZ)
			{
				pData->fCenterZ = pData->iDirectionZ ;
				pData->iDirectionZ = 0 ;
				pData->fMoveZ = 0 ;
			}
		}
		pData->fCenterZ = pData->fCenterZ + pData->fMoveZ ;
		pData->fTranslateZ = pData->fTranslateZ + pData->fMoveZ ;
	}

	return EXIT_SUCCESS ;
}

int drawingGLSetNewDirection(OpenGLData * pData, const AnalyzedTrack * pTrack)
{
	float * pfCoord = NULL ;
	float fSpeed = 40/25 ;		/* Initialisation de la vitesse de déplacement désirée. */
	float fDistance = 0 ;
	float fTravelTime = 0 ;		/* en 25ème de seconde, car actualisation toutes les 40 ms. */

	pfCoord = analyzedTrackGetCoord(pTrack);

	if(pfCoord[0] != pData->fCenterX &&
	   pfCoord[1] != pData->fCenterY &&
	   pfCoord[2] != pData->fCenterZ)
	{
		/* Mise à jour des iDirectionX/Y/Z. */

		pData->iDirectionX = pfCoord[0] ;
		pData->iDirectionY = pfCoord[1] ;
		pData->iDirectionZ = pfCoord[2] ;

			/* Mise à jour des fMoveX/Y/Z. */

		fDistance = sqrt((pData->iDirectionX - pData->fCenterX)*(pData->iDirectionX - pData->fCenterX) +
						 (pData->iDirectionY - pData->fCenterY)*(pData->iDirectionY - pData->fCenterY) +
						 (pData->iDirectionZ - pData->fCenterZ)*(pData->iDirectionZ - pData->fCenterZ)) ;

			fTravelTime = fDistance / fSpeed ;

		pData->fMoveX = (pData->iDirectionX - pData->fCenterX)/fTravelTime ;
		pData->fMoveY = (pData->iDirectionY - pData->fCenterY)/fTravelTime ;
		pData->fMoveZ = (pData->iDirectionZ - pData->fCenterZ)/fTravelTime ;
	}
	return EXIT_SUCCESS ;
}


int drawingGLDraw (AnalyzedTracks * pTracks, OpenGLData * pData,
					int iPrecision, int bPicking)
{
	pData->bPicking = bPicking ;

	/* Gestion de la vision. */

	drawingGLMoveDirection(pData) ;

	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(pData->fRadius * cos(pData->fAlpha) * cos(pData->fBeta)
			  + pData->fTranslateX,
			  pData->fRadius * sin(pData->fAlpha)
			  + pData->fTranslateY,
			  pData->fRadius * cos(pData->fAlpha) * -1*sin(pData->fBeta)
			  + pData->fTranslateZ,
			  pData->fCenterX,
			  pData->fCenterY,
			  pData->fCenterZ,
			  0, 1, 0) ;

	/* Fin de la gestion de la vision. */

	/* Début des dessins. */

	pData->iPrecision = 2*iPrecision + 6 ;

	drawScene(pTracks, pData) ;

	/* Fin des dessins. */

	return EXIT_SUCCESS;
}


int drawingGLSelect (int iX, int iY, OpenGLData* pData,
					AnalyzedTracks* pTracks, int iPrecision,
					GdkGLDrawable* psSurface)
{
	GLuint uiBuffer[64] = {0};
 	GLint iNbHits = 0;
 	GLint piViewport[4] = {0};
 	GLuint *ptr = NULL;
 	int i = 0;

 	glSelectBuffer(64, uiBuffer);
 	glGetIntegerv(GL_VIEWPORT, piViewport);

 	glRenderMode(GL_SELECT);

 	glInitNames();
 	glPushName(0);

 	glMatrixMode(GL_PROJECTION);
 	glPushMatrix();
 		glLoadIdentity();

 		gluPickMatrix((GLdouble) iX - (pData->iWidth)/2,
					(GLdouble) iY-(pData->iHeight)/2,
					100.0, 100.0, piViewport);
 		gluPerspective(45,
				   (GLfloat) pData->iWidth/ (GLfloat) pData->iHeight ,
				   0.001, 100000);
 		glMatrixMode(GL_MODELVIEW);

 		drawingGLDraw(pTracks, pData, iPrecision, TRUE /* TRUE */);

 		glMatrixMode(GL_PROJECTION);
 	glPopMatrix();

 	glMatrixMode(GL_MODELVIEW);
 	glFlush();

 	iNbHits = glRenderMode(GL_RENDER);

	ptr = (GLuint*) uiBuffer;
	printf("Hits : %d\n", iNbHits);

 	for (i = 0; i < iNbHits; i++)
 	{
 		printf("Number: %d\n"
 				"Min Z: %d\n"
 				"Max Z: %d\n"
 				"Name on stack: %d\n",
 				(int)ptr[0],
 				(int)ptr[1],
 				(int)ptr[2],
 				(int)ptr[3]
 				);
 	}

 	glMatrixMode(GL_MODELVIEW);

 	return (int) ptr[3]; /* Name of the object */
}


/* ********************************************************************* */
/*                                                                       */
/*              Accesseurs et mutateurs d'OpenGLData                     */
/*                                                                       */
/* ********************************************************************* */

float drawingGLGetRadius(const OpenGLData * pData)
{
	assert(pData != NULL) ;
	return pData->fRadius ;
}

float drawingGLGetAlpha(const OpenGLData * pData)
{
	assert(pData != NULL) ;
	return pData->fAlpha ;
}

float drawingGLGetBeta(const OpenGLData * pData)
{
	assert(pData != NULL) ;
	return pData->fBeta ;
}

float drawingGLGetCenterX(const OpenGLData * pData)
{
	assert(pData != NULL) ;
	return pData->fCenterX ;
}

float drawingGLGetCenterY(const OpenGLData * pData)
{
	assert(pData != NULL) ;
	return pData->fCenterY ;
}

float drawingGLGetCenterZ(const OpenGLData * pData)
{
	assert(pData != NULL) ;
	return pData->fCenterZ ;
}

float drawingGLGetTranslateX(const OpenGLData * pData)
{
	assert(pData != NULL) ;
	return pData->fTranslateX ;
}

float drawingGLGetTranslateY(const OpenGLData * pData)
{
	assert(pData != NULL) ;
	return pData->fTranslateY ;
}

float drawingGLGetTranslateZ(const OpenGLData * pData)
{
	assert(pData != NULL) ;
	return pData->fTranslateZ ;
}

int drawingGLGetWidth(const OpenGLData * pData)
{
	assert(pData != NULL) ;
	return pData->iWidth ;
}

int drawingGLGetHeight(const OpenGLData * pData)
{
	assert(pData != NULL) ;
	return pData->iHeight ;
}

const float * drawingGLGetTransfertMatrix(const OpenGLData * pData)
{
	assert(pData != NULL) ;
	return pData->pfTransfertMatrix ;
}

int drawingGLSetPlayedTrack(OpenGLData * pData, AnalyzedTrack * pTrack)
{
	assert(pData != NULL) ;

	pData->pPlayedTrack = pTrack ;

	return EXIT_SUCCESS ;
}


/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */

int drawingGLRegressionTest()
{
	/* Après discution avec l'enseignant, cette fonction de test de regression
	 ne testera que les fonction "testables".*/

	return EXIT_SUCCESS ;
}
