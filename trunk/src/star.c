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
/*               Fonction relative à la structure Star                   */
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
	float fOldX = 0 ;
	float fOldY = 0 ;
	float fOldZ = 0 ;
	float fTranslateX = 0 ;
	float fTranslateY = 0 ;
	float fTranslateZ = 0 ;
	float fDist = 0 ;

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
	
		
		/* Définition des couleurs. */
		
		pStar->fColourR = fVal3 ;
		pStar->fColourG = fVal2 ;
		pStar->fColourB = fVal1 ;
		
		
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
		
		
		/* Définition de la position. */
	
		/* Vérifie que la position n'est pas déjà calculée. */
		pfCoord = analyzedTrackGetCoord(pTrack) ;

		if (pfCoord[0] == 0 && pfCoord[1] == 0 && pfCoord[2] == 0)
		{
			/* Si non, calcul et attribution des coordonnées. */
			/*
			fTemp = 1.5 * (1 - (pStar->fSize/0.45) * (pStar->fSize/0.45) *
						   (pStar->fSize/0.45) * (pStar->fSize/0.45)) ;
			*/
			pStar->fPositionX = ((fVal1 * 30) - 15) + 3 ;
			pStar->fPositionY = ((fVal2 * 30) - 15) + 1 ;
			pStar->fPositionZ = ((fVal3 * 30) - 15) - 1 ;
			
			/* Début de la vérification des coordonnées
			(vérifie qu'elles ne sont pas encore prises). */

			fOldX = pStar->fPositionX ;
			fOldY = pStar->fPositionY ;
			fOldZ = pStar->fPositionZ ;

			/* Boucle tant que la vérification est à faire. */
			while (iTest == 0)
			{
				iTest = 1 ; /* A priori, après ces tests, la vérification ne sera pas à refaire. */
				
				/* Si c'est la première étoile à être placée. */
				if (iArraySize == 0)
				{
					fTemp = sqrt(pow(pStar->fPositionX, 2) +
								 pow(pStar->fPositionY, 2) +
								 pow(pStar->fPositionZ, 2)) ;
					
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
							fRadius = fRadius + 1 ;
						}
						fTranslateX = fRadius * cos(fAlpha) * cos(fBeta) ;
						fTranslateY = fRadius * sin(fAlpha) ;
						fTranslateZ = fRadius * cos(fAlpha) * -1*sin(fBeta) ;
						
						pStar->fPositionX = fOldX + fTranslateX ;
						pStar->fPositionY = fOldY + fTranslateY ;
						pStar->fPositionZ = fOldZ + fTranslateZ ;
						
						fTemp = sqrt(pow(pStar->fPositionX, 2) +
									 pow(pStar->fPositionY, 2) +
									 pow(pStar->fPositionZ, 2)) ;
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
						
						fTemp = sqrt(pow(pStar->fPositionX, 2) +
									 pow(pStar->fPositionY, 2) +
									 pow(pStar->fPositionZ, 2)) ;

						fDist = sqrt(pow(pStar->fPositionX - pfTemp[1], 2) +
									 pow(pStar->fPositionY - pfTemp[2], 2) +
									 pow(pStar->fPositionZ - pfTemp[3], 2)) ;
						
						/*Boucle tant que les coordonnées sont prises
						ou tant qu'elles sont trop proche du bulbe central. */
						while ((pfTemp[0] + pStar->fSize + 0.2 > fDist)
							   || (fTemp < 4) )
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
								fRadius = fRadius + 1 ;
							}
							fTranslateX = fRadius * cos(fAlpha) * cos(fBeta) ;
							fTranslateY = fRadius * sin(fAlpha) ;
							fTranslateZ = fRadius * cos(fAlpha) * -1*sin(fBeta) ;
							
							pStar->fPositionX = fOldX + fTranslateX ;
							pStar->fPositionY = fOldY + fTranslateY ;
							pStar->fPositionZ = fOldZ + fTranslateZ ;
							
							fTemp = sqrt(pow(pStar->fPositionX, 2) +
										 pow(pStar->fPositionY, 2) +
										 pow(pStar->fPositionZ, 2)) ;
							
							fDist = sqrt(pow(pStar->fPositionX - pfTemp[1], 2) +
										 pow(pStar->fPositionY - pfTemp[2], 2) +
										 pow(pStar->fPositionZ - pfTemp[3], 2)) ;
						}
					}
				}
			}

			/* Mise à jour des coordonnées dans l'analyse. */

			analyzedTrackSetCoords(pTrack,
								   pStar->fPositionX,
								   pStar->fPositionY,
								   pStar->fPositionZ) ;
		}
		else
		{
			/* Si les coordonnées ont déjà été calculées pour le morceau,
			on met à jour l'étoile correspondante. */

			pStar->fPositionX = pfCoord[0] ;
			pStar->fPositionY = pfCoord[1] ;
			pStar->fPositionZ = pfCoord[2] ;
		}

		/* Ajoute les nouvelles coordonnées aux données existantes. */

		pfTemp = (float *) malloc(4*sizeof(float)) ;
		pfTemp[0] = pStar->fSize ;
		pfTemp[1] = pStar->fPositionX ;
		pfTemp[2] = pStar->fPositionY ;
		pfTemp[3] = pStar->fPositionZ ;

		g_ptr_array_add(psExistingStars, pfTemp) ;
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
