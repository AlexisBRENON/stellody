/**
  * @file gui.h
  * @brief Fichier d'entête du module \em GUI.
  *
  * @author Alexis BRENON in STELLODY TEAM
  */

#ifndef __GUI_H__
#define __GUI_H__

#include <gtk/gtk.h>


/* ********************************************************************* */
/*                                                                       */
/*                        Définitions de types                           */
/*                                                                       */
/* ********************************************************************* */


#define NB_DATA 15 /**< Nombre de cases aux tableaux de données */
/**
  * @enum eGUI_DATA
  * @brief Enumération permettant de retrouver les différentes données dans
  le tableau de données passé aux callbacks.
  */
enum eGUI_DATA
{
	PREFERENCES 		= 0,	/**< Structure de préférences. */
	ANALYZED_TRACKS		= 1,	/**< Arbre des morceaux analysés. */
	MAIN_BUILDER		= 2,	/**< Builder de la fenêtre principale. */
	STELLARIUM_BUILDER	= 3,	/**< Builder du Stellarium */
	PREFERENCES_BUILDER = 4,	/**< Builder des préférences. */
	ABOUT_BUILDER		= 5,	/**< Builder du A Propos. */
	FMOD_CONTEXT		= 6,	/**< Contexte Fmod. */
	PLAYING_CHANNEL		= 7,	/**< Canal FMod de lecture. */
	ANALYZING_CHANNEL	= 8,	/**< Canal FMod d'analyse. */
	PLAYLIST			= 9,	/**< Liste des morceaux à lire */
	ANALYZELIST			= 10,	/**< Liste des morceau à analyser */
	CHECKANALYZE		= 11,	/**< ID du timer de vérification d'analyse */
	MOUSEPOSITION_X		= 12, 	/**< Coordonnées en x de la souris */
	MOUSEPOSITION_Y		= 13,	/**< Coordonnées en y de la souris */
	OPENGLDATA			= 14	/**< Structure de données OpenGL */
};
/**
  * @typedef GUI_DATA
  * @brief Redéfini l'énumération.
  */
typedef enum eGUI_DATA GUI_DATA;


/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions relatives à la fenêtre                    */
/*                                                                       */
/* ********************************************************************* */

/**
  * @fn int guiLoad (gpointer* pData)
  * @brief Charge et affiche l'interface homme machine (GUI).
  *
  * @param[in,out] pData Pointeur sur un tableau qui sera envoyée à tous les
  callbacks
  * @return Le builder créé.
  */
int guiLoad (gpointer* pData);


/* ********************************************************************* */
/*                                                                       */
/*                               CALLBACKS                               */
/*                                                                       */
/* ********************************************************************* */

/* ********************************************************************* */
/*                           FENÊTRE PRINCIPALE                          */
/* ********************************************************************* */
/**
  * @fn int on_Quit_Action_activate (GtkWidget* psWidget, gpointer* pData)
  * @brief Ferme l'interface et stoppe l'application.
  *
  * @param[in,out] psWidget Pointeur sur le widget appellant ce callback
  * @param[in,out] pData Données autres passées au callback.
  * @return EXIT_SUCCESS si tout est OK.
  */
int on_Quit_Action_activate (GtkWidget* psWidget, gpointer* pData);

/**
  * @fn int on_Play_Action_activate (GtkWidget* psWidget, gpointer* pData)
  * @brief Lance la lecture d'un morceau.
  *
  * @param[in,out] psWidget Pointeur sur le widget appellant ce callback
  * @param[in,out] pData Données autres passées au callback.
  * @return EXIT_SUCCESS si tout est OK, EXIT_FAILURE si aucun morceau n'a
  été trouvé.
  */
int on_Play_Action_activate (GtkWidget* psWidget, gpointer* pData);

/**
  * @fn int on_Stop_Action_activate (GtkWidget* psWidget, gpointer* pData)
  * @brief Stoppe la lecture d'un morceau.
  *
  * @param[in,out] psWidget Pointeur sur le widget appellant ce callback
  * @param[in,out] pData Données autres passées au callback.
  * @return EXIT_SUCCESS si tout est OK.
  */
int on_Stop_Action_activate (GtkWidget* psWidget, gpointer* pData);

/**
  * @fn int on_Pause_Action_activate (GtkWidget* psWidget, gpointer* pData)
  * @brief Met en pause lecture d'un morceau.
  *
  * @param[in,out] psWidget Pointeur sur le widget appellant ce callback
  * @param[in,out] pData Données autres passées au callback.
  * @return EXIT_SUCCESS si tout est OK.
  */
int on_Pause_Action_activate (GtkWidget* psWidget, gpointer* pData);

/**
  * @fn int on_AddTrack_Action_activate (GtkWidget* psWidget,
										gpointer* pData)
  * @brief Ajoute un morceau à la base de donnée.
  *
  * @param[in,out] psWidget Pointeur sur le widget appellant ce callback
  * @param[in,out] pData Données autres passées au callback.
  * @return EXIT_SUCCESS si tout est OK.
  */
int on_AddTrack_Action_activate (GtkWidget* psWidget, gpointer* pData);

/**
  * @fn int on_AddDir_Action_activate (GtkWidget* psWidget,
									gpointer* pData)
  * @brief Ajoute un répertoire (et tous ses morceaux) à la BDD.
  *
  * @param[in,out] psWidget Pointeur sur le widget appellant ce callback
  * @param[in,out] pData Données autres passées au callback.
  * @return EXIT_SUCCESS si le répertoire a été ajouté, EXIT_FAILURE si le
  répertoire était déja présent.
  */
int on_AddDir_Action_activate (GtkWidget* psWidget, gpointer* pData);

/**
  * @fn int on_Stellarium_Action_activate (GtkWidget* psWidget,
										gpointer* pData)
  * @brief Affiche le Stellarium.
  *
  * @param[in,out] psWidget Pointeur sur le widget appellant ce callback
  * @param[in,out] pData Données autres passées au callback.
  * @return EXIT_SUCCESS si tout est OK.
  */
int on_Stellarium_Action_activate (GtkWidget* psWidget, gpointer* pData);

/**
  * @fn int on_Preferences_Action_activate (GtkWidget* psWidget,
											gpointer* pData)
  * @brief Affiche la fenêtre de gestion de préférences.
  *
  * @param[in,out] psWidget Pointeur sur le widget appellant ce callback
  * @param[in,out] pData Données autres passées au callback.
  * @return EXIT_SUCCESS si tout est OK.
  */
int on_Preferences_Action_activate (GtkWidget* psWidget, gpointer* pData);

/**
  * @fn int on_About_Action_activate (GtkWidget* psWidget, gpointer* pData)
  * @brief Affiche la fenêtre A Propos.
  *
  * @param[in,out] psWidget Pointeur sur le widget appellant ce callback
  * @param[in,out] pData Données autres passées au callback.
  * @return EXIT_SUCCESS si tout est OK.
  */
int on_About_Action_activate (GtkWidget* psWidget, gpointer* pData);

/**
  * @fn int on_Next_Action_activate (GtkWidget* psWidget, gpointer* pData)
  * @brief Passe au morceau suivant.
  *
  * @param[in,out] psWidget Pointeur sur le widget appellant ce callback
  * @param[in,out] pData Données autres passées au callback.
  * @return EXIT_SUCCESS si tout est OK.
  */
int on_Next_Action_activate (GtkWidget* psWidget, gpointer* pData);

/**
  * @fn int on_Track_Scale_value_changed (GtkWidget* psWidget,
										GdkEventButton* pEvent,
										gpointer* pData)
  * @brief Permet de se déplacer dans un morceau en lecture.
  *
  * @param[in,out] psWidget Pointeur sur le widget appellant ce callback
  * @param[in,out] pEvent Un pointeur sur une structure évenement
  * @param[in,out] pData Données autres passées au callback.
  * @return EXIT_SUCCESS si tout est OK.
  */
int on_Track_Scale_value_changed (GtkWidget* psWidget,
								GdkEventButton* pEvent,
								gpointer* pData);


/* ********************************************************************* */
/*                           FENÊTRE PREFERENCES                         */
/* ********************************************************************* */


/**
  * @fn int on_PrefOKBut_Action_activate (GtkWidget* psWidget,
										gpointer* pData)
  * @brief Sauvegarde les préférences et affiche le Stellarium.
  *
  * @param[in,out] psWidget Widget appelant ce callback
  * @param[in,out] pData Données tierces
  * @return EXIT_SUCCESS si tout est OK
  */
int on_PrefOKBut_Action_activate (GtkWidget* psWidget, gpointer* pData);

/**
  * @fn int on_FlushData_Action_activate (GtkWidget* psWidget,
										gpointer* pData);
  * @brief Vide le fichier de données.
  *
  * @param[in,out] psWidget Widget appelant ce callback
  * @param[in,out] pData Données tierces
  * @return EXIT_SUCCESS si tout est OK
  */
int on_FlushData_Action_activate (GtkWidget* psWidget, gpointer* pData);


/* ********************************************************************* */
/*                           FENÊTRE STELLARIUM                          */
/* ********************************************************************* */

/**
  * @fn int on_Stellarium_DrawingArea_realize(
								GtkWidget* psWidget,
								gpointer* pData)
  * @brief Fonction appelée à la réalisation du widget. Entraine
  l'initialisation d'OpenGL.
  *
  * @param[in,out] psWidget Widget appelant ce callback
  * @param[in,out] pData Données tierces
  * @return EXIT_SUCCESS si tout est OK
  */
int on_Stellarium_DrawingArea_realize(
								GtkWidget* psWidget,
								gpointer* pData);

/**
  * @fn int on_Stellarium_DrawingArea_configure_event(
								GtkWidget* psWidget,
								GdkEventConfigure* psEvent,
								gpointer* pData)
  * @brief Fonction appelée au redimensionnement du widget.
  Entraine le redimenssionnement de la scene OpenGL.
  *
  * @param[in,out] psWidget Widget appelant ce callback
  * @param[in,out] psEvent Structure de l'évenement
  * @param[in,out] pData Données tierces
  * @return EXIT_SUCCESS si tout est OK
  */
int on_Stellarium_DrawingArea_configure_event(
								GtkWidget* psWidget,
								GdkEventConfigure* psEvent,
								gpointer* pData);

/**
  * @fn int on_Stellarium_DrawingArea_expose_event (
							GtkWidget* psWidget,
							GdkEventExpose* psEvent,
							gpointer* pData)
  * @brief Fonction appelée à l'affichage du widget.
  Entraine le rafraichissement de la scene OpenGL.
  *
  * @param[in,out] psWidget Widget appelant ce callback
  * @param[in,out] psEvent Structure de l'évenement
  * @param[in,out] pData Données tierces
  * @return EXIT_SUCCESS si tout est OK
  */
int on_Stellarium_DrawingArea_expose_event (
							GtkWidget* psWidget,
							GdkEventExpose* psEvent,
							gpointer* pData);

/**
  * @fn int on_Stellarium_DrawingArea_key_press_event(
							GtkWidget * psWidget,
							GdkEventKey * psEvent,
							gpointer * pData)
  * @brief Fonction appelée lorsque qu'une touche du clavier est appuyée
  (Widget ayant le focus).
  *
  * @param[in,out] psWidget Widget appelant ce callback
  * @param[in,out] psEvent Structure de l'évenement
  * @param[in,out] pData Données tierces
  * @return EXIT_SUCCESS si tout est OK
  */
int on_Stellarium_DrawingArea_key_press_event(
							GtkWidget * psWidget,
							GdkEventKey * psEvent,
							gpointer * pData);

/**
  * @fn int on_Stellarium_DrawingArea_button_release_event (
								GtkWidget * psWidget,
								GdkEventAny * psEvent,
								gpointer * pData)
  * @brief Fonction appelée lorsqu'un bouton de souris est relaché.
  *
  * @param[in,out] psWidget Widget appelant ce callback
  * @param[in,out] psEvent Structure de l'évenement
  * @param[in,out] pData Données tierces
  * @return EXIT_SUCCESS si tout est OK
  */
int on_Stellarium_DrawingArea_button_release_event (
								GtkWidget * psWidget,
								GdkEventAny * psEvent,
								gpointer * pData);

int on_Stellarium_DrawingArea_motion_notify_event (GtkWidget* psWidget,
												GdkEventMotion* psEvent,
												gpointer* pData);


/* ********************************************************************* */
/* ********************************************************************* */


#endif /* __GUI_H__ */