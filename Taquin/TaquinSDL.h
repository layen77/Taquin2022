//////////////////////////////////////////////////////////////////////
//
//	JEU DE TAQUIN EN SDL
//
//	Utilisation de taquin.h/taquin.c pour augmenter les info et l'afficher en SDL
//	"Surcharge" des fonctions pour transposer le jeu en SDL
//  Ajout de la boucle de jeu graphique (gameLoop)
//
//////////////////////////////////////////////////////////////////////

#ifndef _TAQUIN_SDL_

#define _TAQUIN_SDL_

#include "taquin.h"
#include "SDL.h"

// Structure pour cr�er un taquin en SDL � partir d'une image de fond
typedef struct sTaquinSDL {
	Taquin taquin;					// taquin (cf. taquin.h)
	SDL_Surface * pWindow;			// fen�tre SDL qui contient le taquin
	SDL_Surface * pFond;			// image de fond du taquin
	int resX,resY;					// r�solution d'une case du taquin
}TaquinSDL;


// Fonction permettant de cr�er le taquin en SDL � partir de sa taille et du fichier BMP � utiliser
// Cette fonction cr�e aussi la fen�tre SDL et la stocke dans la structure TaquinSDL
int createTaquinSDL(TaquinSDL * pTaquinSDL,int hauteur, int largeur, char * pathBMPfile);
// Fonction pour dessiner une case sur le fond
int displayCaseTaquin(TaquinSDL * pTaquinSDL,unsigned char caseTaquin, SDL_Rect * pDest, int x, int y, int refresh);
// fonction pour rendre le taquin dans son �tat actuel
int displayTaquinSDL(TaquinSDL * pTaquinSDL);
// fonction permettant de faire le rendu SDL du taquin et de jouer (gestion des �v�nements � l'int�rieur de la fonction)
int gameLoopSDL(int hauteur,int largeur, char * pathBMPfile, int minRandom, int maxRandom);
// fonction pour lib�rer le taquins et les surfaces SDL utilis�es
int freeTaquinSDL(TaquinSDL * pTaquinSDL);
//fonction qui retourne la position X de la case noire (case 0)
int posXCaseNoire(Taquin* taquin);
//fonction qui retourne la position Y de la case noire (case 0)
int posYCaseNoire(Taquin* taquin);
#endif