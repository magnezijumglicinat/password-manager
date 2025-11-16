#include <stdio.h>
#include <direct.h>
typedef struct {
    char naziv[50];
    char korisnickoIme[50];
    char lozinka[50];
} Lozinka;

void mainMenu();
void prikaziLozinke();
void dodajUFajl(Lozinka);
void dodajLozinku();
void obrisiLozinku(Lozinka);
void obrisiSveLozinke();

int main(void) {
    mainMenu();
    return 0;
}
void mainMenu() {
    printf("Dobrodosli na glavni meni.\n"
        "1.Prikazi lozinke\n"
        "2.Dodaj lozinku\n"
        "3.Obrisi lozinku\n"
        "4.Izmeni lozinku\n"
        "5.Obrisi sve lozinke\n\n"
    );

    int izbor;
    do {
        scanf_s("%d", &izbor);
        switch (izbor) {
        case 1:
            
            prikaziLozinke();
            break;
        case 2:
            printf("\nDodavanje lozinke:\n");
			dodajLozinku();
            break;
        case 3:
            printf("Brisanje lozinke:\n");
            break;
        case 4:
            printf("Izmena lozinke:\n");
            break;
        case 5:
            obrisiSveLozinke();
            break;
        default:
            printf("Izlaz iz programa...\n");
            break;
        }
    } while (izbor != 0);
}
void dodajLozinku()
{
    char izbor = 'y';
    do
    {
        Lozinka nova;
        printf("+Unesite naziv servisa: ");
        scanf_s("%s", nova.naziv, (unsigned int)sizeof(nova.naziv));
        printf("+Unesite korisnicko ime: ");
        scanf_s("%s", nova.korisnickoIme, (unsigned int)sizeof(nova.korisnickoIme));
        printf("+Unesite lozinku za taj servis:");
        scanf_s("%s", nova.lozinka, (unsigned int)sizeof(nova.lozinka));
        dodajUFajl(nova);
        printf("++Da li zelite da nastavite unos lozinki?(y/n)");
        scanf_s(" %c", &izbor,(unsigned)1);
        printf("\n");

    } while (izbor == 'y');

}
void dodajUFajl(Lozinka nova)
{

    FILE* fajl = NULL;
    _mkdir("data");
    errno_t err = fopen_s(&fajl, "data\\lozinke.txt", "a");
    if (err != 0 || fajl == NULL)
    {
        printf("Greska pri otvaranju fajla!\n");
        return;
    }

    fprintf(fajl, "\nServis: %s\nKorisnik: %s\nLozinka: %s\n-------------------------------\n", nova.naziv, nova.korisnickoIme, nova.lozinka);
    printf("Uspesno dodavanje lozinke!");
    fclose(fajl);
}
void prikaziLozinke()
{
    FILE* fajl = NULL;
    errno_t err = fopen_s(&fajl, "data\\lozinke.txt", "r");
    if (err != 0 && fajl == NULL)
    {
        printf("Greska pri otvaranju fajla!\n");
        return;
    }
    char linija[256];
    printf("Prikaz lozinki koje su sacuvane:\n\n");
    while (fgets(linija, sizeof(linija), fajl)) {
        printf("%s", linija);
    }
    fclose(fajl);
}
void obrisiSveLozinke()
{
    printf("Da li ste sigurni da zelite da obrisete sve lozinke?(y/n)");
    char izbor;
    scanf_s(" %c", &izbor);
    if (izbor == 'y')
        if (remove("data\\lozinke.txt") == 0)
            printf("\nUspesno obrisane lozinke!\n");
        else
            printf("Doslo je do greske pri brisanju lozinki, proverite da nisu vec obrisane.\n");
}
