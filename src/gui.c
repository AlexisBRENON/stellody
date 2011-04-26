/**
  * @file gui.c
  * @brief Fichier d'implémentation du module \em GUI.
  *
  * @author Alexis BRENON in STELLODY TEAM
  */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <gtk/gtk.h>

#include "gui.h"
#include "preferences.h"


/* ********************************************************************* */
/*                                                                       */
/*                       Définitions de constantes                       */
/*                                                                       */
/* ********************************************************************* */

#define GUI_WIN "data/windows/Window.glade" /**< Nom du fichier GUI
principal. */
#define GUI_STELLARIUM "data/windows/Stellarium.glade" /**< Nom du fichier
GUI contenant la base pour le Stellarium. */
#define GUI_PREFERENCES "data/windows/Preferences.glade" /**< Nom du
fichier GUI contenant la gestion des préférences. */
#define GUI_ABOUT "data/windows/About.glade" /**< Nom du fichier GUI
contenant le A Propos. */

#define GUI_MAIN_WIN "Stellody_Window" /**< Nom du widget de niveau
supérieur. */
#define GUI_MAIN_CONTAINER "Display_HBox" /**< Nom du widget contenant les
différentes fenêtres (variables). */
#define GUI_STELLARIUM_WIN "Stellarium_VBox" /**< Nom du conteneur du
stellarium. */
#define GUI_PREFERENCES_WIN "Preferences_VBox" /**< Nom du conteneur des
preferences. */
#define GUI_ABOUT_WIN "About_VBox" /**< Nom du conteneur de A Propos; */


/* ********************************************************************* */
/*                                                                       */
/*                           Fonctions privées                           */
/*                                                                       */
/* ********************************************************************* */


/**
  * @fn static int guiUnparent (gpointer* pData)
  * @brief Déparente toutes les fenêtres variables.
  *
  * @param[in,out] pData Le tableau de données général
  * @return EXIT_SUCCESS si tout est OK
  */
static int guiUnparent (gpointer* pData)
{
	GtkWidget* psStellarium = NULL;
	GtkWidget* psPreference = NULL;
	GtkWidget* psAbout = NULL;
	GtkWidget* psContainer = NULL;

	psContainer = GTK_WIDGET(gtk_builder_get_object(pData[MAIN_BUILDER],
										GUI_MAIN_CONTAINER));

	/* Récupération des potentiels widgets fils et obturation du widget */
	if (pData[PREFERENCES_BUILDER] != NULL)
	{
		psPreference = GTK_WIDGET(gtk_builder_get_object(
										pData[PREFERENCES_BUILDER],
										GUI_PREFERENCES_WIN));
		gtk_widget_hide_all(psPreference);

		if (gtk_widget_get_parent(psPreference) != NULL)
		{
			gtk_container_remove ((GtkContainer*)psContainer, psPreference);
		}
	}

	if (pData[ABOUT_BUILDER] != NULL)
	{
		psAbout = GTK_WIDGET(gtk_builder_get_object(pData[ABOUT_BUILDER],
												GUI_ABOUT_WIN));
		gtk_widget_hide_all(psAbout);

		if (gtk_widget_get_parent(psAbout) != NULL)
		{
			gtk_container_remove ((GtkContainer*)psContainer, psAbout);
		}
	}

	if (pData[STELLARIUM_BUILDER] != NULL)
	{
		psStellarium = GTK_WIDGET(gtk_builder_get_object(
											pData[STELLARIUM_BUILDER],
											GUI_STELLARIUM_WIN));
		gtk_widget_hide_all(psStellarium);

		if (gtk_widget_get_parent(psStellarium) != NULL)
		{
			gtk_container_remove ((GtkContainer*)psContainer, psStellarium);
		}
	}

	return EXIT_SUCCESS;
}

/* ********************************************************************* */
/*                                                                       */
/*                   Fonctions relatives à la fenêtre                    */
/*                                                                       */
/* ********************************************************************* */

/* ********************************************************************* */
/*                           FENÊTRE PRINCIPALE                          */
/* ********************************************************************* */

GtkBuilder* guiLoad (const gpointer pData)
{
	GtkBuilder* psBuilder = NULL;
	GtkWidget* psWin = NULL;

	psBuilder = gtk_builder_new();
	gtk_builder_add_from_file(psBuilder, GUI_WIN, NULL);
	gtk_builder_connect_signals(psBuilder, pData);
	psWin = GTK_WIDGET(gtk_builder_get_object(psBuilder,
											GUI_MAIN_WIN));
	gtk_widget_show_all(psWin);

	return psBuilder;
}

int on_Quit_Action_activate(GtkWidget* psWidget, gpointer* pData)
{
	gtk_widget_destroy(GTK_WIDGET(
							gtk_builder_get_object(pData[MAIN_BUILDER],
													GUI_MAIN_WIN)));

	if (pData[PREFERENCES_BUILDER] != NULL)
	{
		gtk_widget_destroy(GTK_WIDGET(
					gtk_builder_get_object(pData[PREFERENCES_BUILDER],
											GUI_PREFERENCES_WIN)));
	}

	if (pData[ABOUT_BUILDER] != NULL)
	{
		gtk_widget_destroy(GTK_WIDGET(
					gtk_builder_get_object(pData[ABOUT_BUILDER],
											GUI_ABOUT_WIN)));
	}

	if (pData[STELLARIUM_BUILDER] != NULL)
	{
		gtk_widget_destroy(GTK_WIDGET(
					gtk_builder_get_object(pData[STELLARIUM_BUILDER],
											GUI_STELLARIUM_WIN)));
	}

	gtk_main_quit();

	return EXIT_SUCCESS;
}


int on_Play_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	printf ("Lecture\n");

	return EXIT_SUCCESS;
}

int on_Stop_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	printf ("Stop\n");

	return EXIT_SUCCESS;
}

int on_Pause_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	printf ("Pause\n");

	return EXIT_SUCCESS;
}

int on_AddTrack_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	printf ("Ajout morceau\n");

	return EXIT_SUCCESS;
}

int on_AddDir_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	printf ("Ajout répertoire\n");

	return EXIT_SUCCESS;
}

int on_Stellarium_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	GtkBuilder* psBuilder = NULL;
	GtkWidget* psStellarium = NULL;
	GtkWidget* psContainer = NULL;

	guiUnparent(pData);

	if (pData[STELLARIUM_BUILDER] == NULL)
	{
		psBuilder = gtk_builder_new();
		gtk_builder_add_from_file(psBuilder, GUI_STELLARIUM, NULL);

		psStellarium = GTK_WIDGET(gtk_builder_get_object(psBuilder,
													GUI_STELLARIUM_WIN));
		psContainer = GTK_WIDGET(
							gtk_builder_get_object(pData[MAIN_BUILDER],
												GUI_MAIN_CONTAINER));
		gtk_box_pack_start((GtkBox*)psContainer,
							psStellarium, TRUE, TRUE, 0);
		gtk_widget_show_all(psStellarium);

		pData[STELLARIUM_BUILDER] = psBuilder;
	}
	else
	{
		psStellarium = GTK_WIDGET(gtk_builder_get_object(
												pData[STELLARIUM_BUILDER],
												GUI_STELLARIUM_WIN));
		psContainer = GTK_WIDGET(
						gtk_builder_get_object(pData[MAIN_BUILDER],
											GUI_MAIN_CONTAINER));
		gtk_box_pack_start((GtkBox*)psContainer,
								psStellarium, TRUE, TRUE, 5);
		gtk_widget_show_all(psStellarium);
	}

	return EXIT_SUCCESS;
}

int on_Preferences_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	GtkBuilder* psBuilder = NULL;
	GtkWidget* psPrefWin = NULL;
	GtkWidget* psContainer = NULL;
	GtkWidget* psScale = NULL;

	guiUnparent(pData);

	/* Si la fenêtre n'a jamais été créée. */
	if (pData[PREFERENCES_BUILDER] == NULL)
	{
		psBuilder = gtk_builder_new();
		gtk_builder_add_from_file(psBuilder, GUI_PREFERENCES, NULL);
		gtk_builder_connect_signals(psBuilder, pData);

		pData[PREFERENCES_BUILDER] = psBuilder;
	}

	psPrefWin = GTK_WIDGET(gtk_builder_get_object(
												pData[PREFERENCES_BUILDER],
												GUI_PREFERENCES_WIN));
	psScale = GTK_WIDGET(gtk_builder_get_object(
										pData[PREFERENCES_BUILDER],
										"AnalysisRate_Scale"));
	preferencesGetAnalysisRate(pData[PREFERENCES]);
	gtk_range_set_value((GtkRange*)psScale,
					(double) preferencesGetAnalysisRate(
												pData[PREFERENCES]));


	psContainer = GTK_WIDGET(
						gtk_builder_get_object(pData[MAIN_BUILDER],
											GUI_MAIN_CONTAINER));
	gtk_box_pack_start((GtkBox*)psContainer,
							psPrefWin, TRUE, TRUE, 0);

	gtk_widget_show_all(psPrefWin);

	return EXIT_SUCCESS;
}

int on_About_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	printf ("About\n");

	return EXIT_SUCCESS;
}


/* ********************************************************************* */
/*                           FENÊTRE PREFERENCES                         */
/* ********************************************************************* */

int on_PrefOKBut_Action_activate (GtkWidget* psWidget, gpointer* pData)
{
	GtkWidget* psScale = NULL;

	psScale = GTK_WIDGET(gtk_builder_get_object(
										pData[PREFERENCES_BUILDER],
										"AnalysisRate_Scale"));

	preferencesSetAnalysisRate(pData[PREFERENCES],
						(int) gtk_range_get_value((GtkRange*)psScale));

	on_Stellarium_Action_activate(psWidget, pData);

	return EXIT_SUCCESS;
}






























