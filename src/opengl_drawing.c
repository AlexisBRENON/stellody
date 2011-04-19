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
#include "opengl_drawing.h"


#ifndef M_PI
	#define M_PI 3.14159
#endif

/* ********************************************************************* */
/*                                                                       */
/*                    Variables globales (caca)                          */
/*                                                                       */
/* ********************************************************************* */


float fXOld = 0 ;
float fYOld = 0 ;
float fAngleX = 0 ;
float fAngleY = 0 ;
float fPresse = 0 ;


/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions relatives à OpenGL                        */
/*                                                                       */
/* ********************************************************************* */


static void SphereColour (float alpha, float beta,
						  float x, float y, float z,
						  float * pfr, float * pfv, float * pfb)
{
	/* float gamma = 0 ; */

	float fLight = 0 ;
	float fColor = 0 ;

	fLight = 1 - exp((-3) * (y+1)/2) ;
	fColor = exp((-3) * (y+1)/2) ;

	*pfr = fLight ;
	*pfv = fLight ;
	*pfb = fLight ;

	/*

	Parcours des couleurs verticalement.

	if (y >= 0)
	{
		if (y < 0.5)
		{
			r = fLight * (2*y) ;
		}
		else
		{
			r = fLight * fabs(2 - 2*y) ;
		}
	}
	else
	{
		r = 0 ;
	}

	if (y < -0.5 || y > 0.5)
	{
		v = 0 ;
	}
	else
	{
		if (y < 0)
		{
			v = fLight * fabs(1 + 2*y) ;
		}
		else
		{
			v = fLight * fabs(1 - 2*y) ;
		}
	}

	if (y <= 0)
	{
		if (y < -0.5)
		{
			b = fLight * (2 + 2*y) ;
		}
		else
		{
			b = fLight * fabs(2*y) ;
		}
	}
	else
	{
		b = 0 ;
	}

	Parcours horizontal, avec tous les bugs que ça implique...

	if (alpha <= 0)
	{
		gamma = beta ;

		if (beta <= pi)
		{
			beta = beta + pi ;
		}
		else
		{
			beta = beta - pi ;
		}

		if (beta <= 2*pi/3 || beta >= 4*pi/3)
		{
			if (beta  <= 2*pi/3)
			{
				r = fLight * ((2*pi/3 - beta)/(2*pi/3)) ;
			}
			else
			{
				r = fLight * ((beta-4*pi/3)/(2*pi/3)) ;
			}
		}
		else
		{
			r = 0 ;
		}

		if (beta <= 4*pi/3 || beta >= 0)
		{
			if (beta  <= 2*pi/3)
			{
				v = fLight * (beta/(2*pi/3)) ;
			}
			else
			{
				v = fLight * ((2*pi/3 - (beta-2*pi/3))/(2*pi/3)) ;
			}
		}
		else
		{
			v = 0 ;
		}

		if (beta <= 2*pi || beta >= 2*pi/3)
		{
			if (beta  <= 4*pi/3)
			{
				b = fLight * ((beta-2*pi/3)/(2*pi/3)) ;
			}
			else
			{
				b = fLight * ((2*pi/3 - (beta-4*pi/3))/(2*pi/3)) ;
			}
		}
		else
		{
			b = 0 ;
		}

		beta = gamma ;
	}
	else
	{
		if (beta <= 2*pi/3 || beta >= 4*pi/3)
		{
			if (beta  <= 2*pi/3)
			{
				r = fLight * ((2*pi/3 - beta)/(2*pi/3)) ;
			}
			else
			{
				r = fLight * ((beta-4*pi/3)/(2*pi/3)) ;
			}
		}
		else
		{
			r = 0 ;
		}

		if (beta <= 4*pi/3 || beta >= 0)
		{
			if (beta  <= 2*pi/3)
			{
				v = fLight * (beta/(2*pi/3)) ;
			}
			else
			{
				v = fLight * ((2*pi/3 - (beta-2*pi/3))/(2*pi/3)) ;
			}
		}
		else
		{
			v = 0 ;
		}

		if (beta <= 2*pi || beta >= 2*pi/3)
		{
			if (beta  <= 4*pi/3)
			{
				b = fLight * ((beta-2*pi/3)/(2*pi/3)) ;
			}
			else
			{
				b = fLight * ((2*pi/3 - (beta-4*pi/3))/(2*pi/3)) ;
			}
		}
		else
		{
			b = 0 ;
		}
	}
	*/
}


static void drawSphere(void)
{
	float alpha = -90 ;
	float beta = 0 ;
	float x1 = 0 ;
	float y1 = 0 ;
	float z1 = 0 ;
	float x2 = 0 ;
	float y2 = 0 ;
	float z2 = 0 ;
	float r = 0 ;
	float v = 0 ;
	float b = 0 ;
	double step = 2*M_PI / (32) ;


	for (alpha = -M_PI ; alpha <= M_PI ; alpha = alpha + step)
	{

		glBegin(GL_TRIANGLE_STRIP) ;

		for (beta = 0 ; beta < 2*M_PI ; beta = beta + step)
		{
			x1 = cos (alpha) * cos (beta) ;
			y1 = sin (alpha) ;
			z1 = cos (alpha) * sin (beta) ;
			x2 = cos (alpha + step) * cos (beta + step) ;
			y2 = sin (alpha + step) ;
			z2 = cos (alpha + step) * sin (beta + step) ;

			SphereColour(alpha, beta, x1, y1, z1, &r, &v, &b) ;

			glColor3f(r, v, b) ;
			glVertex3f(x1, y1, z1) ;
			glVertex3f(x2, y2, z2) ;

		}

		glEnd() ;

	}
}

static void drawTetraedre(void)
{
	int i = 0 ;
	int j = 0 ;
	Point ppEdgesPyramide[4] =
	{
		{1, 0, 0.684653196881458, 1, 0, 0},
		{-0.5, 0.86602540378439, 0.684653196881458, 0, 1, 0},
		{-0.5, -0.86602540378438, 0.684653196881458, 0, 0, 1},
		{0, 0, -0.684653196881458, 1, 1, 1}
	} ;

	glBegin(GL_TRIANGLE_STRIP) ;
	for(i = 0 ; j < 6 ; j++)
	{
		i = j ;
		if (i >= 4) i = i - 4 ;

		glColor3f(ppEdgesPyramide[i].fRed, ppEdgesPyramide[i].fGreen, ppEdgesPyramide[i].fBlue) ;
		glVertex3f(ppEdgesPyramide[i].fDimX, ppEdgesPyramide[i].fDimY, ppEdgesPyramide[i].fDimZ) ;
	}
	glEnd() ;
}

static void drawCube (void)
{
	int i = 0 ;
	int j = 0 ;

	Point ppEdgesCube[8] =
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
		{2, 3, 7, 6},
		{0, 1, 5, 4},
		{1, 3, 7, 5},
		{0, 2, 6, 4}
	} ;

	glBegin(GL_QUADS) ;
	for(i = 0 ; i < 6 ; i++)
	{
		for(j = 0 ; j < 4 ; j ++)
		{
			glColor3f(ppEdgesCube[piFacesCube[i][j]].fRed, ppEdgesCube[piFacesCube[i][j]].fGreen, ppEdgesCube[piFacesCube[i][j]].fBlue) ;
			glVertex3f(ppEdgesCube[piFacesCube[i][j]].fDimX, ppEdgesCube[piFacesCube[i][j]].fDimY, ppEdgesCube[piFacesCube[i][j]].fDimZ) ;
		}
	}
	glEnd() ;
}

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;

	drawSphere() ;
	/* drawTetraedre() ; */
	/* drawCube() ; */

	glFlush() ;

	glLoadIdentity() ;

	glRotatef(-fAngleX, 0.0, 1.0, 0.0) ;
	glRotatef(-fAngleY, 1.0, 0.0, 0.0) ;

	glutSwapBuffers() ;
}

static void key(unsigned char ucKey, int iX, int iY)
{
	switch(ucKey)
	{
		case 'f':
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) ;
			glutPostRedisplay() ;
			break ;
		case 'l':
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) ;
			glutPostRedisplay() ;
			break ;
		case 'p':
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT) ;
			glutPostRedisplay() ;
			break ;
		case 'e':
			glEnable(GL_DEPTH_TEST) ;
			glutPostRedisplay() ;
			break ;
		case 'd':
			glDisable(GL_DEPTH_TEST) ;
			glutPostRedisplay() ;
			break ;
		case 'q' :
			printf("\n Succès !\n");
			exit(0) ;
			break ;
	}
}

static void mouse(int iButton, int iState, int iX, int iY)
{
	if(iButton == GLUT_LEFT_BUTTON && iState == GLUT_DOWN)
	{
		fPresse = 1 ;
		fXOld = iX ;
		fYOld = iY ;
	}

	if(iButton == GLUT_LEFT_BUTTON && iState == GLUT_UP)
	{
		fPresse = 0 ;
	}
}

static void motion(int iX, int iY)
{
	if(fPresse == 1)
	{
		fAngleX = fAngleX + (iX-fXOld) ;
		fAngleY = fAngleY + (iY-fYOld) ;
		glutPostRedisplay() ;
	}

	fXOld = iX ;
	fYOld = iY ;
}

static void reshape(int iWidth, int iHeight)
{
	if(iWidth < iHeight) glViewport(0, (iHeight-iWidth)/2, iWidth, iWidth) ;
	if(iHeight < iWidth) glViewport((iWidth-iHeight)/2, 0, iHeight, iHeight) ;
}




/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */


int OpenGLDrawingRegressionTest(int * argc, char * argv[])
{
	printf("\n Test de régression OpenGL_drawing...\n");

	glutInit(argc, argv) ;
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH) ;
	glutInitWindowPosition(400, 200) ;
	glutInitWindowSize(800, 800) ;
	glutCreateWindow("Stellody - OpenGL drawing") ;

	glClearColor(1, 1, 1, 0) ;
	glPointSize(4.0) ;
	glEnable(GL_DEPTH_TEST) ;

	glutDisplayFunc(display) ;
	glutKeyboardFunc(key) ;

	glutMouseFunc(mouse) ;
	glutMotionFunc(motion) ;
	glutReshapeFunc(reshape) ;

	glutMainLoop() ;

	return EXIT_SUCCESS;
}
