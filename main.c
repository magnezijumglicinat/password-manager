#include <stdio.h>
#include <direct.h>
#include <string.h>
#include<stdbool.h>
#define FILE_PATH "data\\lozinke.dat"


typedef struct Lozinka {
    char naziv[50];
    char korisnickoIme[50];
    char lozinka[50];
} LOZINKA;

void mainMenu();
void prikaziLozinke();
void dodajUFajl(LOZINKA);
void dodajLozinku();
void obrisiLozinku();
void obrisiSveLozinke();
void izmeniLozinku();

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
            obrisiLozinku();
            break;
        case 4:
            printf("Izmena lozinke:\n");
            izmeniLozinku();
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
        LOZINKA nova;
        printf("+Unesite naziv servisa: ");
        scanf_s("%s", nova.naziv, (unsigned int)sizeof(nova.naziv));
        printf("+Unesite korisnicko ime: ");
        scanf_s("%s", nova.korisnickoIme, (unsigned int)sizeof(nova.korisnickoIme));
        printf("+Unesite lozinku za taj servis:");
        scanf_s("%s", nova.lozinka, (unsigned int)sizeof(nova.lozinka));
        dodajUFajl(nova);
        printf("\n++Da li zelite da nastavite unos lozinki?(y/n)");
        scanf_s(" %c", &izbor,(unsigned)1);
        printf("\n");

    } while (izbor == 'y');

}
void dodajUFajl(LOZINKA nova)
{
    FILE* fajl = NULL;
    _mkdir("data");
    errno_t err = fopen_s(&fajl, FILE_PATH, "ab");  
    if (err != 0 || fajl == NULL)
    {
        printf("Greska pri otvaranju fajla!\n");
        return;
    }

    if (fwrite(&nova, sizeof(LOZINKA), 1, fajl) != 1) {
        printf("Greska pri upisu u fajl!\n");
    }
    else {
        printf("Uspesno dodavanje lozinke!");
    }

    fclose(fajl);
}
void prikaziLozinke()
{
    FILE* fajl = NULL;
    errno_t err = fopen_s(&fajl, FILE_PATH, "rb");  
    if (err != 0 || fajl == NULL)
    {
        printf("Greska pri otvaranju fajla!\n");
        return;
    }

    LOZINKA l;
    int i = 1;

    printf("Prikaz sacuvanih lozinki:\n--------------------------\n");
    while (fread(&l, sizeof(LOZINKA), 1, fajl) == 1) {
        printf("%d.) Servis: %s | Korisnik: %s | Lozinka: %s\n",
            i++, l.naziv, l.korisnickoIme, l.lozinka);
    }

    fclose(fajl);
}

void obrisiSveLozinke()
{
    printf("Da li ste sigurni da zelite da obrisete sve lozinke?(y/n)");
    char izbor;
    scanf_s(" %c", &izbor);
    if (izbor == 'y')
        if (remove(FILE_PATH) == 0)
            printf("\nUspesno obrisane lozinke!\n");
        else
            printf("Doslo je do greske pri brisanju lozinki, proverite da nisu vec obrisane.\n");
}
 void obrisiLozinku()
{
    
        FILE* citanje = NULL;
        errno_t err = fopen_s(&citanje, FILE_PATH, "rb");
        if (err != 0 || citanje == NULL)
        {
            printf("Greska pri otvaranju fajla!\n");
            return;
        }

        FILE* pisanje = NULL;
        err = fopen_s(&pisanje, "data\\lozinka.dat", "wb");
        if (err != 0 || pisanje == NULL) {
            printf("Greska pri otvaranju fajla!\n");
            return;
        }
        char naziv[50];
        printf("Unesite naziv servisa ciju lozinku biste hteli obrisati: ");
        scanf_s("%s", naziv,(unsigned int)sizeof(naziv));
        LOZINKA l;

        while (fread(&l, sizeof(LOZINKA), 1, citanje))
        {
            if (strcmp(l.naziv, naziv) != 0)
            {
                if (fwrite(&l, sizeof(LOZINKA), 1, pisanje) != 1)
                {
                    printf("Greska pri upisu.\n");
                    return;
                }
                
                 
            }
        }
        fclose(citanje);
        fclose(pisanje);

        if (remove(FILE_PATH) != 0)
        {
            printf("Doslo je do greske...");
            return;
        }
        if (rename("data\\lozinka.dat","data\\lozinke.dat") == 0)
            printf("Uspesno ste obrisali lozinku za servis --> %s\n", naziv);
        else
            printf("Doslo je do greske...");
}

 void izmeniLozinku()
 {
     FILE* fajl = NULL;
     errno_t err = fopen_s(&fajl, FILE_PATH, "r+b");  
     if (err != 0 || fajl == NULL) {
         printf("Greska pri otvaranju fajla!\n");
         return false;
     }

     char naziv[50];
     printf("Unesite naziv servisa ciju lozinku biste da promenite: ");
     scanf_s("%s", naziv, (unsigned)sizeof(naziv));

     LOZINKA l;
     LOZINKA nova;
     bool pronadjen = false;
     while (fread(&l, sizeof(LOZINKA), 1, fajl))
     {
         if (strcmp(l.naziv, naziv) == 0)
         {
             pronadjen = true;
             printf("\nUnesite novo ime servisa (0 -> nema izmene): ");
             scanf_s("%s", nova.naziv, (unsigned)sizeof(nova.naziv));
            
             printf("\nUnesite novo korisnicko ime (0 -> nema izmene): ");
             scanf_s("%s", nova.korisnickoIme, (unsigned)sizeof(nova.korisnickoIme));

             printf("\nUnesite novu lozinku (0 -> nema izmene): ");
             scanf_s("%s", nova.lozinka, (unsigned)sizeof(nova.lozinka));
             if (strcmp(nova.lozinka, "0") == 0)
                 strcpy_s(nova.lozinka,sizeof(l.lozinka), l.lozinka);
             if (strcmp(nova.korisnickoIme, "0") == 0)
                 strcpy_s(nova.korisnickoIme, sizeof(l.korisnickoIme), l.korisnickoIme);
             if (strcmp(nova.naziv, "0") == 0)
                 strcpy_s(nova.naziv, sizeof(l.naziv), l.naziv);

             if (fseek(fajl, -(long)sizeof(LOZINKA), SEEK_CUR) != 0)
             {
                 printf("Greska pri pozicioniranju...\n");
                 fclose(fajl);
                 return;
             }
           
             if (fwrite(&nova, sizeof(LOZINKA), 1, fajl) != 1)
                 printf("Greska pri upisu u fajl!\n");
             else
                 printf("Uspesno uzmenjena lozinka!\n");
             fclose(fajl);
             break;
             
                
             

         }
     }
     
 }
