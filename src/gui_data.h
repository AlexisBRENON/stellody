/**
  * @brief Fichier d'entête du module @em GUI_DATA
  *
  *
  * @author Alexis BRENON in STELLODY TEAM
  * @file gui_data.h
  */

#ifndef __GUI_DATA_H__
#define __GUI_DATA_H__

#include <stdlib.h>
#include <gtk/gtk.h>



/* ********************************************************************* */
/*                                                                       */
/*                        Définitions de types                           */
/*                                                                       */
/* ********************************************************************* */

/**
  * @enum eBuilderName
  * @brief Permet d'identifier les builders de la structure
  */
enum eBuilderName
{
	MAIN, /**< Identifie le builder principal. */
	STELLARIUM, /**< Identifie le builder du Stellarium */
	PREFERENCES, /**< Identifie le builder des Préférences */
	ABOUT /**< Identifie le builder de la fenêtre APropos */
};
/**
  * @typedef BuilderName
  * @brief Redéfinition de l'énumération.
  */
typedef enum eBuilderName BuilderName;


/**
  * @struct sGuiData
  * @brief Structure contenant les données relatives à l'interface.
  */
struct sGuiData
{
	GtkBuilder* pMainBuilder; /**< Builder de l'interface commune */
	GtkBuilder* pStellariumBuilder; /**< Builder du Stellarium */
	GtkBuilder* pPreferencesBuilder; /**< Builder de la gestion de préférences */
	GtkBuilder* pAboutBuilder; /**< Builder du @em A Propos */

	int iIncrementTimerID; /**< ID du timer d'incrémentation */

	int iXMousePosition; /**< Coordonnée en X de la souris sur le Stéllarium */
	int iYMousePosition; /**< Coordonnée en Y de la souris sur le Stéllarium */
};
/**
  * @typedef GuiData
  * @brief Redéfinition de la structure.
  */
typedef struct sGuiData GuiData;



/* ********************************************************************* */
/*                                                                       */
/*          Fonctions relatives à la structure de données                */
/*                                                                       */
/* ********************************************************************* */

/**
  * @fn int guiDataInit (GuiData* pData)
  * @brief Initialisation de la structure de donnée.
  *
  * @param[in,out] pData Pointeur sur la structure à initialiser.
  * @return EXIT_SUCCESS si tout est OK.
  */
int guiDataInit (GuiData* pData);
/**
  * @fn int guiDataInitWithData (GuiData* pData,
						const char* strMainBuilder,
						const char* strStellariumBuilder,
						const char* strPreferencesBuilder,
						const char* strAboutBuilder)
  * @brief Initialisation de la structure en chargeant les fichiers
  d'interface.
  *
  * @param[in,out] pData Pointeur sur la structure à initialiser.
  * @param[in] strMainBuilder Chemin vers le fichier d'interface générale.
  * @param[in] strStellariumBuilder Chemin vers l'interface du Stellarium.
  * @param[in] strPreferencesBuilder Chemin vers l'interface Preferences.
  * @param[in] strAboutBuilder Chemin vers l'interface du APropos.
  * @return EXIT_SUCCESS si tout est OK.
  */
int guiDataInitWithData (GuiData* pData,
						const char* strMainBuilder,
						const char* strStellariumBuilder,
						const char* strPreferencesBuilder,
						const char* strAboutBuilder);
/**
  * @fn int guiDataRelease (GuiData* pData)
  * @brief Libère la mémoire allouée par la structure de donnée.
  *
  * @param[in,out] pData Structure à libérer
  * @return EXIT_SUCCESS
  */
int guiDataRelease (GuiData* pData);

/**
  * @fn GuiData* guiDataCreate (void)
  * @brief Alloue et initialise une structure de donnée.
  *
  * @return Une structure nouvellement allouée
  */
GuiData* guiDataCreate (void);
/**
  * @fn GuiData* guiDataCreateWithData (const char* strMainBuilder,
								const char* strStellariumBuilder,
								const char* strPreferencesBuilder,
								const char* strAboutBuilder)
  * @brief Alloue et initialise une structure de donnée en chargeant les
  fichiers d'interface.
  *
  * @param[in] strMainBuilder Chemin vers le fichier d'interface générale.
  * @param[in] strStellariumBuilder Chemin vers l'interface du Stellarium.
  * @param[in] strPreferencesBuilder Chemin vers l'interface Preferences.
  * @param[in] strAboutBuilder Chemin vers l'interface du APropos.
  * @return Une structure nouvellement allouée.
  */
GuiData* guiDataCreateWithData (const char* strMainBuilder,
								const char* strStellariumBuilder,
								const char* strPreferencesBuilder,
								const char* strAboutBuilder);
/**
  * @fn int guiDataDestroy (GuiData** ppData)
  * @brief Libère l'espace alloué par un morceau et pour le morceau.
  *
  * @param[in,out] ppData Pointeur sur pointeur sur la structure
  * @return EXIT_SUCCESS
  */
int guiDataDestroy (GuiData** ppData);



/**
  * @fn int guiDataSetBuilder(GuiData* pData,
					const BuilderName iBuilderName,
					GtkBuilder* pBuilder);
  * @brief Remplace le GtkBuilder identifié par iBuilderName par pBuilder.
  *
  * @param[in,out] pData Structure à modifier
  * @param[in] iBuilderName Identifiant du builder
  * @param[in] pBuilder Nouveau GtkBuilder
  * @return EXIT_SUCCESS
  */
int guiDataSetBuilder(GuiData* pData,
					const BuilderName iBuilderName,
					GtkBuilder* pBuilder);
/**
  * @fn int guiDataSetBuilderByName(GuiData* pData,
							const BuilderName iBuilderName,
							const char* strBuilder)
  * @brief Charge le fichier pointé par strBuilder dans le GtkBuilder
  identifié par iBuilderName.
  *
  * @param[in,out] pData Structure à modifier
  * @param[in] iBuilderName Identifiant du builder
  * @param[in] strBuilder Chemin vers le fichier d'interface
  * @return EXIT_SUCCESS
  */
int guiDataSetBuilderByName(GuiData* pData,
							const BuilderName iBuilderName,
							const char* strBuilder);
/**
  * @fn GtkBuilder* guiDataGetBuilder(const GuiData* pData,
							const BuilderName iBuilderName)
  * @brief Accesseur sur un GtkBuilder de la structure.
  *
  * @param[in] pData Strucutre à accéder
  * @param[in] iBuilderName Identifiant du builder à obtenir
  * @return Le GtkBuilder correspondant à iBuilderName
  */
GtkBuilder* guiDataGetBuilder(const GuiData* pData,
							const BuilderName iBuilderName);

/**
  * @fn int guiDataSetIncrementTimerID (GuiData* pData, int iID)
  * @brief Mutateur sur l'ID du timer d'incrément.
  *
  * @param[in,out] pData Structure à modifier
  * @param[in] iID Nouvel identifiant
  * @return EXIT_SUCCESS
  */
int guiDataSetIncrementTimerID (GuiData* pData, int iID);
/**
  * @fn int guiDataGetIncrementTimerID (const GuiData* pData)
  * @brief Accesseur sur l'ID du timer d'incrément
  *
  * @param[in] pData Structure à accéder
  * @return l'ID du timer (>0)
  */
int guiDataGetIncrementTimerID (const GuiData* pData);

/**
  * @fn int guiDataSetMousePosition (GuiData* pData,
							int iX, int iY)
  * @brief Mutateur sur les coordonnées de la souris (dans le Stellarium).
  Des valeurs négatives passées à iX ou iY ne modifieront pas la coordonnée.
  *
  * @param[in,out] pData Structure à modifier
  * @param[in] iX Nouvelle coordonnée en X
  * @param[in] iY Nouvelle coordonnée en Y
  * @return EXIT_SUCCESS
  */
int guiDataSetMousePosition (GuiData* pData,
							int iX, int iY);
/**
  * @fn int guiDataGetMousePosition (const GuiData* pData,
							int* iX, int* iY)
  * @brief Accesseur sur les coordonnées de la souris (dans le Stellarium).
  Les valeurs seront stockées aux adresses iX et iY. Ces adresses peuvent
  être NULL si l'utilisateur ne veut récupérer qu'une valeur.
  *
  * @param[in] pData Structure à accéder
  * @param[out] iX Adresse où stocker la coordonnée en X, ou NULL
  * @param[out] iY Adresse où stocker la coordonnée en Y, ou NULL
  * @return EXIT_SUCCESS
  */
int guiDataGetMousePosition (const GuiData* pData,
							int* iX, int* iY);



#endif /* __GUI_DATA_H__ */
