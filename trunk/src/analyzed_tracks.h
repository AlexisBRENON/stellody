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
  * @fn int analyzedTracksInitFromFile (AnalyzedTracks* psTracks,
										const GKeyFile* ppsContext[])
  * @brief Initialise l'arbre avec les morceaux sauvés.
  *
  * @param[in,out] psTracks Pointeur sur l'arbre à initialiser.
  * @param[in] ppsContext Tableau des fichiers ouverts.
  * @return EXIT_SUCCESS si tout est OK
  */
int analyzedTracksInitFromFile (AnalyzedTracks* psTracks,
								const GKeyFile* ppsContext[]);

/**
  * @fn AnalyzedTracks* analyzedTracksCreateFromFile (const GKeyFile* ppsContext[])
  * @brief Charge tous les morceaux sauvés dans un arbre.
  Charge à l'utilisateur d'appeler analyzedTracksDestroy().
  *
  * @param[in] ppsContext Tableau des fichiers ouverts.
  * @return Un arbre binaire balancé contenant toutes les chansons classées
  par chemin.
  */
AnalyzedTracks* analyzedTracksCreateFromFile (const GKeyFile* ppsContext[]);


/* ********************************************************************* */
/*                                                                       */
/*                           Test de regression                          */
/*                                                                       */
/* ********************************************************************* */



#endif /* _ANALYZED_TRACKS_H_ */
