// Inclusion des bibliothèques utiles
#include "taquin.h"
#include "AStar.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>



// Fonction pour copier un plateau de taquin pSrc vers pDest
int copyTaquin(Taquin * pSrc, Taquin * pDest)
{
	if (!pSrc)  return -1;
	if (!pDest) return -1;
	pDest->hauteur = pSrc->hauteur;
	pDest->largeur = pSrc->largeur;

	pDest->x = pSrc->x;
	pDest->y = pSrc->y;

	for (unsigned int i = 0; i < pSrc->hauteur; i++)
	{
		for (unsigned int j = 0; j < pSrc->largeur; j++)
		{
			pDest->plateau[i][j] = pSrc->plateau[i][j];
		}
	}
	//// TODO: copyTaquin

	return 1;
}

// fonction qui renvoie 1 si les 2 taquins sont identiques
// 0 sinon
// -1 si il y a une erreur au passage des paramètres
int equalTaquin(Taquin * pTaquin1, Taquin * pTaquin2)
{
	if (!pTaquin1) return -1;
	if (!pTaquin2) return -1;

	if (pTaquin1->hauteur != pTaquin2->hauteur || pTaquin1->largeur != pTaquin2->largeur || pTaquin1->x != pTaquin2->x || pTaquin1->y != pTaquin2->y)
		return 0;

	for (unsigned int i = 0; i < pTaquin1->hauteur; i++)
	{
		for (unsigned int j = 0; j < pTaquin1->largeur; j++)
		{
			if (pTaquin1->plateau[i][j] != pTaquin2->plateau[i][j])
				return 0;
		}
	}
	return 1;
	//// TODO: equalTaquin
}

// Fonction qui crée un plateau de taquin 
// ATTENTION le plateau doit être NULL avant l'appel à cette fonction 
// Si le plateau n'est pas NULL la fonction essayera de libérer la zone mémoire occupée par le plateau et cela peut donc aboutir à un crash si le plateau n'a pas été initialisé
int createTaquin(Taquin * pTaquin, unsigned char hauteur, unsigned char largeur)
{
	// Test pour vérifier que les données passées ne sont pas corrompues
	if(!pTaquin) return 0;
	
	// Tests pour vérifier les paramètres de taille de taquin
	if(hauteur<SIZE_MINI) hauteur = SIZE_MINI;
	if(hauteur>SIZE_MAXI) hauteur = SIZE_MAXI;
	if(largeur<SIZE_MINI) largeur = SIZE_MINI;
	if(largeur>SIZE_MAXI) largeur = SIZE_MAXI;
	
	{
		int i;

		// On vérifie que le plateau n'existe pas
		// S'il existe on libère la mémoire avant de recréer le plateau
		if(pTaquin->plateau)
		{
			// On libère le plateau ligne par ligne
			for(i=0; i < pTaquin->hauteur; i++) if(pTaquin->plateau[i]) free(pTaquin->plateau[i]);
			// On libère le tableau qui stockait les lignes
			free(pTaquin->plateau);
			pTaquin->plateau = NULL;
		}

		pTaquin->hauteur = hauteur;
		pTaquin->largeur = largeur;

		// on alloue la zone mémoire pour stocker les adresses des lignes du tableau
		pTaquin->plateau = (unsigned char**) malloc(sizeof(unsigned char*)*hauteur);
		
		// si on a pas réussi à allouer la zone mémoire on retourne 0
		if(!pTaquin->plateau) return 0;

		for(i=0; i < hauteur; i++) pTaquin->plateau[i] = NULL;

		for(i=0; i < hauteur; i++)
		{
			// On alloue la zone mémoire pour contenir la ligne i
			pTaquin->plateau[i] = (unsigned char*) malloc(sizeof(unsigned char)*largeur);
			// S'il y a eu un souci à l'allocation on libère tout ce qui a déjàà été alloué et on retourne 0
			if(!pTaquin->plateau[i])
			{
				freeTaquin(pTaquin);
				return 0;
			}
		}
		// On initialise le taquin
		if(!initTaquin(pTaquin)) return 0;
	}

	return 1;
}


// Fonction qui initialise le taquin
int initTaquin(Taquin * pTaquin)
{
	if (!pTaquin)
		return 0;
	unsigned int k = 0;

	for (unsigned int i = 0; i < pTaquin->hauteur; i++) 
	{
		for (unsigned int j = 0; j < pTaquin->largeur; j++) {
			pTaquin->plateau[i][j] = k;
			k++;
		}
	}
		
	pTaquin->x = 0;
	pTaquin->y = 0;
	//// TODO: initTaquin

	return 1;
}

// Fonction qui mélange le taquin en effectuant entre minRandom et maxRandom coups aléatoires
int mixTaquin(Taquin * pTaquin, int minRandom, int maxRandom)
{
	srand(time(NULL));
	int nbCoups = rand() % (maxRandom - minRandom + 1) + minRandom;
	deplacement d = 0;

	for (int i = 0; i < nbCoups; i++)
	{
		d = rand() % (4 - 1 + 1) + 1;//(rand() % (MAX - MIN + 1)) + MIN;
		if (!moveTaquin(pTaquin, d))
			i--;
	}
	//// TODO: mixTaquin

	return 1;
}

// Fonction qui permet de bouger une pièce du taquin (en bougeant la case vide)
int moveTaquin(Taquin * pTaquin, deplacement d)
{
	if (!pTaquin)
		return -1;

	unsigned char tmp = 0;

 	switch (d) 
	{
	case AUCUN:
		break;
	case GAUCHE:
		if (pTaquin->x == 0)
			return 0;
		tmp = pTaquin->plateau[pTaquin->y][pTaquin->x];
		pTaquin->plateau[pTaquin->y][pTaquin->x] = pTaquin->plateau[pTaquin->y][pTaquin->x - 1];
		pTaquin->plateau[pTaquin->y][pTaquin->x - 1] = tmp;
		--pTaquin->x;
		break;
	case DROITE:
		if (pTaquin->x == pTaquin->largeur - 1)
			return 0;
		tmp = pTaquin->plateau[pTaquin->y][pTaquin->x];
		pTaquin->plateau[pTaquin->y][pTaquin->x] = pTaquin->plateau[pTaquin->y][pTaquin->x + 1];
		pTaquin->plateau[pTaquin->y][pTaquin->x + 1] = tmp;
		++pTaquin->x;
		break;
	case HAUT:
		if (pTaquin->y == 0)
			return 0;
		tmp = pTaquin->plateau[pTaquin->y][pTaquin->x];
		pTaquin->plateau[pTaquin->y][pTaquin->x] = pTaquin->plateau[pTaquin->y - 1][pTaquin->x];
		pTaquin->plateau[pTaquin->y - 1][pTaquin->x] = tmp;
		--pTaquin->y;
		break;
	case BAS:
		if (pTaquin->y == pTaquin->hauteur - 1)
			return 0;
		tmp = pTaquin->plateau[pTaquin->y][pTaquin->x];
		pTaquin->plateau[pTaquin->y][pTaquin->x] = pTaquin->plateau[pTaquin->y + 1][pTaquin->x];
		pTaquin->plateau[pTaquin->y + 1][pTaquin->x] = tmp;
		++pTaquin->y;
		break;
	}

	//// TODO: moveTaquin

	return 1;
}

// Fonction qui renvoie le déplacement à effectuer pour annuler le déplacement donné en paramètre
deplacement cancelMove(deplacement d)
{
	if (!d) return -1;
	switch (d)
	{
	case AUCUN:
		break;
	case GAUCHE:
		return DROITE;
	case DROITE:
		return GAUCHE;
	case HAUT:
		return BAS;
	case BAS:
		return HAUT;
	}
	//// TODO: cancelMove

	return 0;
}

// Fonction qui renvoie 1 si le taquin es résolu, 0 sinon
int endTaquin(Taquin * pTaquin)
{
	if (!pTaquin) return 0;
	unsigned char k = 0;
	for (unsigned int i = 0; i < pTaquin->hauteur; i++)
	{
		for (unsigned int j = 0; j < pTaquin->largeur; j++)
		{
			if (pTaquin->plateau[i][j] != k)
				return 0;
			k++;
		}
	}
	//// TODO: endTaquin
	return 1;
}

// fonction d'affichage du taquin
int displayTaquin(Taquin * pTaquin, int offset)
{
	// TODO: displayTaquin
	if (!pTaquin)
	{
		printf("ERREUR: displayTaquin\n");
		return -1;
	}
	printf("______\n");
	for (unsigned int i = 0; i < pTaquin->hauteur; i++)
	{
		for (unsigned int j = 0; j < pTaquin->largeur; j++)
		{
			printf("%d", pTaquin->plateau[i][j]);
			for (unsigned int k = 0; k < (unsigned int) offset; k++)
				printf(" ");
		}
		printf("\n");
		for (unsigned int l = 0; l < (unsigned int) offset; l++)
				printf("\n");
	}
	printf("______\n");
	return 1;
}

// Fonction pour libérer les zones mémoires occupées par un taquin
int freeTaquin(Taquin * pTaquin)
{
	if (!pTaquin)
		return NULL;

	if(pTaquin->plateau)
	{
		for(int i = 0; i < pTaquin->hauteur; i++)
			if(pTaquin->plateau[i])
				free(pTaquin->plateau[i]);
		free(pTaquin->plateau);
		pTaquin->plateau = NULL;
		free(pTaquin);
	}
	else
		return 0;

	return 1;
}
// Boucle de jeu 
int gameLoop(int hauteur,int largeur,int minRandom, int maxRandom)
{
	Taquin* pTaquin = (Taquin*)calloc(1, sizeof(pTaquin));
	if (!createTaquin(pTaquin, 3, 3))
		return -1;
	mixTaquin(pTaquin, minRandom, maxRandom);
	// BOUCLE DE JEU ! A DEVELOPPER
	while(!_kbhit())
	{

		

	}
	displayTaquin(pTaquin, 1);
	freeTaquin(pTaquin);
	return 1;
}

