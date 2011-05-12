/**
  * @file analysis.h
  * @brief Fichier d'entête du module \em Analyse.
  *
  * @author Alexis BRENON in STELLODY TEAM
  */

#ifndef __ANALYSIS_H__
#define __ANALYSIS_H__


#include "analyzed_track.h"
#include "analyzed_tracks.h"


/**
  * @fn int analysisTrack (const char* strPath, gpointer* pData)
  * @brief Lance l'analyse d'un morceau.
  *
  * @param[in] strPath Chemin du fichier à analyser
  * @param[in,out] pData Le tableau de données
  * @return EXIT_SUCCESS si tout est OK
  */
int analysisTrack (const char* strPath, gpointer* pData);


/**
  * @fn gboolean analysisCheckNewAnalyze (gpointer pData[])
  * @brief Un timeout qui vérifie la présence ou non de morceaux à
  analyser.
  *
  * @param[in,out] pData Le tableau de donnée
  * @return TRUE pour continuer la vérification, FALSE lorsque qu'il n'y a
  plus à vérifier.
  */
gboolean analysisCheckNewAnalyze (gpointer pData[]);

#endif /* __ANALYSIS_H__ */
