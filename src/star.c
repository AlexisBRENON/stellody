/**
 * @brief Fichier d'entête du module \em OpenGLDrawing.
 *
 * @author Matthieu VIDAL in STELLODY TEAM
 * @file star.h
 */

/* ********************************************************************* */
/*                                                                       */
/*                             Librairies                                */
/*                                                                       */
/* ********************************************************************* */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "star.h"
#include "analyzed_track.h"


/* ********************************************************************* */
/*                                                                       */
/*              Fonctions relatives à la structure Star                  */
/*                                                                       */
/* ********************************************************************* */

int starCreate(Star * pStar, const AnalyzedTrack * pTrack)
{
	float fAverage = 0 ;
	float fMedian = 0 ;

	fAverage = analyzedTrackGetFrequenciesAverage(pTrack) ;
	fMedian = analyzedTrackGetFrequenciesMedian(pTrack) ;

	pStar->fSize = 0.5 ;
	pStar->fColourR = 1 ;
	pStar->fColourG = 1 ;
	pStar->fColourB = 1 ;
	pStar->fPositionX = 0 ;
	pStar->fPositionY = 0 ;
	pStar->fPositionZ = 0 ;

	return (0) ;
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
	return (0) ;
}
