#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1

typedef struct _stablo {
    int vrijednost;
    struct _stablo* vise;
    struct _stablo* manje;
} Stablo;

Stablo* insert(Stablo* cvor, int dodanaVrijednost);
int replace(Stablo* cvor);
int oslobodiStablo(Stablo* cvor);
void inorderIspis(Stablo* cvor);

int main() {
    
    Stablo* korijen = NULL;
    int niz[] = {2, 5, 7, 8, 11, 1, 4, 2, 3, 7};

    for (int i : niz) {

        if (!(korijen = insert(korijen, i))) return MALLOC_ERROR;
        inorderIspis(korijen);
        printf("\n");
        
    }

    replace(korijen);
    inorderIspis(korijen);
    oslobodiStablo(korijen);

    return 0;
}

Stablo *insert(Stablo *cvor, int dodanaVrijednost) {

    if (cvor == NULL) {

        cvor = (Stablo *)malloc(sizeof(Stablo));
        if (cvor == NULL) return NULL;

        cvor->vrijednost = dodanaVrijednost;
        cvor->vise = NULL;
        cvor->manje = NULL;

    }
    else if (dodanaVrijednost < cvor->vrijednost) {
        cvor->vise = insert(cvor->vise, dodanaVrijednost);
    }
    else { cvor->manje = insert(cvor->manje, dodanaVrijednost); }

    return cvor;
}

int replace(Stablo *cvor) {
    if (cvor == NULL) return 0;

    int vrijednost = cvor->vrijednost;
    cvor->vrijednost = replace(cvor->vise) + replace(cvor->manje);

    return vrijednost + cvor->vrijednost;
}

int oslobodiStablo(Stablo *cvor) {

    if (cvor == NULL) return EXIT_SUCCESS;

    oslobodiStablo(cvor->vise);
    oslobodiStablo(cvor->manje);
    free(cvor);

    return EXIT_SUCCESS;
}

void inorderIspis(Stablo *cvor) {

    if (cvor->manje != NULL) inorderIspis(cvor->manje);
    printf("%d ", cvor->vrijednost);
    if (cvor->vise != NULL) inorderIspis(cvor->vise);

}