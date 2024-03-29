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
/*                   Fonctions relatives à la fenêtre                    */
/*                                                                       */
/* ********************************************************************* */

/**
  * @fn int guiLoad (void** ppDatas)
  * @brief Crée et initialise chaque structure de données.
  *
  * @param[in,out] ppDatas Pointeur sur un tableau qui sera envoyée à tous les
  callbacks
  * @return EXIT_SUCCESS
  */
int guiLoad (void** ppDatas);


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
  * @fn int on_Library_Action_activate (GtkWidget* psWidget,
										gpointer* pData);
  * @brief Affiche la librairie.
  *
  * @param[in,out] psWidget Pointeur sur le widget appellant ce callback
  * @param[in,out] pData Données autres passées au callback.
  * @return EXIT_SUCCESS si tout est OK.
  */
int on_Library_Action_activate (GtkWidget* psWidget, gpointer* pData);

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
  * @fn int on_Previous_Action_activate (GtkWidget* psWidget,
										gpointer* pData)
  * @brief Passe au morceau précédent.
  *
  * @param[in,out] psWidget Pointeur sur le widget appellant ce callback
  * @param[in,out] pData Données autres passées au callback.
  * @return EXIT_SUCCESS si tout est OK.
  */
int on_Previous_Action_activate (GtkWidget* psWidget, gpointer* pData);


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

/**
 * @fn int on_PlayList_TreeView_row_activated (GtkTreeView* psTreeView,
										GtkTreePath* psSelectedTrackPath,
										GtkTreeViewColumn* psColumn,
										gpointer* pData)
 * @brief Permet de sélectionner un morceau dans la playlist.
 *
 * @param[in,out] psTreeView Pointeur sur la vue cliquée
 * @param[in,out] psSelectedTrackPath Chemin vers le morceau cliqué
 * @param[in,out] psColumn Colonne cliquée
 * @param[in,out] pData Données autres passées au callback.
 * @return EXIT_SUCCESS si tout est OK.
 */
int on_PlayList_TreeView_row_activated (GtkTreeView* psTreeView,
										GtkTreePath* psSelectedTrackPath,
										GtkTreeViewColumn* psColumn,
										gpointer* pData);

/**
  * @fn int on_PlayList_TreeView_key_release_event (GtkWidget* psWidget,
											GdkEvent* psEvent,
											gpointer* pData);
  * @brief Permet la suppression d'un morceau de la playlist.
  *
  * @param[in,out] psWidget Widget appellant
  * @param[in,out] psEvent Données de l'évenement
  * @param[in,out] pData Données autres
  * @return EXIT_SUCCESS
  */
int on_PlayList_TreeView_key_release_event (GtkWidget* psWidget,
											GdkEvent* psEvent,
											gpointer* pData);


/* ********************************************************************* */
/*                            FONCTIONS TIMEOUT                          */
/* ********************************************************************* */

/**
  * @fn gboolean guiTimeoutAnalyze (gpointer* pData)
  * @brief Fonction mettant à jour l'affichage et analysant un morceau.
  *
  * @param[in,out] pData Tableau de données du programme
  * @return TRUE si le canal d'analyse joue toujours, FALSE sinon.
  */
gboolean guiTimeoutAnalyze (gpointer* pData);

/**
  * @fn gboolean guiTimeoutCheckForAnalyze (gpointer* pData)
  * @brief Vérifie si un morceau doit être analysé ou pas.
  *
  * @param[in,out] pData Tableau de données du programme
  * @return TRUE s'il reste un morceau à analyser, FALSE sinon.
  */
gboolean guiTimeoutCheckForAnalyze (gpointer* pData);




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
  * @param[in,out] psEvent Evenement d'affichage
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

/**
  * @fn int on_Stellarium_DrawingArea_motion_notify_event (
								GtkWidget * psWidget,
								GdkEventMotion * psEvent,
								gpointer * pData)
  * @brief Fonction appelée lorsque la souris est déplacée.
  *
  * @param[in,out] psWidget Widget appelant ce callback
  * @param[in,out] psEvent Structure de l'évenement
  * @param[in,out] pData Données tierces
  * @return EXIT_SUCCESS si tout est OK
  */
int on_Stellarium_DrawingArea_motion_notify_event (GtkWidget* psWidget,
												GdkEventMotion* psEvent,
												gpointer* pData);


/* ********************************************************************* */
/*                           FENETRE LIBRAIRIE                           */
/* ********************************************************************* */


/**
  * @fn int on_Library_TreeView_row_activated (GtkTreeView* psTreeView,
										GtkTreePath* psSelectedTrackPath,
										GtkTreeViewColumn* psColumn,
										gpointer* pData);
  * @brief Ajoute un morceau à la playlist depuis la liste de la
  bibliothèque.
  *
  * @param[in,out] psTreeView Pointeur sur la vue cliquée
  * @param[in,out] psSelectedTrackPath Chemin vers le morceau cliqué
  * @param[in,out] psColumn Colonne cliquée
  * @param[in,out] pData Données autres passées au callback.
  * @return EXIT_SUCCESS si tout est OK.
  */
int on_Library_TreeView_row_activated (GtkTreeView* psTreeView,
										GtkTreePath* psSelectedTrackPath,
										GtkTreeViewColumn* psColumn,
										gpointer* pData);

/**
  * @fn int on_Library_Columns_clicked (GtkTreeViewColumn* psColumn,
								gpointer* pData);
  * @brief Change l'ordre de tri de la librarie.
  *
  * @param[in,out] psColumn Colonne cliquée.
  * @param[in,out] pData Données autres passées au callback.
  * @return EXIT_SUCCESS si tout est OK.
  */
int on_Library_Columns_clicked (GtkTreeViewColumn* psColumn,
								gpointer* pData);

/**
  * @fn int on_Library_TreeView_key_release_event (GtkWidget* psWidget,
											GdkEvent* psEvent,
											gpointer* pData);
  * @brief Supprime un morceau de la bibliothèque.
  *
  * @param[in,out] psWidget Widget appellant
  * @param[in,out] psEvent Données de l'évenement
  * @param[in,out] pData Données autres
  * @return EXIT_SUCCESS
  */
int on_Library_TreeView_key_release_event (GtkWidget* psWidget,
											GdkEvent* psEvent,
											gpointer* pData);

/* ********************************************************************* */
/* ********************************************************************* */

void test (GtkWidget 			*widget,
			GdkDragContext   	*drag_context,
			gint              	x,
			gint              	y,
			GtkSelectionData 	*data,
			guint             	info,
			guint             	time,
			gpointer          	user_data);


gboolean test2 (GtkWidget *widget,
			GdkDragContext *context,
			gint x, gint y,
			guint time,
			gpointer user_data);



#endif /* __GUI_H__ */
