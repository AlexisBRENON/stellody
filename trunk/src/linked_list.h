/**
  * @brief Fichier d'entête du module @em LINKED_LIST.
  *
  *
  * @author Alexis BRENON in STELLODY TEAM
  * @file linked_list.h
  */

#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

/* ********************************************************************* */
/*                                                                       */
/*                        Définitions de types                           */
/*                                                                       */
/* ********************************************************************* */

/**
  * @enum eListError
  * @brief Liste des erreurs possibles sur les listes. Codes d'erreur liste.
  */
enum eListError
{
	LIST_OK, /**< Aucune erreur */
	LIST_INDEX_OVERFLOW, /**< L'index dépasse la longueur de la liste */
	LIST_NULL_ADRESS, /**< Le pointeur est NULL */
	LIST_BAD_ARGUMENTS /**< Les arguments ne sont pas bons (pas d'adresse de stockage) */
};
/**
  * @typedef ListError
  * @brief Redéfinition de type.
  */
typedef enum eListError ListError;

/**
  * @struct sLinkedList
  * @brief Structure permettant la représentation d'une liste doublement chainée.
  */
struct sLinkedList
{
	void* pData; /**< Donnée contenue dans la cellule */
	struct sLinkedList* pNext; /**< Pointeur sur la cellule suivante */
	struct sLinkedList* pPrevious; /**< Pointeur sur la cellule précedente */
};
/**
  * @typedef LinkedList
  * @brief Redéfinition de type.
  */
typedef struct sLinkedList LinkedList;


/* ********************************************************************* */
/*                                                                       */
/*                 Fonctions relatives à la structure                    */
/*                                                                       */
/* ********************************************************************* */

/**
  * @fn LinkedList* linkedListCreate (void)
  * @brief Crée une nouvelle liste.
  *
  * @return La nouvelle liste
  */
LinkedList* linkedListCreate (void);
/**
  * @fn ListError linkedListDestroy (LinkedList** ppList)
  * @brief Fonction de destruction d'une liste.
  *
  * @param[in,out] ppList Pointeur sur pointeur sur la liste
  * @return Code d'erreur de liste
  */
ListError linkedListDestroy (LinkedList** ppList);

/**
  * @fn ListError linkedListInit (LinkedList* pList)
  * @brief Initialise une liste déjà allouée.
  *
  * @param[in,out] pList La liste allouée
  * @return Code d'erreur de liste
  */
ListError linkedListInit (LinkedList* pList);
/**
  * @fn ListError linkedListRelease (LinkedList* pList)
  * @brief Libère une liste chainée.
  *
  * @param[in,out] pList Pointeur sur la liste
  * @return Code d'erreur liste
  */
ListError linkedListRelease (LinkedList* pList);

/**
  * @fn ListError linkedListPrepend (LinkedList* pList,
							void* pData);
  * @brief Ajoute un élément en tête de liste.
  *
  * @param[in,out] pList Pointeur sur la liste
  * @param[in] pData Pointeur recopié dans la cellule
  * @return Code d'erreur de liste
  */
ListError linkedListPrepend (LinkedList* pList,
					void* pData);

/**
  * @fn ListError linkedListAppend (LinkedList* pList,
							void* pData);
  * @brief Ajoute un élément en queue de liste.
  *
  * @param[in,out] pList Pointeur sur la liste
  * @param[in] pData Pointeur recopié dans la cellule
  * @return Code d'erreur de liste
  */
ListError linkedListAppend (LinkedList* pList,
					void* pData);

/**
  * @fn ListError linkedListInsert (LinkedList* pList,
							int iIndex,
							void* pData);
  * @brief Ajoute un élément à la (i+1)eme place.
  *
  * @param[in,out] pList Pointeur sur la liste
  * @param[in] iIndex Index où placer la donnée (0 = premiere place)
  * @param[in] pData Pointeur recopié dans la cellule
  * @return Code d'erreur de liste
  */
ListError linkedListInsert (LinkedList* pList,
					int iIndex,
					void* pData);

/**
  * @fn ListError linkedListRemoveFirst (LinkedList* pList,
								void** pData)
  * @brief Enlève le premier élément de la liste.
  *
  * @param[in,out] pList Pointeur sur la liste
  * @param[out] pData Donnée de l'utilisateur (qu'il doit libérer) ou NULL
  * @return Code d'erreur de liste
  */
ListError linkedListRemoveFirst (LinkedList* pList,
								void** pData);

/**
  * @fn ListError linkedListRemoveLast (LinkedList* pList,
								void** pData)
  * @brief Enlève le dernier élément de la liste.
  *
  * @param[in,out] pList Pointeur sur la liste
  * @param[out] pData Donnée de l'utilisateur (qu'il doit libérer) ou NULL
  * @return Code d'erreur de liste
  */
ListError linkedListRemoveLast (LinkedList* pList,
								void** pData);

/**
  * @fn ListError linkedListRemoveIeme (LinkedList* pList,
								int iIndex,
								void** pData);
  * @brief Enlève le (i+1)eme élément de la liste.
  *
  * @param[in,out] pList Pointeur sur la liste
  * @param[in] iIndex Index de la cellule à supprimer (0 = premier)
  * @param[out] pData Donnée de l'utilisateur (qu'il doit libérer) ou NULL
  * @return Code d'erreur de liste
  */
ListError linkedListRemoveIeme (LinkedList* pList,
								int iIndex,
								void** pData);

/**
  * @fn ListError linkedListRemoveByData (LinkedList* pList,
										const void* pData)
  * @brief Enlève la cellule contenant la donnée pData.
  *
  * @param[in,out] pList Pointeur sur la liste
  * @param[in] pData Pointeur à comparer
  * @return Code d'erreur de liste
  */
ListError linkedListRemoveByData (LinkedList* pList,
								const void* pData);

/**
  * @fn ListError linkedListGetLength (const LinkedList* pList,
						int* iLength)
  * @brief Donne la longueur de la liste.
  *
  * @param[in] pList La liste à mesurer
  * @param[out] iLength Adresse ou stocker le résultat
  * @return Code d'erreur de liste
  */
ListError linkedListGetLength (const LinkedList* pList,
						int* iLength);

/**
  * @fn ListError linkedListGetFirst (const LinkedList* pList,
						LinkedList** ppFirst);
  * @brief Récupère la première cellule de la liste.
  *
  * @param[in] pList Liste étudiée
  * @param[out] ppFirst Pointeur sur la première cellule de la liste
  * @return Code d'erreur de liste
  */
ListError linkedListGetFirst (const LinkedList* pList,
						LinkedList** ppFirst);

/**
  * @fn ListError linkedListGetLast (const LinkedList* pList,
					LinkedList** ppLast);
  * @brief Récupère la dernière cellule de la liste.
  *
  * @param[in] pList Liste étudiée
  * @param[out] ppLast Pointeur sur la dernière cellule de la liste
  * @return Code d'erreur de liste
  */
ListError linkedListGetLast (const LinkedList* pList,
					LinkedList** ppLast);

/**
  * @fn ListError linkedListGetIeme (const LinkedList* pList,
					int iIndex,
					LinkedList** ppIeme);
  * @brief Récupère la (i+1)eme cellule de la liste.
  *
  * @param[in] pList Liste étudiée
  * @param[in] iIndex Index de la cellule voulue
  * @param[out] ppIeme Pointeur sur la (i+1)eme cellule de la liste
  * @return Code d'erreur de liste
  */
ListError linkedListGetIeme (const LinkedList* pList,
					int iIndex,
					LinkedList** ppIeme);


/* ********************************************************************* */
/*                                                                       */
/*                          Test de regression                           */
/*                                                                       */
/* ********************************************************************* */


/**
  * @fn int linkedListRegressionTest (void)
  * @brief Test de regression pour les listes chainées.
  *
  * @return EXIT_SUCCESS
  */
int linkedListRegressionTest (void);


#endif /* __LINKED_LIST_H__ */
