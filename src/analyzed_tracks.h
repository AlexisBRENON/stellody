/**
  * @brief Entête des morceaux analysés
  *
  * Fichier d'entête des morceaux analysés.
  * @author Alexis BRENON in STELLODY TEAM
  * @file analyzed_tracks.h
  */

#ifndef _ANALYZED_TRACKS_H_
#define _ANALYZED_TRACKS_H_

#include <gtk/gtk.h>
#include "analyzed_track.h"

/* ********************************************************************* */
/*                                                                       */
/*                        Définitions de types                           */
/*                                                                       */
/* ********************************************************************* */

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
/*         Fonctions relatives à la structure AnalyzedTracks             */
/*                                                                       */
/* ********************************************************************* */

/**
  * @fn int analyzedTracksInit(AnalyzedTracks* psTracks)
  * @brief Initialise un arbre à morceaux
  *
  * Il est recommandé de ne pas utiliser cette fonction mais plutôt
  analyzedTracksCreate().
  *
  * @param[in,out] psTracks Pointeur sur l'arbre à initialiser.
  * @return EXIT_SUCCESS si tout est OK.
  */
int analyzedTracksInit(AnalyzedTracks* psTracks);
/**
  * @fn int analyzedTracksInitFromFile (AnalyzedTracks* psTracks,
										GKeyFile* ppsContext[])
  * @brief Initialise l'arbre avec les morceaux sauvés.
  *
  * @param[in,out] psTracks Pointeur sur l'arbre à initialiser.
  * @param[in,out] ppsContext Tableau des fichiers ouverts.
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTracksInitFromFile (AnalyzedTracks* psTracks,
								GKeyFile* ppsContext[]);
/**
  * @fn int analyzedTracksRelease(AnalyzedTracks* psTracks)
  * @brief Libère un arbre à morceaux
  *
  * @param[in,out] psTracks Pointeur sur l'arbre à libérer.
  * @return EXIT_SUCCESS si tout est OK.
  */
int analyzedTracksRelease(AnalyzedTracks* psTracks);

/**
  * @fn AnalyzedTracks* analyzedTracksCreate(void);
  * @brief Crée un arbre à morceaux.
  *
  * @return Un pointeur sur l'arbre créé.
  */
AnalyzedTracks* analyzedTracksCreate(void);
/**
  * @fn AnalyzedTracks* analyzedTracksCreateFromFile (
												GKeyFile* ppsContext[])
  * @brief Charge tous les morceaux sauvés dans un arbre.
  Charge à l'utilisateur d'appeler analyzedTracksDestroy().
  *
  * @param[in,out] ppsContext Tableau des fichiers ouverts.
  * @return Un arbre binaire balancé contenant toutes les chansons classées
  par chemin.
  */
AnalyzedTracks* analyzedTracksCreateFromFile (GKeyFile* ppsContext[]);
/**
  * @fn int analyzedTracksDestroy(AnalyzedTracks** ppsTracks);
  * @brief Détruit un arbre à morceaux.
  *
  * @warning Les morceaux contenus dans l'arbre ne sont pas détruits.
  L'utilisateur doit le faire lui-même.
  *
  * @param[in,out] ppsTracks Pointeur sur le pointeur sur l'arbre à
  détruire.
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTracksDestroy(AnalyzedTracks** ppsTracks);


/**
  * @fn int analyzedTracksInsertTrack(AnalyzedTracks* psTracks,
									AnalyzedTrack* psTrack)
  * @brief Ajoute un morceau dans l'arbre.
  *
  * @param[in,out] psTracks Pointeur sur l'arbre à mettre à jour
  * @param[in,out] psTrack Pointeur sur le morceau à ajouter.
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTracksInsertTrack(AnalyzedTracks* psTracks,
							AnalyzedTrack* psTrack);
/**
  * @fn AnalyzedTrack* analyzedTracksRemoveTrack(AnalyzedTracks* psTracks,
												int iKey)
  * @brief Enlève un morceau de l'arbre.
  *
  * @param[in,out] psTracks Pointeur sur l'arbre à modifier
  * @param[in] iKey Clé/identifiant du morceau à supprimer.
  * @return L'adresse du morceau enlevé (penser à libérer le morceau)
  */
AnalyzedTrack* analyzedTracksRemoveTrack(AnalyzedTracks* psTracks,
									int iKey);

/**
  * @fn const AnalyzedTrack* analyzedTracksGetConstTrack(
							AnalyzedTracks* psTracks,
							int iKey);
  * @brief Récupère un pointeur constant sur le morceau de clé/coordonnées
  \em key.
  *
  * @param[in] psTracks Pointeur sur l'arbre contenant le morceau
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
  * @param[in] psTracks Pointeur sur l'arbre contenant le morceau
  * @param[in] iKey Clé/coordonnées du morceau
  * @return Un pointeur sur le morceau ou NULL
  */
AnalyzedTrack* analyzedTracksGetTrack(
							AnalyzedTracks* psTracks,
							int iKey);


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
