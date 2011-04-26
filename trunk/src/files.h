/**
  * @file files.h
  * @brief Fichier d'entête du module \em Files.
  *
  * @author Alexis BRENON in STELLODY TEAM
  */

#ifndef _FILES_H_
#define _FILES_H_

#include "preferences.h"
#include "analyzed_tracks.h"


/* ********************************************************************* */
/*                                                                       */
/*                        Définitions de types                           */
/*                                                                       */
/* ********************************************************************* */


/**
  * @enum eSTELLODY_FILES
  * @brief Un énumération pour retrouver les différents fichiers ouverts.
  */
enum eSTELLODY_FILES
{
	ERROR	= 2, /**< Fichier d'erreurs (inutilisé) */
	CONFIG	= 0, /**< Fichier de configuration (paramètres...) */
	DATA    = 1  /**< Fichier de données (tous les morceaux) */
};
/**
  * @typedef STELLODY_FILES
  * @brief Redéfini l'énumération.
  */
typedef enum eSTELLODY_FILES STELLODY_FILES;


/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions relatives aux fichiers                    */
/*                                                                       */
/* ********************************************************************* */


/**
  * @fn GKeyFile** filesOpen (void)
  * @brief Ouvre les fichiers nescessaires.
  *
  * @return Un tableau de GKeyFile* que l'utilisateur n'a pas à manipuler !
  */
GKeyFile** filesOpen (void);

/**
  * @fn int filesClose (GKeyFile*** pppsContext)
  * @brief Ferme les fichiers et libère le tableau sans sauvegarder.
  *
  * @param[in,out] pppsContext Pointeur sur le tableau de GKeyFile*
  * @return EXIT_SUCCESS si tout est OK
  */
int filesClose (GKeyFile*** pppsContext);

/**
  * @fn int filesCloseAndSave (GKeyFile*** pppsContext
							const Preferences* psPref,
							const AnalyzedTracks* psTracks)
  * @brief Ferme les fichiers et libère le tableau en sauvegardant.
  *
  * @param[in,out] pppsContext Pointeur sur le tableau de GKeyFile*
  * @param[in] psPref Pointeur sur les préférences à sauvegarder
  * @param[in] psTracks Pointeur sur les morceau à sauvegarder
  * @return EXIT_SUCCESS si tout est OK
  */
int filesCloseAndSave (GKeyFile*** pppsContext,
					const Preferences* psPref,
					const AnalyzedTracks* psTracks);

/**
  * @fn filesSave (GKeyFile** ppsContext
				const Preferences* psPref,
				const AnalyzedTracks* psTracks);)
  * @brief Sauvegarde les données des GKeyFile*
  *
  * @param[in,out] ppsContext Tableau de GKeyFile*
  * @param[in] psPref Pointeur sur les préférences à sauvegarder
  * @param[in] psTracks Pointeur sur les morceau à sauvegarder
  * @return EXIT_SUCCESS si tout est OK
  */
int filesSave (GKeyFile** ppsContext,
			const Preferences* psPref,
			const AnalyzedTracks* psTracks);


/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */


/**
  * @fn int filesRegressionTest (void)
  * @brief Test de régression du module Files.
  *
  * @return EXIT_SUCCESS si tout est OK
  */
int filesRegressionTest (void);

#endif
