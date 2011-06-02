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
/*            Fonction(s) relative(s) à la structure Star                */
/*                                                                       */
/* ********************************************************************* */

int starCreate(Star * pStar,
				AnalyzedTrack * pTrack,
				GPtrArray * psExistingStars)
{
	int i = 0 ;
	int iTest = 0 ;
	int iArraySize = 0 ;
	int iAnalyzed = 0 ;
	unsigned int uiSize = 0 ;
	float fTemp = 0 ;
	float fVal1 = 0 ;
	float fVal2 = 0 ;
	float fVal3 = 0 ;
	float * pfCoord = NULL ;
	float * pfRate = NULL;
	float * pfTemp ;

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
	uiSize = analyzedTrackGetLength(pTrack) ;

	/* Définition des coordonnées en fonction de l'analyse. */

	/* Vérifie que l'analyse a bien été faite. */
	iAnalyzed = (int) analyzedTrackGetAnalyzed(pTrack) ;

	if (iAnalyzed == 1)
	{
		pfRate = analyzedTrackGetRate(pTrack) ;
		
		fTemp = pfRate[0] + pfRate[1] + pfRate[2] ;
		
		pfRate[0] = pfRate[0]/fTemp ;
		pfRate[1] = pfRate[1]/fTemp ;
		pfRate[2] = pfRate[2]/fTemp ;
		
		fVal1 = (((pfRate[0] - 0.40) / 0.60)) ;
		fVal2 = ((pfRate[1] * 2)) ;
		fVal3 = ((pfRate[2] * 4)) ;
	
		/* Vérifie que la position n'est pas déjà calculée. */
		pfCoord = analyzedTrackGetCoord(pTrack) ;

		if (pfCoord[0] == 0 && pfCoord[1] == 0 && pfCoord[2] == 0)
		{
			/* Si non, calcul et attribution des coordonnées. */
			
			pStar->iPositionX = (int) ((fVal1 * 60) - 30) ;
			pStar->iPositionY = (int) ((fVal2 * 60) - 30) ;
			pStar->iPositionZ = (int) ((fVal3 * 60) - 30) ;
			
			/* Début de la vérification des coordonnées
			(vérifie qu'elles ne sont pas encore prises). */

			iOldX = pStar->iPositionX ;
			iOldY = pStar->iPositionY ;
			iOldZ = pStar->iPositionZ ;

			/* Boucle tant que la vérification est à faire. */
			while (iTest == 0)
			{
				iTest = 1 ; /* A priori, après ces tests, la vérification ne sera pas à refaire. */
				
				/* Si c'est la première étoile à être placée. */
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
						pfTemp = g_ptr_array_index(psExistingStars, i) ;

						/*Boucle tant que les coordonnées sont prises
						ou tant qu'elles sont trop proche du bulbe central. */
						while ((pStar->iPositionX == pfTemp[0] &&
								pStar->iPositionY == pfTemp[1] &&
								pStar->iPositionZ == pfTemp[2])
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

			/* Mise à jour des coordonnées dans l'analyse. */

			analyzedTrackSetCoords(pTrack,
								   pStar->iPositionX,
								   pStar->iPositionY,
								   pStar->iPositionZ) ;
		}
		else
		{
			/* Si les coordonnées ont déjà été calculées pour le morceau,
			on met à jour l'étoile correspondante. */

			pStar->iPositionX = pfCoord[0] ;
			pStar->iPositionY = pfCoord[1] ;
			pStar->iPositionZ = pfCoord[2] ;
		}

		/* Ajoute les nouvelles coordonnées aux données existantes. */

		pfTemp = (float *) malloc(3*sizeof(float)) ;
		pfTemp[0] = pStar->iPositionX ;
		pfTemp[1] = pStar->iPositionY ;
		pfTemp[2] = pStar->iPositionZ ;

		g_ptr_array_add(psExistingStars, pfTemp) ;


		/* Définition de la taille. */

		if (uiSize > 420000)
		{
			pStar->fSize = 0.4 ;
		}
		else if (uiSize < 105000)
		{
			pStar->fSize = 0.1 ;
		}
		else
		{
			pStar->fSize = (float) uiSize/1050000.0f ;
		}

		/* Définition des couleurs. */

		pStar->fColourR = fVal1 ;
		pStar->fColourG = fVal2 ;
		pStar->fColourB = fVal3 ;
	}
	
	return (0) ;
}


/* ********************************************************************* */
/*                                                                       */
/*                  Accesseur de la structure Star                       */
/*                                                                       */
/* ********************************************************************* */

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

int starGetX (const Star * pStar)
{
	assert(pStar != NULL) ;

	return pStar->iPositionX ;
}

int starGetY (const Star * pStar)
{
	assert(pStar != NULL) ;

	return pStar->iPositionY ;
}

int starGetZ (const Star * pStar)
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
