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
/*              Fonctions relatives à la structure Star                  */
/*                                                                       */
/* ********************************************************************* */

int starCreate(Star * pStar,
			   const AnalyzedTrack * pTrack,
			   GPtrArray * psExistingStars)
{
	int i = 0 ;
	int iTest = 0 ;
	int iArraySize = 0 ;
	float fAverage = 0 ;
	float fMedian = 0 ;
	float fValue1 = 0 ;
	float fValue2 = 0 ;
	float * fTemp ;

	float fStep = 2*M_PI/64 ;
	float fAlpha = -M_PI/2 ;
	float fBeta = -fStep ;
	float fRadius = 1 ;
	int iOldX = 0 ;
	int iOldY = 0 ;
	int iOldZ = 0 ;
	int iTranslateX = 0 ;
	int iTranslateY = 0 ;
	int iTranslateZ = 0 ;

	iArraySize = psExistingStars->len ;

	fAverage = analyzedTrackGetFrequenciesAverage(pTrack) ;
	fMedian = analyzedTrackGetFrequenciesMedian(pTrack) ;

	/* Définition des coordonnées en fonction de l'analyse. */
	
	pStar->iPositionX = (int) fAverage * (fAverage - fMedian) ;
	pStar->iPositionY = (int) fMedian * (fMedian - fAverage) ;
	pStar->iPositionZ = 0 ;

	/* Début de la vérification des coordonnées
	 (vérifie qu'elles ne sont pas encore prises). */

	iOldX = pStar->iPositionX ;
	iOldY = pStar->iPositionY ;
	iOldZ = pStar->iPositionZ ;

	/* Boucle tant que la vérification est à faire. */
	while (iTest == 0)
	{
		iTest = 1 ; /* A priori, après ces tests, la vérification ne sera pas à refaire. */
		if (iArraySize == 0)
		{	
			/* Boucle tant que les coordonnées sont trop proches du bulbe central.*/
			while ((pStar->iPositionX < 3 && pStar->iPositionX > -3) &&
				   (pStar->iPositionY < 3 && pStar->iPositionY > -3) &&
				   (pStar->iPositionZ < 3 && pStar->iPositionZ > -3))
			{
				/* Boucle tant que les coordonnées n'ont pas été modifiées. */
				while ((pStar->iPositionX == iOldX + iTranslateX) &&
					   (pStar->iPositionY == iOldY + iTranslateY) &&
					   (pStar->iPositionZ == iOldZ + iTranslateZ))
				{
					fBeta = fBeta + fStep ;
					if (fBeta >= 2 * M_PI)
					{
						fBeta = 0 ;
						fAlpha = fAlpha + fStep ;
					}
					if (fAlpha > M_PI/2)
					{
						fAlpha = -M_PI/2 ;
						fRadius = fRadius + 1 ;
					}
					
					iTranslateX = (int) (fRadius * cos(fAlpha) * cos(fBeta)) ;
					iTranslateY = (int) (fRadius * sin(fAlpha)) ;
					iTranslateZ = (int) (fRadius * cos(fAlpha) * -1*sin(fBeta)) ;
				}
				
				pStar->iPositionX = iOldX + iTranslateX ;
				pStar->iPositionY = iOldY + iTranslateY ;
				pStar->iPositionZ = iOldZ + iTranslateZ ;
			}
		}
		else
		{
			/* Vérifie les coordonnées de toutes les étoiles déjà placées
			 mais s'arrête si les coordonnées en cours de test ont été modifiées
			 (puisque que de toutes façons, il faudra refaire le test depuis le début).*/
			for (i = 0 ; i < iArraySize && iTest == 1 ; i ++)
			{
				fTemp = g_ptr_array_index(psExistingStars, i) ;
				
				/*Boucle tant que les coordonnées sont prises
				 ou tant qu'elles sont trop proche du bulbe central. */
				while ((pStar->iPositionX == fTemp[0] &&
						pStar->iPositionY == fTemp[1] &&
						pStar->iPositionZ == fTemp[2])
					   ||
					   (pStar->iPositionX < 3 && pStar->iPositionX > -3 &&
						pStar->iPositionY < 3 && pStar->iPositionY > -3 &&
						pStar->iPositionZ < 3 && pStar->iPositionZ > -3))
				{
					iTest = 0 ; /* Puisqu'on modifie les coordonnées,
								 le test est à refaire depuis le début. */
					
					/* Boucle tant que les coordonnées n'ont pas été modifiées. */
					while ((pStar->iPositionX == iOldX + iTranslateX) &&
						   (pStar->iPositionY == iOldY + iTranslateY) &&
						   (pStar->iPositionZ == iOldZ + iTranslateZ))
					{
						fBeta = fBeta + fStep ;
						if (fBeta >= 2 * M_PI)
						{
							fBeta = 0 ;
							fAlpha = fAlpha + fStep ;
						}
						if (fAlpha > M_PI/2)
						{
							fAlpha = -M_PI/2 ;
							fRadius = fRadius + 1 ;
						}
						
						iTranslateX = (int) (fRadius * cos(fAlpha) * cos(fBeta)) ;
						iTranslateY = (int) (fRadius * sin(fAlpha)) ;
						iTranslateZ = (int) (fRadius * cos(fAlpha) * -1*sin(fBeta)) ;
					}
					
					pStar->iPositionX = iOldX + iTranslateX ;
					pStar->iPositionY = iOldY + iTranslateY ;
					pStar->iPositionZ = iOldZ + iTranslateZ ;
				}
			}			
		}
	}

	/* Ajoute les nouvelles coordonnées aux données existantes. */

	fTemp = (float *) malloc(3*sizeof(float)) ;
	fTemp[0] = pStar->iPositionX ;
	fTemp[1] = pStar->iPositionY ;
	fTemp[2] = pStar->iPositionZ ;

	g_ptr_array_add(psExistingStars, fTemp) ;


	/* Définition de la taille. */

	fValue1 = analyzedTrackGetIemeFrequenciesValues(pTrack, 0) ;
	fValue2 = analyzedTrackGetIemeFrequenciesValues(pTrack, 7) ;

	pStar->fSize = 0.2 * (1 - fValue1)*(1 - fValue1)*(1 - fValue2)*(1 - fValue2) ;

	/* Définition des couleurs. */

	fValue1 = analyzedTrackGetIemeFrequenciesValues(pTrack, 1) ;
	fValue2 = analyzedTrackGetIemeFrequenciesValues(pTrack, 6) ;
	pStar->fColourR = (1 - fValue1)*(1 - fValue1)*(1 - fValue2)*(1 - fValue2)/2 ;

	fValue1 = analyzedTrackGetIemeFrequenciesValues(pTrack, 2) ;
	fValue2 = analyzedTrackGetIemeFrequenciesValues(pTrack, 5) ;
	pStar->fColourG = (1 - fValue1)*(1 - fValue1)*(1 - fValue2)*(1 - fValue2)/2 ;

	fValue1 = analyzedTrackGetIemeFrequenciesValues(pTrack, 3) ;
	fValue2 = analyzedTrackGetIemeFrequenciesValues(pTrack, 4) ;
	pStar->fColourB = (1 - fValue1)*(1 - fValue1)*(1 - fValue2)*(1 - fValue2)/4 ;

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

	return pStar->iPositionX ;
}

float starGetY (const Star * pStar)
{
	assert(pStar != NULL) ;

	return pStar->iPositionY ;
}

float starGetZ (const Star * pStar)
{
	assert(pStar != NULL) ;

	return pStar->iPositionZ ;
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
