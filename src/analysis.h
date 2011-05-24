/**
  * @file analysis.h
  * @brief Fichier d'entête du module \em Analyse.
  *
  * @author Alexis BRENON in STELLODY TEAM
  */

#ifndef __ANALYSIS_H__
#define __ANALYSIS_H__

#if defined(__linux)
#include <fmodex/fmod.h>
#endif
#if defined (__APPLE__) || defined (_WIN32)
#include <fmod.h>
#endif

#include "analyzed_track.h"
#include "analyzed_tracks.h"


/**
  * @fn int analysisAnalyze (FMOD_CHANNEL* pChannel,
					float fAnalysisRate,
					AnalyzedTrack* pTrack,
					int* piAnalyzingCounter)
  * @brief Récupère les amplitudes des fréquences et les stockent.
  *
  * @param[in,out] pChannel Canal sur lequel analyser
  * @param[in] fAnalysisRate Taux d'analyse
  * @param[in,out] pTrack Morceau en cours d'analyse
  * @param[in,out] piAnalyzingCounter Conpteur de boucle pour calculer les moyennes
  * @return EXIT_SUCCESS si tout est OK
  */
int analysisAnalyze (FMOD_CHANNEL* pChannel,
					float fAnalysisRate,
					AnalyzedTrack* pTrack,
					int* piAnalyzingCounter);


/**
  * @fn int analysisTrack (const char* strPath, gpointer* pData)
  * @brief Lance l'analyse d'un morceau.
  *
  * @param[in] strPath Chemin du fichier à analyser
  * @param[in,out] pData Le tableau de données
  * @return EXIT_SUCCESS si tout est OK
  */
int analysisTrack (const char* strPath, gpointer* pData);

#endif /* __ANALYSIS_H__ */
