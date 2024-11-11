#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define EXIT_SUCCESS 0
#define FILE_ERROR_OPEN -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

typedef struct _monom {
    int koeficijent;
    int eksponent;
    struct _monom* iduci;
} Monom;

Monom* stvoriNoviMonom(int, int);
int ucitajPolinomeIzDatoteke(Monom*, Monom*);
int pretvoranjaNizaUPolinom(Monom*, char*);
int dodajMonomUPolinom(Monom*, Monom*);
int ispisPolinoma(Monom*);
int zbrojiPolinome(Monom*, Monom*, Monom*);
int pomnoziPolinome(Monom*, Monom*, Monom*);

int main() {
    Monom polinom1 = { 0 };
    Monom polinom2 = { 0 };
    Monom suma = { 0 };
    Monom produkt = { 0 };

    if (ucitajPolinomeIzDatoteke(&polinom1, &polinom2) != EXIT_SUCCESS) {
        printf("Greška prilikom učitavanja polinoma.\n");
        return FILE_ERROR_OPEN;
    }

    printf("Polinom 1: ");
    ispisPolinoma(polinom1.iduci);
    printf("Polinom 2: ");
    ispisPolinoma(polinom2.iduci);

    zbrojiPolinome(&suma, &polinom1, &polinom2);
    printf("Suma polinoma: ");
    ispisPolinoma(suma.iduci);

    pomnoziPolinome(&produkt, &polinom1, &polinom2);
    printf("Produkt polinoma: ");
    ispisPolinoma(produkt.iduci);

    return EXIT_SUCCESS;
}

Monom* stvoriNoviMonom(int koeficijent, int eksponent) {
    Monom* noviMonom = (Monom*)malloc(sizeof(Monom));
    if (!noviMonom) return NULL;

    noviMonom->koeficijent = koeficijent;
    noviMonom->eksponent = eksponent;
    noviMonom->iduci = NULL;
    return noviMonom;
}

int ucitajPolinomeIzDatoteke(Monom* polinom1, Monom* polinom2) {
    char buffer[MAX_LINE] = { 0 };

    FILE* pokazivacDatoteke = fopen("polinomi.txt", "r");
    if (pokazivacDatoteke == NULL) return FILE_ERROR_OPEN;

    fgets(buffer, MAX_LINE, pokazivacDatoteke);
    pretvoranjaNizaUPolinom(polinom1, buffer);

    fgets(buffer, MAX_LINE, pokazivacDatoteke);
    pretvoranjaNizaUPolinom(polinom2, buffer);

    fclose(pokazivacDatoteke);
    return EXIT_SUCCESS;
}

int pretvoranjaNizaUPolinom(Monom* polinom, char* nizMonoma) {
    int pomak = 0, i, koeficijent, eksponent;
    Monom* noviMonom = NULL;

    while (sscanf(nizMonoma + pomak, " %dx^%d %n", &koeficijent, &eksponent, &i) == 2) {
        noviMonom = stvoriNoviMonom(koeficijent, eksponent);
        if (noviMonom == NULL) return MALLOC_ERROR;

        dodajMonomUPolinom(polinom, noviMonom);
        pomak += i;
    }

    return EXIT_SUCCESS;
}

int dodajMonomUPolinom(Monom* polinom, Monom* monom) {
    Monom* prethodni = polinom;
    Monom* temp = NULL;

    if (monom->koeficijent == 0) {
        free(monom);
        return EXIT_SUCCESS;
    }

    while (prethodni->iduci != NULL && prethodni->iduci->eksponent > monom->eksponent) {
        prethodni = prethodni->iduci;
    }

    if (prethodni->iduci != NULL && monom->eksponent == prethodni->iduci->eksponent) {
        prethodni->iduci->koeficijent += monom->koeficijent;
        free(monom);

        if (prethodni->iduci->koeficijent == 0) {
            temp = prethodni->iduci;
            prethodni->iduci = prethodni->iduci->iduci;
            free(temp);
        }
    } else {
        monom->iduci = prethodni->iduci;
        prethodni->iduci = monom;
    }

    return EXIT_SUCCESS;
}

int ispisPolinoma(Monom* polinom) {
    Monom* trenutni = polinom;

    if (trenutni == NULL) {
        printf("Polinom je prazan.\n");
        return EXIT_SUCCESS;
    }

    while (trenutni != NULL) {
        if (trenutni->eksponent == 0) {
            printf("%d ", trenutni->koeficijent);
        } else if (trenutni->eksponent == 1) {
            printf("%dx ", trenutni->koeficijent);
        } else {
            printf("%dx^%d ", trenutni->koeficijent, trenutni->eksponent);
        }

        trenutni = trenutni->iduci;
    }
    printf("\n");

    return EXIT_SUCCESS;
}

int zbrojiPolinome(Monom* zbroj, Monom* polinom1, Monom* polinom2) {
    Monom* temp = NULL;

    while (polinom1 != NULL) {
        temp = (Monom*)malloc(sizeof(Monom));
        if (temp == NULL) return MALLOC_ERROR;

        temp->koeficijent = polinom1->koeficijent;
        temp->eksponent = polinom1->eksponent;

        dodajMonomUPolinom(zbroj, temp);
        polinom1 = polinom1->iduci;
    }

    while (polinom2 != NULL) {
        temp = (Monom*)malloc(sizeof(Monom));
        if (temp == NULL) return MALLOC_ERROR;

        temp->koeficijent = polinom2->koeficijent;
        temp->eksponent = polinom2->eksponent;

        dodajMonomUPolinom(zbroj, temp);
        polinom2 = polinom2->iduci;
    }

    return EXIT_SUCCESS;
}

int pomnoziPolinome(Monom* proizvod, Monom* polinom1, Monom* polinom2) {
    Monom* temp = NULL;
    Monom* polinom2Prvi = polinom2;

    // Za svaki monom iz prvog polinoma
    while (polinom1 != NULL) {
        polinom2 = polinom2Prvi;

        // Množi se s monomima iz drugog polinoma
        while (polinom2 != NULL) {
            temp = (Monom*)malloc(sizeof(Monom));
            if (temp == NULL) return MALLOC_ERROR;

            temp->koeficijent = polinom1->koeficijent * polinom2->koeficijent;
            temp->eksponent = polinom1->eksponent + polinom2->eksponent;

            dodajMonomUPolinom(proizvod, temp);
            polinom2 = polinom2->iduci;
        }
        polinom1 = polinom1->iduci;
    }

    return EXIT_SUCCESS;
}