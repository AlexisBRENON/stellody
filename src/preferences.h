/**
  * @file preferences.h
  * @brief Fichier d'entête du module \em Preferences.
  *
  * Module de manipulation des préférences de l'application.
  * @author Alexis BRENON in STELLODY TEAM
  */

#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_

#include <gtk/gtk.h>

/* ********************************************************************* */
/*                                                                       */
/*                        Définitions de types                           */
/*                                                                       */
/* ********************************************************************* */


/**
  * @struct sPreferences
  * @brief Structure manipulant les préférences.
  *
  * Cette structure est remplie au démarage de l'application et contient
  les différents paramètres que l'utilisateur peut modifier.
  */
struct sPreferences
{
	int iWindowXSize;		/**< Largeur de la fenêtre en pixels */
	int iWindowYSize; 		/**< Hauteur de la fenêtre en pixels */
	int iAnalysisRate; 		/**< Taux d'analyse (1 - 100) */
	int i3DQuality;			/**< Qualité de la représentation 3D */
	int iMinTID;			/**< Plus petit identifiant dans l'arbre */
	int iMaxTID;			/**< Plus grand identifiant dans l'arbre */
	int iNbPath; 			/**< Nombre de chemin/taille du tableau pstrFilesPath */
	int iMoveCam;			/**< 1 S'il faut bouger la caméra lorsque l'on clique sur une étoile */
	int iDispGrid;			/**< 1 S'il faut afficher une grille dans le stellarium */
	char** pstrFilesPath;	/**< Chemins des dossiers contenant les
	morceaux */
};
/**
  * @typedef Preferences
  * @brief Alias le mot Preference pour représenter la structure associée.
  */
typedef struct sPreferences Preferences;


/* ********************************************************************* */
/*                                                                       */
/*           Fonctions relatives à la structure Preferences              */
/*                                                                       */
/* ********************************************************************* */


/**
  * @fn int preferencesInit (Preferences* psPref)
  * @brief Initialise les préférences à 0 ou NULL.
  *
  * Charge à l'utilisateur d'appeler preferencesRelease()
  *
  * @param[in,out] psPref Pointeur sur la structure à initialiser
  * @return EXIT_SUCCESS si tout est OK
  */
int preferencesInit (Preferences* psPref);
/**
  * @fn int preferencesInitFromFile (Preferences* psPref,
									GKeyFile* ppsContext[])
  * @brief Charge les préférences sauvegardées.
  *
  * Charge à l'utilisateur d'appeler preferencesRelease()
  *
  * @param[in,out] psPref Pointeur sur l'arbre initialisé
  * @param[in,out] ppsContext Tableau des fichiers ouverts
  * @return EXIT_SUCCESS si tout est OK
  */
int preferencesInitFromFile (Preferences* psPref,
							GKeyFile* ppsContext[]);
/**
  * @fn int preferencesRelease (Preferences* psPref)
  * @brief Libère les préférences.
  *
  * @param[in,out] psPref Pointeur sur la structure à libérer
  * @return EXIT_SUCCESS si tout est OK
  */
int preferencesRelease (Preferences* psPref);

/**
  * @fn Preferences* preferencesCreate (void)
  * @brief Crée une structure de préférences en mémoire.
  *
  * Charge à l'utilisateur d'appeler preferencesDestroy().
  *
  * @return Une structure de préférences nouvellement allouée
  */
Preferences* preferencesCreate (void);
/**
  * @fn Preferences* preferencesCreateFromFile (GKeyFile* ppsContext[])
  * @brief Crée une structure de préférences à partir du fichier de
  sauvegarde.
  *
  * @param[in,out] ppsContext Tableau des fichiers ouverts.
  * @return Une structure de préférences nouvellement allouée et remplie
  avec les préférences du fichier CONFIG
  */
Preferences* preferencesCreateFromFile (GKeyFile* ppsContext[]);
/**
  * @fn int preferencesDestroy (Preferences** ppsPref)
  * @brief Détruit une structure de préférences.
  *
  * @param[in,out] ppsPref Un pointeur sur un pointeur de préférences
  * @return EXIT_SUCCESS si tout est OK
  *
  * Cette fonction libère l'espace allouée pour les préférences lors de
  l'appel à preferencesCreate() et met le pointeur de préférences à NULL.
  */
int preferencesDestroy (Preferences** ppsPref);


/**
  * @fn int preferencesGetWindowXSize (const Preferences* psPref)
  * @brief Accesseur sur la largeur de la fenêtre.
  *
  * @param[in] psPref Pointeur sur les préférences à accéder.
  * @return La valeur de iWindowXSize dans la structure donnée.
  */
int preferencesGetWindowXSize (const Preferences* psPref);
/**
  * @fn int preferencesSetWindowXSize (Preferences* psPref, int iValue)
  * @brief Mutateur pour modifier la largeur de la fenêtre.
  *
  * @param[in,out] psPref Pointeur sur la structure à modifier.
  * @param[in] iValue Valeur à stocker dans la variable iWindowXSize
  * @return EXIT_SUCCESS si tout est OK
  */
int preferencesSetWindowXSize (Preferences* psPref, int iValue);

/**
  * @fn int preferencesGetWindowYSize (const Preferences* psPref)
  * @brief Accesseur sur la hauteur de la fenêtre.
  *
  * @param[in] psPref Pointeur sur les préférences à accéder.
  * @return La valeur de iWindowYSize dans la structure donnée.
  */
int preferencesGetWindowYSize (const Preferences* psPref);
/**
  * @fn int preferencesSetWindowYSize (Preferences* psPref, int iValue)
  * @brief Mutateur pour modifier la hauteur de la fenêtre.
  *
  * @param[in,out] psPref Pointeur sur la structure à modifier
  * @param[in] iValue Valeur à stocker dans la variable iWindowYSize
  * @return EXIT_SUCCESS si tout est OK
  */
int preferencesSetWindowYSize (Preferences* psPref, int iValue);

/**
  * @fn int preferencesGetAnalysisRate (const Preferences* psPref)
  * @brief Accesseur sur le taux d'analyse.
  *
  * @param[in] psPref Pointeur sur les préférences à accéder
  * @return La valeur de iAnalysisRate de la structure donnée
  */
int preferencesGetAnalysisRate (const Preferences* psPref);
/**
  * @fn int preferencesSetAnalysisRate (Preferences* psPref, int iValue)
  * @brief Mutateur pour modifier le taux d'analyse.
  *
  * @param[in,out] psPref Pointeur sur la structure à modifier
  * @param[in] iValue Valeur à stocker dans la variable iAnalysisRate
  (1 - 100)
  * @return EXIT_SUCCESS si tout est OK
  */
int preferencesSetAnalysisRate (Preferences* psPref, int iValue);

/**
  * @fn int preferencesGetMaxTID(const Preferences* psPref)
  * @brief Accesseur sur le TID maximum.
  *
  * @param[in] psPref Pointeur sur la structure à accéder
  * @return L'identifiant maximal de l'arbre.
  */
int preferencesGetMaxTID(const Preferences* psPref);
/**
  * @fn int preferencesSetMaxTID(Preferences* psPref, int iValue)
  * @brief Mutateur
  *
  * @param[in,out] psPref Pointeur sur la structure à modifier
  * @param[in] iValue Nouvelle valeur de TID maximal
  * @return EXIT_SUCCESS si tout est OK
  */
int preferencesSetMaxTID(Preferences* psPref, int iValue);

/**
  * @fn int preferencesGetMinTID(const Preferences* psPref)
  * @brief Accesseur sur le TID minimum.
  *
  * @param[in] psPref Pointeur sur la structure à accéder
  * @return L'identifiant minimal de l'arbre.
  */
int preferencesGetMinTID(const Preferences* psPref);
/**
  * @fn int preferencesSetMinTID(Preferences* psPref, int iValue)
  * @brief Mutateur
  *
  * @param[in,out] psPref Pointeur sur la structure à modifier
  * @param[in] iValue Nouvelle valeur de TID minimal
  * @return EXIT_SUCCESS si tout est OK
  */
int preferencesSetMinTID(Preferences* psPref, int iValue);

/**
  * @fn int preferencesGetNbPath(const Preferences* psPref)
  * @brief Accesseur sur le nombre de dossiers indexés.
  *
  * @param[in] psPref Structure à accéder
  * @return Le nombre de dossiers indexés
  */
int preferencesGetNbPath(const Preferences* psPref);
/**
  * @fn const char** preferencesGetFilesPath (const Preferences* psPref,
										int* piSize)
  * @brief Accesseur sur les chemins des dossiers.
  *
  * @param[in] psPref Préférences à accéder.
  * @param[out] piSize Pointeur sur un entier pour récupérer la taille du
  tableau.
  * @return Un tableau de chaine de caracèteres contenant les chemins vers
  les dossiers contenant les morceaux
  */
char** preferencesGetFilesPath (const Preferences* psPref,
										int* piSize);
/**
  * @fn int preferencesSetFilesPath (Preferences* psPref, int iSize,
							char* pstrPath[])
  * @brief Mutateur sur les chemins de fichiers.
  *
  * @param[in,out] psPref Pointeur sur la structure à modifier
  * @param[in] iSize Taille du tableau pstrPath
  * @param[in,out] pstrPath Tableau de chaine de caractère, contenant le chemin
  des dossiers contenant les morceaux
  * @return EXIT_SUCCESS si tout est OK
  */
int preferencesSetFilesPath (Preferences* psPref, int iSize,
							char* pstrPath[]);

/**
  * @fn int preferencesAddFilesPath (Preferences* psPref,
									const char* strPath)
  * @brief Ajoute un chemin aux chemins existant.
  *
  * @param[in,out] psPref Pointeur sur la structure à modifier
  * @param[in] strPath Chemin à ajouter
  * @return EXIT_SUCCESS si tout est OK
  */
int preferencesAddFilesPath (Preferences* psPref, const char* strPath);


/**
  * @fn int preferencesGet3DQuality (const Preferences* psPref)
  * @brief Accesseur sur la qualité 3D.
  *
  * @param[in] psPref Pointeur sur la structure à accéder
  * @return Le taux de la qualité 3D
  */
int preferencesGet3DQuality (const Preferences* psPref);
/**
  * @fn int preferencesSet3DQuality (Preferences* psPref, int iValue)
  * @brief Mutateur sur la qualité 3D.
  *
  * @param[in,out] psPref Pointeur sur la structure à modifier.
  * @param[in] iValue Valeur à assigner
  * @return EXIT_SUCCESS si tout est OK
  */
int preferencesSet3DQuality (Preferences* psPref, int iValue);

/**
  * @fn int preferencesGetMoveCam (const Preferences* psPref)
  * @brief Accesseur sur l'option de déplacement.
  *
  * @param[in] psPref Pointeur sur la structure à accéder
  * @return 0 s'il ne faut pas se déplacer, 1 sinon.
  */
int preferencesGetMoveCam (const Preferences* psPref);
/**
  * @fn int preferencesSetMoveCam (Preferences* psPref, int iValue)
  * @brief Mutateur sur l'option de déplacement.
  *
  * @param[in,out] psPref Pointeur sur la structure à modifier.
  * @param[in] iValue Valeur à assigner
  * @return EXIT_SUCCESS si tout est OK
  */
int preferencesSetMoveCam (Preferences* psPref, int iValue);

/**
  * @fn int preferencesGetGrid (const Preferences* psPref)
  * @brief Accesseur sur l'option de grille.
  *
  * @param[in] psPref Pointeur sur la structure à accéder
  * @return 0 s'il ne faut pas de grille, 1 sinon.
  */
int preferencesGetGrid (const Preferences* psPref);
/**
  * @fn int preferencesSetGrid (Preferences* psPref, int iValue)
  * @brief Mutateur sur l'option de grille.
  *
  * @param[in,out] psPref Pointeur sur la structure à modifier.
  * @param[in] iValue Valeur à assigner
  * @return EXIT_SUCCESS si tout est OK
  */
int preferencesSetGrid (Preferences* psPref, int iValue);

/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */


/**
  * @fn int preferencesRegressionTest (void)
  * @brief Teste toutes les fonctions du module.
  *
  * @return EXIT_SUCCESS si tout est OK
  */
int preferencesRegressionTest (void);


#endif /* _PREFERENCES_H_ */
