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
					int iAnalyzingCounter)
  * @brief Récupère les amplitudes des fréquences et les stockent.
  *
  * @param[in,out] pChannel Canal sur lequel analyser
  * @param[in] fAnalysisRate Taux d'analyse
  * @param[in,out] pTrack Morceau en cours d'analyse
  * @param[in] iAnalyzingCounter Conpteur de boucle pour calculer les moyennes
  * @return EXIT_SUCCESS si tout est OK
  */
int analysisAnalyze (FMOD_CHANNEL* pChannel,
					float fAnalysisRate,
					AnalyzedTrack* pTrack,
					int iAnalyzingCounter);


/**
  * @fn void analysisAddTrackToAnalyze (char* strFileName,
										gpointer* pData);
  * @brief Ajoute un morceau à la liste des morceaux à analyser.
  *
  * @param[in,out] strFileName Nom de l'entité (Dossier/Fichier)
  * @param[in,out] pData Données annexes
  */
void analysisAddTrackToAnalyze (char* strFileName, gpointer* pData);

#endif /* __ANALYSIS_H__ */
