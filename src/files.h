/**
  * @file files.h
  * @brief Fichier d'entête des fichiers.
  *
  * Fichier d'entête de gestion des fichiers (DATA, CONFIG).
  *
  * @author Alexis BRENON in STELLODY TEAM
  */

#ifndef _FILES_H_
#define _FILES_H_

/**
  * @enum eSTELLODY_FILES
  * @brief Un énumération pour retrouver les différents fichiers ouverts.
  */
enum eSTELLODY_FILES
{
	ERROR, /**< Fichier d'erreurs (inutilisé) */
	CONFIG, /**< Fichier de configuration (paramètres...) */
	DATA /**< Fichier de données (tous les morceaux) */
};
/**
  * @typedef STELLODY_FILES
  * @brief Redéfini l'énumération.
  */
typedef enum eSTELLODY_FILES STELLODY_FILES;



#endif
