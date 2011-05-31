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
	float fAverage = 0 ;
	float * pfCoord = NULL ;
	float * pfRate = NULL;
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
	uiSize = analyzedTrackGetLength(pTrack) ;
	fAverage = analyzedTrackGetAverage(pTrack) ;

	/* Définition des coordonnées en fonction de l'analyse. */

	/* Vérifie que l'analyse a bien été faite. */
	iAnalyzed = (int) analyzedTrackGetAnalyzed(pTrack) ;

	if (iAnalyzed == 1)
	{
		/* Vérifie que la position n'est pas déjà calculée. */
		pfCoord = analyzedTrackGetCoord(pTrack) ;

		if (pfCoord[0] == 0 && pfCoord[1] == 0 && pfCoord[2] == 0)
		{
			/* Si non, calcul et attribution des coordonnées. */

			pfRate = analyzedTrackGetRate(pTrack);

			pStar->iPositionX = pfRate[0]*30 - 15 ;
			pStar->iPositionY = pfRate[1]*30 - 15 ;
			pStar->iPositionZ = pfRate[2]*30 - 15 ;

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

		fTemp = (float *) malloc(3*sizeof(float)) ;
		fTemp[0] = pStar->iPositionX ;
		fTemp[1] = pStar->iPositionY ;
		fTemp[2] = pStar->iPositionZ ;

		g_ptr_array_add(psExistingStars, fTemp) ;


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

		pfRate = analyzedTrackGetRate(pTrack);

		pStar->fColourR = pfRate[0] ;

		pStar->fColourG = pfRate[1] ;

		pStar->fColourB = pfRate[2] ;
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
