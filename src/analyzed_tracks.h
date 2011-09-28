/**
  * @brief Fichier d'entête du module \em AnalyzedTracks.
  *
  * @author Alexis BRENON in STELLODY TEAM
  * @file analyzed_tracks.h
  */

#ifndef _ANALYZED_TRACKS_H_
#define _ANALYZED_TRACKS_H_

#include <gtk/gtk.h>
#include "analyzed_track.h"
#include "dynamic_array.h"

/* ********************************************************************* */
/*                                                                       */
/*                        Définitions de types                           */
/*                                                                       */
/* ********************************************************************* */

/**
  * @typedef AnalyzedTracks
  * @brief Utilisation d'un tableau dynamique pour représenter un ensemble de
  morceaux
  *
  * Toutes les fonctions sur les tableaux dynamiques sont
  utilisables avec cette structure.
  */
typedef DynamicArray AnalyzedTracks;


/* ********************************************************************* */
/*                                                                       */
/*         Fonctions relatives à la structure AnalyzedTracks             */
/*                                                                       */
/* ********************************************************************* */

/**
  * @fn gboolean analyzedTracksSaveTracks (AnalyzedTrack* pValue,
								GKeyFile* pData)
  * @brief Sauvegarde les morceau du tableau dans le fichier.
  *
  * @param[in,out] pValue Morceau associé
  * @param[in,out] pData Données tierces
  * @return FALSE pour continuer
  */
gboolean analyzedTracksSaveTracks (AnalyzedTrack* pValue,
								GKeyFile* pData);


/**
  * @fn int analyzedTracksInit(AnalyzedTracks* psTracks)
  * @brief Initialise un tableau à morceaux
  *
  * Il est recommandé de ne pas utiliser cette fonction mais plutôt
  analyzedTracksCreate().
  *
  * @param[in,out] psTracks Pointeur sur le tableau à initialiser.
  * @return EXIT_SUCCESS si tout est OK.
  */
int analyzedTracksInit(AnalyzedTracks* psTracks);
/**
  * @fn int analyzedTracksInitFromFile (AnalyzedTracks* psTracks,
										GKeyFile* ppsContext[])
  * @brief Initialise le tableau avec les morceaux sauvés.
  *
  * @param[in,out] psTracks Pointeur sur le tableau à initialiser.
  * @param[in,out] ppsContext Tableau des fichiers ouverts.
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTracksInitFromFile (AnalyzedTracks* psTracks,
								GKeyFile* ppsContext[]);
/**
  * @fn int analyzedTracksRelease(AnalyzedTracks* psTracks)
  * @brief Libère un tableau à morceaux
  *
  * @param[in,out] psTracks Pointeur sur le tableau à libérer.
  * @return EXIT_SUCCESS si tout est OK.
  */
int analyzedTracksRelease(AnalyzedTracks* psTracks);

/**
  * @fn AnalyzedTracks* analyzedTracksCreate(void);
  * @brief Crée un tableau à morceaux.
  *
  * @return Un pointeur sur le tableau créé.
  */
AnalyzedTracks* analyzedTracksCreate(void);
/**
  * @fn AnalyzedTracks* analyzedTracksCreateFromFile (
												GKeyFile* ppsContext[])
  * @brief Charge tous les morceaux sauvés dans un tableau.
  Charge à l'utilisateur d'appeler analyzedTracksDestroy().
  *
  * @param[in,out] ppsContext Tableau des fichiers ouverts.
  * @return Un tableau contenant toutes les chansons classées
  par chemin.
  */
AnalyzedTracks* analyzedTracksCreateFromFile (GKeyFile* ppsContext[]);
/**
  * @fn int analyzedTracksDestroy(AnalyzedTracks** ppsTracks);
  * @brief Détruit un tableau à morceaux.
  *
  *
  * @param[in,out] ppsTracks Pointeur sur le pointeur sur le tableau à
  détruire.
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTracksDestroy(AnalyzedTracks** ppsTracks);


/**
  * @fn int analyzedTracksInsertTrack(AnalyzedTracks* psTracks,
									AnalyzedTrack* psTrack)
  * @brief Ajoute un morceau dans le tableau.
  *
  * @param[in,out] psTracks Pointeur sur le tableau à mettre à jour
  * @param[in,out] psTrack Pointeur sur le morceau à ajouter.
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTracksInsertTrack(AnalyzedTracks* psTracks,
							AnalyzedTrack* psTrack);
/**
  * @fn AnalyzedTrack* analyzedTracksRemoveTrack(AnalyzedTracks* psTracks,
												int iKey)
  * @brief Enlève un morceau du tableau.
  *
  * @param[in,out] psTracks Pointeur sur le tableau à modifier
  * @param[in] iKey Clé/identifiant du morceau à supprimer.
  * @return L'adresse du morceau enlevé (penser à libérer le morceau)
  */
AnalyzedTrack* analyzedTracksRemoveTrack(AnalyzedTracks* psTracks,
									int iKey);

/**
  * @fn int analyzedTracksRemoveByData (AnalyzedTracks* psTracks,
										const AnalyzedTrack* psTrack);
  * @brief Enlève un morceau du tableau.
  *
  * @param[in,out] psTracks Tableau de morceaux
  * @param [in] psTrack Morceau à supprimer
  * @return EXIT_SUCCESS
  */
int analyzedTracksRemoveByData (AnalyzedTracks* psTracks,
										const AnalyzedTrack* psTrack);

/**
  * @fn const AnalyzedTrack* analyzedTracksGetConstTrack(
							AnalyzedTracks* psTracks,
							int iKey);
  * @brief Récupère un pointeur constant sur le morceau de clé/coordonnées
  \em key.
  *
  * @param[in] psTracks Pointeur sur le tableau contenant le morceau
  * @param[in] iKey Clé/coordonnées du morceau
  * @return Un pointeur constant sur le morceau ou NULL
  */
const AnalyzedTrack* analyzedTracksGetConstTrack(
							AnalyzedTracks* psTracks,
							int iKey);
/**
  * @fn AnalyzedTrack* analyzedTracksGetTrack(
							AnalyzedTracks* psTracks,
							int iKey);
  * @brief Récupère un pointeur sur le morceau de clé/coordonnées
  \em key.
  *
  * @param[in] psTracks Pointeur sur le tableau contenant le morceau
  * @param[in] iKey Clé du morceau
  * @return Un pointeur sur le morceau ou NULL
  */
AnalyzedTrack* analyzedTracksGetTrack(
							AnalyzedTracks* psTracks,
							int iKey);

/**
  * @fn AnalyzedTrack* analyzedTracksGetTrackInArray(
							AnalyzedTracks* psTracks,
							int iKey);
  * @brief Fonction permettant de parcourir tous les morceaux, sans
  le TID de chacun d'entre eux.
  *
  * @param[in,out] psTracks L'ensemble des morceaux
  * @param[in] iKey Compris entre 0 et le nombre de morceaux. C'est l'index
  et non le TID du morceau à récupèrer.
  * @return Retourne la morceau d'index iKey
  */
AnalyzedTrack* analyzedTracksGetTrackInArray(
							AnalyzedTracks* psTracks,
							int iKey);

/**
  * @fn int analyzedTracksGetNbTracks (const AnalyzedTracks* psTracks);
  * @brief Retourne le nombre de morceaux dans le tableau.
  *
  * @param[in] psTracks Ensembles des morceaux.
  * @return Le nombre de morceaux.
  */
int analyzedTracksGetNbTracks (const AnalyzedTracks* psTracks);


/**
  * @fn gboolean analyzedTracksCheckForAnalyze (int* piKey,
										AnalyzedTrack* psTrack,
										gpointer* pData)
  * @brief Enlève tous les morceau du tableau qui ne sont pas encore
  analysés pour les rajouter à la liste d'analyse.
  *
  * @param[in,out] piKey pointeur sur la clé dans le tableau
  * @param[in,out] psTrack pointeur sur le morceau
  * @param[in,out] pData Tableau des données utilisées
  * @return FALSE pour continuer la traversée
  */
gboolean analyzedTracksCheckForAnalyze (int* piKey,
										AnalyzedTrack* psTrack,
										gpointer* pData);

/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */

/**
  * @fn int analyzedTracksRegressionTest (void)
  * @brief Test de (non-)regression du module AnalyzedTracks.
  *
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTracksRegressionTest (void);

#endif /* _ANALYZED_TRACKS_H_ */
