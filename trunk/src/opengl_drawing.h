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
#include "stellarium.h"

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
 structure par des accesseurs et des mutateurs.
 */
struct sOpenGLData
{
	float fRadius ;
	/**< Rayon de la sphère de vision de la caméra */
	float fAlpha ;
	/**< Angle de vision latitudinal de la caméra */
	float fBeta ;
	/**< Angle de vision longitudinal de la caméra */
	float fCenterX ;
	/**< Position en x de la direction de la caméra */
	float fCenterY ;
	/**< Position en y de la direction de la caméra */
	float fCenterZ ;
	/**< Position en z de la direction de la caméra */
	float fTranslateX ;
	/**< Translation en x de la caméra */
	float fTranslateY ;
	/**< Translation en y de la caméra */
	float fTranslateZ ;
	/**< Translation en z de la caméra */
	int iDirectionX ;
	/**< Direction en x de la caméra lors d'une translation automatique */
	int iDirectionY ;
	/**< Direction en y de la caméra lors d'une translation automatique */
	int iDirectionZ ;
	/**< Direction en z de la caméra lors d'une translation automatique */
	float fMoveX ;
	/**< Mouvement unitaire en x lors d'une translation automatique */
	float fMoveY ;
	/**< Mouvement unitaire en y lors d'une translation automatique */
	float fMoveZ ;
	/**< Mouvement unitaire en z lors d'une translation automatique */
	unsigned int uiTexture ;
	/**< Nom/numéro de la texture du cubemap */
	int iWidth ;
	/**< Largeur de la fenêtre */
	int iHeight ;
	/**< Hauteur de la fenêtre */
	int iPrecision ;
	/**< Precision choisie en préférence pour la qualité des dessins. */
	unsigned char bPicking ;
	/**< Booléen indiquant si on est en mode picking ou non. */
	float pfTransfertMatrix[9] ;
	/**< Matrice de passage de la base du monde à la base caméra. */
	AnalyzedTrack * pPlayedTrack ;
	/**< Analyse du morceau en train d'être lu. */
	Stellarium * psStellarium ;
	/**< Stellarium/Tableau dynamique contenant les étoiles éxistantes */
} ;

/**
 * @typedef OpenGLData
 * @brief Redéfinit la structure sOpenGLData.
 */
typedef struct sOpenGLData OpenGLData;


/* ********************************************************************* */
/*                                                                       */
/*                     Fonctions générales OpenGL                        */
/*                                                                       */
/* ********************************************************************* */

/**
 * @fn int drawingGLTranslate (OpenGLData* pData,
				float fTranslateX, float fTranslateY, float fTranslateZ)
 * @brief Permet de gérer le positionnement latitudinal,
 longitudinal de la caméra ainsi que le zoom.
 *
 * @param[in,out] pData Données utilisées par OpenGL
 * @param[in] fTranslateX Translation effectuée par l'utilisateur en X
 * @param[in] fTranslateY Translation effectuée par l'utilisateur en Y
 * @param[in] fTranslateZ Translation effectuée par l'utilisateur en Z
 * @return EXIT_SUCCESS si tout est OK
 */
int drawingGLTranslate (OpenGLData* pData,
						float fTranslateX, float fTranslateY, float fTranslateZ) ;

/**
 * @fn int drawingGLRotate (OpenGLData* pData,
							float fTranslateX, float fTranslateY)
 * @brief Permet de gérer le positionnement latitudinal et longitudinal
 de la caméra.
 *
 * @param[in,out] pData Données utilisées par OpenGL
 * @param[in] fTranslateX Translation effectuée par l'utilisateur en X
 * @param[in] fTranslateY Translation effectuée par l'utilisateur en Y
 * @return EXIT_SUCCESS si tout est OK
 */
int drawingGLRotate (OpenGLData* pData,
					 float fTranslateX, float fTranslateY) ;

/**
 * @fn int drawingGLZoom (OpenGLData* pData,
					float fPositionX, float fPositionY, float fMovedRadius)
 * @brief Permet de gérer le lzoomde la caméra.
 *
 * @param[in,out] pData Données utilisées par OpenGL
 * @param[in] fPositionX Position de la souris en X
 * @param[in] fPositionY Position de la souris en Y
 * @param[in] fMovedRadius Modification effectuée par l'utilisateur sur
 le rayon (zoom)
 * @return EXIT_SUCCESS si tout est OK
 */
int drawingGLZoom (OpenGLData* pData,
				   float fPositionX, float fPositionY, float fMovedRadius) ;

/**
 * @fn int drawingGLSetNewDirection(OpenGLData * pData, const AnalyzedTrack * pTrack)
 * @brief Indique une nouvelle direction de la caméra.
 *
 * @param[in, out] pData Données OpenGL.
 * @param[in] pTrack Analyse du morceau vers lequel se diriger.
 * @return EXIT_SUCCESS si tout s'est bien passé.
 */
int drawingGLSetNewDirection(OpenGLData * pData, const AnalyzedTrack * pTrack) ;

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
 * @fn int drawingGLStellariumInit (OpenGLData* pData)
 * @brief Initialise le Stellarium.
 *
 * @param[in,out] pData Données utilisées par OpenGL
 * @return EXIT_SUCCESS si tout est OK
 */
int drawingGLStellariumInit(OpenGLData* pData) ;

/**
 * @fn int drawingGLInit (OpenGLData* pData)
 * @brief Initialise les paramètres OpenGL (caméra, lumière...).
 *
 * @param[in,out] pData Données utilisées par OpenGL
 * @return EXIT_SUCCESS si tout est OK
 */
int drawingGLInit (OpenGLData* pData) ;

/**
 * @fn int drawingGLFree (OpenGLData* pData)
 * @brief Libère ce qu'il faut libérer dans OpenGLData.
 *
 * @param[in,out] pData Données utilisées par OpenGL
 * @return EXIT_SUCCESS si tout est OK
 */
int drawingGLFree (OpenGLData* pData) ;

/**
 * @fn int drawingGLSelect (OpenGLData* pData, AnalyzedTracks* pTracks,
							int iPositionX, int iPositionY)
 * @brief Renvoie le TID de l'étoile selectionné par un clic souris..
 *
 * @param[in,out] pData Données OpenGL
 * @param[in,out] pTracks Données des chansons
 * @param[in] iPositionX Position de la souris en X
 * @param[in] iPositionY Position de la souris en Y
 * @return EXIT_SUCCESS si tout est OK
 */
int drawingGLSelect (OpenGLData* pData, AnalyzedTracks* pTracks,
					 int iPositionX, int iPositionY) ;

/**
 * @fn int drawingGLDraw (OpenGLData* pData, AnalyzedTracks* pTracks,
						  int iPrecision)
  * @brief Rafraichit la scene.
  *
  * @param[in, out] pTracks Données des chansons
  * @param[in,out] pData Données OpenGL
  * @param[in] iPrecision Precision des dessins
  * @return EXIT_SUCCESS si tout est OK
  */
int drawingGLDraw (OpenGLData* pData, AnalyzedTracks* pTracks,
				   int iPrecision) ;


/* ********************************************************************* */
/*                                                                       */
/*              Accesseurs et mutateurs d'OpenGLData                     */
/*                                                                       */
/* ********************************************************************* */

/**
 * @fn int drawingGLSetPlayedTrack(OpenGLData * pData, AnalyzedTrack * pTrack)
 * @brief Mutateur du champ PlayedTrack de la structure OpenGLData.
 *
 * @param[in, out] pData Données OpenGL.
 * @param[in, out] pTrack Chanson lue.
 * @return EXIT_SUCCESS si tout est OK
 */
int drawingGLSetPlayedTrack(OpenGLData * pData, AnalyzedTrack * pTrack) ;


/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */

/**
 * @fn drawingGLRegressionTest()
 * @brief Teste toutes les fonctions du module opengl_drawing.h
 *
 * @return EXIT_SUCCESS si tout est OK
 */
int drawingGLRegressionTest(void) ;


#endif
