/**
  * @brief Fichier d'entête du module \em AnalyzedTrack.
  *
  * @author Alexis BRENON in STELLODY TEAM
  * @file analyzed_track.h
  */

#ifndef _ANALYZED_TRACK_H_
#define _ANALYZED_TRACK_H_

#include <gtk/gtk.h>


#define iNUMVALUES 512 /**< Nombre de valeur récupérées à chaque analyse.*/
#define iSAVEDVALUES 128 /**< Nombre de valeurs sauvegardées pour traintement.*/

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
	int iTID; /**< Identifiant du morceau (dans l'abre, le Stellarium) */
	char bAnalyzed; /**< Permet de savoir si le morceau a déjà été analysé */
	char * strPath; /**< Emplacement du fichier sur l'ordinateur */
	float fFrequenciesAverage; /**< Réel codant la moyenne des
	fréquences du morceau */
	float fFrequenciesMedian; /**< Réel codant la médiane des fréquences */
	float fValues[iSAVEDVALUES]; /**< Tableau contenant les moyennes des
	amplitudes des harmoniques, du do 0 au do 5 par plage de ~40Hz.*/
};
/**
  * @typedef AnalyzedTrack
  * @brief Redéfinit la structure sAnalyzedTrack.
  */
typedef struct sAnalyzedTrack AnalyzedTrack;


/* ********************************************************************* */
/*                                                                       */
/*         Fonctions relatives à la structure AnalyzedTrack              */
/*                                                                       */
/* ********************************************************************* */


/**
  * @fn int analyzedTrackCompare(const int* iTID1,
						const int* iTID2)
  * @brief Compare les deux entiers pointés.
  *
  * Cette fonction est utilisée par le module AnalyzedTracks pour inserer
  un morceau dans l'arbre
  *
  * @param[in] iTID1 Pointeur sur un entier à comparer
  * @param[in] iTID2 Pointeur sur le second entier à comparer
  * @return 0 si les entiers sont égaux, -1 si iTID1<=iTID2, 1 sinon
  */
int analyzedTrackCompare(const int* iTID1,
						const int* iTID2);
/**
  * @fn int analyzedTrackDataCompare(const int* iTID1,
						const int* iTID2,
						void* pData)
  * @brief Compare les deux entiers pointés.
  *
  * Cette fonction est utilisée par le module AnalyzedTracks pour ordonner
  les morceaux dans l'arbre.
  *
  * @param[in] iTID1 Pointeur sur un entier à comparer
  * @param[in] iTID2 Pointeur sur le second entier à comparer
  * @param[in,out] pData Pointeur sur des données supplémentaires (NULL)
  * @return 0 si les entiers sont égaux, -1 si iTID1<=iTID2, 1 sinon
  */
int analyzedTrackDataCompare(const int* iTID1,
						const int* iTID2,
						void* pData);

/**
  * @fn int analyzedTrackInitWithData(AnalyzedTrack* psTrack, int iTID,
									const char* strPath, float fAverage,
									float fMedian)
  * @brief Initialise un morceau alloué.
  Charge à l'utilisateur d'appeler la fonction analyzedTrackRelease().
  *
  * @param[in,out] psTrack Pointeur sur le morceau à initialiser
  * @param[in] iTID Identifiant du morceau
  * @param[in] strPath Chemin vers le fichier (sur le disque)
  * @param[in] fAverage Réel à stocker dans l'attribut Average (0 <= x <= 1)
  * @param[in] fMedian Réel à stocker dans l'attribut Median (0 <= x <= 1)
  * @return EXIT_SUCCESS si tout est OK.
  */
int analyzedTrackInitWithData(AnalyzedTrack* psTrack, int iTID,
							const char* strPath, float fAverage,
							float fMedian,float fValues[]);
/**
  * @fn int analyzedTrackInit(AnalyzedTrack* psTrack)
  * @brief Initialise un morceau avec des valeurs pas défauts.
  *
  * Initialise le morceau avec un identifiant de valeur 0, un chemin NULL,
  et les statistiques à 0.
  *
  * @param[in,out] psTrack Pointeur sur le morceau à initialiser
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTrackInit(AnalyzedTrack* psTrack);
/**
  * @fn int analyzedTrackRelease(AnalyzedTrack* psTrack)
  * @brief Libère un morceau initialisé.
  *
  * @param[in,out] psTrack Pointeur sur le morceau à libérer
  * @return EXIT_SUCCESS si tout est OK.
  */
int analyzedTrackRelease(AnalyzedTrack* psTrack);
/**
  * @fn gboolean analyzedTrackReleaseFromTree(gpointer pKey,
									gpointer pValue,
									gpointer pData)
  * @brief Libère un morceau contenu dans un arbre.
  * @warning Cette fonction ne doit pas être appelée par l'utilisateur.
  *
  * @param[in,out] pKey Pointeur sur la clé du noeud
  * @param[in,out] pValue Pointeur sur le noeud
  * @param[in,out] pData Pointeur sur des données utiles (NULL)
  * @return \b FALSE si tout est OK
  */
gboolean analyzedTrackReleaseFromTree(gpointer pKey, gpointer pValue,
									gpointer pData);


/**
  * @fn AnalyzedTrack* analyzedTrackCreateWithData(int iTID,
												const char* strPath,
										float fAverage, float fMedian)
  * @brief Alloue un nouveau morceau en mémoire.
  Charge à l'utilisateur d'appeler la fonction analyzedTrackDestroy().
  *
  * @param[in] iTID Identifiant du morceau
  * @param[in] strPath Chemin vers le fichier (sur le disque)
  * @param[in] fAverage Réel à stocker comme moyenne (0.0 - 1.0)
  * @param[in] fMedian Réel à stocker comme médiane (0.0 - 1.0)
  * @return Pointeur sur AnalyzedTrack nouvellement allouée.
  */
AnalyzedTrack* analyzedTrackCreateWithData(int iTID, const char* strPath,
										float fAverage, float fMedian,
										float fValues[]);
/**
  * @fn AnalyzedTrack* analyzedTrackCreate(void)
  * @brief Crée un morceau avec des valeurs par défauts.
  *
  * @return Un morceau nouvellement alloué.
  */
AnalyzedTrack* analyzedTrackCreate(void);
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
  * @fn int analyzedTrackGetTID (const AnalyzedTrack* psTrack)
  * @brief Accesseur sur l'identifiant du morceau.
  *
  * @param[in] psTrack Pointeur sur le morceau à accéder.
  * @return L'identifiant (entier) du morceau
  */
int analyzedTrackGetTID (const AnalyzedTrack* psTrack);
/**
  * @fn int analyzedTrackSetTID (AnalyzedTrack* psTrack,
						int iValue)
  * @brief Mutateur sur l'identifiant d'un morceau.
  *
  * @param[in,out] psTrack Pointeur sur le morceau à modifier
  * @param[in] iValue Nouvelle valeur à stocker
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTrackSetTID (AnalyzedTrack* psTrack,
						int iValue);

/**
  * @fn char analyzedTrackGetAnalyzed (const AnalyzedTrack *psTrack)
  * @brief Accesseur sur l'état d'analyse du morceau.
  *
  * @param[in] psTrack Pointeur sur le morceau à accéder.
  * @return 0 si le morceau n'est pas analysé, 1 sinon.
  */
char analyzedTrackGetAnalyzed (const AnalyzedTrack *psTrack);
/**
  * @fn int analyzedTrackSetAnalyzed (AnalyzedTrack* psTrack,
									 char bValue)
  * @brief Mutateur sur l'état d'analyse du morceau.
  *
  * @param[in,out] psTrack Pointeur sur le morceau à modifier
  * @param[in] bValue 0 si le morceau n'est pas analysé, 1 sinon.
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTrackSetAnalyzed (AnalyzedTrack* psTrack, char bValue);

/**
  * @fn char* analyzedTrackGetPath (const AnalyzedTrack* psTrack)
  * @brief Accesseur pour obtenir l'emplacement du morceau.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder.
  * @return Le chemin vers le morceau (chaine de caractères).
  */
char* analyzedTrackGetPath (const AnalyzedTrack* psTrack);
/**
  * @fn int analyzedTrackSetPath (AnalyzedTrack* psTrack,
									const char* strNewPath)
  * @brief Mutateur pour modifier le chemin vers le fichier
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à modifier.
  * @param[in] strNewPath Chemin vers le fichier.
  * @return EXIT_SUCCESS si tout s'est bien passé.
  */
int analyzedTrackSetPath (AnalyzedTrack* psTrack,
						 const char* strNewPath);


/**
  * @fn float analyzedTrackGetFrequenciesAverage (
									const AnalyzedTrack* psTrack)
  * @brief Accesseur pour obtenir la fréquence moyenne du morceau.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder.
  * @return La valeur de la moyenne des fréquences.
  */
float analyzedTrackGetFrequenciesAverage (const AnalyzedTrack* psTrack);
/**
  * @fn int analyzedTrackSetFrequenciesAverage (AnalyzedTrack* psTrack,
												float fValue)
  * @brief Mutateur pour modifier la valeur de la moyenne des fréquences.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder.
  * @param[in] fValue Le réel à enregistrer.
  * @return EXIT_SUCCESS si tout s'est bien passé.
  */
int analyzedTrackSetFrequenciesAverage (AnalyzedTrack* psTrack,
									float fValue);


/**
  * @fn float analyzedTrackGetFrequenciesMedian (
										const AnalyzedTrack* psTrack)
  * @brief Accesseur pour obtenir la fréquence médiane du morceau.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder
  * @return La valeur médiane des fréquences.
  */
float analyzedTrackGetFrequenciesMedian (const AnalyzedTrack* psTrack);
/**
  * @fn int analyzedTrackSetFrequenciesMedian (AnalyzedTrack* psTrack,
											float fValue)
  * @brief Mutateur pour modifier la valeur médiane des fréquences.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder
  * @param[in] fValue Le réel à stocker.
  * @return EXIT_SUCCESS si tout s'est bien passé
  */
int analyzedTrackSetFrequenciesMedian (AnalyzedTrack* psTrack,
									float fValue);

/**
  * @fn const float* analyzedTrackGetFrequenciesValues (
											const AnalyzedTrack* psTrack)
  * @brief Accesseur sur le tableau de fréquences.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder
  * @return Le tableau de fréquences
  */
const float* analyzedTrackGetFrequenciesValues (
									const AnalyzedTrack* psTrack);

/**
  * @fn int analyzedTrackSetFrequenciesValues (AnalyzedTrack* psTrack,
									fValues[])
  * @brief Mutateur sur l'ensemble du tableau.
  *
  * @param[in,out] psTrack Pointeur sur la structure AnalyzedTrack à accéder
  * @param[in] fValues Le tableau de réels à copier.
  * @return EXIT_SUCCESS si tout est OK.
  */
int analyzedTrackSetFrequenciesValues (AnalyzedTrack* psTrack,
									const float fValues[]);

/**
  * @fn float analyzedTrackGetIemeFrequenciesValues (
										const AnalyzedTrack* psTrack,
										int i);
  * @brief Accesseur sur une unique case du tableau de fréquences.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder
  * @param[in] i Indice de la case à accéder (0 -> 127)
  * @return La valeur de la case indexée.
  */
float analyzedTrackGetIemeFrequenciesValues (const AnalyzedTrack* psTrack,
										int i);
/**
  * @fn int analyzedTrackSetIemeFrequenciesValues (AnalyzedTrack* psTrack,
										int i, float fValue);
  * @brief Mutateur sur une unique case du tableau de fréquences.
  *
  * @param[in,out] psTrack Pointeur sur la structure AnalyzedTrack à accéder
  * @param[in] i Indice de la case à accéder (0 -> 127)
  * @param[in] fValue Réel à stocker
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTrackSetIemeFrequenciesValues (AnalyzedTrack* psTrack,
										int i, float fValue);

/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */


/**
  * @fn int analyzedTrackRegressionTest (void)
  * @brief Teste toutes les fonctions du module analyzed_tracks
  *
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTrackRegressionTest (void);


#endif /* _ANALYZED_TRACK_H_ */
