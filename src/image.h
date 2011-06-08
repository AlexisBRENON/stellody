/**
 * @file image.h
 * @brief Fichier d'entête du module \em Image.
 *
 * @author Matthieu VIDAL in STELLODY TEAM
 */

#ifndef _IMAGE_H_
#define _IMAGE_H_


/* ********************************************************************* */
/*                                                                       */
/*                        Définitions de types                           */
/*                                                                       */
/* ********************************************************************* */

/**
 * @struct sImage
 * @brief Structure utilisée pour la gestion d'une image.
 *
 * Cette structure est utilisée pour gérer une image.
 Les champs de cette structures doivent être modifiés ou lus
 par les accesseurs et mutateurs.
 */
struct sImage
{
    unsigned char * pData ; /**< Données de l'image */
    int iDimX ;				/**< Dimension en X de l'image */
    int iDimY ;				/**< Dimension en Y de l'image */
    int iDimC ;				/**< Nombre de couleur de l'image (1 : niveaux de gris, 3 : RGB, 4 : RGBA) */
} ;

/**
 * @typedef Image
 * @brief Redéfinit la structure sImage.
 */
typedef struct sImage Image ;


/* ********************************************************************* */
/*                                                                       */
/*                 Fonctions relatives aux images                        */
/*                                                                       */
/* ********************************************************************* */

/**
 * @fn int imInit (Image * piImage, const int iDimX, const int iDimY, const int iDimC)
 * @brief Initialise une image.
 *
 * @param[in, out] piImage Pointeur vers l'image à initialiser.
 * @param[in] iDimX Largeur de l'image.
 * @param[in] iDimY Hauteur de l'image.
 * @param[in] iDimC Nombre de couleurs de l'image.
 * @return EXIT_SUCCESS si tout est OK
 */
int imInit (Image * piImage, const int iDimX, const int iDimY, const int iDimC) ;

/**
 * @fn int imInitPPM (Image * piImage, const char * pcFileName)
 * @brief Initialise une image à partir d'un ficher au format ppm.
 *
 * @param[in, out] piImage Pointeur vers l'image à initialiser.
 * @param[in] pcFileName Nom de l'image source.
 * @return EXIT_SUCCESS si tout est OK
 */
int imInitPPM (Image * piImage, const char * pcFileName) ;

/**
 * @fn int imFree (Image * piImage)
 * @brief Libère une image de la mémoire.
 *
 * @param[in, out] piImage Pointeur vers l'image à libérer.
 * @return EXIT_SUCCESS si tout est OK
 */
int imFree (Image * piImage) ;


/* ********************************************************************* */
/*                                                                       */
/*                 Accesseurs de la structure Image                      */
/*                                                                       */
/* ********************************************************************* */

/**
 * @fn int imGetDimX (const Image * piImage)
 * @brief Renvoie la largeur d'une image.
 *
 * @param[in] piImage Pointeur vers l'image.
 * @return iDimX de l'image.
 */
int imGetDimX (const Image * piImage) ;

/**
 * @fn int imGetDimY (const Image * piImage)
 * @brief Renvoie la hauteur d'une image.
 *
 * @param[in] piImage Pointeur vers l'image.
 * @return iDimY de l'image.
 */
int imGetDimY (const Image * piImage) ;

/**
 * @fn int imGetDimC (const Image * piImage)
 * @brief Renvoie le nombre de couleur d'une image.
 *
 * @param[in] piImage Pointeur vers l'image.
 * @return iDimC de l'image.
 */
int imGetDimC (const Image * piImage) ;

/**
 * @fn int imGetData (const Image * piImage)
 * @brief Renvoie les données d'une image.
 *
 * @param[in] piImage Pointeur vers l'image.
 * @return Un pointeur vers les données de l'image.
 */
const unsigned char * imGetData (const Image * piImage) ;


/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */

/**
 * @fn int ImRegressionTest(void)
 * @brief Teste toutes les fonctions du module image.h
 *
 * @return EXIT_SUCCESS si tout est OK
 */
int ImRegressionTest(void) ;


#endif