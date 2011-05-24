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
#include "analyzed_tracks.h"

#if defined(__linux)
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif
#if defined(__APPLE__)
	#include <openGL/gl.h>
	#include <openGL/glu.h>
#endif


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
 sans l'utilisation de variables globales. On accède aux données de cette
 structure par des accesseurs.
 */
struct sOpenGLData
{
	float fRadius ;					/**< Rayon de la sphère de vision de la caméra */
	float fAlpha ;					/**< Angle de vision latitudinal de la caméra */
	float fBeta ;					/**< Angle de vision longitudinal de la caméra */
	float fCenterX ;				/**< Position en x de la direction de la caméra */
	float fCenterY ;				/**< Position en y de la direction de la caméra */
	float fCenterZ ;				/**< Position en z de la direction de la caméra */
	float fTranslateX ;				/**< Translation en x de la caméra */
	float fTranslateY ;				/**< Translation en y de la caméra */
	float fTranslateZ ;				/**< Translation en z de la caméra */
	unsigned int uiTexture ;		/**< Numéro de la texture du cubemap */
	int iWidth ;					/**< Largeur de la fenêtre */
	int iHeight ;					/**< Hauteur de la fenêtre */
	int iPrecision ;				/**< Precision choisie en préférence pour la qualité des dessins. */
	float pfTransfertMatrix[9] ;	/**< Matrice de passage de la base du monde à la base caméra. */
	AnalyzedTrack * pPlayedTrack ;	/**< Analyse du morceau en train d'être lu. */
	GPtrArray * psExistingStars ;	/**< Tableau dynamique GTK contenant la position des étoiles éxistantes */
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
 * @fn int drawingGLTranslate (OpenGLData* pData, float fTranslateX, float fTranslateY, float fTranslateZ) ;
 * @brief Permet de gérer le positionnement latitudinal, longitudinal de la caméra ainsi que le zoom.
 *
 * @param[in,out] pData Données utilisées par OpenGL
 * @param[in] fTranslateX Translation effectuée par l'utilisateur en X
 * @param[in] fTranslateY Translation effectuée par l'utilisateur en Y
 * @param[in] fTranslateZ Translation effectuée par l'utilisateur en Z
 * @return EXIT_SUCCESS si tout est OK
 */
int drawingGLTranslate (OpenGLData* pData, float fTranslateX, float fTranslateY, float fTranslateZ) ;

/**
 * @fn int drawingGLRotate (OpenGLData* pData, float fTranslateX, float fTranslateY, float fRadius) ;
 * @brief Permet de gérer le positionnement latitudinal et longitudinal de la caméra.
 *
 * @param[in,out] pData Données utilisées par OpenGL
 * @param[in] fTranslateX Translation effectuée par l'utilisateur en X
 * @param[in] fTranslateY Translation effectuée par l'utilisateur en Y
 * @param[in] fMovedRadius Modification effectuée par l'utilisateur sur le rayon (zoom)
 * @return EXIT_SUCCESS si tout est OK
 */
int drawingGLRotate (OpenGLData* pData, float fTranslateX, float fTranslateY, float fMovedRadius) ;

/**
 * @fn int drawingGLZoom (OpenGLData* pData, float fTranslateX, float fTranslateY, float fRadius) ;
 * @brief Permet de gérer le lzoomde la caméra.
 *
 * @param[in,out] pData Données utilisées par OpenGL
 * @param[in] fPositionX Position de la souris en X
 * @param[in] fPositionY Position de la souris en Y
 * @param[in] fMovedRadius Modification effectuée par l'utilisateur sur le rayon (zoom)
 * @return EXIT_SUCCESS si tout est OK
 */
int drawingGLZoom (OpenGLData* pData, float fTranslateX, float fTranslateY, float fMovedRadius) ;

/**
  * @fn int drawingGLInit (OpenGLData* pData)
  * @brief Initialise les paramètres OpenGL (caméra, lumière...).
  *
  * @param[in,out] pData Données utilisées par OpenGL
  * @return EXIT_SUCCESS si tout est OK
  */
int drawingGLInit (OpenGLData* pData) ;

/**
  * @fn int drawingGLResize (OpenGLData* pData, int iWidth, int iHeight)
  * @brief Redimenssionne la scene
  *
  * @param[in,out] pData Données utilisées par OpenGL
  * @param[in] iWidth Nouvelle largeur du widget
  * @param[in] iHeight Nouvelle hauteur du widget
  * @return EXIT_SUCCESS si tout est OK
  */
int drawingGLResize (OpenGLData* pData, int iWidth, int iHeight) ;

/**
  * @fn int drawingGLDraw (AnalyzedTracks * pTracks, OpenGLData * pData, int iPrecision) ;
  * @brief Rafraichit la scene.
  *
  * @param[in, out] pTracks Données des chansons
  * @param[in,out] pData Données OpenGL
  * @param[in] iPrecision Precision des dessins
  * @return EXIT_SUCCESS si tout est OK
  */
int drawingGLDraw (AnalyzedTracks * pTracks, OpenGLData * pData, int iPrecision) ;


/* ********************************************************************* */
/*                                                                       */
/*              Accesseurs et mutateurs d'OpenGLData                     */
/*                                                                       */
/* ********************************************************************* */

/**
 * @fn float drawingGLGetRadius(OpenGLData * pData)
 * @brief Accesseur du rayon de la caméra.
 *
 * @param[in] pData Données OpenGL.
 * @return fRadius des données OpenGL.
 */
float drawingGLGetRadius(const OpenGLData * pData) ;

/**
 * @fn float drawingGLGetAlpha(const OpenGLData * pData)
 * @brief Accesseur de l'angle alpha de la caméra.
 *
 * @param[in] pData Données OpenGL.
 * @return fAlpha des données OpenGL.
 */
float drawingGLGetAlpha(const OpenGLData * pData) ;

/**
 * @fn float drawingGLGetBeta(const OpenGLData * pData)
 * @brief Accesseur de l'angle beta de la caméra.
 *
 * @param[in] pData Données OpenGL.
 * @return fBeta des données OpenGL.
 */
float drawingGLGetBeta(const OpenGLData * pData) ;

/**
 * @fn float drawingGLGetTranslateX(OpenGLData * pData)
 * @brief Accesseur de la position en x de la direction de la caméra.
 *
 * @param[in] pData Données OpenGL.
 * @return fCenterX des données OpenGL.
 */
float drawingGLGetCenterX(const OpenGLData * pData) ;

/**
 * @fn float drawingGLGetTranslateY(OpenGLData * pData)
 * @brief Accesseur de la position en y de la direction de la caméra.
 *
 * @param[in] pData Données OpenGL.
 * @return fCenterY des données OpenGL.
 */
float drawingGLGetCenterY(const OpenGLData * pData) ;

/**
 * @fn float drawingGLGetTranslateZ(OpenGLData * pData)
 * @brief Accesseur de la position en z de la direction de la caméra.
 *
 * @param[in] pData Données OpenGL.
 * @return fCenterZ des données OpenGL.
 */
float drawingGLGetCenterZ(const OpenGLData * pData) ;

/**
 * @fn float drawingGLGetTranslateX(OpenGLData * pData)
 * @brief Accesseur de la translation en X de la caméra.
 *
 * @param[in] pData Données OpenGL.
 * @return fTranslateX des données OpenGL.
 */
float drawingGLGetTranslateX(const OpenGLData * pData) ;

/**
 * @fn float drawingGLGetTranslateY(OpenGLData * pData)
 * @brief Accesseur de la translation en Y de la caméra.
 *
 * @param[in] pData Données OpenGL.
 * @return fTranslateY des données OpenGL.
 */
float drawingGLGetTranslateY(const OpenGLData * pData) ;

/**
 * @fn float drawingGLGetTranslateZ(OpenGLData * pData)
 * @brief Accesseur de la translation en Z de la caméra.
 *
 * @param[in] pData Données OpenGL.
 * @return fTranslateZ des données OpenGL.
 */
float drawingGLGetTranslateZ(const OpenGLData * pData) ;

/**
 * @fn float drawingGLGetWidth(OpenGLData * pData)
 * @brief Accesseur de la largeur de la fenêtre OpenGL.
 *
 * @param[in] pData Données OpenGL.
 * @return iWidth des données OpenGL.
 */
int drawingGLGetWidth(const OpenGLData * pData) ;

/**
 * @fn float drawingGLGetHeight(OpenGLData * pData)
 * @brief Accesseur de la hauteur de la fenêtre OpenGL.
 *
 * @param[in] pData Données OpenGL.
 * @return iHeight des données OpenGL.
 */
int drawingGLGetHeight(const OpenGLData * pData) ;

/**
 * @fn const float * drawingGLGetTransfertMatrix(const OpenGLData * pData)
 * @brief Accesseur de la matrice de passage (repère monde au repère caméra).
 *
 * @param[in] pData Données OpenGL.
 * @return fRadius des données OpenGL.
 */
const float * drawingGLGetTransfertMatrix(const OpenGLData * pData) ;

/**
 * @fn float drawingGLGetRadius(OpenGLData * pData) ;
 * @brief Accesseur de l'angle Alpha de la caméra.
 *
 * @param[in, out] pData Données OpenGL.
 * @param[in] pTrack Analyse du morceau lu.
 * @return EXIT_SUCCESS si tout s'est bien passé.
 */
int drawingGLSetPlayedTrack(OpenGLData * pData, AnalyzedTrack * pTrack) ;


/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */

/**
 * @fn drawingGLRegressionTest(int * argc, char * argv[])
 * @brief Teste toutes les fonctions du module opengl_drawing.h
 *
 * @param [in] argc pointeur vers argc
 * @param [in] argv pointeur vers argv[]
 *
 * @return EXIT_SUCCESS si tout est OK
 */
int drawingGLRegressionTest(int * argc, char * argv[]) ;


#endif
