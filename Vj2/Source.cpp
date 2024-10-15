#include<stdio.h>
#include<stdlib.h>
#include<cstring>

struct clanListe;
typedef struct clanListe* adresaClana;
typedef struct clanListe {
    char ime[20];
    char prezime[20];
    int godinaRodenja;
    adresaClana prev;
    adresaClana next;
} ClanListe;

typedef struct lista {
    adresaClana prvi;
    adresaClana zadnji;
} Lista;

adresaClana stvoriClanaListe();
void dodajClanaNaPocetak(Lista*);
void ispisClanova(Lista*);

int main() {
    Lista osobe = { NULL, NULL };
    ispisClanova(&osobe);
    dodajClanaNaPocetak(&osobe);
    ispisClanova(&osobe);
    dodajClanaNaPocetak(&osobe);
    ispisClanova(&osobe);
    return 0;
}

adresaClana stvoriClanaListe() {
    
    adresaClana clan = NULL;
	char ime[20], prezime[20];
	int godrod;
	
	clan = (adresaClana)malloc(sizeof(ClanListe));
	if (clan == NULL) {
		printf("Error allocating memory");
		return NULL;
	}
	
	scanf("%s", ime);
	strcpy(clan->ime, ime);
	scanf("%s", prezime);
	strcpy(clan->prezime, prezime);
	scanf("%d", &godrod);
	clan->godinaRodenja = godrod;
	return clan;
}

void dodajClanaNaPocetak(Lista* osobe) {
    adresaClana osoba = stvoriClanaListe();
    if(osobe->prvi == NULL) {
        osobe->prvi = osoba;
        osobe->zadnji = osoba;
    }
    else{
        adresaClana temp = osobe->prvi;
        temp->prev = osoba;
        osoba->next = temp;
        osobe->prvi = osoba;
    }
}

void ispisClanova(Lista* osobe) {
    printf("Ispis:\n");
    adresaClana temp = osobe->prvi;
    while (temp != NULL) {
        printf("%s %s %d\n", temp->ime, temp->prezime, temp->godinaRodenja);
        temp = temp->next;
    }
}