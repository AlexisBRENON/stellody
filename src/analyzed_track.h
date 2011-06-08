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
	unsigned int uiLength; /**< Durée du morceau */
	float fAverage; /**< Fréquences moyenne */
	float pfRate[3]; /**< Pourcentage des aigus, mediums et graves */
	float pfCoord[3]; /**< Coordonnées du morceau une fois représenté. */
	unsigned char bAdded ; /**< Booléen indiquant que le morceau a été ajouté au stellarium */
} ;
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
							const char* strPath, unsigned int uiLength,
							float fAverage, const float fRate[],
							const float fCoord[])
  * @brief Initialise un morceau alloué.
  Charge à l'utilisateur d'appeler la fonction analyzedTrackRelease().
  *
  * @param[in,out] psTrack Pointeur sur le morceau à initialiser
  * @param[in] iTID Identifiant du morceau
  * @param[in] strPath Chemin vers le fichier (sur le disque)
  * @param[in] uiLength Longueur du morceau en ms
  * @param[in] fAverage Réel à stocker dans l'attribut Average (0 <= x <= 1)
  * @param[in] fRate Fréquence des aigus, mediums et graves
  * @param[in] fCoord Coordonnées de l'étoile associée
  * @return EXIT_SUCCESS si tout est OK.
  */
int analyzedTrackInitWithData(AnalyzedTrack* psTrack, int iTID,
							const char* strPath, unsigned int uiLength,
							float fAverage, const float fRate[],
							const float fCoord[]);
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
							const char* strPath, unsigned int uiLength,
							float fAverage, const float fRate[],
							const float fCoord[])
  * @brief Alloue un nouveau morceau en mémoire.
  Charge à l'utilisateur d'appeler la fonction analyzedTrackDestroy().
  *
  * @param[in] iTID Identifiant du morceau
  * @param[in] strPath Chemin vers le fichier (sur le disque)
  * @param[in] uiLength Longueur du morceau en ms
  * @param[in] fAverage Réel à stocker comme moyenne (0.0 - 1.0)
  * @param[in] fRate Pourcentage des aigus, mediums et graves
  * @param[in] fCoord Coordonnées de l'étoile associée
  * @return Pointeur sur AnalyzedTrack nouvellement allouée.
  */
AnalyzedTrack* analyzedTrackCreateWithData(int iTID,
							const char* strPath, unsigned int uiLength,
							float fAverage, const float fRate[],
							const float fCoord[]);
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
  * @fn const char* analyzedTrackConstGetPath (
											const AnalyzedTrack* psTrack)
  * @brief Accesseur pour obtenir l'emplacement du morceau.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder.
  * @return Le chemin constant vers le morceau (chaine de caractères).
  */
const char* analyzedTrackConstGetPath (const AnalyzedTrack* psTrack);
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
  * @fn unsigned int analyzedTrackGetLength(const AnalyzedTrack* psTrack)
  * @brief Accesseur sur la longueur du morceau en ms.
  *
  * @param[in] psTrack Morceau à accéder
  * @return la longueur du morceau en millisecondes
  */
unsigned int analyzedTrackGetLength(const AnalyzedTrack* psTrack);
/**
  * @fn int analyzedTrackSetLength(AnalyzedTrack* psTrack,
						unsigned int uiValue)
  * @brief Mutateur sur la longueur du morceau.
  *
  * @param[in,out] psTrack Morceau à modifier
  * @param[in] uiValue Longueur du morceau en ms
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTrackSetLength(AnalyzedTrack* psTrack,
						unsigned int uiValue);
/**
  * @fn float analyzedTrackGetAverage (
									const AnalyzedTrack* psTrack)
  * @brief Accesseur pour obtenir la fréquence moyenne du morceau.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder.
  * @return La valeur de la moyenne des fréquences.
  */
float analyzedTrackGetAverage (const AnalyzedTrack* psTrack);
/**
  * @fn int analyzedTrackSetAverage (AnalyzedTrack* psTrack,
												float fValue)
  * @brief Mutateur pour modifier la valeur de la moyenne des fréquences.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder.
  * @param[in] fValue Le réel à enregistrer.
  * @return EXIT_SUCCESS si tout s'est bien passé.
  */
int analyzedTrackSetAverage (AnalyzedTrack* psTrack,
									float fValue);


/**
  * @fn const float* analyzedTrackConstGetRate (
											const AnalyzedTrack* psTrack)
  * @brief Accesseur sur le tableau de pourcentage de fréquences.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder
  * @return Le tableau de poucentage de fréquence en lecture seule
  */
const float* analyzedTrackConstGetRate (
									const AnalyzedTrack* psTrack);
/**
  * @fn const float* analyzedTrackGetRate (
											const AnalyzedTrack* psTrack)
  * @brief Accesseur sur le tableau de pourcentage de fréquences.
  *
  * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder
  * @return Le tableau de poucentage de fréquence
  */
float* analyzedTrackGetRate (const AnalyzedTrack* psTrack);
/**
  * @fn int analyzedTrackSetRate (AnalyzedTrack* psTrack,
									const float fRate[])
  * @brief Mutateur sur le tableau de poucentage.
  *
  * @param[in,out] psTrack Pointeur sur la structure AnalyzedTrack à accéder
  * @param[in] fRate Le tableau de réels à copier.
  * @return EXIT_SUCCESS si tout est OK.
  */
int analyzedTrackSetRate (AnalyzedTrack* psTrack,
									const float fRate[]);


/**
  * @fn const float* analyzedTrackConstGetCoord (
											const AnalyzedTrack* psTrack)
  * @brief Accesseur sur les coordonnées du morceau
  *
  * @param[in] psTrack Morceau à accéder
  * @return Les coordonnées de l'étoile en lecture seule
  */
const float* analyzedTrackConstGetCoord (const AnalyzedTrack* psTrack);
/**
  * @fn float* analyzedTrackGetCoord (const AnalyzedTrack* psTrack)
  * @brief Accesseur sur les coordonnées du morceau
  *
  * @param[in] psTrack Morceau à accéder
  * @return Les coordonnées de l'étoile
  */
float* analyzedTrackGetCoord (const AnalyzedTrack* psTrack);
/**
  * @fn int analyzedTrackSetCoord(AnalyzedTrack* psTrack,
								const float pfCoord[])
  * @brief Mutateur sur les coordonnées du morceau.
  *
  * @param[in,out] psTrack Morceau à modifier
  * @param[in] pfCoord Coordonnées du morceau
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTrackSetCoord(AnalyzedTrack* psTrack,
						const float pfCoord[]);
/**
  * @fn int analyzedTrackSetCoords(AnalyzedTrack* psTrack,
									float fX,
									float fY,
									float fZ)
  * @brief Mutateur sur les coordonnées du morceau.
  *
  * @param[in,out] psTrack Morceau à modifier
  * @param[in] fX Coordonnée sur X
  * @param[in] fY Coordonnée sur Y
  * @param[in] fZ Coordonnée sur Z
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTrackSetCoords(AnalyzedTrack* psTrack,
						float fX,
						float fY,
						float fZ);

/**
 * @fn unsigned char analyzedTrackGetbAdded (const AnalyzedTrack* psTrack)
 * @brief Accesseur pour obtenir le booléen added du morceau.
 *
 * @param[in] psTrack Pointeur sur la structure AnalyzedTrack à accéder.
 * @return Le booléen indiquant si le morceau est placé sur le stellarium.
 */
unsigned char analyzedTrackGetbAdded (const AnalyzedTrack* psTrack) ;

/**
 * @fn int analyzedTrackSetbAdded (AnalyzedTrack* psTrack, unsigned char bAdded)
 * @brief Mutateur pour modifier la booléen added du morceau.
 *
 * @param[in,out] psTrack Pointeur sur la structure AnalyzedTrack à accéder.
 * @param[in] bAdded Le booléen à enregistrer.
 * @return EXIT_SUCCESS si tout s'est bien passé.
 */
int analyzedTrackSetbAdded (AnalyzedTrack* psTrack, unsigned char bAdded) ;


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
