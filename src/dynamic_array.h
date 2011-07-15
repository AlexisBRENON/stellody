/**
  * @brief Fichier d'entête du module @em DYNAMIC_ARRAY.
  * Module de tableau dynamique.
  *
  * @author Alexis BRENON in STELLODY TEAM
  * @file dynamic_array.h
  */

#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__


/* ********************************************************************* */
/*                                                                       */
/*                        Définitions de types                           */
/*                                                                       */
/* ********************************************************************* */


/**
  * @enum eArrayError
  * @brief 	Liste des erreurs possibles sur les tableaux dynamiques.
  Codes d'erreur tableau.
  */
enum eArrayError
{
	ARRAY_OK, /**< Aucune erreur */
	ARRAY_NULL_POINTER, /**< L'adresse du tableau est nulle */
	ARRAY_BAD_ARGUMENTS, /**< Les arguments ne sont pas bons */
	ARRAY_INDEX_OVERFLOW /**< L'index dépasse la taille du tableau */
};
/**
  * @typedef ArrayError
  * @brief Redéfinition de type.
  */
typedef enum eArrayError ArrayError;

/**
  * @struct sDynamicArray
  * @brief Structure représentant un tableau dynamique.
  */
struct sDynamicArray
{
	void** ppFisrtCell; /**< Adresse de la première case du tableau */

	int iAllocatedSize; /**< Taille maximale du tableau */
	int iFirstEmptyCell; /**< Index de la case de fin du tableau (vide) */
};
/**
  * @typedef DynamicArray
  * @brief Redéfinition de type.
  */
typedef struct sDynamicArray DynamicArray;


/* ********************************************************************* */
/*                                                                       */
/*                 Fonctions relatives à la structure                    */
/*                                                                       */
/* ********************************************************************* */

/**
  * @fn DynamicArray* dynamicArrayCreate(void)
  * @brief Fonction de création et initialisation du tableau (10 cases).
  *
  * @return L'adresse (dans le tas) de la structure du tableau.
  */
DynamicArray* dynamicArrayCreate(void);
/**
  * @fn ArrayError dynamicArrayDestroy (DynamicArray** ppArray)
  * @brief Fonction de destruction du tableau.
  *
  * @param[in,out] ppArray Pointeur sur pointeur sur le tableau
  * @return Code d'erreur tableau
  */
ArrayError dynamicArrayDestroy (DynamicArray** ppArray);

/**
  * @fn ArrayError dynamicArrayInit (DynamicArray* pArray)
  * @brief Initialisation d'un tableau dynamique.
  *
  * @param[in,out] pArray Pointeur sur le tableau dynamique
  * @return Code d'erreur tableau
  */
ArrayError dynamicArrayInit (DynamicArray* pArray);
/**
  * @fn ArrayError dynamicArrayRelease (DynamicArray* pArray)
  * @brief Libération du tableau dynamique.
  *
  * @param[in,out] pArray Pointeur sur le tableau dynamique
  * @return Code d'erreur tableau
  */
ArrayError dynamicArrayRelease (DynamicArray* pArray);

/**
  * @fn ArrayError dynamicArraySet (DynamicArray* pArray,
							int iIndex,
							void* pData)
  * @brief Remplace le pointeur contenu dans la case d'index iIndex par
  pData.
  *
  * @param[in,out] pArray Pointeur sur le tableau
  * @param[in] iIndex Index de la case à modifier (>= 0)
  * @param[in] pData Nouvelle valeur à stocker.
  * @return Code d'erreur tableau
  */
ArrayError dynamicArraySet (DynamicArray* pArray,
							int iIndex,
							void* pData);

/**
  * @fn ArrayError dynamicArrayGet (const DynamicArray* pArray,
							int iIndex,
							void** pData)
  * @brief Cette fonction permet de récupérer l'adresse contenue dans la
  case d'index iIndex dans la variable pData.
  *
  * @param[in] pArray Pointeur sur le tableau
  * @param[in] iIndex Index de la case à lire
  * @param[out] pData Pointeur où stocker l'adresse
  * @return Code d'erreur tableau.
  */
ArrayError dynamicArrayGet (const DynamicArray* pArray,
							int iIndex,
							void** pData);

/**
  * @fn ArrayError dynamicArrayPush (DynamicArray* pArray,
							void* pData)
  * @brief Ajoute une case au tableau (en fin).
  *
  * @param[in,out] pArray Adresse du tableau
  * @param[in] pData Valeur à stocker (peut être NULL)
  * @return Code d'erreur tableau
  */
ArrayError dynamicArrayPush (DynamicArray* pArray,
							void* pData);
/**
  * @fn ArrayError dynamicArrayPop (DynamicArray* pArray)
  * @brief Supprime la dernière case du tableau.
  *
  * @param[in,out] pArray Pointeur sur le tableau
  * @return Code d'erreur tableau
  */
ArrayError dynamicArrayPop (DynamicArray* pArray);

/**
  * @fn ArrayError dynamicArrayGetSize (const DynamicArray* pArray,
								int* piSize)
  * @brief Obtenir la taille actuelle du tableau.
  *
  * @param[in] pArray Adresse du tableau
  * @param[out] piSize Adresse où stocker le résultat
  * @return Code d'erreur tableau
  */
ArrayError dynamicArrayGetSize (const DynamicArray* pArray,
								int* piSize);
/**
  * @fn ArrayError dynamicArrayGetMaxSize (const DynamicArray* pArray,
								int* piMaxSize)
  * @brief Obtenir la taille maximale instantannée du tableau.
  *
  * @param[in] pArray Adresse du tableau
  * @param[out] piMaxSize Adresse où stocker le résultat
  * @return Code d'erreur tableau
  */
ArrayError dynamicArrayGetMaxSize (const DynamicArray* pArray,
								int* piMaxSize);


/* ********************************************************************* */
/*                                                                       */
/*                          Test de regression                           */
/*                                                                       */
/* ********************************************************************* */


/**
  * @fn int dynamicArrayRegressionTest ()
  * @brief Test de regression du tableau.
  *
  * @return EXIT_SUCCESS
  */
int dynamicArrayRegressionTest ();


#endif /* __DYNAMIC_ARRAY_H__ */
