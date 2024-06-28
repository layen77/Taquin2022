#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "taquinSDL.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>


void pause()
{
	system("pause");
}

void usage()
{
	printf("Usage:\n");
	printf("TAQUIN [HAUTEUR] [LARGEUR] [FICHIER BMP] [MIN_RAND] [MAX_RAND]\n");
	printf("  - HAUTEUR : hauteur du taquin (= %d si non renseignee)\n",SIZE_MINI);
	printf("  - LARGEUR : largeur du taquin (= hauteur si non renseignee)\n");
	printf("  - FICHIER BMP : fichier utilise comme motif du taquin\n");
	printf("  - MIN RAND : nombre de coups minimum pour melanger\n");
	printf("  - MAX RAND : nombre de coups maximum pour melanger\n");

}

int main(int argc, char ** argv)
{	
	// Detection fuites memoire
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	atexit(pause);

	//_CrtSetBreakAlloc(3966);

	// Tests pour v�rifier que les param�tres pass�s en ligne de commande sont corrects
	switch(argc)
	{
		case 1:
			usage();
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			{
				// On r�cup�re la taille du taquin
				int hauteur,largeur;
				int minRandom = 10;
				int maxRandom = 100;
				char taquinFile[1024]="taquin.BMP";

				hauteur = SIZE_MINI;
				largeur = SIZE_MINI;

				switch(argc)
				{
					case 2:
						hauteur = largeur= atoi(argv[1]);
						break;
					case 3:
					case 4:					
					case 5:
					case 6:
					case 7:
						hauteur= atoi(argv[1]);
						largeur= atoi(argv[2]);
						break;
				}
				switch(argc)
				{
					case 5:
						minRandom = atoi(argv[4]);
						maxRandom = atoi(argv[4]);
						break;
					case 6:
						minRandom = atoi(argv[4]);
						maxRandom = atoi(argv[5]);
						break;
				}

				// PARAMETRES OK --- On continue avec la "m�canique" de l'application

				printf("TAQUIN [%d;%d]\n",hauteur,largeur);
				printf("--------------\n");
				if(argc>=4) strcpy_s(taquinFile,1024,argv[3]);
				printf("fichier BMP : %s\n",taquinFile);
				printf("aleatoire : [%d;%d]\n",minRandom,maxRandom);


				// En mode console
				//gameLoop(hauteur,largeur, minRandom,maxRandom);
				// En mode graphique
				gameLoopSDL(hauteur, largeur, taquinFile, minRandom, maxRandom);

			}
			break;
	}

	return 0;
}