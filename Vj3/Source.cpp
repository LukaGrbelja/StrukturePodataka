#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS 0
#define FILE_ERROR_OPEN -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

struct _osoba;
typedef struct _osoba* Pozicija;

typedef struct _osoba {
    char ime[50];
    char prezime[50];
    int godinaRodjenja;
    Pozicija sljedeca;
} Osoba;

Pozicija kreirajOsobu();
int dodajNaPocetakListe(Pozicija);
int dodajNaKrajListe(Pozicija);
int ispisiOsobu(Pozicija);
int ispisiListu(Pozicija);
Pozicija pronadiOsobu(Pozicija, char*);
int obrisiOsobu(Pozicija, char*);
int obrisiListu(Pozicija);
int dodajIza(Pozicija, char*);
int dodajIspred(Pozicija, char*);
int upisiUFajl(Pozicija);
int _procitajFajl();
int procitajUFajl(Pozicija);

void main()
{
    char ulaz = 0;
    FILE* pokazivacFajla = NULL;

    char prezime[50];
    Osoba glava = { .sljedeca = NULL, .ime = {0}, .prezime = {0}, .godinaRodjenja = 0 };
    Osoba glavaR = { .sljedeca = NULL, .ime = {0}, .prezime = {0}, .godinaRodjenja = 0 };

    do
    {
        printf("\nOdaberi akciju: ");
        if (scanf(" %c", &ulaz) != 1) return SCANF_ERROR;

        switch (ulaz)
        {
            case 'f':
                printf("Dodavanje na pocetak liste\n");
                dodajNaPocetakListe(&glava);
                break;
            case 'e':
                printf("Dodavanje na kraj liste\n");
                dodajNaKrajListe(&glava);
                break;
            case 'a':
                printf("Dodaj iza osobe s prezimenom: ");
                if (scanf(" %s", prezime) != 1) return SCANF_ERROR;
                dodajIza(&glava, prezime);
                break;
            case 'b':
                printf("Dodaj ispred osobe s prezimenom: ");
                if (scanf(" %s", prezime) != 1) return SCANF_ERROR;
                dodajIspred(&glava, prezime);
                break;
            case 'p':
                printf("Ispisivanje liste:\n");
                ispisiListu(glava.sljedeca);
                break;
            case 's':
                printf("Pronađi osobu s prezimenom: ");
                if (scanf(" %s", prezime) != 1) return SCANF_ERROR;
                pronadiOsobu(glava.sljedeca, prezime) ?
                    ispisiOsobu(pronadiOsobu(glava.sljedeca, prezime)) : printf("\tOsoba s prezimenom '%s' nije pronađena.\n", prezime);
                break;
            case 'd':
                printf("Obriši osobu s prezimenom: ");
                if (scanf(" %s", prezime) != 1) return SCANF_ERROR;
                obrisiOsobu(&glava, prezime);
                break;
            case 'w':
                printf("Upisivanje liste u fajl...\n");
                upisiUFajl(glava.sljedeca);
                break;
            case 'r':
                printf("Čitanje liste iz fajla:\n");
                procitajUFajl(&glavaR);
                ispisiListu(glavaR.sljedeca);
                break;
            case '0':
                // Kraj programa
                break;
            default:
                printf("\tNeispravan unos. Pokušaj ponovo.\n");
                break;
        }
    } while (ulaz != '0');

    obrisiListu(&glava);
    obrisiListu(&glavaR);

    return EXIT_SUCCESS;
}

Pozicija kreirajOsobu()
{
    Pozicija osoba = NULL;
    osoba = (Pozicija)malloc(sizeof(Osoba));
    if (!osoba) return NULL;

    printf("\tIme: ");
    if (scanf(" %s", osoba->ime) != 1) return SCANF_ERROR;

    printf("\tPrezime: ");
    if (scanf(" %s", osoba->prezime) != 1) return SCANF_ERROR;

    printf("\tGodina rođenja: ");
    if (scanf(" %d", &osoba->godinaRodjenja) != 1) return SCANF_ERROR;

    return osoba;
}

int dodajNaPocetakListe(Pozicija glava)
{
    Pozicija novaOsoba = NULL;
    novaOsoba = kreirajOsobu();
    if (!novaOsoba) {
        printf("Greška u malloc-u!\n");
        return MALLOC_ERROR;
    }

    novaOsoba->sljedeca = glava->sljedeca;
    glava->sljedeca = novaOsoba;

    return EXIT_SUCCESS;
}

int dodajNaKrajListe(Pozicija glava)
{
    Pozicija novaOsoba = NULL;
    novaOsoba = kreirajOsobu();
    if (!novaOsoba) {
        printf("Greška u malloc-u!\n");
        return MALLOC_ERROR;
    }

    while (glava->sljedeca != NULL)
        glava = glava->sljedeca;

    novaOsoba->sljedeca = glava->sljedeca;
    glava->sljedeca = novaOsoba;

    return EXIT_SUCCESS;
}

int ispisiOsobu(Pozicija osoba)
{
    printf("\t%s %s, rođen(a) %d. godine\n", osoba->ime, osoba->prezime, osoba->godinaRodjenja);
    return EXIT_SUCCESS;
}

int ispisiListu(Pozicija trenutna)
{
    if (trenutna == NULL)
        printf("\tPrazna lista!\n");

    while (trenutna != NULL) {
        ispisiOsobu(trenutna);
        trenutna = trenutna->sljedeca;
    }
    
    return EXIT_SUCCESS;
}

Pozicija pronadiOsobu(Pozicija trenutna, char* sur)
{
    while (trenutna != NULL && strcmp(trenutna->prezime, sur))
        trenutna = trenutna->sljedeca;

    return trenutna;
}

Pozicija pronadiOsobuPrethodnu(Pozicija trenutna, char* sur)
{
    while (trenutna->sljedeca != NULL && strcmp(trenutna->sljedeca->prezime, sur))
        trenutna = trenutna->sljedeca;

    if (trenutna->sljedeca == NULL) return NULL;

    return trenutna;
}

int obrisiOsobu(Pozicija glava, char* sur)
{
    Pozicija temp = NULL, prethodni = NULL;

    prethodni = pronadiOsobuPrethodnu(glava, sur);

    if (prethodni != NULL)
    {
        temp = prethodni->sljedeca;
        prethodni->sljedeca = temp->sljedeca;
        free(temp);

        printf("\tOsoba obrisana.\n");
    }
    else printf("\tOsoba s prezimenom '%s' nije pronađena.\n", sur);

    return EXIT_SUCCESS;
}

int obrisiListu(Pozicija glava)
{
    Pozicija temp = NULL;

    while (glava->sljedeca != NULL)
    {
        temp = glava->sljedeca;
        glava->sljedeca = glava->sljedeca->sljedeca;
        free(temp);
    }

    return EXIT_SUCCESS;
}

int dodajIza(Pozicija glava, char* sur)
{
    Pozicija novaOsoba = NULL, osobaSPrezimenom = NULL;

    osobaSPrezimenom = pronadiOsobu(glava, sur);

    if (osobaSPrezimenom != NULL)
    {
        novaOsoba = kreirajOsobu();
        if (!novaOsoba) {
            printf("Greška u malloc-u!\n");
            return MALLOC_ERROR;
        }

        novaOsoba->sljedeca = osobaSPrezimenom->sljedeca;
        osobaSPrezimenom->sljedeca = novaOsoba;
    }
    else printf("\tOsoba s prezimenom '%s' nije pronađena.\n", sur);

    return EXIT_SUCCESS;
}

int dodajIspred(Pozicija glava, char* sur)
{
        Pozicija novaOsoba = NULL, osobaIspredPrezimenom = NULL;

    osobaIspredPrezimenom = pronadiOsobuPrethodnu(glava, sur);
    
    if (osobaIspredPrezimenom != NULL)
    {
        novaOsoba = kreirajOsobu();
        if (!novaOsoba) {
            printf("Greška u malloc-u!\n");
            return MALLOC_ERROR;
        }

        novaOsoba->sljedeca = osobaIspredPrezimenom->sljedeca;
        osobaIspredPrezimenom->sljedeca = novaOsoba;
    }
    else printf("\tOsoba s prezimenom '%s' nije pronađena.\n", sur);
    
    return EXIT_SUCCESS;
}

// Upisuje sadržaj liste u fajl
int upisiUFajl(Pozicija trenutna)
{
    FILE* pokazivacFajla = NULL;
    pokazivacFajla = fopen("osobe.txt", "w");

    if (pokazivacFajla == NULL) {
        printf("\nFajl nije mogao biti otvoren.\n");
        return FILE_ERROR_OPEN;
    }

    while (trenutna != NULL) {
        fprintf(pokazivacFajla, "%s %s %d\n", trenutna->ime, trenutna->prezime, trenutna->godinaRodjenja);
        trenutna = trenutna->sljedeca;
    }

    fclose(pokazivacFajla);

    printf("\tUspješno!\n");

    return EXIT_SUCCESS;
}

// Čita i ispisuje sadržaj fajla
int _procitajFajl()
{
    char c = 0;
    FILE* fp = NULL;
    fp = fopen("osobe.txt", "r");

    if (fp == NULL) {
        printf("Fajl nije mogao biti otvoren.\n");
        return FILE_ERROR_OPEN;
    }

    c = getc(fp);
    while (c != EOF) {
        printf("%c", c);
        c = getc(fp);
    }
        
    fclose(fp);

    return EXIT_SUCCESS;
}

// Čita i pohranjuje sadržaj fajla u listu
int procitajUFajl(Pozicija glava)
{
    FILE* pokazivacFajla = NULL;
    pokazivacFajla = fopen("osobe.txt", "r");

    if (!pokazivacFajla) {
        printf("Fajl nije mogao biti otvoren.\n");
        return FILE_ERROR_OPEN;
    }

    Pozicija prethodni = NULL, trenutni = NULL;

    obrisiListu(glava); // da ponovo učitavanje ne dodaje sadržaj fajla u prethodni sadržaj
    prethodni = glava;

    while (!feof(pokazivacFajla))
    {
        trenutni = (Pozicija)malloc(sizeof(Osoba)); // pokazivač na novog učitanog čovjeka
        if (!trenutni) {
            printf("Greška u malloc-u!\n");
            return MALLOC_ERROR;
        }
        if (fscanf(pokazivacFajla, " %s %s %d ", trenutni->ime, trenutni->prezime, &trenutni->godinaRodjenja) != 3)
            return SCANF_ERROR;

        // dodaj novu osobu nakon prethodnog
        trenutni->sljedeca = prethodni->sljedeca;
        prethodni->sljedeca = trenutni;

        prethodni = trenutni; // nova učitana osoba će biti prethodna osoba u sljedećoj iteraciji
    }

    fclose(pokazivacFajla);

    return EXIT_SUCCESS;
}
