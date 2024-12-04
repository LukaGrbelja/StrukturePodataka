#define _CRT_SECURE_NO_WARNINGS

///Dodaj komentare
///Promjena iz Datum u Datum * u slanju među funkcija
///Dodaj posebnu funkviju za kretanje kroz liste

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define SSCANF_ERROR -3
#define EMPTY_FILE 1
#define INVALID_INPUT 2

typedef struct _datum {
	char godina[5];
	char misec[3];
	char dan[3];
} Datum;

typedef struct _artikl {
	char ime[20];
	int kolicina;
	float cijena;
	struct _artikl* iduci;
} Artikl;

typedef struct _racun {
	Datum datum;
    Artikl* listaArtikla;
	struct _racun* iduci;
} Racun;

int ucitajDatoteku(const char *, bool, Racun *);
int upit(Racun *);
void pretraziArtikleSaOpcijom(Racun *, const char *, Datum, Datum);
Racun* stvoriRacun(Datum);
Artikl* stvoriArtikl(char *);
void dodajRacun(Racun *, Racun *);
int dodajArtikl(Artikl *, Racun *);
int usporediDatume(Datum, Datum);
Datum parsirajStringUDatum(char *);
void ispisiRacun(Racun *);
void ispisiArtikl(Artikl *);
void ispisiDatum(Datum);
void oslobodiMemoriju(Racun *);

int main() {
    
    Racun listaRacuna = { 0 };

    int status = ucitajDatoteku("racuni.txt", true, &listaRacuna);
    if (status) return status;

    //ispisiRacun(listaRacuna.iduci);

    char odluka;
    do {
        printf("Napravi upit - y\nZavrši program - n\n");
        
        if (scanf(" %c", &odluka) != 1) {
            printf("Neispravan unos. Pokušajte ponovo.bbb\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (odluka == 'y' || odluka == 'Y') {
            upit(&listaRacuna);
        }

    } while (odluka == 'y' || odluka == 'Y');

    oslobodiMemoriju(listaRacuna);
    return EXIT_SUCCESS;
}

int ucitajDatoteku(const char* imeDatoteke, bool glavnaDatoteka, Racun* listaRacuna) {
    
    FILE* pokazivacDatoteke = NULL;
    pokazivacDatoteke = fopen(imeDatoteke, "r");

    if (!pokazivacDatoteke) {
        printf("Greška u otvaranju datoteke.\n");
        return FILE_OPEN_ERROR;
    }
    
    char buffer[MAX_LINE] = { 0 };

    if (!fgets(buffer, MAX_LINE, pokazivacDatoteke)) {
        printf("Datoteka %s prazna.\n", imeDatoteke);
        fclose(pokazivacDatoteke);
        return EMPTY_FILE;
    }
    
    if (glavnaDatoteka) {

        do {
            buffer[strcspn(buffer, "\n")] = 0;
            int status = ucitajDatoteku(buffer, false, listaRacuna);

            if (status) {
                fclose(pokazivacDatoteke);
                return status;
            }
            

        } while (fgets(buffer, MAX_LINE, pokazivacDatoteke));

    }
    else {
        Racun* racun = stvoriRacun(parsirajStringUDatum(buffer));

        if (racun == NULL) {
            fclose(pokazivacDatoteke);
            return MALLOC_ERROR;
        }

        while (fgets(buffer, MAX_LINE, pokazivacDatoteke)) {
            if (dodajArtikl(stvoriArtikl(buffer), racun)) {
                fclose(pokazivacDatoteke);
                return MALLOC_ERROR;
            }
        }
        
        dodajRacun(racun, listaRacuna);
    }
    
    fclose(pokazivacDatoteke);
    return EXIT_SUCCESS;
}

int upit(Racun* listaRacuna) {

    char imeArtikla[20], unos[MAX_LINE];
    Datum pocetak = { "0000", "01", "01" };
    Datum kraj = { "9999", "12", "31" };

    printf("Unesite ime artikla: ");
    scanf("%s", imeArtikla);

    printf("Unesite početni datum (YYYY-MM-DD) ili \"Dalje\" za bilo koji: ");
    scanf("%s", unos);

    if (strcmp(unos, "Dalje") != 0) {
        pocetak = parsirajStringUDatum(unos);
    }

    printf("Unesite krajnji datum (YYYY-MM-DD) ili \"Dalje\" za bilo koji: ");
    scanf("%s", unos);

    if (strcmp(unos, "Dalje") != 0) {
        kraj = parsirajStringUDatum(unos);
    }

    if (!usporediDatume(pocetak, kraj)) {
        printf("Početni datum je veći od krajnjeg.\n");
        return INVALID_INPUT;
    }

    pretraziArtikleSaOpcijom(listaRacuna, imeArtikla, pocetak, kraj);

    return EXIT_SUCCESS;
}

void pretraziArtikleSaOpcijom(Racun* listaRacuna, const char* imeArtikla, Datum pocetak, Datum kraj) {

    int ukupnaKolicina = 0;
    float ukupnaPotrosnja = 0;
    
    Racun* trenutni = listaRacuna->iduci;
    while (trenutni != NULL) {
        
        if (usporediDatume(trenutni->datum, pocetak)) {
            trenutni = trenutni->iduci;
            continue;
        }

        if (!usporediDatume(trenutni->datum, kraj)) {
            break;
        }

        Artikl* trenutniArtikl = trenutni->listaArtikla;

        while (trenutniArtikl != NULL) {
            if (strcmp(trenutniArtikl->ime, imeArtikla) > 0) {
                break;
            }

            if (strcmp(trenutniArtikl->ime, imeArtikla) == 0) {
                ukupnaKolicina += trenutniArtikl->kolicina;
                ukupnaPotrosnja += trenutniArtikl->kolicina * trenutniArtikl->cijena;
            }

            trenutniArtikl = trenutniArtikl->iduci;
        }

        trenutni = trenutni->iduci;
    }

    printf("Ukupno kupljeno: %d komada\nUkupno potrošeno: %.2f€\n", ukupnaKolicina, ukupnaPotrosnja);
}

Racun* stvoriRacun(Datum datum) {

    Racun* racun = NULL;
    racun = (Racun *)malloc(sizeof(Racun));

    if (racun == NULL) {
        printf("Greška pri alociranju memorije!\n");
        return NULL;
    }

    racun->datum = datum;
    racun->listaArtikla = NULL;
    racun->iduci = NULL;

    return racun;
}

Artikl* stvoriArtikl(char* detalji) {

    Artikl* artikl = NULL;
    artikl = (Artikl *)malloc(sizeof(Artikl));

    if (artikl == NULL) {
        printf("Greška pri alociranju memorije!\n");
        return NULL;
    }

    sscanf(detalji, "%[^,], %d, %f", artikl->ime, &artikl->kolicina, &artikl->cijena);
    artikl->iduci = NULL;

    return artikl;
}

void dodajRacun(Racun* noviRacun, Racun* listaRacuna) {

    Racun* trenutni = listaRacuna;

    while (trenutni->iduci != NULL && usporediDatume(trenutni->iduci->datum, noviRacun->datum)) {
        trenutni = trenutni->iduci;
    }

    noviRacun->iduci = trenutni->iduci;
    trenutni->iduci = noviRacun;
}

int dodajArtikl(Artikl* noviArtikl, Racun* racun) {///ovo triba provjerit nisan siguran kako triba napravit bez da stavin prvi u poseban uvjet

    if (noviArtikl == NULL) return MALLOC_ERROR;

    Artikl* trenutni = racun->listaArtikla;

    if (trenutni == NULL || strcmp(trenutni->ime, noviArtikl->ime) > 0) {
        noviArtikl->iduci = racun->listaArtikla;
        racun->listaArtikla = noviArtikl;
        return EXIT_SUCCESS;
    }

    while (trenutni->iduci && strcmp(trenutni->iduci->ime, noviArtikl->ime) < 0) {
        trenutni = trenutni->iduci;
    }

    noviArtikl->iduci = trenutni->iduci;
    trenutni->iduci = noviArtikl;

    return EXIT_SUCCESS;
}

int usporediDatume(Datum trenutni, Datum novi) {
    
    int godina1 = atoi(trenutni.godina), godina2 = atoi(novi.godina);
    if (godina1 > godina2) return 0;
    if (godina1 < godina2) return 1;

    int misec1 = atoi(trenutni.misec), misec2 = atoi(novi.misec);
    if (misec1 > misec2) return 0;
    if (misec1 < misec2) return 1;

    int dan1 = atoi(trenutni.dan), dan2 = atoi(novi.dan);
    return dan1 < dan2;
    /*
    if (dan1 > dan1) return 0;
    if (dan1 < dan1) return 1;
    return -1;
    */
}

Datum parsirajStringUDatum(char* datumChar) {///moguće buduće minjanje tipa funkcije iz Datum (char*) u int (char *, Datum *)

    /*if (!datumChar || strlen(datumChar) != 10 || datumChar[4] != '-' || datumChar[7] != '-') {
        printf("Neispravan format datuma.\n");
        return INVALID_INPUT;
    }*/

    Datum datum = { "0000", "00", "00" };
    sscanf(datumChar, "%4[0-9]-%2[0-9]-%2[0-9]", datum.godina, datum.misec, datum.dan);

    /*if (brojParsiranih != 3) {
        printf("Pogreška prilikom skeniranja\n");
        return SS;
    }*/

    return datum;
    //return EXIT_SUCCESS;
}

void ispisiRacun(Racun* racun) {
    
    while (racun != NULL) {
        printf("Adresa racuna: %p\nAdresa datuma: %p\n", racun, &racun->datum);
        ispisiDatum(racun->datum);
        ispisiArtikl(racun->listaArtikla);
        printf("Adresa iduceg racuna: %p", racun->iduci);
        racun = racun->iduci;
    }
}

void ispisiArtikl(Artikl* artikl) {
    
    while (artikl != NULL) {
        printf("Adresa artikla: %p\n\tIme: %s\n\tKolicina: %d\n\tCijena: %.2f\n", artikl, artikl->ime, artikl->kolicina, artikl->cijena);
        artikl = artikl->iduci;
    }
}

void ispisiDatum(Datum datum) {
    printf("\tDan: %s\n\tMisec: %s\n\tGodina: %s\n", datum.dan, datum.misec, datum.godina);
}

void oslobodiMemoriju(Racun* listaRacuna) {
    Racun* trenutniRacun = listaRacuna;
    
    while (trenutniRacun != NULL) {
        Racun* tempRacun = trenutniRacun;
        
        Artikl* trenutniArtikl = trenutniRacun->listaArtikla;
        while (trenutniArtikl != NULL) {
            Artikl* tempArtikl = trenutniArtikl;
            trenutniArtikl = trenutniArtikl->iduci;
            free(tempArtikl);
        }

        trenutniRacun = trenutniRacun->iduci;
        free(tempRacun);
    }

    printf("Memorija za račune i artikle je uspješno oslobođena.\n");
}