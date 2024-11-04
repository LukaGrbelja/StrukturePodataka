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
int procitajDatoteku();
int _procitajDatoteku(Pozicija);

void main()
{
    char ulaz = 0;
    FILE* pokazivacFajla = NULL;

    char prezime[50];
    Osoba glava = { .sljedeca = NULL, .ime = {0}, .prezime = {0}, .godinaRodjenja = 0 };

    dodajNaPocetakListe(&glava);
    dodajNaKrajListe(&glava);
    ispisiListu(glava.sljedeca);
    
    scanf(" %s", prezime);
    dodajIza(&glava, prezime);
    dodajIspred(&glava, prezime);
    ispisiListu(glava.sljedeca);
    
    scanf(" %s", prezime);
    Osoba* pronadenaOsoba = pronadiOsobu(glava.sljedeca, prezime);
    if (pronadenaOsoba) {
        ispisiOsobu(pronadenaOsoba);
    } else {
        printf("Osoba s prezimenom '%s' nije pronađena.\n", prezime);
    }

    scanf(" %s", prezime);
    obrisiOsobu(&glava, prezime);
    
    //FILE
    upisiUFajl(glava.sljedeca);
    Osoba glavaKopija = { .sljedeca = NULL, .ime = {0}, .prezime = {0}, .godinaRodjenja = 0 };
    _procitajDatoteku(&glavaKopija);
    ispisiListu(glavaKopija.sljedeca);

    obrisiListu(&glava);
    obrisiListu(&glavaKopija);

    return EXIT_SUCCESS;
}

Pozicija kreirajOsobu()
{
    Pozicija osoba = NULL;
    osoba = (Pozicija)malloc(sizeof(Osoba));
    if (!osoba) return NULL;

    printf("Ime: ");
    if (scanf(" %s", osoba->ime) != 1) return SCANF_ERROR;

    printf("Prezime: ");
    if (scanf(" %s", osoba->prezime) != 1) return SCANF_ERROR;

    printf("Godina rođenja: ");
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
    printf("%s %s, rođen(a) %d. godine\n", osoba->ime, osoba->prezime, osoba->godinaRodjenja);
    return EXIT_SUCCESS;
}

int ispisiListu(Pozicija trenutna)
{
    if (trenutna == NULL)
        printf("Prazna lista!\n");

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

        printf("Osoba obrisana.\n");
    }
    else printf("Osoba s prezimenom '%s' nije pronađena.\n", sur);

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
    else printf("Osoba s prezimenom '%s' nije pronađena.\n", sur);

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
    else printf("Osoba s prezimenom '%s' nije pronađena.\n", sur);
    
    return EXIT_SUCCESS;
}

int upisiUFajl(Pozicija trenutna)
{
    FILE* pokazivacFajla = NULL;
    pokazivacFajla = fopen("osobe.txt", "w");

    if (pokazivacFajla == NULL) {
        return FILE_ERROR_OPEN;
    }

    while (trenutna != NULL) {
        fprintf(pokazivacFajla, "%s %s %d\n", trenutna->ime, trenutna->prezime, trenutna->godinaRodjenja);
        trenutna = trenutna->sljedeca;
    }

    fclose(pokazivacFajla);

    printf("Uspješno!\n");

    return EXIT_SUCCESS;
}

int procitajDatoteku()
{
    char c = 0;
    FILE* fp = NULL;
    fp = fopen("osobe.txt", "r");

    if (fp == NULL) {
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

int _procitajDatoteku(Pozicija glava)
{
    FILE* pokazivacFajla = NULL;
    pokazivacFajla = fopen("osobe.txt", "r");

    if (!pokazivacFajla) {
        return FILE_ERROR_OPEN;
    }

    Pozicija prethodni = NULL, trenutni = NULL;

    obrisiListu(glava);
    prethodni = glava;

    while (!feof(pokazivacFajla))
    {
        trenutni = (Pozicija)malloc(sizeof(Osoba));
        if (!trenutni) {
            return MALLOC_ERROR;
        }
        if (fscanf(pokazivacFajla, " %s %s %d ", trenutni->ime, trenutni->prezime, &trenutni->godinaRodjenja) != 3)
            return SCANF_ERROR;

        trenutni->sljedeca = prethodni->sljedeca;
        prethodni->sljedeca = trenutni;

        prethodni = trenutni;
    }

    fclose(pokazivacFajla);

    return EXIT_SUCCESS;
}
