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
#include <gtk/gtk.h>
#include "star.h"
#include "analyzed_track.h"


/* ********************************************************************* */
/*                                                                       */
/*              Fonctions relatives à la structure Star                  */
/*                                                                       */
/* ********************************************************************* */

int starCreate(Star * pStar, const AnalyzedTrack * pTrack, GPtrArray * psExistingStars)
{
	int i = 0 ;
	int iTest = 0 ;
	int iCounter = 0 ;
	int iArraySize = 0 ;
	float fAverage = 0 ;
	float fMedian = 0 ;
	float fValue1 = 0 ;
	float fValue2 = 0 ;
	float * fTemp ;

	iArraySize = psExistingStars->len ;

	fAverage = analyzedTrackGetFrequenciesAverage(pTrack) ;
	fMedian = analyzedTrackGetFrequenciesMedian(pTrack) ;
	
	pStar->fPositionX = (int) fAverage ;
	pStar->fPositionY = (int) fMedian ;	
	pStar->fPositionZ = 0 ;
	
	/* Vérifie que les coordonnées ne sont pas encore prises. */
	while (iTest == 0)
	{
		iTest = 1 ;
		for (i = 0 ; i < iArraySize && iTest == 1 ; i ++)
		{
			fTemp = g_ptr_array_index(psExistingStars, i) ;
		
			if (pStar->fPositionX == fTemp[0] && pStar->fPositionY == fTemp[1] && pStar->fPositionZ == fTemp[2])
			{
				iTest = 0 ;
				if (iCounter%3 == 0)
				{
					pStar->fPositionX = pStar->fPositionX + 1 ;

				}
				else if (iCounter%3 == 1)
				{
					pStar->fPositionY = pStar->fPositionY + 1 ;

				}
				else
				{
					/* Incrément du z, quand il sera mobile. */
				}

				iCounter = iCounter + 1 ;
			}
		}
	}
	
	/* Ajoute les nouvelles coordonnées aux données existantes. */
	
	fTemp = (float *) malloc(3*sizeof(float)) ;
	fTemp[0] = pStar->fPositionX ;
	fTemp[1] = pStar->fPositionY ;
	fTemp[2] = pStar->fPositionZ ;
	
	g_ptr_array_add(psExistingStars, fTemp) ;
	

	/* Définition de la taille. */
	
	fValue1 = analyzedTrackGetIemeFrequenciesValues(pTrack, 0) ;
	fValue2 = analyzedTrackGetIemeFrequenciesValues(pTrack, 7) ;
	
	pStar->fSize = 0.5 * (1 - fValue1)*(1 - fValue1)*(1 - fValue2)*(1 - fValue2) ;
	
	/* Définition des couleurs. */
	
	fValue1 = analyzedTrackGetIemeFrequenciesValues(pTrack, 1) ;
	fValue2 = analyzedTrackGetIemeFrequenciesValues(pTrack, 6) ;
	pStar->fColourR = (1 - fValue1)*(1 - fValue1)*(1 - fValue2)*(1 - fValue2) ;
	
	fValue1 = analyzedTrackGetIemeFrequenciesValues(pTrack, 2) ;
	fValue2 = analyzedTrackGetIemeFrequenciesValues(pTrack, 5) ;
	pStar->fColourG = (1 - fValue1)*(1 - fValue1)*(1 - fValue2)*(1 - fValue2) ;
	
	fValue1 = analyzedTrackGetIemeFrequenciesValues(pTrack, 3) ;
	fValue2 = analyzedTrackGetIemeFrequenciesValues(pTrack, 4) ;
	pStar->fColourB = (1 - fValue1)*(1 - fValue1)*(1 - fValue2)*(1 - fValue2) ;
	
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
