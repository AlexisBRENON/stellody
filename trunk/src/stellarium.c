/**
 * @brief Fichier d'implémentation du module \em Star.
 *
 * @author Matthieu VIDAL in STELLODY TEAM
 * @file stellarium.c
 */


/* ********************************************************************* */
/*                                                                       */
/*                      Librairies du stellarium                         */
/*                                                                       */
/* ********************************************************************* */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <gtk/gtk.h>

#include "star.h"
#include "stellarium.h"

#ifndef M_PI
#define M_PI 3.14159 /**< Défini la constante PI si elle ne l'est pas.*/
#endif


/* ********************************************************************* */
/*                                                                       */
/*                 Fonctions relatives au Stellarium                     */
/*                                                                       */
/* ********************************************************************* */


int stellariumInit(Stellarium * pStellarium, unsigned int uiSize)
{
	assert(pStellarium != NULL) ;

	dynamicArrayInit(pStellarium);

	return EXIT_SUCCESS ;
}

int stellariumFree(Stellarium * pStellarium)
{
	assert(pStellarium != NULL) ;

	dynamicArrayRelease(pStellarium);

	return EXIT_SUCCESS ;
}

int stellariumCreateStar(Stellarium * pStellarium, AnalyzedTrack * pTrack)
{
	unsigned int i = 0 ;
	int iTest = 0 ;
	unsigned int uiStellariumSize = 0 ;
	unsigned char bAnalyzed = 0 ;
	unsigned char bAdded = 0 ;
	unsigned int uiSize = 0 ;
	float fTemp = 0 ;
	float fVal1 = 0 ;
	float fVal2 = 0 ;
	float fVal3 = 0 ;
	float * pfRate = NULL;

	Star* psStar ;
	const Star * cpStarTemp ;

	float fStep = 2*M_PI/64 ;
	float fAlpha = -M_PI/2 ;
	float fBeta = -fStep ;
	float fRadius = 0.2 ;
	float fOldX = 0 ;
	float fOldY = 0 ;
	float fOldZ = 0 ;
	float fTranslateX = 0 ;
	float fTranslateY = 0 ;
	float fTranslateZ = 0 ;
	float fDist = 0 ;

	uiStellariumSize = stellariumGetLastPosition(pStellarium) ;
	uiSize = analyzedTrackGetLength(pTrack) ;

	psStar = (Star*) malloc(sizeof(Star));

	/* Définition des coordonnées en fonction de l'analyse. */

	/* Vérifie que l'analyse a bien été faite. */
	bAnalyzed = (unsigned char) analyzedTrackGetAnalyzed(pTrack) ;
	bAdded = analyzedTrackGetbAdded(pTrack) ;

	if (bAnalyzed == 1 && bAdded == 0)
	{
		pfRate = analyzedTrackGetRate(pTrack) ;

		fTemp = pfRate[0] + pfRate[1] + pfRate[2] ;

		pfRate[0] = pfRate[0]/fTemp ;
		pfRate[1] = pfRate[1]/fTemp ;
		pfRate[2] = pfRate[2]/fTemp ;

		fVal1 = (((pfRate[0] - 0.40) / 0.60)) ;
		fVal2 = ((pfRate[1] * 2)) ;
		fVal3 = ((pfRate[2] * 4)) ;

		/* Définition du iTID. */

		psStar->iTID = analyzedTrackGetTID(pTrack) ;


		/* Définition des couleurs. */

		psStar->fColourR = fVal3 ;
		psStar->fColourG = fVal2 ;
		psStar->fColourB = fVal1 ;


		/* Définition de la taille. */

		if (uiSize > 420000)
		{
			psStar->fSize = 0.4 ;
		}
		else if (uiSize < 105000)
		{
			psStar->fSize = 0.1 ;
		}
		else
		{
			psStar->fSize = (float) uiSize/1050000.0f ;
		}


		/* Définition de la position. */

		/*
		 fTemp = 1.5 * (1 - (psStar->fSize/0.45) * (psStar->fSize/0.45) *
		 (psStar->fSize/0.45) * (psStar->fSize/0.45)) ;
		 */
		psStar->fPositionX = ((fVal1 * 30) - 15) + 3 ;
		psStar->fPositionY = ((fVal2 * 30) - 15) + 1 ;
		psStar->fPositionZ = ((fVal3 * 30) - 15) - 0.5 ;

		/* Début de la vérification des coordonnées
		 (vérifie qu'elles ne sont pas encore prises). */

		fOldX = psStar->fPositionX ;
		fOldY = psStar->fPositionY ;
		fOldZ = psStar->fPositionZ ;

		/* Boucle tant que la vérification est à faire. */
		while (iTest == 0)
		{
			iTest = 1 ; /* A priori, après ces tests, la vérification ne sera pas à refaire. */

			/* Si c'est la première étoile à être placée. */
			if (uiStellariumSize == 0)
			{
				fTemp = sqrt(pow(psStar->fPositionX, 2) +
							 pow(psStar->fPositionY, 2) +
							 pow(psStar->fPositionZ, 2)) ;

				/* Boucle tant que les coordonnées sont trop proches du bulbe central.*/
				while (fTemp < 4)
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
						fRadius = fRadius + 0.2 ;
					}
					fTranslateX = fRadius * cos(fAlpha) * cos(fBeta) ;
					fTranslateY = fRadius * sin(fAlpha) ;
					fTranslateZ = fRadius * cos(fAlpha) * -1*sin(fBeta) ;

					psStar->fPositionX = fOldX + fTranslateX ;
					psStar->fPositionY = fOldY + fTranslateY ;
					psStar->fPositionZ = fOldZ + fTranslateZ ;

					fTemp = sqrt(pow(psStar->fPositionX, 2) +
								 pow(psStar->fPositionY, 2) +
								 pow(psStar->fPositionZ, 2)) ;
				}
			}
			else
			{
				/* Vérifie les coordonnées de toutes les étoiles déjà placées
				 mais s'arrête si les coordonnées en cours de test ont été modifiées
				 (puisque que de toutes façons, il faudra refaire le test depuis le début).*/
				for (i = 0 ; i < uiStellariumSize && iTest == 1 ; i ++)
				{
					cpStarTemp = stellariumGetStar(pStellarium, i) ;

					fTemp = sqrt(pow(psStar->fPositionX, 2) +
								 pow(psStar->fPositionY, 2) +
								 pow(psStar->fPositionZ, 2)) ;

					fDist = sqrt(pow(psStar->fPositionX - cpStarTemp->fPositionX, 2) +
								 pow(psStar->fPositionY - cpStarTemp->fPositionY, 2) +
								 pow(psStar->fPositionZ - cpStarTemp->fPositionZ, 2)) ;

					/*Boucle tant que les coordonnées sont prises
					 ou tant qu'elles sont trop proche du bulbe central. */
					while ((cpStarTemp->fSize + psStar->fSize + 0.2 > fDist) || (fTemp < 4) )
					{
						iTest = 0 ; /* Puisqu'on modifie les coordonnées,
									 le test est à refaire depuis le début. */

						fBeta = fBeta + fStep ;
						if (fBeta >= 2 * M_PI)
						{
							fBeta = 0 ;
							fAlpha = fAlpha + fStep ;
						}
						if (fAlpha > M_PI/2)
						{
							fAlpha = -M_PI/2 ;
							fRadius = fRadius + 0.2 ;
						}
						fTranslateX = fRadius * cos(fAlpha) * cos(fBeta) ;
						fTranslateY = fRadius * sin(fAlpha) ;
						fTranslateZ = fRadius * cos(fAlpha) * -1*sin(fBeta) ;

						psStar->fPositionX = fOldX + fTranslateX ;
						psStar->fPositionY = fOldY + fTranslateY ;
						psStar->fPositionZ = fOldZ + fTranslateZ ;

						fTemp = sqrt(pow(psStar->fPositionX, 2) +
									 pow(psStar->fPositionY, 2) +
									 pow(psStar->fPositionZ, 2)) ;

						fDist = sqrt(pow(psStar->fPositionX - cpStarTemp->fPositionX, 2) +
									 pow(psStar->fPositionY - cpStarTemp->fPositionY, 2) +
									 pow(psStar->fPositionZ - cpStarTemp->fPositionZ, 2)) ;
					}
				}
			}
		}

		/* Mise à jour des coordonnées dans l'analyse. */

		analyzedTrackSetCoords(pTrack,
							   psStar->fPositionX,
							   psStar->fPositionY,
							   psStar->fPositionZ) ;

		/* Ajoute la nouvelle étoile au stellarium. */

		stellariumAddStar(pStellarium, psStar) ;
		analyzedTrackSetbAdded(pTrack, 1) ;
	}

	return EXIT_SUCCESS ;
}


/* ********************************************************************* */
/*                                                                       */
/*          Accesseurs et mutateurs de la structure Stellarium           */
/*                                                                       */
/* ********************************************************************* */

int stellariumAddStar(Stellarium * pStellarium, Star * pStar)
{
	assert(pStellarium != NULL) ;

	dynamicArrayPush(pStellarium, pStar);

	return EXIT_SUCCESS ;
}

int stellariumRemoveStar (Stellarium* pStellarium, int iTID)
{
	int i = 0;
	int iStop = 0;
	int iNbStars = 0;
	Star* psStar = NULL;
	int iTIDToCompare = 0;

	assert(pStellarium != NULL);

	iNbStars = stellariumGetLastPosition(pStellarium);
	for (i = 0; i < iNbStars && iStop == 0; i++)
	{
		psStar = stellariumGetStar(pStellarium, i);
		iTIDToCompare = starGetTID(psStar);

		if (iTID == iTIDToCompare)
		{
			dynamicArrayRemove((DynamicArray*) pStellarium, i);
			iStop = 1;
		}
	}

	return EXIT_SUCCESS;
}



Star * stellariumGetStar(const Stellarium * pStellarium, unsigned int uiNumber)
{
	Star* psStar = NULL;

	assert(pStellarium != NULL) ;

	dynamicArrayGet(pStellarium, uiNumber, (void**) &psStar);

	return psStar;
}

unsigned int stellariumGetLastPosition(const Stellarium * pStellarium)
{
	int iLength = 0;

	assert(pStellarium != NULL) ;

	dynamicArrayGetSize(pStellarium, &iLength);

	return iLength ;
}


/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */

int stellariumRegressionTest (void)
{
	Stellarium sStellarium ;
	Star sStar ;

	printf("\n\nTest de regression du module STELLARIUM :\n\n") ;

	printf("Test de stellariumInit : ");
	assert (stellariumInit(& sStellarium, 1)) ;
	printf("ok.\n") ;

	printf("Test de stellariumAddStar : ");
	assert (stellariumAddStar(& sStellarium, & sStar)) ;
	assert (stellariumAddStar(& sStellarium, & sStar)) ;
	printf("ok.\n") ;

	printf("Test de stellariumGetStar : ");
	assert(stellariumGetStar(& sStellarium, 1) != NULL ) ;
	printf("ok.\n") ;

	printf("Test de stellariumFree : ");
	assert (stellariumGetLastPosition(& sStellarium) == 2) ;
	printf("ok.\n") ;


	printf("Test de stellariumFree : ");
	assert (stellariumFree(& sStellarium)) ;
	printf("ok.\n") ;

	printf("\nTest de regression du module STELLARIUM terminé avec succès.\n\n");

	return EXIT_SUCCESS ;
}
