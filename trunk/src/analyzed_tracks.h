/**
  * @brief Entête des morceaux analysés
  *
  * Fichier d'entête des structures de morceaux analysés.
  * @author Alexis BRENON in STELLODY TEAM
  * @file analyzed_tracks.h
  */

#ifndef _ANALYZED_TRACKS_H_
#define _ANALYZED_TRACKS_H_

#include <gtk.h>


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
	char * strPlace; /**< Emplacement du fichier sur l'ordinateur */
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
  morceaux */
typedef GTree AnalyzedTracks;

/**
  * @fn char * analyzedTrackGetPlace (const AnalyzedTrack* psTrack)
  * @brief Accesseur pour obtenir l'emplacement du morceau.
  *
  * @param psTrack Pointeur sur la structure AnalyzedTrack à accéder.
  * @return Renvoi le chemin vers le morceau.
  */
char * analyzedTrackGetPlace (const AnalyzedTrack* psTrack);
/**
  * @fn int analyzedTrackSetPlace (const char* strPlace)
  * @brief Mutateur pour modifier le chemin vers le fichier
  *
  * @param psTrack Pointeur sur la structure AnalyzedTrack à modifier.
  * @param strPlace Chemin vers le fichier.
  * @return 0 si tout c'est bien passé.
  */
int analyzedTrackSetPlace (const AnalyzedTrack* psTrack, const char* strPlace);

#endif /* _ANALYZED_TRACKS_H_ */
