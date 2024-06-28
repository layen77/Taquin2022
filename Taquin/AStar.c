#include "AStar.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "SDL.h"
#include "SDL_image.h"


// fonction pour créer (allouer) un noeud de liste et l'initialiser avec le taquin passé en paramètre
ptrListAStar createNodeList(Taquin* pTaquin, int gValue, int fValue, deplacement d, ptrListAStar pPrevPlay)
{
	ptrListAStar node = (ptrListAStar)calloc(1, sizeof(ListAStar));
	node->pTaquin = pTaquin;
	node->g = gValue;
	node->f = fValue;
	node->pPrevNode = pPrevPlay;
	node->prevD = d;
	return node;
}

// Insertion dans la liste de façon triée ou non
// si on passe le paramètre tri à 0, on insère en tête de liste
int insertList(ptrListAStar* ppHead, ptrListAStar pNode, int tri)
{
	if (!ppHead || !pNode)
		return -1;

	if (!tri)
	{
		pNode->pNextNode = (*ppHead);
		(*ppHead) = pNode;
		return 1;
	}

	if (!(*ppHead))
	{
		(*ppHead) = pNode;
		return 1;
	}

	ptrListAStar* ppCursor = ppHead;

	while ((*ppCursor)->pNextNode && ((*ppCursor)->f <= pNode->f))
	{
		ppCursor = &((*ppCursor)->pNextNode);
	}
	if ((*ppCursor)->pNextNode)
	{
		pNode->pNextNode = (*ppCursor);
		(*ppCursor) = pNode;
		return 1;
	}
	(*ppCursor)->pNextNode = pNode;
	return 1;
}

// Fonction pour prélever le noeud en tête de liste
// Retourne le noeud prélevé
ptrListAStar popList(ptrListAStar* ppHead)
{
	if (!ppHead || !*ppHead)
		return NULL;
	ptrListAStar pNode = (*ppHead);

	(*ppHead) = (*ppHead)->pNextNode;
	pNode->pNextNode = NULL;
	return pNode;
}

// Fonction qui retourne le noeud dans lequel on trouve le taquin passé en paramètre (pointeur sur le pointeur dans la liste)
ptrListAStar* isInList(ptrListAStar* ppHead, Taquin* pTaquin)
{
	if (!ppHead || !*ppHead)
		return NULL;

	ptrListAStar ppCursor = *ppHead;
	while (ppCursor->pNextNode)
	{
		if (equalTaquin(ppCursor->pTaquin, pTaquin) == 1)
			return ppCursor;
		ppCursor = ppCursor->pNextNode;
	}

	return NULL;
}

// fonction pour afficher une liste
// si on met displayFGH à 0 les valeur de F, G et H ne sont pas affichées
int displayList(ptrListAStar pHead, int displayFGH)
{
	if (!pHead)
		return -1;
	printf("F: %d\n", pHead->f);
	printf("G: %d\n", pHead->g);
	printf("H: %d\n", h(pHead->pTaquin));
	printf("Deplacement: %d\n", pHead->prevD);
	displayTaquin(pHead->pTaquin, 1);
	if (pHead->pNextNode)
		displayList(pHead->pNextNode, displayFGH);

	return 1;
}

// Fonction pour résoudre le taquin en utilisant l'algorithme A*
// La fonction prend comme taquin initial le taquin spécifié par pTaquin
// elle remplit 
//   - pTabDeplacement (un tableau contenant les déplacements à effectuer pour résoudre le taquin)
//   - pNbDeplacements
//   - pNbTaquinsGeneres
//   - pTimeElapsed
// Si stepByStep est différent de 0 on affiche dans la console toutes les étapes de la résolution
// pWindow
int solveTaquin(Taquin* pTaquin, deplacement** pTabDeplacement, unsigned long* pNbDeplacements, unsigned long* pNbTaquinsGeneres, unsigned long* pTimeElapsed, int stepByStep, SDL_Surface* pWindow)
{
	int debut = SDL_GetTicks();
	ptrListAStar pPrevPlay = createNodeList(NULL, 0, 0, AUCUN, NULL);
	ptrListAStar pClose = NULL;

	Taquin* pCpyTaquin = (Taquin*)calloc(1, sizeof(Taquin));
	createTaquin(pCpyTaquin, pTaquin->hauteur, pTaquin->largeur);
	copyTaquin(pTaquin, pCpyTaquin);

	ptrListAStar pOpen = createNodeList(pCpyTaquin, 0, h(pTaquin), AUCUN, NULL);

	while (!endTaquin(pPrevPlay->pTaquin))
	{
		for (int i = 1; i < 5; i++)
		{
			Taquin* pTmpTaquin = (Taquin*)calloc(1, sizeof(Taquin));
			createTaquin(pTmpTaquin, pTaquin->hauteur, pTaquin->largeur);
			copyTaquin(pCpyTaquin, pTmpTaquin);

			if (moveTaquin(pTmpTaquin, i) && !isInList(&pClose, pTmpTaquin) && pPrevPlay && cancelMove(pPrevPlay->prevD) != i)
			{
				insertList(&pOpen, createNodeList(pTmpTaquin, pPrevPlay->g + 1, (pPrevPlay->g + 1 + h(pTmpTaquin)), i, pPrevPlay), 1);
			}
			else
				freeTaquin(pTmpTaquin);
		}
		(*pNbTaquinsGeneres)++;

		pPrevPlay = popList(&pOpen);
		insertList(&pClose, pPrevPlay, 0);
		copyTaquin(pPrevPlay->pTaquin, pCpyTaquin);
	}

	*pTimeElapsed = (SDL_GetTicks() - debut);

	(*pTabDeplacement) = (unsigned int*)calloc((*pNbTaquinsGeneres), sizeof(unsigned int));

	ptrListAStar* pCloseCursor = &(pClose);
	while (*pCloseCursor)
	{
		(*pNbDeplacements)++;
		pCloseCursor = &((*pCloseCursor)->pPrevNode);
	}

	for (int i = (*pNbDeplacements) - 1; i >= 0 && pClose; i--)
	{
		(*pTabDeplacement)[i] = pClose->prevD;
		pClose = pClose->pPrevNode;
	}

	freeList(pOpen);
	freeList(pClose);
	freeList(pPrevPlay);

	return 1;
}

// fonction d'évaluation pour la résolution avec AStar
int h(Taquin* pTaquin)
{
	if (!pTaquin)
		return 0;

	unsigned int res = 0;
	int k = 0;

	for (unsigned int i = 0; i < pTaquin->hauteur; i++)
	{
		for (unsigned int j = 0; j < pTaquin->largeur; j++)
		{
			if (pTaquin->plateau[i][j] != k)
			{
				res += abs(pTaquin->plateau[i][j] / pTaquin->largeur - i) + abs(pTaquin->plateau[i][j] % pTaquin->largeur - j);
			}

			k++;
		}
	}

	return res;
}

int freeList(ptrListAStar ppHead)
{
	if (!ppHead)
		return 0;

	if (!(ppHead)->pNextNode)
	{
		free(ppHead);
	}

	ptrListAStar ppCursor1 = ppHead;
	ptrListAStar ppCursor2 = ppHead->pNextNode;

	while (ppCursor1->pNextNode && ppCursor2 && (ppCursor2->pNextNode))
	{
		freeTaquin(ppCursor1->pTaquin);
		ppCursor1->pNextNode = NULL;
		free(ppCursor1);

		ppCursor1 = ppCursor2;
		ppCursor2 = ppCursor2->pNextNode;
	}
	freeTaquin(ppCursor2->pTaquin);
	free(ppCursor2);

	return 1;
}