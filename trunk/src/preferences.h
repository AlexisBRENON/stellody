/**
  * @file preferences.h
  * @brief Fichier d'entête du module \em preferences.
  *
  * Module de manipulation des préférences de l'application.
  * @author Alexis BRENON in STELLODY TEAM
  */

#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_

/**
  * @struct sPreferences
  * @brief Structure manipulant les préférences.
  *
  * Cette structure est remplie au démarage de l'application et contient
  les différents paramètres que l'utilisateur peut modifier.
  */
struct sPreferences
{
	int iWindowXSize; /**< Largeur de la fenêtre en pixels */
	int iWindowYSize; /**< Hauteur de la fenêtre en pixels */
	int iAnalysisRate; /**< Taux d'analyse (1 - 100) */
};
/**
  * @typedef Preferences
  * @brief Alias le mot Preference pour représenter la structure associée.
  */
typedef struct sPreferences Preferences;


/**
  * @fn int preferencesInit (Preferences* psPref, int iX, int iY, int iRate)
  * @brief Initialise les préférences avec les arguments
  L'appel à preferencesRelease() est facultatif mais recommandé.
  *
  * @param[in,out] psPref Pointeur sur la structure à initialiser
  * @param[in] iX Largeur de la fenêtre en pixels
  * @param[in] iY Hauteur de la fenêtre en pixels
  * @param[in] iRate Taux d'analyse (1 - 100)
  * @return EXIT_SUCCESS si tout est OK
  */
int preferencesInit (Preferences* psPref, int iX, int iY, int iRate);
/**
  * @fn int preferencesRelease (Preferences* psPref)
  * @brief Libère les préférences.
  *
  * @param[in,out] psPref Pointeur sur la structure à libérer
  * @return EXIT_SUCCESS si tout est OK
  *
  * Cette fonction n'a pas d'utilité concrète car à l'initialisation des
  préférences, aucune allocation dynamique n'est faites. Il est recommandé
  de quand même y faire appel, des fois que l'initialisation soit modifiée.
  */
int preferencesRelease (Preferences* psPref);

/**
  * @fn Preferences* preferencesCreate (int iX, int iY, int iRate)
  * @brief Crée une structure de préférences en mémoire.
  Charge à l'utilisateur d'appeler preferencesDestroy().
  *
  * @param[in] iX Largeur de la fenêtre en pixels
  * @param[in] iY Hauteur de la fenêtre en pixels
  * @param[in] iRate Taux d'analyse (1 - 100)
  * @return Une structure de préférences nouvellement allouée
  */
Preferences* preferencesCreate (int iX, int iY, int iRate);
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


#endif /* _PREFERENCES_H_ */
