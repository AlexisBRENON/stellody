/**
 * @brief Fichier d'entête du module \em Star.
 *
 * @author Matthieu VIDAL in STELLODY TEAM
 * @file stellarium.h
 */

#ifndef _STELLARIUM_H_
#define _STELLARIUM_H_


/* ********************************************************************* */
/*                                                                       */
/*                      Librairies du stellarium                         */
/*                                                                       */
/* ********************************************************************* */

#include "star.h"
#include "analyzed_track.h"
#include "dynamic_array.h"


/* ********************************************************************* */
/*                                                                       */
/*                        Définitions de types                           */
/*                                                                       */
/* ********************************************************************* */

/**
 * @typedef Stellarium
 * @brief Le Stellarium est un tableau comprenant toutes le étoiles.
 */
typedef DynamicArray Stellarium ;


/* ********************************************************************* */
/*                                                                       */
/*                 Fonctions relatives au Stellarium                     */
/*                                                                       */
/* ********************************************************************* */

/**
 * @fn int stellariumInit(Stellarium * pStellarium, unsigned int uiSize)
 * @brief Initialise "un" stellarium
 *
 * @param[in,out] pStellarium Pointeur sur le stellarium à initialiser.
 * @param[in] uiSize Taille initiale du stellarium.
 * @return EXIT_SUCCESS si tout est OK.
 */
int stellariumInit(Stellarium * pStellarium, unsigned int uiSize) ;

/**
 * @fn int stellariumFree(Stellarium * pStellarium)
 * @brief Libère le stellarium
 *
 * @param[in,out] pStellarium Pointeur sur le stellarium à libérer.
 * @return EXIT_SUCCESS si tout est OK.
 */
int stellariumFree(Stellarium * pStellarium) ;

/**
 * @fn int stellariumCreateStar(Stellarium * pStellarium, AnalyzedTrack * pTrack)
 * @brief Ajoute une étoile au stellarium.
 *
 * @param[in,out] pStellarium Pointeur sur le stellarium à libérer.
 * @param[in, out] pTrack Pointeur sur l'analyse.
 * @return EXIT_SUCCESS si tout est OK.
 */
int stellariumCreateStar(Stellarium * pStellarium, AnalyzedTrack * pTrack) ;


/* ********************************************************************* */
/*                                                                       */
/*          Accesseurs et mutateurs de la structure Stellarium           */
/*                                                                       */
/* ********************************************************************* */

/**
 * @fn int stellariumAddStar(Stellarium * pStellarium, Star * pStar)
 * @brief Ajoute une étoile au stellarium.
 *
 * @param[in,out] pStellarium Pointeur sur le stellarium à libérer.
 * @param[in,out] pStar Pointeur sur l'étoile.
 * @return EXIT_SUCCESS si tout est OK.
 */
int stellariumAddStar(Stellarium * pStellarium, Star * pStar) ;

/**
 * @fn const Star * stellariumGetStar(const Stellarium * pStellarium,
									unsigned int uiNumber)
 * @brief Renvoie la i-ème étoile du stellarium
 *
 * @param[in] pStellarium Pointeur sur le stellarium.
 * @param[in] uiNumber Numéro de l'étoile recherchée.
 * @return La i-ème étoile du stellarium.
 */
const Star * stellariumGetStar(const Stellarium * pStellarium, unsigned int uiNumber) ;

/**
 * @fn unsigned int stellariumGetLastPosition(const Stellarium * pStellarium)
 * @brief Renvoie la taille du stellarium
 *
 * @param[in] pStellarium Pointeur sur le stellarium.
 * @return Le nombre d'étoile uiLastPosition du stellarium.
 */
unsigned int stellariumGetLastPosition(const Stellarium * pStellarium) ;


/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */

/**
 * @fn int stellariumRegressionTest (void)
 * @brief Test de (non-)regression du module Stellarium.
 *
 * @return EXIT_SUCCESS si tout est OK
 */
int stellariumRegressionTest (void) ;


#endif
