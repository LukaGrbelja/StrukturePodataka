#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_NUM 10

#define INVALID_INPUT 2
#define EMPTY_STACK 1
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1
#define FILE_OPEN_ERROR -2

typedef struct _elementStoga {
    float vrijednost;
    struct _elementStoga* iduci;
} ElementStoga;

typedef struct {
    ElementStoga* vrh;
} Stog;

int push(Stog*, float);
int pop(Stog*, float*);
int izracunaj(Stog*, char);
int ucitajIzDatoteke(char*);
void oslobodiStog(Stog*);

int main() {

    char trenutniChar[MAX_NUM] = { 0 }, postfiks[MAX_LINE] = { 0 };
    
    if (int greska = ucitajIzDatoteke(postfiks)) return greska;
    printf("Postfiks: %s\n", postfiks);

    int pomak = 0, ukupniPomak = 0;
    Stog stog = { 0 };

    while (sscanf(postfiks + ukupniPomak, " %s %n", trenutniChar, &pomak) == 1) {

        if (trenutniChar[0] >= 48 && trenutniChar[0] <= 57) {
            if (push(&stog, atof(trenutniChar))) return MALLOC_ERROR;
        }
        else {
            int greska = izracunaj(&stog, trenutniChar[0]);
            if (greska) return greska;
        }

        ukupniPomak += pomak;
    }

    float rezultat = 0;
    pop(&stog, &rezultat);

    printf("Rezultat: %g\n", rezultat);

    oslobodiStog(&stog);

    return 0;
}

int push(Stog* stog, float dodanaVrijednost) {

    ElementStoga* clanJednadzbe = NULL;
    clanJednadzbe = (ElementStoga*)malloc(sizeof(ElementStoga));

    if (clanJednadzbe == NULL) {
        printf("Greška pri alociranju memorije!\n");
        return MALLOC_ERROR;
    }

    clanJednadzbe->vrijednost = dodanaVrijednost;
    clanJednadzbe->iduci = stog->vrh;
    stog->vrh = clanJednadzbe;

    return EXIT_SUCCESS;
}

int pop(Stog* stog, float* spremljenaVrijednost) {

    if (stog->vrh == NULL) return EMPTY_STACK;

    ElementStoga* vrh = stog->vrh;
    *spremljenaVrijednost = vrh->vrijednost;
    stog->vrh = vrh->iduci;
    free(vrh);

    return EXIT_SUCCESS;
}

int izracunaj(Stog* stog, char racunskiOperator) {
    
    float prviOperand, drugiOperand, rezultat;

    if (pop(stog, &drugiOperand) || pop(stog, &prviOperand)) {
        printf("Greška: Stog je prazan.\n");
        return EMPTY_STACK;
    }

    switch (racunskiOperator) {
        case '+':
            rezultat = prviOperand + drugiOperand;
            break;
        case '-':
            rezultat = prviOperand - drugiOperand;
            break;
        case '*':
            rezultat = prviOperand * drugiOperand;
            break;
        case '/':
            if (!drugiOperand) {
                printf("Greška: Dijeljenje s nulom.\n");
                return INVALID_INPUT;
            }
            rezultat = prviOperand / drugiOperand;
            break;
        default:
            printf("Neispravan operator: %c\n", racunskiOperator);
            return INVALID_INPUT;
    }

    return push(stog, rezultat);
}

int ucitajIzDatoteke(char* postfiks) {
    
    FILE* pokazivacDatoteke = NULL;
    pokazivacDatoteke = fopen("postfix.txt", "r");

    if (!pokazivacDatoteke) {
        printf("Greška u otvaranju datoteke.\n");
        return FILE_OPEN_ERROR;
    }

    if (!fgets(postfiks, MAX_LINE, pokazivacDatoteke)) {
        printf("Greška: Neispravan format ili čitanje podataka iz datoteke.\n");
        fclose(pokazivacDatoteke);
        return INVALID_INPUT;
    }

    fclose(pokazivacDatoteke);
    
    return EXIT_SUCCESS;
}

void oslobodiStog(Stog* stog) {

    float temp;
    while (pop(stog, &temp) == EXIT_SUCCESS) {}

}