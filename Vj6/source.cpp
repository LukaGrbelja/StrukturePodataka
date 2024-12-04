#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

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
int dodajRacun(Racun *, Racun *);
int dodajArtikl(Artikl *, Racun *);
int usporediDatume(Datum, Datum);
Datum parsirajStringUDatum(char *);
void ispisiRacun(Racun *);
void ispisiArtikl(Artikl *);
void ispisiDatum(Datum);

int main() {
    
    Racun listaRacuna = { 0 };
    ucitajDatoteku("racuni.txt", true, &listaRacuna);
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
    
    return  0;
}

int ucitajDatoteku(const char* imeDatoteke, bool glavnaDatoteka, Racun* listaRacuna) {
    
    FILE* pokazivacDatoteke = NULL;
    pokazivacDatoteke = fopen(imeDatoteke, "r");

    if (!pokazivacDatoteke) {
        printf("Greška u otvaranju datoteke.\n");
        return -1;
    }
    
    char buffer[MAX_LINE] = { 0 };
    
    if (glavnaDatoteka) {

        while (fgets(buffer, MAX_LINE, pokazivacDatoteke) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;
            ucitajDatoteku(buffer, false, listaRacuna);
        }

        fclose(pokazivacDatoteke);
    }
    else {
        
        if (!fgets(buffer, MAX_LINE, pokazivacDatoteke)) {
            printf("Greška: Neispravan format ili čitanje podataka iz datoteke.\n");
            fclose(pokazivacDatoteke);
            return -2;
        }

        Racun* racun = stvoriRacun(parsirajStringUDatum(buffer));

        while (fgets(buffer, MAX_LINE, pokazivacDatoteke) != NULL) {
            dodajArtikl(stvoriArtikl(buffer), racun);
        }

        fclose(pokazivacDatoteke);
        dodajRacun(racun, listaRacuna);
        
    }
    
    return 0;
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

    printf("Unesite početni datum (YYYY-MM-DD) ili \"Dalje\" za bilo koji: ");
    scanf("%s", unos);

    if (strcmp(unos, "Dalje") != 0) {
        kraj = parsirajStringUDatum(unos);
    }

    pretraziArtikleSaOpcijom(listaRacuna, imeArtikla, pocetak, kraj);

    return 0;
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

int dodajRacun(Racun* noviRacun, Racun* listaRacuna) {

    Racun* trenutni = listaRacuna;

    while (trenutni->iduci != NULL && usporediDatume(trenutni->iduci->datum, noviRacun->datum)) {
        trenutni = trenutni->iduci;
    }

    noviRacun->iduci = trenutni->iduci;
    trenutni->iduci = noviRacun;

    return 0;
}

int dodajArtikl(Artikl* noviArtikl, Racun* racun) {///ovo triba provjerit nisan siguran kako triba napravit bez da stavin prvi u poseban uvjet

    Artikl* trenutni = racun->listaArtikla;

    if (trenutni == NULL || strcmp(trenutni->ime, noviArtikl->ime) > 0) {
        noviArtikl->iduci = racun->listaArtikla;
        racun->listaArtikla = noviArtikl;
        return 0;
    }

    while (trenutni->iduci && strcmp(trenutni->iduci->ime, noviArtikl->ime) < 0) {
        trenutni = trenutni->iduci;
    }

    noviArtikl->iduci = trenutni->iduci;
    trenutni->iduci = noviArtikl;

    return 0;
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
}

Datum parsirajStringUDatum(char* datumChar) {

    Datum datum;
    sscanf(datumChar, "%4s-%2s-%2s", datum.godina, datum.misec, datum.dan);

    return datum;
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