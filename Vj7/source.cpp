#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50

#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1
#define INVALID_INPUT -2

typedef struct _stablo {
    char imeStabla[MAX_LENGTH];
    struct _stablo* iduci;
    struct _stablo* dijete;
} Stablo;

typedef struct _elementListe {
    struct _elementListe* prosli;
    Stablo* element;
    struct _elementListe* iduci;
} ElementListe;

typedef struct _lista {
    int duzina;
    ElementListe* prvi;
    ElementListe* zadnji;
} Lista;

ElementListe* stvoriElementListe(Stablo *);
Stablo* stvoriDirektorij(char *);
int unos(ElementListe *, char *, char *);
Stablo* pronadiDijete(Stablo *, char *);
int dodajElementNaKrajListe(Lista *, ElementListe *);
int izbrisiZadnjiElement(Lista *);
int ispisDirektorija(Stablo *);
int dodajDijete(Stablo *, char *);
void ispisiPutanju(ElementListe *);
void ispisiPomoc();
void oslobodiListu(Lista *);
void oslobodiStablo(Stablo *);

int main() {

    Stablo* stabloDirektorija = stvoriDirektorij((char *)"C:");
    if (stabloDirektorija == NULL) return MALLOC_ERROR;
    
    ElementListe* glavaDirektorija = stvoriElementListe(stabloDirektorija);
    if (glavaDirektorija == NULL) return MALLOC_ERROR;

    Lista putanjaDirektorija = { 1, glavaDirektorija, glavaDirektorija };
    char komanda[MAX_LENGTH], argument[MAX_LENGTH];
    
    ispisiPomoc();
    while (1) {

        strncpy(komanda, "", MAX_LENGTH - 1);
        strncpy(argument, "", MAX_LENGTH - 1);
        unos(putanjaDirektorija.prvi, komanda, argument);
        
        if (!strcmp(komanda, "")) continue;
        else if (!strcmp(komanda, "exit") || !strcmp(komanda, "EXIT")) break;
        else if (!strcmp(komanda, "cd")) {

            if (!strcmp(argument, "..")) izbrisiZadnjiElement(&putanjaDirektorija);
            else if (strlen(argument) > 0) {

                Stablo* dijete = pronadiDijete(putanjaDirektorija.zadnji->element->dijete, argument);

                if (dijete != NULL) {

                    ElementListe* noviElement = stvoriElementListe(dijete);

                    if (noviElement == NULL) {
                        printf("Neuspješno alociranje memorije za novi poddirektorij.\n");
                        continue;
                    }

                    dodajElementNaKrajListe(&putanjaDirektorija, noviElement);
                }
                else printf("Sustav ne prepoznaje navedenu putanju.\n");

            }
            else {
                ispisiPutanju(putanjaDirektorija.prvi);
                printf("\n\n");
            }
        }
        else if (!strcmp(komanda, "dir")) {
            if (strlen(argument) > 0) printf("Sintaksa naredbe je netočna.\n");
            else ispisDirektorija(putanjaDirektorija.zadnji->element);
        }
        else if (!strcmp(komanda, "mkdir")) {
            if (strlen(argument) > 0) dodajDijete(putanjaDirektorija.zadnji->element, argument);
            else printf("Sintaksa naredbe je netočna.\n");
        }else if (!strcmp(komanda, "help")) ispisiPomoc();
        else printf("'%s' se ne prepoznaje kao unutarnja ili vanjska naredba, operativni program ili skupna datoteka.\n", komanda);
    }

    oslobodiListu(&putanjaDirektorija);
    oslobodiStablo(stabloDirektorija);

	return EXIT_SUCCESS;
}

ElementListe* stvoriElementListe(Stablo* element) {
    
    ElementListe* noviElement = NULL;
    noviElement = (ElementListe *)malloc(sizeof(ElementListe));

    if (noviElement == NULL) {
        printf("Greška pri alociranju memorije!\n");
        return NULL;
    }

    noviElement->element = element;
    noviElement->iduci = NULL;
    noviElement->prosli = NULL;

    return noviElement;
}

Stablo* stvoriDirektorij(char* imeStabla) {

    Stablo* novoDijete = NULL;
    novoDijete = (Stablo *)malloc(sizeof(Stablo));

    if (novoDijete == NULL) {
        printf("Greška pri alociranju memorije!\n");
        return NULL;
    }

    strcpy(novoDijete->imeStabla, imeStabla);
    novoDijete->iduci = NULL;
    novoDijete->dijete = NULL;

    return novoDijete;
}

int unos(ElementListe* prviElement, char* komanda, char* argument) {
    
    ispisiPutanju(prviElement);
    printf(">");
    
    char linijaUnosa[MAX_LENGTH] = {0};
    if (fgets(linijaUnosa, MAX_LENGTH, stdin) == NULL) {
        printf("Greška prilikom čitanja unosa.\n");
        return INVALID_INPUT;
    }

    sscanf(linijaUnosa, "%s", komanda);

    char* ostatak = linijaUnosa + strlen(komanda);
    while (*ostatak == ' ' || *ostatak == '\t') {
        ostatak++;
    }

    strncpy(argument, ostatak, MAX_LENGTH - 1);
    argument[strcspn(argument, "\n")] = '\0';
    
    return EXIT_SUCCESS;
}

Stablo* pronadiDijete(Stablo* prvoDijete, char* imeTrazenogDijeteta) {

    Stablo* trenutno = prvoDijete;

    while (trenutno != NULL) {
        if (!strcmp(trenutno->imeStabla, imeTrazenogDijeteta)) return trenutno;
        trenutno = trenutno->iduci;
    }

    return NULL;
}

int izbrisiZadnjiElement(Lista* putanjaDirektorija) {

    if (putanjaDirektorija->duzina == 1) return EXIT_SUCCESS;

    ElementListe* zadnjiElement = putanjaDirektorija->zadnji;
    ElementListe* predzadnjiElement = zadnjiElement->prosli;

    predzadnjiElement->iduci = NULL;
    putanjaDirektorija->zadnji = predzadnjiElement;

    free(zadnjiElement);
    putanjaDirektorija->duzina--;

    return EXIT_SUCCESS;
}

int dodajElementNaKrajListe(Lista* putanjaDirektorija, ElementListe* zadnjiElement) {

    ElementListe* prethodniZadnjiElement = putanjaDirektorija->zadnji;

    prethodniZadnjiElement->iduci = zadnjiElement;
    zadnjiElement->prosli = prethodniZadnjiElement;

    putanjaDirektorija->zadnji = zadnjiElement;
    putanjaDirektorija->duzina++;

    return EXIT_SUCCESS;

}

int ispisDirektorija(Stablo* roditelj) {

    printf("Direktorij: %s\n", roditelj->imeStabla);
    Stablo* trenutno = roditelj->dijete;

    while (trenutno != NULL) {
        printf("\t%s\n", trenutno->imeStabla);
        trenutno = trenutno->iduci;
    }

    printf("\n");
    return EXIT_SUCCESS;
}

int dodajDijete(Stablo* roditelj, char* imeDijeteta) {
    
    Stablo* dijete = stvoriDirektorij(imeDijeteta);
    if (dijete == NULL) return MALLOC_ERROR;

    Stablo* trenutno = roditelj->dijete;
    
    if (trenutno == NULL) roditelj->dijete = dijete;
    else {
        while (trenutno->iduci != NULL) trenutno = trenutno->iduci;
        trenutno->iduci = dijete;
    }

    return EXIT_SUCCESS;
}

void ispisiPutanju(ElementListe* prviElement) {

    ElementListe* trenutni = prviElement;

    while (trenutni != NULL) {
        printf("%s\\", trenutni->element->imeStabla);
        trenutni = trenutni->iduci;
    }

}

void ispisiPomoc() {
    printf("\tcd [putanja]\t- Promjena trenutnog direktorija.\n");
    printf("\t\t\t- '..' za povratak u roditeljski direktorij.\n");
    printf("\tdir\t- Ispis svih poddirektorija trenutnog direktorija.\n");
    printf("\tmkdir [ime]\t- Kreiranje novog poddirektorija u trenutnom direktoriju.\n");
    printf("\texit\t- Izlazak iz programa.\n\n");
    printf("\t- Koristite 'help' za ponovni prikaz ove liste.\n\n");
}


void oslobodiListu(Lista* lista) {

    ElementListe* trenutni = lista->prvi;

    while (trenutni != NULL) {

        ElementListe* temp = trenutni;
        trenutni = trenutni->iduci;
        free(temp);

    }

    lista->prvi = NULL;
    lista->zadnji = NULL;
    lista->duzina = 0;

}

void oslobodiStablo(Stablo* korijen) {

    if (korijen == NULL) return;

    oslobodiStablo(korijen->dijete);
    oslobodiStablo(korijen->iduci);
    free(korijen);
    
}