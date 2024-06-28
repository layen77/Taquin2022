//////////////////////////////////////////////////////////////////////
//
//	AStar / A*
//
//	Structures de donn�es utiles � la repr�sentation de liste pour la r�solution AStar du taquin
//	Primitives pour ins�rer un �l�ment dans une liste tri�e et pour pr�lever en t�te de liste
//
//////////////////////////////////////////////////////////////////////

#ifndef _ASTAR_

#define _ASTAR_


// On a besoin des structures et des fonctions pour g�rer le taquin, on inclut donc la bibliloth�que
#include "Taquin.h"
#include "SDL.h"

// Structure de donn�e pour r�pr�senter un noeud de liste en m�moire
typedef struct sListAStar {
	int g;							// Distance d�j� parcourue entre l'�tat initial et l'�tat courant
	int f;							// Evaluation de la distance � parcourir entre l'�tat initial et l'�tat final
	Taquin *pTaquin;				// Taquin contenu dans le noeud courant
	deplacement	prevD;				// dernier d�placement effectu� pour arriver � ce taquin
	struct sListAStar* pPrevNode;	// Pointeur vers le noeud correspondant � l'�tape pr�c�dente dans la solution
	struct sListAStar* pNextNode;	// Pointeur vers le noeud suivant (liste simplement cha�n�e)
}ListAStar;

typedef ListAStar * ptrListAStar;		// Red�finition pour faciliter l'�criture dans les en-t�tes des fonctions

// fonction pour cr�er (allouer) un noeud de liste et l'initialiser avec le taquin pass� en param�tre
ptrListAStar createNodeList(Taquin * pTaquin, int gValue, int fValue, deplacement d, ptrListAStar pPrevPlay);

// Insertion dans la liste de fa�on tri�e ou non
// si on passe le param�tre tri � 0, on ins�re en t�te de liste
int insertList(ptrListAStar * ppHead, ptrListAStar pNode, int tri);

// Fonction pour pr�lever le noeud en t�te de liste
// Retourne le noeud pr�lev�
ptrListAStar popList(ptrListAStar * ppHead);

// fonction qui retourne le noeud dans lequel on trouve le taquin pass� en param�tre (pointeur sur le pointeur dans la liste)
ptrListAStar * isInList(ptrListAStar * ppHead, Taquin * pTaquin);

// fonction pour afficher une liste
// si on met displayFGH � 0 les valeur de F, G et H ne sont pas affich�es
int displayList(ptrListAStar pHead, int displayFGH);

// Fonction pour r�soudre le taquin en utilisant l'algorithme A*
// La fonction prend comme taquin initial le taquin sp�cifi� par pTaquin
// elle remplit 
//   - pTabDeplacement (un tableau contenant les d�placements � effectuer pour r�soudre le taquin)
//   - pNbDeplacements
//   - pNbTaquinsGeneres
//   - pTimeElapsed
// Si stepByStep est diff�rent de 0 on affiche dans la console toutes les �tapes de la r�solution
// pWindow
int solveTaquin(Taquin *pTaquin, deplacement ** pTabDeplacement, unsigned long *pNbDeplacements, unsigned long * pNbTaquinsGeneres, unsigned long * pTimeElapsed, int stepByStep, SDL_Surface * pWindow);

// fonction d'�valuation pour la r�solution avec AStar
int h(Taquin * pTaquin);

int freeList(ptrListAStar ppHead);


#endif