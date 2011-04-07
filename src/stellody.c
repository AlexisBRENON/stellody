/**
  * @brief Fichier contenant le <i>main</i>.
  *
  * @author STELLODY TEAM
  * @file stellody.c
  */

#include <stdio.h>

#include "analyzed_tracks.h"
#include "preferences.h"


/**
  * @fn int main (int argc, char* argv[])
  * @brief Fonction exécutée au lancement du programme.
  *
  * @param[in] argc Nombre d'argument donnés au programme (>0)
  * @param[in,out] argv Tableau de chaine de caractères contenant les
  arguments.
  * @return 0 si tout est OK.
  */
int main (int argc, char* argv[])
{
	printf("Returned value : %d\n", preferencesRegressionTest());

	return 0;
}
