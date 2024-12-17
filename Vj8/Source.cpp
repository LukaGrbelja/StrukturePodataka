#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1
#define SCANF_ERROR -2

typedef struct _stablo {
    int vrijednost;
    struct _stablo* manje;
    struct _stablo* vise;
} Stablo;

Stablo* stvoriStablo(int);
int dodaj(Stablo *, int);
Stablo* trazi(Stablo *, int);
Stablo* brisi(Stablo *, int);
Stablo* nadiMinimum(Stablo *);
void inorderIspis(Stablo *);
void postorderIspis(Stablo *);
void preorderIspis(Stablo *);
void levelorderIspis(Stablo *, int);
void oslobodiStablo(Stablo *);

int main() {
    
    int brojevi[] = {8, 3, 12, 5, 7, 15, 6, 4, 9, 13, 10};

    Stablo* korijen = stvoriStablo(brojevi[0]);
    if (korijen == NULL) return MALLOC_ERROR;
    
    for (int i = 1; i < (int)(sizeof(brojevi)/sizeof(int)); i++) {
        if (dodaj(korijen, brojevi[i])) return MALLOC_ERROR;
    }
    
    inorderIspis(korijen);
    printf("\n");
    preorderIspis(korijen);
    printf("\n");
    postorderIspis(korijen);
    printf("\n");
    levelorderIspis(korijen, 0);
    printf("\n");
    
    Stablo* rezultat = trazi(korijen, 4);
    if (rezultat) printf("Pronađena vrijednost: %d\n", rezultat->vrijednost);
    else printf("Vrijednost nije pronađena.\n");

    printf("\nBrisanje vrijednosti 3:\n");
    korijen = brisi(korijen, 12);
    levelorderIspis(korijen, 0);
    
    oslobodiStablo(korijen);
    
    return EXIT_SUCCESS;
}

Stablo* stvoriStablo(int vrijednost) {

    Stablo* novoStablo = NULL;
    novoStablo = (Stablo*)malloc(sizeof(Stablo));

    if (novoStablo == NULL) {
        printf("Greška u alociranju memorije!\n");
        return NULL;
    }

    novoStablo->vrijednost = vrijednost;
    novoStablo->manje = NULL;
    novoStablo->vise = NULL;
    
    return novoStablo;
}

int dodaj(Stablo* cvor, int vrijednost) {
    
    if (cvor == NULL) return MALLOC_ERROR;

    if (cvor->vrijednost > vrijednost) {
        if (cvor->manje == NULL) {

            cvor->manje = stvoriStablo(vrijednost);
            if (cvor->manje == NULL) return MALLOC_ERROR;

        } else return dodaj(cvor->manje, vrijednost);
    }
    else if (cvor->vrijednost < vrijednost) {
        if (cvor->vise == NULL) {

            cvor->vise = stvoriStablo(vrijednost);
            if (cvor->vise == NULL) return MALLOC_ERROR;

        } else return dodaj(cvor->vise, vrijednost);
    }
    
    return EXIT_SUCCESS;
}

Stablo* trazi(Stablo* cvor, int trazenaVrijednost) {
    
    if (cvor == NULL || cvor->vrijednost == trazenaVrijednost) return cvor;
    if (cvor->vrijednost > trazenaVrijednost) return trazi(cvor->manje, trazenaVrijednost);
    else return trazi(cvor->vise, trazenaVrijednost);
    
}

Stablo* brisi(Stablo* cvor, int trazenaVrijednost) {
    
    if (cvor == NULL) return NULL;

    if (cvor->vrijednost > trazenaVrijednost) cvor->manje = brisi(cvor->manje, trazenaVrijednost);
    else if (cvor->vrijednost < trazenaVrijednost) cvor->vise = brisi(cvor->vise, trazenaVrijednost);
    else {

        if (cvor->manje == NULL) {

            Stablo* trenutni = cvor->vise;
            free(cvor);
            return trenutni;

        }
        else if (cvor->vise == NULL) {

            Stablo* trenutni = cvor->manje;
            free(cvor);
            return trenutni;
        
        }
        
        Stablo* trenutni = nadiMinimum(cvor->vise);
        cvor->vrijednost = trenutni->vrijednost;
        cvor->vise = brisi(cvor->vise, trenutni->vrijednost);
        
    }
    
    return cvor;
}

Stablo* nadiMinimum(Stablo* cvor) {

    while (cvor->manje != NULL) cvor = cvor->manje;
    return cvor;

}

void inorderIspis(Stablo* cvor) {

    if (cvor->manje != NULL) inorderIspis(cvor->manje);
    printf("%d ", cvor->vrijednost);
    if (cvor->vise != NULL) inorderIspis(cvor->vise);

}

void postorderIspis(Stablo* cvor) {

    if (cvor->manje != NULL) postorderIspis(cvor->manje);
    if (cvor->vise != NULL) postorderIspis(cvor->vise);
    printf("%d ", cvor->vrijednost);

}

void preorderIspis(Stablo* cvor) {

    printf("%d ", cvor->vrijednost);
    if (cvor->manje != NULL) preorderIspis(cvor->manje);
    if (cvor->vise != NULL) preorderIspis(cvor->vise);

}

void levelorderIspis(Stablo* cvor, int trenutnaRazina) {

    static int trenutnaTrazenaRazina = 0, brojNadenihElemenata = 0;
    
    if (trenutnaTrazenaRazina == trenutnaRazina) {
        printf("%d ", cvor->vrijednost);
        brojNadenihElemenata = 1;
    }
    else {
        if (cvor->manje != NULL) levelorderIspis(cvor->manje, trenutnaRazina + 1);
        if (cvor->vise != NULL) levelorderIspis(cvor->vise, trenutnaRazina + 1);
    }
    
    if (!trenutnaRazina && brojNadenihElemenata) {
        trenutnaTrazenaRazina++;
        brojNadenihElemenata = 0;
        levelorderIspis(cvor, trenutnaRazina);
    }
    
    if (!trenutnaRazina && !brojNadenihElemenata) {
        trenutnaTrazenaRazina = 0;
        brojNadenihElemenata = 0;
    }
}

void oslobodiStablo(Stablo* cvor) {
    if (cvor != NULL) {

        oslobodiStablo(cvor->manje);
        oslobodiStablo(cvor->vise);
        free(cvor);

    }
}