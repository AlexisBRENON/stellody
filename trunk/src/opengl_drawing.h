/**
 * @brief Entête de Tests OpenGL
 *
 * Fichier d'entête des tests faits pour OpenGL.
 * @author Matthieu VIDAL in STELLODY TEAM
 * @file opengl_drawing.h
 */


#ifndef _OPENGL_DRAWING_H_
#define _OPENGL_DRAWING_H_


/* ********************************************************************* */
/*                                                                       */
/*                     Librairies OpenGL MAC OS X                        */
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


/* ********************************************************************* */
/*                                                                       */
/*                        Définitions de types                           */
/*                                                                       */
/* ********************************************************************* */


/**
 * @struct sPoint
 * @brief Structure représentant un point.
 *
 * Cette structure est utilisée pour représenter un point, elle contient ses
   coordonnées en trois dimension et sa couleur en rouge, vert et bleu.
 */
struct sPoint
{
	float fDimX ;  /**< Réel codant la dimension en x */
	float fDimY ;  /**< Réel codant la dimension en y */
	float fDimZ ;  /**< Réel codant la dimension en z */
	float fRed ;   /**< Réel codant la couleur rouge */
	float fGreen ; /**< Réel codant la couleur verte */
	float fBlue ;  /**< Réel codant la couleur bleue */
} ;
/**
 * @typedef Point
 * @brief Redéfinit la structure sPoint.
 */
typedef struct sPoint Point;


/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions relatives à OpenGL                        */
/*                                                                       */
/* ********************************************************************* */





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
