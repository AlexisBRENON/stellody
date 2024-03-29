/**
 * @brief Fichier d'entête du module \em Star.
 *
 * @author Matthieu VIDAL in STELLODY TEAM
 * @file star.h
 */

#ifndef _STAR_H_
#define _STAR_H_


/* ********************************************************************* */
/*                                                                       */
/*                      Librairies d'une étoile                          */
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
 morceau analysé. On accède aux données de l'étoile par des accesseurs.
 */
struct sStar
{
	int iTID; /**< Identifiant du morceau (dans l'abre, le Stellarium) */
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
/*                 Accesseurs de la structure Star                       */
/*                                                                       */
/* ********************************************************************* */

/**
 * @fn int starGetTID (const Star* pStar)
 * @brief Accesseur sur l'identifiant du morceau.
 *
 * @param[in] pStar Pointeur sur l'étoile à accéder.
 * @return L'identifiant (entier) du morceau
 */
int starGetTID (const Star * pStar) ;

/**
 * @fn int starSetTID (Star *pStar,
						int iValue)
 * @brief Mutateur sur l'identifiant d'un morceau.
 *
 * @param[in,out] pStar Pointeur sur l'étoile à accéder.
 * @param[in] iValue Nouvelle valeur à stocker
 * @return EXIT_SUCCESS si tout est OK
 */
int starSetTID (Star * pStar, int iValue) ;

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
 * @fn int starGetX (const Star * pStar)
 * @brief Accesseur de la position x d'une étoile.
 *
 * @param[in] pStar Pointeur sur l'étoile à accéder.
 * @return fPositionX de l'étoile
 */
float starGetX (const Star * pStar) ;

/**
 * @fn int starGetY (const Star * pStar)
 * @brief Accesseur de la position y d'une étoile.
 *
 * @param[in] pStar Pointeur sur l'étoile à accéder.
 * @return fPositionY de l'étoile
 */
float starGetY (const Star * pStar) ;

/**
 * @fn int starGetZ (const Star * pStar)
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
 * @fn int StarRegressionTest(void)
 * @brief Teste toutes les fonctions du module star.h
 *
 * @return EXIT_SUCCESS si tout est OK
 */
int StarRegressionTest(void) ;


#endif
