    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <limits.h>

    // les constantes
    #define NB_LIGNES 6
    #define NB_COLONNES 7
    #define PION_A 'X'
    #define PION_B 'O'
    #define VIDE ' '
    #define INCONNU ' '

    // les types
    typedef int Grille[NB_LIGNES][NB_COLONNES];

    // prototypes des fonctions
    void initGrille(Grille laGrille);
    void afficher(Grille laGrille, char pion);
    bool grillePleine(Grille laGrille);
    void jouerA(Grille laGrille, char pion, int * ligne, int * colonne);
    void jouerB(Grille laGrille, char pion, int * ligne, int * colonne);
    int choisirColonneB(Grille laGrille, char pion, int MaxProf);
    int minimax(Grille laGrille, int profondeur, int MaxProf, bool estMax, int alpha, int beta);
    int evaluer(Grille laGrille, char pion);
    int choisirColonne(Grille laGrille, char pion);
    int chercherLigne(Grille laGrille, int col);
    bool estVainqueur(Grille laGrille, int lig, int col);
    void finDePartie(char vainqueur);


    // Programme principal. C'est le pion A qui commence Ã  jouer
    int main()
    {
        Grille laGrille;
        char vainqueur=INCONNU;
        int ligne, colonne;
        initGrille(laGrille);
        afficher(laGrille, PION_A);
        while (vainqueur==INCONNU && !grillePleine(laGrille)){
            jouerA(laGrille,PION_A, &ligne, &colonne);
            afficher(laGrille, PION_B);
            if (estVainqueur(laGrille, ligne, colonne) ){
                vainqueur = PION_A;
            } else if (!grillePleine(laGrille)){
                jouerB(laGrille, PION_B, &ligne, &colonne);
                afficher(laGrille, PION_A);
                if (estVainqueur(laGrille, ligne, colonne) ){
                    vainqueur = PION_B;
                }
            }
        }
        finDePartie(vainqueur);
        system(EXIT_SUCCESS);
    }


    void initGrille(Grille laGrille){
        int l, c;
        for (l=0 ; l<NB_LIGNES ; l++){
            for (c=0 ; c<NB_COLONNES ; c++){
                laGrille[l][c] = VIDE;
            }
        }
    }

    void afficher(Grille laGrille, char pion){
        int l, c;
        system("cls");
        printf("\t");
        printf("  %c\n", pion);
        printf("\t");
        for (c=0; c<NB_COLONNES ; c++){
            printf("----");
        }
        printf("-\n");
        for (l=0 ; l<NB_LIGNES ; l++){
            printf("\t");
            for (c=0; c<NB_COLONNES ; c++){
                printf("| %c ", laGrille[l][c]);
            }
            printf("|\n");
            printf("\t");
            for (c=0; c<NB_COLONNES ; c++){
                printf("----");
            }
            printf("-\n");
        }
        printf("\t");
        for (c=0; c<NB_COLONNES ; c++){
                printf("  %d ",c);
        }
        printf("\n\n");

    }

    bool grillePleine(Grille laGrille){
        bool pleine = true;
        int c = 0;
        while (pleine && c<NB_COLONNES){
            if (laGrille[0][c] == VIDE){
                pleine = false;
            }
            c++;
        }
        return pleine;
    }

    void jouerA(Grille laGrille, char pion, int * ligne, int * colonne ){
    *ligne = -1;
        do {
            *colonne = choisirColonne(laGrille, pion);
            *ligne = chercherLigne(laGrille, *colonne);
        }while (*ligne==-1);
        laGrille[*ligne][*colonne] = pion;
    }
    void jouerB(Grille laGrille, char pion, int * ligne, int * colonne) {
    int MaxProf = 7;
    *colonne = choisirColonneB(laGrille, pion, MaxProf);
    printf("Colonne choisie : %d", *colonne);
    *ligne = chercherLigne(laGrille, *colonne);
    laGrille[*ligne][*colonne] = pion;
    }

    int choisirColonneB(Grille laGrille, char pion, int MaxProf) {
    int col = 0;
    int score = -1000;
    int scoreTemp;
    int ligne;
    int colonne;
    
    for (int i = 0; i < NB_COLONNES; i++) {
        ligne = chercherLigne(laGrille, i);
        if (ligne != -1) {
            laGrille[ligne][i] = pion;
            scoreTemp = minimax(laGrille, 0, MaxProf, false, -1000, 1000);
            laGrille[ligne][i] = VIDE;
            if (scoreTemp > score) {
                score = scoreTemp;
                col = i;
            }
        }
    }
    return col;
    }



    int choisirColonne(Grille laGrille, char pion){
        int col;

        do{
            printf("Numero de colonne ? ");
            scanf("%d", &col);
        } while (col<0 ||col>6);
        return col;
    }

    int chercherLigne(Grille laGrille, int col){
        int ligne = -1;

        while (ligne<NB_LIGNES-1 && laGrille[ligne+1][col]==VIDE){
            ligne++;
        }
        return ligne;
    }

int minimax(Grille laGrille, int profondeur, int MaxProf, bool estMax, int alpha, int beta) {

    int score = evaluer(laGrille, PION_B);

    if (profondeur == MaxProf || score == 100 || score == -100) {
        return score;
    }

    if (estMax) {
        int best = INT_MIN;
        int bestCol = -1;
        for (int i = 0; i < 7; i++) {
            if (laGrille[0][i] == VIDE) {
                int ligne = chercherLigne(laGrille, i);
                laGrille[ligne][i] = PION_B;
                int currentScore = minimax(laGrille, profondeur + 1, MaxProf, !estMax, alpha, beta);
                laGrille[ligne][i] = VIDE;
                if (currentScore > best) {
                    best = currentScore;
                    bestCol = i;
                }
                alpha = fmax(alpha, best);
                if (beta <= alpha) {
                    break;
                }
            }
        }
        if (profondeur == 0) {
            return bestCol;
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int i = 0; i < 7; i++) {
            if (laGrille[0][i] == VIDE) {
                int ligne = chercherLigne(laGrille, i);
                laGrille[ligne][i] = PION_A;
                best = fmin(best, minimax(laGrille, profondeur + 1, MaxProf, !estMax, alpha, beta));
                laGrille[ligne][i] = VIDE;
                beta = fmin(beta, best);
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return best;
    }
}



int evaluer(Grille laGrille, char pion) {
    int score = 0;
    int contre = (pion == 'X') ? 'O' : 'X';

    // Check for horizontal wins
    for (int lig = 0; lig < 6; lig++) {
        for (int col = 0; col < 4; col++) {
            if (laGrille[lig][col] == pion && laGrille[lig][col+1] == pion && laGrille[lig][col+2] == pion && laGrille[lig][col+3] == pion) {
                score += 10000;
            }
            if (laGrille[lig][col] == contre && laGrille[lig][col+1] == contre && laGrille[lig][col+2] == contre && laGrille[lig][col+3] == contre) {
                score -= 10000;
            }
        }
    }

    // Check for vertical wins
    for (int col = 0; col < 7; col++) {
        for (int lig = 0; lig < 3; lig++) {
            if (laGrille[lig][col] == pion && laGrille[lig+1][col] == pion && laGrille[lig+2][col] == pion && laGrille[lig+3][col] == pion) {
                score += 10000;
            }
            if (laGrille[lig][col] == contre && laGrille[lig+1][col] == contre && laGrille[lig+2][col] == contre && laGrille[lig+3][col] == contre) {
                score -= 10000;
            }
        }
    }

    // Check for diagonal wins
    for (int lig = 0; lig < 3; lig++) {
        for (int col = 0; col < 4; col++) {
            if (laGrille[lig][col] == pion && laGrille[lig+1][col+1] == pion && laGrille[lig+2][col+2] == pion && laGrille[lig+3][col+3] == pion) {
                score += 10000;
            }
            if (laGrille[lig][col] == contre && laGrille[lig+1][col+1] == contre && laGrille[lig+2][col+2] == contre && laGrille[lig+3][col+3] == contre) {
                score -= 10000;
            }
        }
    }

    for (int lig = 0; lig < 3; lig++) {
        for (int col = 3; col < 7; col++) {
            if (laGrille[lig][col] == pion && laGrille[lig+1][col-1] == pion && laGrille[lig+2][col-2] == pion && laGrille[lig+3][col-3] == pion) {
                score += 10000;
            }
            if (laGrille[lig][col] == contre && laGrille[lig+1][col-1] == contre && laGrille[lig+2][col-2] == contre && laGrille[lig+3][col-3] == contre) {
                score -= 10000;
            }
        }
    }

        // Check for 3 in a row horizontal
    for (int lig = 0; lig < 6; lig++) {
        for (int col = 0; col < 4; col++) {
            if (laGrille[lig][col] == pion && laGrille[lig][col+1] == pion && laGrille[lig][col+2] == pion) {
                score += 500;
            }
            if (laGrille[lig][col] == contre && laGrille[lig][col+1] == contre && laGrille[lig][col+2] == contre) {
                score -= 500;
            }
        }
    }

    // Check for 3 in a row vertical
    for (int col = 0; col < 7; col++) {
        for (int lig = 0; lig < 3; lig++) {
            if (laGrille[lig][col] == pion && laGrille[lig+1][col] == pion && laGrille[lig+2][col] == pion) {
                score += 500;
            }
            if (laGrille[lig][col] == contre && laGrille[lig+1][col] == contre && laGrille[lig+2][col] == contre) {
                score -= 500;
            }
        }
    }

    // Check for 3 in a row diagonal (left to right)
    for (int lig = 0; lig < 3; lig++) {
        for (int col = 0; col < 4; col++) {
            if (laGrille[lig][col] == pion && laGrille[lig+1][col+1] == pion && laGrille[lig+2][col+2] == pion) {
                score += 500;
            }
            if (laGrille[lig][col] == contre && laGrille[lig+1][col+1] == contre && laGrille[lig+2][col+2] == contre) {
                score -= 500;
            }
        }
    }

    // Check for 3 in a row diagonal (bot-right to top-left)

    for (int lig = 3; lig < 6; lig++) {
        for (int col = 0; col < 4; col++) {
            if (laGrille[lig][col] == pion && laGrille[lig-1][col+1] == pion && laGrille[lig-2][col+2] == pion) {
                score += 500;
            }
            if (laGrille[lig][col] == contre && laGrille[lig-1][col+1] == contre && laGrille[lig-2][col+2] == contre) {
                score -= 500;
            }
        }
    }



    // Check for 3 in a row diagonal (right to left)
    for (int lig = 0; lig < 3; lig++) {
        for (int col = 3; col < 7; col++) {
            if (laGrille[lig][col] == pion && laGrille[lig+1][col-1] == pion && laGrille[lig+2][col-2] == pion) {
                score += 500;
            }
            if (laGrille[lig][col] == contre && laGrille[lig+1][col-1] == contre && laGrille[lig+2][col-2] == contre) {
                score -= 500;
            }
        }
    }

    // Check for 2 in a row horizontal  

    for (int lig = 0; lig < 6; lig++) {
        for (int col = 0; col < 5; col++) {
            if (laGrille[lig][col] == pion && laGrille[lig][col+1] == pion) {
                score += 100;
            }
            if (laGrille[lig][col] == contre && laGrille[lig][col+1] == contre) {
                score -= 100;
            }
        }
    }

    // Check for 2 in a row vertical

    for (int col = 0; col < 7; col++) {
        for (int lig = 0; lig < 4; lig++) {
            if (laGrille[lig][col] == pion && laGrille[lig+1][col] == pion) {
                score += 100;
            }
            if (laGrille[lig][col] == contre && laGrille[lig+1][col] == contre) {
                score -= 100;
            }
        }
    }

    // Check for 2 in a row diagonal (left to right)

    for (int lig = 0; lig < 4; lig++) {
        for (int col = 0; col < 5; col++) {
            if (laGrille[lig][col] == pion && laGrille[lig+1][col+1] == pion) {
                score += 100;
            }
            if (laGrille[lig][col] == contre && laGrille[lig+1][col+1] == contre) {
                score -= 100;
            }
        }
    }

    // Check for 2 in a row diagonal (right to left)

    for (int lig = 0; lig < 4; lig++) {
        for (int col = 3; col < 7; col++) {
            if (laGrille[lig][col] == pion && laGrille[lig+1][col-1] == pion) {
                score += 100;
            }
            if (laGrille[lig][col] == contre && laGrille[lig+1][col-1] == contre) {
                score -= 100;
            }
        }
    }

    // Joue la column 3 si VIDE

    if (laGrille[0][3] == VIDE) {
        score += 100000;
    }

    // Si winning move on next turn, play it

    for (int col = 0; col < 7; col++) {
        if (laGrille[0][col] == VIDE) {
            laGrille[0][col] = pion;
            if (estVainqueur(laGrille, 0, col)) {
                score += 1000000;
            }
            laGrille[0][col] = VIDE;
        }
    }

    return score;

}



    bool estVainqueur(Grille laGrille, int lig, int col){
        // consiste Ã  regarder si une ligne de 4 pions s'est formÃ©e autour du pion qui vient de tomber en (lig, col)
        char lePion = laGrille[lig][col];
        int cpt,i,j;

        // regarder la verticale, en comptant le nombre de pions au Sud (inutile de regarder au Nord du pion qui vient de tomber)
        i = lig;
        cpt = 0;
        while (i<NB_LIGNES && laGrille[i][col]==lePion){
            cpt++;
            i++;
        }
        if (cpt>=4){
            return true;
        }
        // regarder l'horizontale, en comptant le nombre de pions Ã  l'Est et Ã  l'Ouest
        j = col;
        cpt = 0;
        // on regarde Ã  l'est
        while (j>=0 && laGrille[lig][j]==lePion){
            cpt++;
            j--;
        }
        j = col+1;
        // on regarde Ã  l'ouest
        while (j<NB_COLONNES && laGrille[lig][j]==lePion){
            cpt++;
            j++;
        }
        if (cpt>=4 ){
            return true;
        }
        // regarder la diagonale descendante, en comptant le nombre de pions au Nord Ouest et au Sud Est
        i = lig;
        j = col;
        cpt = 0;
        // on regarde au Nord Ouest
        while (j>=0 && i>=0 && laGrille[i][j]==lePion){
            cpt++;
            i--;
            j--;
        }
        i = lig+1;
        j = col+1;
        // on regarde au Sud Est
        while (i<NB_LIGNES && j<NB_COLONNES && laGrille[i][j]==lePion){
            cpt++;
            i++;
            j++;
        }
        if (cpt>=4 ){
            return true;
        }
        // regarder la diagonale descendante, en comptant le nombre de pions au Nord Est et au Sud Ouest
        i = lig;
        j = col;
        cpt = 0;
        // on regarde au Nord Est
        while (j<NB_COLONNES && i>=0 && laGrille[i][j]==lePion){
            cpt++;
            i--;
            j++;
        }
        i = lig+1;
        j = col-1;
        // on regarde au Sud Ouest
        while (i<NB_LIGNES && j>=0 && laGrille[i][j]==lePion){
            cpt++;
            i++;
            j--;
        }
        if (cpt>=4 ){
            return true;
        }
        return false;
    }

    void finDePartie(char vainqueur){
        if (vainqueur != INCONNU){
            printf("Joueur %c vainqueur\n", vainqueur);
        } else {
            printf("MATCH NUL");
        }
    }

