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

/**
 * @struct sOpenGLData
 * @brief Structure permettant l'utilisation de données OpenGL.
 *
 * Cette structure est utilisée pour passer des données en paramètres via
 pData. Elle contient les données nécessaires à la bonne utilisation d'OpenGL
 sans l'utilisation de variables globales.
 */
struct sOpenGLData
{
	float fCenterX ; /**< Position en x de la direction de la caméra */
	float fCenterY ; /**< Position en y de la direction de la caméra */
	float fCenterZ ; /**< Position en z de la direction de la caméra */
	float fEyeX ;	 /**< Position en x de la caméra */
	float fEyeY ;	 /**< Position en y de la caméra */
	float fEyeZ ;	 /**< Position en z de la caméra */
} ;
/**
 * @typedef OpenGLData
 * @brief Redéfinit la structure sOpenGLData.
 */
typedef struct sOpenGLData OpenGLData;

/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions relatives à OpenGL                        */
/*                                                                       */
/* ********************************************************************* */

/**
  * @fn int drawingGlInit (OpenGLData* pData)
  * @brief Initialise les paramètres OpenGL (caméra, lumière...).
  *
  * @param[in,out] pData Données utilisées par OpenGL
  * @return EXIT_SUCCESS si tout est OK
  */
int drawingGlInit (OpenGLData* pData);

/**
  * @fn int drawingGlResize (int width, int height)
  * @brief Redimenssionne la scene
  *
  * @param[in] width Nouvelle largeur du widget
  * @param[in] height Nouvelle hauteur du widget
  * @return EXIT_SUCCESS si tout est OK
  */
int drawingGlResize (int width, int height);

/**
  * @fn int drawingGlDraw (gpointer* pData);
  * @brief Rafraichit la scene.
  *
  * @param[in,out] pData Données de l'application
  * @return EXIT_SUCCESS si tout est OK
  */
int drawingGlDraw (gpointer* pData);

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
