/**
 * @brief Fichier d'entête du module \em OpenGLDrawing.
 *
 * @author Matthieu VIDAL in STELLODY TEAM
 * @file star.h
 */


#ifndef _OPENGL_DRAWING_H_
#define _OPENGL_DRAWING_H_


/* ********************************************************************* */
/*                                                                       */
/*                          Librairies OpenGL                            */
/*                                                                       */
/* ********************************************************************* */

#include <gtk/gtk.h>
#include "analyzed_track.h"


/* ********************************************************************* */
/*                                                                       */
/*                        Définitions de types                           */
/*                                                                       */
/* ********************************************************************* */

/**
 * @struct sStar
 * @brief Structure contenant les informations d'un visuel (étoile) d'un
 morceau.
 *
 * Cette structure contient les données permettant l'afichage de chaque
 morceau analysé. On accède aux données de l'étoile par des accesseurs ou
 des mutateurs.
 */
struct sStar
{
	float fSize ;		/**< Taille de l'étoile */
	float fColourR ;	/**< Couleur rouge  de l'étoile */
	float fColourG ;	/**< Couleur verte  de l'étoile */
	float fColourB ;	/**< Couleur bleue  de l'étoile */
	float fPositionX ;	/**< Position en x de l'étoile */
	float fPositionY ;	/**< Position en y de l'étoile */
	float fPositionZ ;	/**< Position en z de l'étoile */
} ;
/**
 * @typedef Star
 * @brief Redéfinit la structure sStar.
 */
typedef struct sStar Star ;


/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions relatives à OpenGL                        */
/*                                                                       */
/* ********************************************************************* */

/**
 * @fn int starCreate (Star * pStar, AnalyzedTrack* pTrack)
 * @brief Initialise une étoile avec les données issues d'un morceau analysé.
 *
 * @param[in] pStar Pointeur sur l'étoile à initialiser.
 * @param[in,out] pAnalysedTrack Pointeur sur l'analyse.
 * @return 0 si tout s'est bien passé.
 */
int starCreate(Star * pStar, const AnalyzedTrack * pAnalyzedTrack) ;

/**
 * @fn float starGetSize (const Star * pStar)
 * @brief Accesseur de la taille d'une étoile.
 *
 * @param[in] pStar Pointeur sur l'étoile à accéder.
 * @return fSize de l'étoile
 */
float starGetSize (const Star * pStar) ;

/**
 * @fn float starGetColourR (const Star * pStar)
 * @brief Accesseur de la couleur rouge d'une étoile.
 *
 * @param[in] pStar Pointeur sur l'étoile à accéder.
 * @return fColourR de l'étoile
 */
float starGetColourR (const Star * pStar) ;

/**
 * @fn float starGetColourG (const Star * pStar)
 * @brief Accesseur de la couleur verte d'une étoile.
 *
 * @param[in] pStar Pointeur sur l'étoile à accéder.
 * @return fColourG de l'étoile
 */
float starGetColourG (const Star * pStar) ;

/**
 * @fn float starGetColourB (const Star * pStar)
 * @brief Accesseur de la couleur bleue d'une étoile.
 *
 * @param[in] pStar Pointeur sur l'étoile à accéder.
 * @return fColourB de l'étoile
 */
float starGetColourB (const Star * pStar) ;

/**
 * @fn float starGetX (const Star * pStar)
 * @brief Accesseur de la position x d'une étoile.
 *
 * @param[in] pStar Pointeur sur l'étoile à accéder.
 * @return fPositionX de l'étoile
 */
float starGetX (const Star * pStar) ;

/**
 * @fn float starGetY (const Star * pStar)
 * @brief Accesseur de la position y d'une étoile.
 *
 * @param[in] pStar Pointeur sur l'étoile à accéder.
 * @return fPositionY de l'étoile
 */
float starGetY (const Star * pStar) ;

/**
 * @fn float starGetZ (const Star * pStar)
 * @brief Accesseur de la position z d'une étoile.
 *
 * @param[in] pStar Pointeur sur l'étoile à accéder.
 * @return fPositionZ de l'étoile
 */
float starGetZ (const Star * pStar) ;


/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */


/**
 * @fn StarRegressionTest(void)
 * @brief Teste toutes les fonctions du module star.h
 *
 * @return EXIT_SUCCESS si tout est OK
 */
int StarRegressionTest(void) ;


#endif