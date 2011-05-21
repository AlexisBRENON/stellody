/**
 * @file image.h
 * @brief Fichier d'implémentation du module \em Image.
 *
 * @author Professeurs de LIF7/LIFO41 simplifié/adapté par Matthieu VIDAL in STELLODY TEAM
 */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "image.h"

int imInit (Image * piImage, const int iDimX, const int iDimY, const int iDimC)
{
	assert(piImage != NULL) ;
	
	piImage->iDimX = iDimX ;
	piImage->iDimY = iDimY ;
	piImage->iDimC = iDimC ;
	piImage->pData = (unsigned char *) malloc(iDimX * iDimY * iDimC) ;

	return EXIT_SUCCESS ;
}

int imInitPPM (Image * piImage, const char * pcFileName)
{
	FILE * fFile ;
	char pcText[512] ;
	int iDimX = 0 ;
	int iDimY = 0 ;
	int iType = 0 ;
	int iBin = 0 ;
	int i = 0 ;
	int j = 0 ;
	int k = 0 ;
	int p = 0 ;

	fFile = fopen(pcFileName, "rb") ;
	
	if (fFile == NULL)
	{
		printf("Erreur lors de l'ouverture de %s\n", pcFileName) ;
		exit(1) ;
	}
	
	do
	{
		fgets(pcText, 512, fFile);
	} while(pcText[0] == '#') ;
	
	assert(sscanf(pcText, "P%d\n", & iType) == 1) ;
	
	do
	{
		fgets(pcText, 512, fFile) ;
	} while(pcText[0] == '#') ;
	
	assert(sscanf(pcText, "%d %d\n", & iDimX, & iDimY) == 2) ;
	
	do
	{
		fgets(pcText, 512, fFile) ;
	} while(pcText[0] == '#') ;
	
	switch (iType)
	{
		case 6 :
			iBin = 1 ;
		case 3 :
			imInit(piImage, iDimX, iDimY, 3) ;
			break ;
		case 5 :
			iBin = 1 ;
		case 2 :
			imInit(piImage, iDimX, iDimY, 1) ;
			break ;
		default :
			printf("Format PPM non gÈrÈ.\n") ;
			exit(1) ;
			break ;
	}
		
	for (j = 0 ; j < piImage->iDimY ; j++)
	{
		for (i = 0 ; i < piImage->iDimX ; i++)
		{
			for (k = 0 ; k < piImage->iDimC ; k++)
			{
				if (iBin)
					fread(&(piImage->pData[p]), 1, 1, fFile) ;
				else
					assert(fscanf(fFile, "%c", &(piImage->pData[p])) == 1) ;
				p = p + 1 ;
			}
		}
	}
	fclose(fFile);
	printf("Lecture de l'image %s : ok !\n", pcFileName);

	return EXIT_SUCCESS ;
}

int imFree (Image * piImage)
{
    assert(piImage != NULL) ;
	
    piImage->iDimX = 0 ;
    piImage->iDimY = 0 ;
    piImage->iDimC = 0 ;
    free(piImage->pData) ;
    piImage->pData = NULL ;
	
	return EXIT_SUCCESS ;
}

int imGetDimX (const Image * piImage)
{
	assert(piImage != NULL) ;
	return piImage->iDimX ;
}

int imGetDimY (const Image * piImage)
{
	assert(piImage != NULL) ;
	return piImage->iDimY ;
}

int imGetDimC (const Image * piImage)
{
	assert(piImage != NULL) ;
	return piImage->iDimC ;
}

const unsigned char * imGetData (const Image * piImage)
{
	assert(piImage != NULL) ;
	return piImage->pData ;
}