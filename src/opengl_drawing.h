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

#include "analyzed_tracks.h"


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
	float fRadius ;		/**< Rayon de la sphère de vision de la caméra */
	float fAlpha ;		/**< Angle de vision latitudinal de la caméra */
	float fBeta ;		/**< Angle de vision longitudinal de la caméra */	
	float fCenterX ;	/**< Position en x de la direction de la caméra */
	float fCenterY ;	/**< Position en y de la direction de la caméra */
	float fCenterZ ;	/**< Position en z de la direction de la caméra */
	float fTranslateX ;	/**< Translation en x de la caméra */
	float fTranslateY ;	/**< Translation en y de la caméra */
	float fTranslateZ ;	/**< Translation en z de la caméra */
	GPtrArray * psExistingStars ; /**< Tableau dynamique GTK contenant la position des étoiles éxistantes */
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
 * @fn drawingTranslate (OpenGLData* pData, float fTranslateX, float fTranslateY, float fTranslateZ) ;
 * @brief Permet de gérer le positionnement latitudinal, longitudinal de la caméra ainsi que le zoom.
 *
 * @param[in,out] pData Données utilisées par OpenGL
 * @param[in] fTranslateX Translation effectuée par l'utilisateur en X
 * @param[in] fTranslateY Translation effectuée par l'utilisateur en Y
 * @param[in] fTranslateZ Translation effectuée par l'utilisateur en Z
 * @return EXIT_SUCCESS si tout est OK
 */
int drawingTranslate (OpenGLData* pData, float fTranslateX, float fTranslateY, float fTranslateZ) ;

/**
 * @fn drawingRotate (OpenGLData* pData, float fTranslateX, float fTranslateY, float fRadius) ;
 * @brief Permet de gérer le positionnement latitudinal, longitudinal de la caméra ainsi que le zoom.
 *
 * @param[in,out] pData Données utilisées par OpenGL
 * @param[in] fTranslateX Translation effectuée par l'utilisateur en X
 * @param[in] fTranslateY Translation effectuée par l'utilisateur en Y
 * @param[in] fMovedRadius Modification effectuée par l'utilisateur sur le rayon (zoom)
 * @return EXIT_SUCCESS si tout est OK
 */
int drawingRotate (OpenGLData* pData, float fTranslateX, float fTranslateY, float fMovedRadius) ;

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
  * @param[in, out] pTracks Données des chansons
 * @param[in,out] pData Données OpenGL
  * @return EXIT_SUCCESS si tout est OK
  */
int drawingGlDraw (AnalyzedTracks * pTracks, OpenGLData * pData);

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
int drawingRegressionTest(int * argc, char * argv[]) ;


#endif
