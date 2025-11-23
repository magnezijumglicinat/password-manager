#include <stdio.h>
#include <direct.h>
#include <string.h>
#include<stdbool.h>

#include "typewriter.h"

#define USERS_PATH "data\\korisnici.dat"

char kljuc[64];
typedef struct Lozinka {
    char naziv[50];
    char korisnickoIme[50];
    char lozinka[50];
} LOZINKA;
typedef struct Korisnik {
    char korisnickoIme[50];
    char lozinka[50];
}KORISNIK;

char trenutniUser[50] = "";
char FILE_PATH[260] = "";

void start();
void prijaviSe();
void registrujSe();
void mainMenu();
void prikaziLozinke();
void dodajUFajl(LOZINKA);
void dodajLozinku();
void obrisiLozinku();
void obrisiSveLozinke();
void izmeniLozinku();

int main(void) {
    start();
    return 0;
}
void start()
{
    typewrite(1, "Autor: Vukasin Zivaljevic.");
    int unos;
    typewrite(1, "Dobrodosli.\n");
    typewrite(1, "1.Prijavite se\n");
    typewrite(1, "2.Registrujte se\n");
  
    do {
        scanf_s("%d", &unos);
        switch (unos)
        {
            case 1:
                prijaviSe();
                break;
            case 2:
                registrujSe();
                break;
            default:
                typewrite(1, "greska...");
        }
    
    } while(unos != 0);

}
void generisi_xor_lozinku(const char *lozinka)
{
    strcpy_s(kljuc, sizeof(kljuc), lozinka);
}
void xor_operacija(char* data, size_t duzina)
{
    unsigned char* bajtovi = (unsigned char*)data;//konvert
    size_t kljuc_duzina = strlen(kljuc);
        for (size_t i = 0; i < duzina; i++)
        {
            bajtovi[i] ^= (unsigned char)kljuc[i % kljuc_duzina];
        }
}
void prijaviSe()
{
    char ime[50];
    char lozinka[50];

    FILE* users = NULL;
    errno_t err = fopen_s(&users, USERS_PATH, "rb");
    if (err != 0 || users == NULL)
    {
        typewrite(3, "Nema registrovanih korisnika. Registrujte se.\n");
        return;
    }

    KORISNIK k;

    while (1) {
        rewind(users); 

        typewrite(1, "\nUnesite korisnicko ime (0 za odustajanje): ");
        scanf_s("%s", ime, (unsigned)sizeof(ime));
        if (strcmp(ime, "0") == 0) {
            fclose(users);
            return;
        }

        typewrite(1, "\nUnesite lozinku: ");
        scanf_s("%s", lozinka, (unsigned)sizeof(lozinka));
        generisi_xor_lozinku(lozinka);
        bool pronadjen = false;
        while (fread(&k, sizeof(KORISNIK), 1, users) == 1) {
            xor_operacija(&k, sizeof(KORISNIK));
            if (strcmp(k.korisnickoIme, ime) == 0 && strcmp(k.lozinka, lozinka) == 0)
            {
                pronadjen = true;
                break;
            }
        }

        if (pronadjen) {
            typewrite(3, "Uspesno logovanje.\n");
            fclose(users);
            strcpy_s(trenutniUser, sizeof(trenutniUser), ime);
            sprintf_s(FILE_PATH, sizeof(FILE_PATH), "data\\%s_file.dat", ime);
            mainMenu();
            return;
        }

        typewrite(1, "Korisnicko ime/sifra su netacni.\n");
    }
}

bool duplikat(KORISNIK k)
{
    FILE* provera = NULL;
    errno_t err = fopen_s(&provera, USERS_PATH, "rb");
    if (err != 0 || provera == NULL)
    {
        return false;
    }
    KORISNIK p;
    while (fread(&p, sizeof(KORISNIK), 1, provera))
    {
        xor_operacija(&p, sizeof(KORISNIK));
        if (strcmp(p.korisnickoIme, k.korisnickoIme) == 0) {
            fclose(provera);
            return true;
        }
    }
    fclose(provera);
    return false;
}
void registrujSe()
{
    char ime[50];
    char lozinka[50];
    _mkdir("data");
    typewrite(1, "\nUnesite korisnicko ime: ");
    scanf_s("%s", ime, (unsigned int)sizeof(ime));
    typewrite(1, "\nUnesite lozinku: ");
    scanf_s("%s", lozinka, (unsigned int)sizeof(lozinka));
    KORISNIK k;
    strcpy_s(k.korisnickoIme,sizeof(k.korisnickoIme), ime);
    strcpy_s(k.lozinka,sizeof(k.lozinka), lozinka);
    generisi_xor_lozinku(lozinka);
    if (duplikat(k)) {
        typewrite(3, "Postoji vec korisnik sa tim imenom. Probajte ponovo.\n");
        
        return;
    }
    FILE* users = NULL;
    errno_t err = fopen_s(&users, USERS_PATH, "ab");
    if (err != 0 || users == NULL)
    {
        typewrite(3, "Greska pri otvaranju fajla korisnika.\n");
        return;
    }
    xor_operacija(&k, sizeof(KORISNIK));
    if (fwrite(&k, sizeof(KORISNIK), 1, users) != 1) {
        typewrite(3, "Greska pri upisu u fajl!\n");
        fclose(users);
    }
    else
    {
        typewrite(3, "Uspesna registracija.");
        strcpy_s(trenutniUser, sizeof(trenutniUser), ime);
        sprintf_s(FILE_PATH, sizeof(FILE_PATH), "data\\%s_file.dat", ime);
        fclose(users);

        mainMenu();
    }
    

}
void mainMenu() {
 
    typewrite(0.1f,"Dobrodosli na glavni meni, %s!\n"
        "1.Prikazi lozinke\n"
        "2.Dodaj lozinku\n"
        "3.Obrisi lozinku\n"
        "4.Izmeni lozinku\n"
        "5.Obrisi sve lozinke\n\n",trenutniUser
    );

    int izbor;
    do {
        scanf_s("%d", &izbor);
        switch (izbor) {
        case 1:
            
            prikaziLozinke();
            break;
        case 2:
            typewrite(3,"\nDodavanje lozinke:\n");
			dodajLozinku();
            break;
        case 3:
            typewrite(3,"Brisanje lozinke:\n");
            obrisiLozinku();
            break;
        case 4:
            typewrite(3,"Izmena lozinke:\n");
            izmeniLozinku();
            break;
        case 5:
            obrisiSveLozinke();
            break;
        default:
            typewrite(3,"Izlaz iz programa...\n");
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
        typewrite(3,"+Unesite naziv servisa: ");
        scanf_s("%s", nova.naziv, (unsigned int)sizeof(nova.naziv));
        typewrite(3,"+Unesite korisnicko ime: ");
        scanf_s("%s", nova.korisnickoIme, (unsigned int)sizeof(nova.korisnickoIme));
        typewrite(3,"+Unesite lozinku za taj servis: ");
        scanf_s("%s", nova.lozinka, (unsigned int)sizeof(nova.lozinka));
        dodajUFajl(nova);
        typewrite(3,"\n++Da li zelite da nastavite unos lozinki?(y/n)");
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
        typewrite(3,"Greska pri otvaranju fajla!\n");
        return;
    }
    xor_operacija(&nova, sizeof(LOZINKA));
    if (fwrite(&nova, sizeof(LOZINKA), 1, fajl) != 1) {
        typewrite(3,"Greska pri upisu u fajl!\n");
    }
    else {
        typewrite(3,"Uspesno dodavanje lozinke!");
    }

    fclose(fajl);
}
void prikaziLozinke()
{
    FILE* fajl = NULL;
    errno_t err = fopen_s(&fajl, FILE_PATH, "rb");  
    if (err != 0 || fajl == NULL)
    {
        typewrite(3,"Greska pri otvaranju fajla!\n");
        return;
    }

    LOZINKA l;
    int i = 1;
    typewrite(3,"Prikaz sacuvanih lozinki:\n--------------------------\n");
    while (fread(&l, sizeof(LOZINKA), 1, fajl) == 1) {
        xor_operacija(&l, sizeof(LOZINKA));
        typewrite(0.5f,"%d.) Servis: %s | Korisnik: %s | Lozinka: %s\n",
            i++, l.naziv, l.korisnickoIme, l.lozinka);
    }
  

    fclose(fajl);
}

void obrisiSveLozinke()
{
    typewrite(3,"Da li ste sigurni da zelite da obrisete sve lozinke?(y/n)");
    char izbor;
    scanf_s(" %c", &izbor);
    if (izbor == 'y')
        if (remove(FILE_PATH) == 0)
            typewrite(3,"\nUspesno obrisane lozinke!\n");
        else
            typewrite(3,"Doslo je do greske pri brisanju lozinki, proverite da nisu vec obrisane.\n");
}
 void obrisiLozinku()
{
    
        FILE* citanje = NULL;
        errno_t err = fopen_s(&citanje, FILE_PATH, "rb");
        if (err != 0 || citanje == NULL)
        {
            typewrite(3,"Greska pri otvaranju fajla!\n");
            return;
        }

        FILE* pisanje = NULL;
        err = fopen_s(&pisanje, "data\\lozinka.dat", "wb");
        if (err != 0 || pisanje == NULL) {
            typewrite(3,"Greska pri otvaranju fajla!\n");
            return;
        }
        char naziv[50];
        typewrite(3,"Unesite naziv servisa ciju lozinku biste hteli obrisati: ");
        scanf_s("%s", naziv,(unsigned int)sizeof(naziv));
        LOZINKA l;

        while (fread(&l, sizeof(LOZINKA), 1, citanje))
        {
            xor_operacija(&l, sizeof(LOZINKA));

            if (strcmp(l.naziv, naziv) != 0)
            {
                xor_operacija(&l, sizeof(LOZINKA));

                if (fwrite(&l, sizeof(LOZINKA), 1, pisanje) != 1)
                {
                    typewrite(3,"Greska pri upisu.\n");
                    return;
                }
                
                 
            }
        }
        fclose(citanje);
        fclose(pisanje);

        if (remove(FILE_PATH) != 0)
        {
            typewrite(3,"Doslo je do greske...");
            return;
        }
        if (rename("data\\lozinka.dat","data\\lozinke.dat") == 0)
            typewrite(3,"Uspesno ste obrisali lozinku za servis --> %s\n", naziv);
        else
            typewrite(3,"Doslo je do greske...");

            
 
   
     
 }
 

 void izmeniLozinku()
 {
     FILE* fajl = NULL;
     errno_t err = fopen_s(&fajl, FILE_PATH, "r+b");  
     if (err != 0 || fajl == NULL) {
         typewrite(3,"Greska pri otvaranju fajla!\n");
         return false;
     }

     char naziv[50];
     typewrite(3,"Unesite naziv servisa ciju lozinku biste da promenite: ");
     scanf_s("%s", naziv, (unsigned)sizeof(naziv));

     LOZINKA l;
     LOZINKA nova;
  
     while (fread(&l, sizeof(LOZINKA), 1, fajl))
     {
         xor_operacija(&l, sizeof(LOZINKA));
         if (strcmp(l.naziv, naziv) == 0)
         {
             
             typewrite(3,"\nUnesite novo ime servisa (0 -> nema izmene): ");
             scanf_s("%s", nova.naziv, (unsigned)sizeof(nova.naziv));
            
             typewrite(3,"\nUnesite novo korisnicko ime (0 -> nema izmene): ");
             scanf_s("%s", nova.korisnickoIme, (unsigned)sizeof(nova.korisnickoIme));

             typewrite(3,"\nUnesite novu lozinku (0 -> nema izmene): ");
             scanf_s("%s", nova.lozinka, (unsigned)sizeof(nova.lozinka));
             if (strcmp(nova.lozinka, "0") == 0)
                 strcpy_s(nova.lozinka,sizeof(l.lozinka), l.lozinka);
             if (strcmp(nova.korisnickoIme, "0") == 0)
                 strcpy_s(nova.korisnickoIme, sizeof(l.korisnickoIme), l.korisnickoIme);
             if (strcmp(nova.naziv, "0") == 0)
                 strcpy_s(nova.naziv, sizeof(l.naziv), l.naziv);

             if (fseek(fajl, -(long)sizeof(LOZINKA), SEEK_CUR) != 0)
             {
                 typewrite(3,"Greska pri pozicioniranju...\n");
                 fclose(fajl);
                 return;
             }
             xor_operacija(&nova, sizeof(LOZINKA));
             if (fwrite(&nova, sizeof(LOZINKA), 1, fajl) != 1)
                 typewrite(3,"Greska pri upisu u fajl!\n");
             else
                 typewrite(3,"Uspesno uzmenjena lozinka!\n");
             fclose(fajl);
             break;
             
                
             

         }
     }
     
 }
