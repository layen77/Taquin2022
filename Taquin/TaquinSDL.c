#include "TaquinSDL.h"
#include "AStar.h"
#include <time.h>
#include "SDL_gfxprimitives.h"


// Fonction permettant de cr�er le taquin en SDL � partir de sa taille et du fichier BMP � utiliser
// Cette fonction cr�e aussi la fen�tre SDL et la stocke dans la structure TaquinSDL
int createTaquinSDL(TaquinSDL* pTaquinSDL, int hauteur, int largeur, char* pathBMPfile)
{
	// Test pour v�rifier que les donn�es pass�es ne sont pas corrompues
	if (!pTaquinSDL) return 0;

	// initialisation de la fen�tre SDL
	pTaquinSDL->pWindow = NULL;
	// initialisation de l'image � NULL
	pTaquinSDL->pFond = NULL;
	// On cr�e le taquin qui sera utilis� pour jouer
	pTaquinSDL->taquin.plateau = NULL;
	createTaquin(&(pTaquinSDL->taquin), hauteur, largeur);

	// On met � jour la taille du taquin
	largeur = pTaquinSDL->taquin.largeur;
	hauteur = pTaquinSDL->taquin.hauteur;

	// On initialise la SDL
	// On initialise non seulement le mode vid�o mais aussi la gestion du temps pour pouvoir utiliser SDL_GetTicks()
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
	{
		freeTaquinSDL(pTaquinSDL);
		return 0;
	}

	// On charge le BMP qui servira de fond au taquin
	pTaquinSDL->pFond = SDL_LoadBMP(pathBMPfile);
	if (!pTaquinSDL->pFond)
	{
		freeTaquinSDL(pTaquinSDL);
		return 0;
	}

	// Initialisation de la fen�tre

	// On r�cup�re la taille d'une case
	pTaquinSDL->resX = pTaquinSDL->pFond->w / largeur;
	pTaquinSDL->resY = pTaquinSDL->pFond->h / hauteur;

	// On cr�e la fen�tre en fonction de la r�solution de l'image d'entr�e
	pTaquinSDL->pWindow = SDL_SetVideoMode(pTaquinSDL->resX * largeur, pTaquinSDL->resY * hauteur, 32, SDL_DOUBLEBUF | SDL_HWSURFACE);
	if (!pTaquinSDL->pWindow)
	{
		freeTaquinSDL(pTaquinSDL);
		return 0;
	}


	SDL_WM_SetCaption("Taquin - Baptiste LEFEBVRE & Luan PRAUD", "TAQUIN");

	SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);

	return 1;

}


int displayCaseTaquin(TaquinSDL* pTaquinSDL, unsigned char caseTaquin, SDL_Rect* pDest, int x, int y, int refresh)
{//                  (pTaquinSDL,           pTaquinSDL->taquin.plateau[x][y],    pDest,     x,     y,     1);
	//// TODO: displayCaseTaquin

	//On affiche l'image compl�te si le taquin est r�solu
	if (endTaquin(&(pTaquinSDL->taquin)) == 1)
	{
		SDL_BlitSurface(pTaquinSDL->pFond, NULL, pTaquinSDL->pWindow, NULL);
		SDL_UpdateRect(pTaquinSDL->pWindow, 0, 0, 0, 0);
		return 1;
	}


	// Si la case n'est pas vide ...
	// on calcule o� est la case "caseTaquin" dans l'image initiale pour -par la suite - venir d�couper la zone qui correspond � la case

	int IDtabX, IDtabY;

	IDtabX = (caseTaquin % pTaquinSDL->taquin.largeur);//Position X de caseTaquin
	IDtabY = (caseTaquin / pTaquinSDL->taquin.largeur);//Position Y de caseTaquin

	SDL_Rect* pSource = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));
	//On d�fini les caract�ristiques du rectange SDL_Rect*
	pSource->w = pTaquinSDL->resX - 2;           //largeur du rectangle
	pSource->h = pTaquinSDL->resY - 2;           //hauteur du rectangle
	pSource->x = pTaquinSDL->resX * IDtabX + 1;  //position X du pixel en haut � gauche
	pSource->y = pTaquinSDL->resY * IDtabY + 1;  //position Y du pixel en haut � gauche

	if (caseTaquin == 0)//affichage d'un carr� noir � la place d'une portion d'image pour la case 0
	{
		SDL_Surface* CarreNoir = SDL_CreateRGBSurface(0, pTaquinSDL->resX, pTaquinSDL->resY, 32, 255, 255, 255, 0);
		SDL_BlitSurface(CarreNoir, pSource, pTaquinSDL->pWindow, pDest);
		SDL_FreeSurface(CarreNoir);
	}
	else
	{
		SDL_BlitSurface(pTaquinSDL->pFond, pSource, pTaquinSDL->pWindow, pDest);
	}


	// On dessine la case dans la fen�tre (en d�coupant dans l'image initiale avec la zone d�finie ci-dessus)
	//SDL_BlitSurface(pTaquinSDL->pFond, NULL, pTaquinSDL->pWindow, NULL);

	if (refresh)
		SDL_UpdateRect(pTaquinSDL->pWindow, pDest->x, pDest->y, pDest->w, pDest->h);
	free(pSource);
	return 1;
}


// fonction pour rendre le taquin dans son �tat actuel
int displayTaquinSDL(TaquinSDL* pTaquinSDL)
{
	// Test pour v�rifier que les donn�es pass�es ne sont pas corrompues
	if (!pTaquinSDL || !pTaquinSDL->taquin.plateau || !pTaquinSDL->pWindow)
		return 0;


	//On recouvre tout le fond de l'�cran en blanc, par dessus viendront les cases individuelles ce qui laissera des bordures de 2 pixels entre chaque case
	SDL_Surface* fondLignes = SDL_CreateRGBSurface(0, pTaquinSDL->resX * pTaquinSDL->taquin.largeur, pTaquinSDL->resY * pTaquinSDL->taquin.hauteur, 32, 0, 0, 0, 0);
	SDL_FillRect(fondLignes, 0, SDL_MapRGB(fondLignes->format, 255, 255, 255));
	SDL_BlitSurface(fondLignes, NULL, pTaquinSDL->pWindow, NULL);
	SDL_FreeSurface(fondLignes);
	//SDL_UpdateRect(pTaquinSDL->pWindow, 0, 0, pTaquinSDL->resX * pTaquinSDL->taquin.largeur, pTaquinSDL->resY * pTaquinSDL->taquin.hauteur);


	SDL_Rect* pDest = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));
	//On appelle la fonction displayCaseTaquin autant de fois qu'il y a de cases dans le plateau de jeu
	for (int x = 0; x < pTaquinSDL->taquin.largeur; x++)
	{
		for (int y = 0; y < pTaquinSDL->taquin.hauteur; y++)
		{
			//On d�fini les caract�ristiques du rectange SDL_Rect*
			pDest->w = pTaquinSDL->resX - 2;      //largeur du rectangle
			pDest->h = pTaquinSDL->resY - 2;      //hauteur du rectangle
			pDest->x = x * pTaquinSDL->resX + 1;  //position X du pixel en haut � gauche
			pDest->y = y * pTaquinSDL->resY + 1;  //position Y du pixel en haut � gauche
			displayCaseTaquin(pTaquinSDL, pTaquinSDL->taquin.plateau[y][x], pDest, x, y, 1);
		}
	}

	free(pDest);
	// On dessine le taquin termin� pour afficher quelque chose mais il faudra le changer
	//SDL_BlitSurface(pTaquinSDL->pFond, NULL, pTaquinSDL->pWindow, NULL);

	//// TODO: displayTaquinSDL

	// On met � jour la fen�tre compl�te
	SDL_UpdateRect(pTaquinSDL->pWindow, 0, 0, 0, 0);
	//displayTaquin((&pTaquinSDL->taquin), 1);
	return 1;
}


// fonction permettant de faire le rendu SDL du taquin et de jouer (gestion des �v�nements � l'int�rieur de la fonction)
int gameLoopSDL(int hauteur, int largeur, char* pathBMPfile, int minRandom, int maxRandom)
{
	int end = 0;
	TaquinSDL t;

	srand((unsigned)time(NULL));

	// On cr�e le taquin et la fen�tre pour le dessiner
	if (!createTaquinSDL(&t, hauteur, largeur, pathBMPfile)) return 0;

	//ptrListAStar testList = createNodeList(pTaquin, 0, 3, 0, NULL);

	// On boucle sur le jeu tant qu'on a pas demand� de quitter
	while (end >= 0)
	{
		int tick = SDL_GetTicks();

		// On m�lange le taquin
		mixTaquin(&(t.taquin), minRandom, maxRandom);

		// On affiche le taquin m�lang�
		displayTaquinSDL(&t);


		end = 0;


		// On boucle tant que la solution n'a pas �t� trouv�e
		while (!end)
		{
			// Gestion des �v�nements
			SDL_Event e;

			int dt = SDL_GetTicks() - tick;
			tick += dt;

			while (!end && SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_KEYDOWN:
				{
					deplacement d = AUCUN;

					switch (e.key.keysym.sym)
					{
						// On d�finit le mouvement � effectuer
					case SDLK_q:;
					case SDLK_LEFT:
						d = GAUCHE; break;
					case SDLK_d:
					case SDLK_RIGHT:
						d = DROITE; break;
					case SDLK_z:
					case SDLK_UP:
						d = HAUT; break;
					case SDLK_s:
					case SDLK_DOWN:
						d = BAS; break;
					case SDLK_ESCAPE:
						// On quitte l'application
						end = -1;
						break;
					case SDLK_F1:
						// On d�clenche la r�solution du taquin
						// r�solution SDL � faire par la suite pour laisser la main � l'utilisateur :
						// - Arr�ter la r�solution (appui sur n'importe qu'elle touche
						// - Quitter l'application (ECHAP ou CROIX en haut � droite)
					{

						//on initialise les variables permettant de r�cup�rer les informations issues de la r�solution
						deplacement* tabDeplacements = NULL;
						unsigned long nbDeplacements = 0;
						unsigned long nbSommetsParcourus = 0;
						unsigned long timeElapsed = 0;

						// On demande la r�solution du taquin � l'ordinateur
						if (solveTaquin(&(t.taquin), &tabDeplacements, &nbDeplacements, &nbSommetsParcourus, &timeElapsed, 0, t.pWindow))
						{
							// Si on a trouv� une solution on affiche les informations issues de la r�solution
							unsigned long i;
							int res = 0;
							printf("Nombre de deplacements = %d\n", nbDeplacements);
							printf("Nombre de sommets parcourus = %d\n", nbSommetsParcourus);
							printf("Temps ecoule = %d ms\n", timeElapsed);

							// On affiche la solution �tape par �tape
							for (i = 0; i < nbDeplacements; i++)
							{
								// On effectue le d�placement, on affiche le nouveau plateau et on attend un appui sur une touche pour continuer
								if (tabDeplacements[i] != AUCUN)
								{
									if (moveTaquin(&(t.taquin), tabDeplacements[i])) displayTaquinSDL(&t);
									else break;
								}
							}
						}
						// Si la r�solution n'a pas fonctionn�, on affiche le taquin tel qu'il �tait avant r�solution (on efface l'icone de "progression" si elle avait �t� dessin�e)
						else displayTaquinSDL(&t);

						if (tabDeplacements)
						{
							free(tabDeplacements);
							tabDeplacements = NULL;
						}
					}

					break;
					}

					if (d != AUCUN && moveTaquin(&(t.taquin), d)) displayTaquinSDL(&t);

				}
				break;

				case SDL_MOUSEBUTTONDOWN:
				{
					// On r�cup�re la position et l'�tat des boutons de la souris
					int x, y;
					Uint8 state;
					state = SDL_GetMouseState(&x, &y);

					if (state & SDL_BUTTON_LEFT)
					{

						deplacement d = AUCUN;

						// On r�cup�re la position de la case dans le taquin
						int posX = x / t.resX;
						int posY = y / t.resY;

						//// TODO: On d�finit le d�placement � effectuer (diff�rence en X de +/- 1 ou en Y de +/-1 => DEPLACEMENT sinon AUCUN)

						//On effectue un d�placement uniquement si la case cliqu�e est voisine de la case noire
						if (posXCaseNoire(&t.taquin) == posX && posYCaseNoire(&t.taquin) == (posY - 1))
						{
							d = BAS;
						}
						else if (posXCaseNoire(&t.taquin) == posX && posYCaseNoire(&t.taquin) == (posY + 1))
						{
							d = HAUT;
						}
						else if (posXCaseNoire(&t.taquin) == (posX - 1) && posYCaseNoire(&t.taquin) == posY)
						{
							d = DROITE;
						}
						else if (posXCaseNoire(&t.taquin) == (posX + 1) && posYCaseNoire(&t.taquin) == posY)
						{
							d = GAUCHE;
						}

						/*system("CLS");
						printf("La case Noire est en position X%d Y%d\n", posXCaseNoire(&t.taquin), posYCaseNoire(&t.taquin));
						printf("La souris est en %d %d\n", posX, posY); */

						// On applique le d�placement
						if (moveTaquin(&(t.taquin), d)) displayTaquinSDL(&t);
					}
				}
				break;

				case SDL_QUIT:
					// On quitte
					end = -1;
					break;
				}
			}

			// On relache du temps processeur pour les autres processus
			SDL_Delay(1);

			if (!end) end = endTaquin(&(t.taquin));

		}

		// Si on n'a pas demand� � quitter c'est qu'on a termin� le taquin
		if (end > 0)
		{
			// Affichage de l'image compl�te par dessus le tout

			SDL_Delay(2000);
		}

		// On r�initialise le taquin pour le prochain tour de jeu
		initTaquin(&(t.taquin));
	}

	// On lib�re le taquin et les surfaces SDL
	freeTaquinSDL(&t);

	/* Shut them both down */
	SDL_Quit();


	return 1;
}

// fonction pour lib�rer le taquins et les surfaces SDL utilis�es
int freeTaquinSDL(TaquinSDL* pTaquinSDL)
{
	// test pour savoir si les donn�es pass�es ne sont pas corrompues
	if (!pTaquinSDL) return 0;

	// On lib�re la fen�tre SDL
	if (pTaquinSDL->pWindow) SDL_FreeSurface(pTaquinSDL->pWindow);

	// On lib�re l'image de fond
	if (pTaquinSDL->pFond) SDL_FreeSurface(pTaquinSDL->pFond);

	// On lib�re le taquin (mode console)
	freeTaquin(&(pTaquinSDL->taquin));

	return 1;
}

//fonction qui retourne la position X de la case noire (case 0)
int posXCaseNoire(Taquin* taquin)
{
	for (int y = 0; y < taquin->hauteur; y++)
		for (int x = 0; x < taquin->largeur; x++)
			if (taquin->plateau[y][x] == 0)
				return x;
}

//fonction qui retourne la position Y de la case noire (case 0)
int posYCaseNoire(Taquin* taquin)
{
	for (int y = 0; y < taquin->hauteur; y++)
		for (int x = 0; x < taquin->largeur; x++)
			if (taquin->plateau[y][x] == 0)
				return y;
}