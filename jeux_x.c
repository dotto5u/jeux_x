/* déclaration des directives */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* création d'un type structure "joueur" */
typedef struct
{
    char pseudo[50];
    int jetons[24];
    int score_total[50];
}   joueur;

/* déclaration des prototypes */
void creer_plateau(int tab[7][7], int* L, int* C, int* N);
void remplir_plateau(int tab[7][7], int L, int C);
void donner_jetons(joueur* j1, joueur* j2, int N);
void affiche_plateau(int tab[7][7], int L, int C, int N, int t, int id_game);
void affiche_couleurs_jetons(int tab[7][7], int x, int y);
void placer_jetons(int tab[7][7], joueur player, int L, int C, int N, int numero_jeton, int id_game);
void affiche_resultat_partie(int tab[7][7], joueur* j1, joueur* j2, int L, int C, int N, int id_game);
void calcul_resultat_partie(int tab[7][7], int x, int y, int L, int C, int *score1, int* score2);
void fin_partie(joueur* j1, joueur* j2, short* play, int id_game);

/* fonction principale */

int main()
{   
    /* déclaration des variables */

    joueur j1, j2; // --> 2 joueurs qui ont chacun avec un pseudo, des jetons et l'ensemble de ses points pour ses parties jouées 
    int jeu[7][7]; // --> tableau à 2 dimensions de 7 sur 7 (limite du jeu)
    int L, C, N, id_game = 0; // --> lignes / colonnes / nombre de jeton pour chaque joueur / le numéro de la partie 
    short play = 1; // --> statut de la partie : 1 quand on joue, sinon 0

    /* demande du pseudo des joueurs */
    system("clear");
    printf("Quel est le pseudo du joueur 1 ? : ");
    scanf("%s", j1.pseudo);
    printf("Quel est le pseudo du joueur 2 ? : ");
    scanf("%s", j2.pseudo);

    /* boucle principale qui permet de lancer le jeu */
    while (play)
    {   
        /* création du plateau de jeu et distribution des jetons à chaque joueur */
        ++id_game;
        creer_plateau(jeu, &L, &C, &N);
        donner_jetons(&j1, &j2, N);

        /* boucle qui permet de faire placer les jetons des joueurs sur la plateau */ 
        for (int t = 1; t <= N; ++t)
        {   
            placer_jetons(jeu, j1, L, C, N, t, id_game);
            placer_jetons(jeu, j2, L, C, N, t, id_game);
        }
        affiche_plateau(jeu, L, C, N, N, id_game);
        affiche_resultat_partie(jeu, &j1, &j2, L, C, N, id_game);
        fin_partie(&j1, &j2, &play, id_game);
    }
}

/* déclaration des fonctions */

void creer_plateau(int tab[7][7], int* L, int* C, int* N)
{   
    do
    {   
        system("clear");
        printf("\nEntrez le nombre de lignes de la grille : ");
        scanf("%d", L);
        while(getchar()!='\n');
        printf("Entrez le nombre de colonnes de la grille : ");
        scanf("%d", C);
        while(getchar()!='\n');
    }
    while ((*L < 3 || 7 < *L) || (*C < 3 || 7 < *C) || (*L % 2 == 0) || (*C % 2 == 0));
    /* on demande le nombre de lignes et colonnes tant que ces valeurs sont paires et qu'ils ne sont pas compris entre 3 et 7 (limite du jeu) */

    remplir_plateau(tab, *L, *C);
    *N = ((*L) * (*C) - 1) / 2;
}


void remplir_plateau(int tab[7][7], int L, int C)
{   
    /* on remplit chaque case du plateau avec des 0 */
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < C; j++) tab[i][j] = 0;
    }
}


void donner_jetons(joueur* j1, joueur* j2, int N)
{
    /* on donne à chaque joueur des jetons */
    for (int i = 0; i < N; i++)
    {
        (*j1).jetons[i] = i+1;
        (*j2).jetons[i] = -(i+1);
    }
}


void affiche_plateau(int tab[7][7], int L, int C, int N, int t, int id_game)
{   
    system("clear");
    printf("\n   \e[0;35mJeux X - Partie %d\e[0m | \e[0;36mtour %d / %d\e[0m\n\n    ", id_game, t, N);
    for (int j = 0; j < C; j++) printf("%d    ", j+1);

    printf("\n  -");
    for (int j = 0; j < C; j++) printf("-----");
    printf("\n");

    for (int i = 0; i < L; i++)
    {
        printf("%d |", i+1);
        for (int j = 0; j < C; j++) affiche_couleurs_jetons(tab, i, j); 

        printf("\n  -");
        for (int j = 0; j < C; j++) printf("-----");
        printf("\n");
    }
}


void affiche_couleurs_jetons(int tab[7][7], int x, int y)
{   
    if (tab[x][y] == 0) printf("\e[8;30m %-2d\e[0m |", tab[x][y]); 
    else if (tab[x][y] > 0) printf("\e[0;34m %-2d\e[0m |", tab[x][y]);  
    else printf("\e[0;31m %-2d\e[0m |", abs(tab[x][y]));
}


void placer_jetons(int tab[7][7], joueur player, int L, int C, int N, int t, int id_game)
{
    int x, y;
    do
    {  
        affiche_plateau(tab, L, C, N, t, id_game);

        if (player.jetons[0] > 0) printf("\n\n\e[0;34m%s\e[0m, à votre tour de placer un jeton : \n\n", player.pseudo);
        else printf("\n\n\e[0;31m%s\e[0m, à votre tour de placer un jeton : \n\n", player.pseudo);

        printf("numéro de la ligne : ");
        scanf("%d", &x);
        while(getchar()!='\n');
        printf("numéro de la colonne : ");
        scanf("%d", &y);
        while(getchar()!='\n');
    }
    while ((x < 1 || L < x) || (y < 1 || C < y) || tab[x-1][y-1] != 0);
    /* on demande à quelle position on veut placer le jeton tant que la position n'est pas comprise dans les limites du jeu et que la case n'est pas vide */

    for (int i = 0; i < L; i++)
    {   
        for (int j = 0; j < C; j++) if (tab[i][j] == tab[x-1][y-1]) tab[x-1][y-1] = player.jetons[t-1];
        /* lorsque l'on trouve la bonne case, on place le jeton */
    }
}


void affiche_resultat_partie(int tab[7][7], joueur* j1, joueur* j2, int L, int C, int N, int id_game)  
{   
    int score1 = 0, score2 = 0;

    for (int i = 0; i < L; i++)
    {   
        for (int j = 0; j < C; j++)
        {   
            /* on cherche d'abord la case vide */
            if (tab[i][j] == 0)
            {    
                /* on calcul puis affiche les scores et on les ajoute au score total de chaque joueur */
                calcul_resultat_partie(tab, i, j, L, C, &score1, &score2);
                printf("\nLa somme \e[0;34mbleu \e[0m est égale à \e[0;34m%d\e[0m\n", score1);
                printf("La somme \e[0;31mrouge\e[0m est égale à \e[0;31m%d\e[0m\n", score2);
                (*j1).score_total[id_game-1] = score1;
                (*j2).score_total[id_game-1] = score2;
                break;
            }  
        } 
    }
}


void calcul_resultat_partie(int tab[7][7], int x, int y, int L, int C, int* score1, int* score2)
{   
    /* on crée un tableau à 2 dimensions qui contient 8 combinaisons. Elles représentent chacune l'ensemble des cases autour de la case vide */
    int c0, c1, combinaisons_resultat[8][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, -1}, {-1, 1}, {1, 1}, {-1, -1}};

    for (int i = 0; i < 8; i++)
    {   
        /* c0 représente la combinaison qu'on affecte à la coordonnée x et c1 celle de la coordonnée y */
        c0 = combinaisons_resultat[i][0]; c1 = combinaisons_resultat[i][1];
        /* si les 2 coordonnées, avec l'ajout de leur combinaison, sont encore comprises dans les limites du tableau... */
        if ((0 <= x+c0 && x+c0 <= L-1) && (0 <= y+c1 && y+c1 <= C-1))
        {   
            /* ...alors cela veut dire qu'il existe une valeur dans la case de cette coordonnée et on l'ajoute à la somme qui lui correspond */
            if (tab[x+c0][y+c1] > 0) *score1 += tab[x+c0][y+c1]; 
            else *score2 += abs(tab[x+c0][y+c1]);
        }
    }
}


void fin_partie(joueur* j1, joueur* j2, short* play, int id_game)
{
    char temp1[50], rejouer; 
    int  temp2[50];
 
    printf("\n\nVoulez vous rejouer une partie ? (O/N) ");
    scanf("%c", &rejouer);
    
    if (rejouer == 'O' || rejouer == 'o')
    {   
        /* on inverse le pseudo et le score total des joueurs. Cela fait en sorte que les joueurs s'alternent pour commencer le jeu */
        strncpy(temp1, (*j1).pseudo, 50);
        strncpy((*j1).pseudo, (*j2).pseudo, 50);
        strncpy((*j2).pseudo, temp1, 50);
        
        memcpy(temp2, (*j1).score_total, 50);
        memcpy((*j1).score_total, (*j2).score_total, 50);
        memcpy((*j2).score_total, temp2, 50);
    }
    else
    {   
        int score_total_1 = 0, score_total_2 = 0;
        /* en cas de fin de partie, on affiche les scores finaux de chaque joueur pour toutes les parties et les scores totaux */
        system("clear");
        for (int i = 0; i < id_game; i++)
        {   
            printf("\n\n\e[4;36mRésultat de la partie %d :\e[0m\n", i+1);
            printf("\n%-8s : %d\n%-8s : %d\n", (*j1).pseudo, (*j1).score_total[i], (*j2).pseudo, (*j2).score_total[i]);
            score_total_1 += (*j1).score_total[i];
            score_total_2 += (*j2).score_total[i];
        }
        printf("\n\e[0;35mScore total du joueur %-8s : %d \n", (*j1).pseudo, score_total_1);
        printf("Score total du \e[0;35mjoueur %-8s : %d \e[0m\n", (*j2).pseudo, score_total_2);

        /* on affiche le nom du gagnant et son score */
        if (score_total_1 < score_total_2) printf("\n\e[0;33mVictoire pour le joueur %s avec un total de %d points ! \U0001F947\e[0m\n\n", (*j1).pseudo, score_total_1);
        else if (score_total_2 < score_total_1) printf("\n\e[0;33mVictoire pour le joueur %s avec un total de %d points ! \U0001F947\e[0m\n\n", (*j2).pseudo, score_total_2);
        else printf("\n\e[0;33mMatch nul entre les deux joueurs \U0001F642\e[0m\n\n");

        *play = 0;
    }
}


