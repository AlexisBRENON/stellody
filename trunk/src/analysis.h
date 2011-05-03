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
  * @fn AnalyzedTrack* analysisTrack (const char* strPath, gpointer* pData)
  * @brief Analyse un morceau et renvoie le morceau créé.
  *
  * @param[in] strPath Chemin du fichier à analyser
  * @param[in,out] pData Le tableau de données
  * @return Un pointeur sur une AnalyzedTrack nouvellement allouée
  */
AnalyzedTrack* analysisTrack (const char* strPath, gpointer* pData);

/**
  * @fn int analysisTrackToTracks (const char* strPath,
								AnalyzedTracks* psTracks,
								gpointer* pData)
  * @brief Analyse un morceau et l'ajoute à un arbre.
  *
  * @param[in] strPath Chemin du fichier à analyser
  * @param[in,out] psTracks Pointeur sur l'arbre
  * @param[in,out] pData Le tableau de données
  * @return EXIT_SUCCESS si tout est OK
  */
int analysisTrackToTracks (const char* strPath,
						AnalyzedTracks* psTracks,
						gpointer* pData);










#endif /* __ANALYSIS_H__ */
