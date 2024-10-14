#include<stdio.h>
#include<stdlib.h>

typedef struct _osoba {
    char ime[20];
    char prezime[20];
    int godinaRodenja;
} Osoba;

struct clanListe;
typedef struct clanListe* adresaClana;
typedef struct clanListe {
    adresaClana prev;
    Osoba strosoba;
    adresaClana next;
} ClanListe;

typedef struct lista {
    adresaClana prvi;
    adresaClana zadnji;
} Lista;

Osoba stvoriOsobu();
adresaClana stvoriClana(adresaClana, Osoba, adresaClana);
void dodajClanaNaPocetak(Lista);

int main() {
    Lista osobe = { NULL, NULL };
    dodajClanaNaPocetak(osobe);
    return 0;
}

Osoba stvoriOsobu() {
    Osoba placeholder = { "Ime", "Prezime", 2025 };
    return placeholder;
}

adresaClana stvoriClana(adresaClana prev, Osoba osoba, adresaClana zadnji) {
    ClanListe clan = { prev, osoba, zadnji };
    adresaClana aClan = &clan;
    return aClan;
}

void dodajClanaNaPocetak(Lista osobe) {
    Osoba prvaOsoba = stvoriOsobu();
    osobe.prvi = stvoriClana(osobe.prvi, prvaOsoba, osobe.zadnji);
}