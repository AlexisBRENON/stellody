/**
  * @brief Entête des morceaux analysés
  *
  * Fichier d'entête des structures de morceaux analysés.
  * @author Alexis BRENON in STELLODY TEAM
  * @file analyzed_tracks.h
  */

#ifndef _ANALYZED_TRACKS_H_
#define _ANALYZED_TRACKS_H_

#include <gtk/gtk.h>


/* ********************************************************************* */
/*                                                                       */
/*                        Définitions de types                           */
/*                                                                       */
/* ********************************************************************* */


/**
  * @struct sAnalyzedTrack
  * @brief Structure représentant un morceau analysé.
  *
  * Cette structure est utilisée pour représenter un morceau analyzé.
  Elle contient les différentes informations nescéssaires pour le placer,
  et le lire. Les champs de cette structures doivent être modifiés ou lus
  par les accesseurs et mutateurs.
  */
struct sAnalyzedTrack
{
	char * strPath; /**< Emplacement du fichier sur l'ordinateur */
	float fFrequenciesAverage; /**< Réel codant la moyenne des fréquences du morceau */
	float fFrequenciesMedian; /**< Réel codant la médiane des fréquences */
};
/**
  * @typedef AnalyzedTrack
  * @brief Redéfinit la structure sAnalyzedTrack.
  */
typedef struct sAnalyzedTrack AnalyzedTrack;

/**
  * @typedef AnalyzedTracks
  * @brief Utilisation d'un arbre binaire pour représenter un ensemble de
  morceaux
  *
  * Toutes les fonctions sur les Arbres de la bibliothèque GTK sont
  utilisables avec cette structure.
  */
typedef GTree AnalyzedTracks;


/* ********************************************************************* */
/*                                                                       */
/*         Fonctions relatives à la structure AnalyzedTrack              */
/*                                                                       */
/* ********************************************************************* */


/**
  * @fn int analyzedTrackInit(AnalyzedTrack* psTrack, const char* strPath, float fAverage, float fMedian)
  * @brief Initialise un morceau alloué.
  Charge à l'utilisateur d'appeler la fonction analyzedTrackRelease().
  *
  * @param[in,out] psTrack Pointeur sur le morceau à initialiser
  * @param[in] strPath Chemin vers le fichier (sur le disque)
  * @param[in] fAverage Réel à stocker dans l'attribut Average (0 <= x <= 1)
  * @param[in] fMedian Réel à stocker dans l'attribut Median (0 <= x <= 1)
  * @return EXIT_SUCCESS si tout est OK.
  */
int analyzedTrackInit(AnalyzedTrack* psTrack, const char* strPath,
					float fAverage, float fMedian);
/**
  * @fn int analyzedTrackRelease(AnalyzedTrack* psTrack)
  * @brief Libère un morceau initialisé.
  *
  * @param[in,out] psTrack Pointeur sur le morceau à libérer
  * @return EXIT_SUCCESS si tout est OK.
  */
int analyzedTrackRelease(AnalyzedTrack* psTrack);


/**
  * @fn AnalyzedTrack* analyzedTrackCreate(const char* strPath, float fAverage, float fMedian)
  * @brief Alloue un nouveau morceau en mémoire.
  Charge à l'utilisateur d'appeler la fonction analyzedTrackDestroy().
  *
  * @param[in] strPath Chemin vers le fichier (sur le disque)
  * @param[in] fAverage Réel à stocker comme moyenne (0.0 - 1.0)
  * @param[in] fMedian Réel à stocker comme médiane (0.0 - 1.0)
  * @return Pointeur sur AnalyzedTrack nouvellement allouée.
  */
AnalyzedTrack* analyzedTrackCreate(const char* strPath, float fAverage,
								float fMedian);
/**
  * @fn int analyzedTrackDestroy(AnalyzedTrack** ppsTrack)
  * @brief Désalloue un morceau en mémoire.
  *
  * Libère toutes les variables du morceau, le morceau en lui même et met
  pointeur à NULL.
  * @param[in,out] ppsTrack Pointeur sur pointeur sur morceau à désallouer.
  * @return EXIT_SUCCESS si tout est OK.
  */
int analyzedTrackDestroy(AnalyzedTrack** ppsTrack);


/**
  * @fn const char* analyzedTrackGetPath (const AnalyzedTrack* psTrack)
  * @brief Accesseur pour obtenir l'emplacement du morceau.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder.
  * @return Le chemin vers le morceau (chaine de caractères).
  */
const char* analyzedTrackGetPath (const AnalyzedTrack* psTrack);
/**
  * @fn int analyzedTrackSetPath 	(AnalyzedTrack* psTrack, const char* strNewPath)
  * @brief Mutateur pour modifier le chemin vers le fichier
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à modifier.
  * @param[in] strNewPath Chemin vers le fichier.
  * @return EXIT_SUCCESS si tout s'est bien passé.
  */
int analyzedTrackSetPath (AnalyzedTrack* psTrack,
						 const char* strNewPath);


/**
  * @fn float analyzedTrackGetFrequenciesAverage (const AnalyzedTrack* psTrack)
  * @brief Accesseur pour obtenir la fréquence moyenne du morceau.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder.
  * @return La valeur de la moyenne des fréquences.
  */
float analyzedTrackGetFrequenciesAverage (const AnalyzedTrack* psTrack);
/**
  * @fn int analyzedTrackSetFrequenciesAverage (AnalyzedTrack* psTrack, float fValue)
  * @brief Mutateur pour modifier la valeur de la moyenne des fréquences.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder.
  * @param[in] fValue Le réel à enregistrer.
  * @return EXIT_SUCCESS si tout s'est bien passé.
  */
int analyzedTrackSetFrequenciesAverage (AnalyzedTrack* psTrack,
									float fValue);


/**
  * @fn float analyzedTrackGetFrequenciesMedian (const AnalyzedTrack* psTrack)
  * @brief Accesseur pour obtenir la fréquence médiane du morceau.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder
  * @return La valeur médiane des fréquences.
  */
float analyzedTrackGetFrequenciesMedian (const AnalyzedTrack* psTrack);
/**
  * @fn int analyzedTrackSetFrequenciesMedian (AnalyzedTrack* psTrack, float fValue)
  * @brief Mutateur pour modifier la valeur médiane des fréquences.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder
  * @param[in] fValue Le réel à stocker.
  * @return EXIT_SUCCESS si tout s'est bien passé
  */
int analyzedTrackSetFrequenciesMedian (AnalyzedTrack* psTrack,
									float fValue);


/* ********************************************************************* */
/*                                                                       */
/*         Fonctions relatives à la structure AnalyzedTracks             */
/*                                                                       */
/* ********************************************************************* */


/**
  * @fn int analyzedTracksInitFromFile (AnalyzedTracks* psTracks,
										GKeyFile* ppsContext[])
  * @brief Initialise l'arbre avec les morceaux sauvés.
  *
  * @param psTracks Pointeur sur l'arbre à initialiser.
  * @param ppsContext Tableau des fichiers ouverts.
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTracksInitFromFile (AnalyzedTracks* psTracks,
								GKeyFile* ppsContext[]);

/**
  * @fn AnalyzedTracks* analyzedTracksCreateFromFile (GKeyFile* ppsContext)
  * @brief Charge tous les morceaux sauvés dans un arbre.
  Charge à l'utilisateur d'appeler analyzedTracksDestroy().
  *
  * @param ppsContext Tableau des fichiers ouverts.
  * @return Un arbre binaire balancé contenant toutes les chansons classées
  par chemin.
  */
AnalyzedTracks* analyzedTracksCreateFromFile (GKeyFile* ppsContext[]);


/* ********************************************************************* */
/*                                                                       */
/*                          Tests de regressions                         */
/*                                                                       */
/* ********************************************************************* */


/**
  * @fn int analyzedTrackRegressionTest (void)
  * @brief Teste toutes les fonctions du module analyzed_tracks
  *
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTrackRegressionTest (void);

#endif /* _ANALYZED_TRACKS_H_ */
