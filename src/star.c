/**
 * @brief Fichier d'implémentation du module \em Star.
 *
 * @author Matthieu VIDAL in STELLODY TEAM
 * @file star.c
 */


/* ********************************************************************* */
/*                                                                       */
/*                             Librairies                                */
/*                                                                       */
/* ********************************************************************* */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <gtk/gtk.h>
#include "star.h"
#include "analyzed_track.h"

#ifndef M_PI
	#define M_PI 3.14159 /**< Défini la constante PI si elle ne l'est pas.*/
#endif


/* ********************************************************************* */
/*                                                                       */
/*                  Accesseur de la structure Star                       */
/*                                                                       */
/* ********************************************************************* */

int starGetTID (const Star * pStar)
{
	assert (pStar != NULL) ;
	
	return pStar->iTID ;
}

int starSetTID (Star * pStar, int iValue)
{
	assert (pStar != NULL && iValue >= 0) ;
	
	pStar->iTID = iValue ;
	
	return EXIT_SUCCESS ;
}

float starGetSize (const Star * pStar)
{
	assert(pStar != NULL) ;

	return pStar->fSize ;
}

float starGetColourR (const Star * pStar)
{
	assert(pStar != NULL) ;

	return pStar->fColourR ;
}

float starGetColourG (const Star * pStar)
{
	assert(pStar != NULL) ;

	return pStar->fColourG ;
}

float starGetColourB (const Star * pStar)
{
	assert(pStar != NULL) ;

	return pStar->fColourB ;
}

float starGetX (const Star * pStar)
{
	assert(pStar != NULL) ;

	return pStar->fPositionX ;
}

float starGetY (const Star * pStar)
{
	assert(pStar != NULL) ;

	return pStar->fPositionY ;
}

float starGetZ (const Star * pStar)
{
	assert(pStar != NULL) ;

	return pStar->fPositionZ ;
}


/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */


int StarRegressionTest(void)
{
	Star sStar ;
	printf("\n\nTest de regression du module STAR :\n\n") ;
	
	printf("Test des accesseurs : ");
	
	sStar.fSize = 0.25 ;
	assert(starGetSize(& sStar) == sStar.fSize) ;
	
	sStar.fColourR = 0.25 ;
	assert(starGetColourR(& sStar) == sStar.fColourR) ;

	sStar.fColourG = 0.50 ;
	assert(starGetColourG(& sStar) == sStar.fColourG) ;

	sStar.fColourB = 0.75 ;
	assert(starGetColourB(& sStar) == sStar.fColourB) ;

	sStar.fPositionX = 1 ;
	assert(starGetX(& sStar) == sStar.fPositionX) ;

	sStar.fPositionY = 2 ;
	assert(starGetY(& sStar) == sStar.fPositionY) ;

	sStar.fPositionZ = 3 ;
	assert(starGetZ(& sStar) == sStar.fPositionZ) ;
	
	printf("ok.\n") ;
	
	printf("\nTest de regression du module STAR terminé avec succès.\n\n");
	
	return (0) ;
}
