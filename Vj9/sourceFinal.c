#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1
#define FILE_OPEN_ERROR -2

typedef struct _stablo {
    int vrijednost;
    struct _stablo* vise;
    struct _stablo* manje;
} Stablo;

Stablo* insert(Stablo* cvor, int dodanaVrijednost);
int replace(Stablo* cvor);
int oslobodiStablo(Stablo* cvor);
int ispisUDAtoteku(Stablo* cvor, FILE* pokazivacDatoteke, int level);
void inorderIspis(Stablo* cvor);

int main() {

    Stablo* korijen = NULL;
    int niz[15];
    unsigned int seed;
    
    printf("Upiši vrijednost za randomiziranje: ");
    scanf("%u", &seed);
    srand(seed);
    
    for (int i = 0; i < 15; i++) {
        niz[i] = rand() % 79 + 11;
        printf("%d ", niz[i]);
    }
    printf("\n");

    for (int i : niz) if (!(korijen = insert(korijen, i))) return MALLOC_ERROR;
    
    FILE* pokazivacDatoteke = NULL;
	pokazivacDatoteke = fopen("ispis.txt", "w");
	if (!pokazivacDatoteke) return FILE_OPEN_ERROR;

    inorderIspis(korijen);
    printf("\n");
	fprintf(pokazivacDatoteke, "Originalno stablo:\n");
    ispisUDAtoteku(korijen, pokazivacDatoteke, 0);

    replace(korijen);

    inorderIspis(korijen);
    printf("\n");
	fprintf(pokazivacDatoteke, "\nStablo nakon promjene:\n");
    ispisUDAtoteku(korijen, pokazivacDatoteke, 0);
    
    fclose(pokazivacDatoteke);
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


int ispisUDAtoteku(Stablo* cvor, FILE* pokazivacDatoteke, int level) {
    
	if (cvor->vise != NULL) ispisUDAtoteku(cvor->vise, pokazivacDatoteke, level + 1);
	for (int i = 0; i < level; i++) fprintf(pokazivacDatoteke, "\t");
	
	fprintf(pokazivacDatoteke, "%d\n", cvor->vrijednost);
	if (cvor->manje != NULL) ispisUDAtoteku(cvor->manje, pokazivacDatoteke, level + 1);

	return 0;
}

void inorderIspis(Stablo *cvor) {

    if (cvor->manje != NULL) inorderIspis(cvor->manje);
    printf("%d ", cvor->vrijednost);
    if (cvor->vise != NULL) inorderIspis(cvor->vise);

}