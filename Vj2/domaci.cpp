#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct osoba;
typedef struct osoba* Adresa;

typedef struct osoba {
	char ime[20];
	char prezime[20];
	int godRodenja;
	Adresa iduci;
} Osoba;

Adresa stvoriOsobu();
void dodajOsobuNaPocetak(Adresa head);
void ispisListe(Adresa prvaOsoba);
void dodajOsobuNaKraj(Adresa head);
void pronadiOsobu(Adresa prvaOsoba, char* prezime);
void izbrisiOsobu(Adresa head, int index);

int main()
{
    Osoba head = {"", "", 0, NULL};
    ispisListe(head.iduci);
    dodajOsobuNaKraj(&head);
    ispisListe(head.iduci);
    dodajOsobuNaPocetak(&head);
    ispisListe(head.iduci);
    dodajOsobuNaKraj(&head);
    ispisListe(head.iduci);
    pronadiOsobu(head.iduci, "Grbelja");
    pronadiOsobu(head.iduci, "Peeder");
    izbrisiOsobu(&head, 1);
    ispisListe(head.iduci);

    return 0;
}

Adresa stvoriOsobu() {
    
	Adresa novaOsoba = NULL;
	novaOsoba = (Adresa)malloc(sizeof(Osoba));
	if (novaOsoba == NULL) {
	    printf("Greška pri stvaranju osobe");
	    return NULL;
	}
	
	printf("Unesi ime: ");
	scanf("%s", novaOsoba->ime);

	printf("Unesi prezime: ");
	scanf("%s", novaOsoba->prezime);

	printf("Unesi godinu rođenja: ");
    scanf("%d", &novaOsoba->godRodenja);

	return novaOsoba;
}

void dodajOsobuNaPocetak(Adresa head) {
    
    Adresa novaOsoba = stvoriOsobu();
    novaOsoba->iduci = head->iduci;
    head->iduci = novaOsoba;
    
}

void ispisListe(Adresa prvaOsoba) {
    
    if (prvaOsoba == NULL) {
        printf("Prazna lista!\n");
        return;
    }
    
    printf("Ispis liste:\n");
    while(prvaOsoba != NULL) {
        printf("%s %s %d\n", prvaOsoba->ime, prvaOsoba->prezime, prvaOsoba->godRodenja);
        prvaOsoba = prvaOsoba->iduci;
    }
    
}

void dodajOsobuNaKraj(Adresa head) {
    
    while(head->iduci != NULL) {
        head = head->iduci;
    }
    
    Adresa novaOsoba = stvoriOsobu();
    head->iduci = novaOsoba;
    
}

void pronadiOsobu(Adresa prvaOsoba, char* prezime) {
    
    int index = 0;
    
    while(prvaOsoba != NULL) {
        if(!strcmp(prvaOsoba->prezime, prezime)) {
            printf("%d", strcmp(prvaOsoba->prezime, prezime));
            printf("%d %s %s %d\n", index, prvaOsoba->ime, prvaOsoba->prezime, prvaOsoba->godRodenja);
            return;
        }
        prvaOsoba = prvaOsoba->iduci;
        index++;
    }
    
    printf("Osoba nije pronađena");
    
}

void izbrisiOsobu(Adresa head, int index) {
    
    Adresa prev = head, temp = head->iduci;
    for(int i = 0; i < index; i++) {
        prev = prev->iduci;
        temp = temp->iduci;
    }
    
    prev->iduci = temp->iduci;
    free(temp);
    
}