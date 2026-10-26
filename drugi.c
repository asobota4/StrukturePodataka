#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person* Position; 
/* ovaj typedef nam sluzi da bi skratili 
pisanje jer unutar strukture ispod imamo 
pokazivac next za iduci element vezane liste
pa da ne pisemo "struct Person*", jednostavno
pisemo Position. tj. struct Person* smo nazvali
Position */

typedef struct Person {
    char name[50];
    char surname[50];
    int birthYear;
    Position next;
} Person;

int insertAfter (Position p, Position newPerson){
    newPerson->next = p->next;      
    p->next = newPerson; 

    /* najbolje zapamtit kao: "->next" je ruka.
    Znaci nova osoba daje ruku osobi B koja je bila spojena na head,
    u iducem redu head daje ruku novoj osobi */

    return 0;
}

int insertBegin (char* name, char* surname, int birthYear, Position p){

    Person* person;     //pokazivac person
    person = (Person*)malloc(sizeof(Person));
    if (person == NULL){
        printf("Greska prilikom dinamicke alokacije!");
        return 1;
    }
    strcpy(person->name, name);
    strcpy(person->surname, surname);
    person->birthYear = birthYear;
    insertAfter(p, person);

    return 0;
}

int printList (Position p){
    Position q = p->next;
    while (q != NULL){
        printf("Ime: %s | Prezime: %s | Godina rodenja: %d\n", q->name, q->surname, q->birthYear);
        q = q->next;
    }

    return 0;
}

int insertEnd (char* name, char* surname, int birthYear, Position p){
   
    Person* person;
    person = (Person*) malloc (sizeof(Person));
    if (person == NULL){
        printf("Greska prilikom alociranja!");
        return 1;
    }

    strcpy(person->name, name);
    strcpy(person->surname, surname);
    person->birthYear = birthYear;
    person->next = NULL;
    /* mi stavljamo NULL zato sto u vezanim listama, 
    zadnji element mora u ruci imati NULL s obzirom da 
    je on zadnji tj. nema elemenata poslije njega. 
    Da se nebi desilo da racunalo samo upise neku 
    vrijednost umjesto NULL sto nam moze poremetiti 
    cijeli program */

    while (p->next != NULL){
        p = p->next;
    }

    if (p->next == NULL){
        insertAfter(p, person);
    }

    return 0;

}

Position findLastName(char* surname, Position p){

    Position q = p->next;
    while (q != NULL){
        
        if(strcmp(q->surname, surname) == 0){
            return q;
        }
        q = q->next;
    }

    return NULL;
}

int deletePerson(char* surname, Position p){
    Position prev = p;
    Position curr = p->next;

    while (curr != NULL && strcmp(curr->surname, surname)){
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL){
        printf("Osoba nije pronadena!\n");
        return 1;
    }

    prev->next = curr->next;
    /* ovime preskacemo / izbacujemo
    element koji smo pronasli tj premoscujemo
    i stavljamo da na prev neide vise current
    onaj koji ide poslije currenta */

    free(curr);

    return 0;
}

// TRECI ZADATAK
int insertAfterEl(char* targetSurname, char* name, char* surname, int birthYear, Position p){
    Position q = p->next;

    while (q != NULL && strcmp(q->surname, targetSurname) != 0){     //podsjetnik - strcmp vraca 0 ako su oba teksta potpuno ista!
        q = q->next;
    }

    if (q == NULL){
        printf ("Osoba nije pronadena!");
        return 1;
    }

    Person* person;
    person = (Person*) malloc (sizeof(Person));

    if (person == NULL){
        printf("Dinamicka alokacija nije uspjela!\n");
        return 1;
    }

    strcpy(person->name, name);
    strcpy(person->surname, surname);
    person->birthYear = birthYear;

    insertAfter(q, person);

    return 0;

}

int insertBeforeEl(char* targetSurname, char* name, char* surname, int birthYear, Position p){
    Position prev = p;
    Position q = p->next;

    while (q != NULL && strcmp(q->surname, targetSurname) != 0){
        prev = q;
        q = q->next;
    }

    if (q == NULL){
        printf("Osoba nije pronadena!");
        return 1;
    }

    Person* person;
    person = (Person*) malloc (sizeof(Person));

    if (person == NULL){
        printf("Dinamicka alokacija nije uspjela!\n");
        return 1;
    }

    strcpy(person->name, name);
    strcpy(person->surname, surname);
    person->birthYear = birthYear;

    insertAfter(prev, person);

    return 0;

}

//C 
int sortList(Position p) {
    if (p->next == NULL) {
        return 0; // Lista je prazna ili ima samo jedan element, već je sortirana
    }

    Position i, j;
    char tempName[50], tempSurname[50];
    int tempYear;
    int swapped;

    do {
        swapped = 0;
        i = p->next; // Krećemo od prvog pravog elementa

        while (i->next != NULL) {
            // Uspoređujemo susjede koristeći strcmp
            // Ako je prvo prezime "veće" po abecedi od drugog (> 0)
            if (strcmp(i->surname, i->next->surname) > 0) {
                
                // 1. Privremeno spremamo podatke od i
                strcpy(tempName, i->name);
                strcpy(tempSurname, i->surname);
                tempYear = i->birthYear;

                // 2. Prebacujemo podatke od i nadjačavamo s i->next
                strcpy(i->name, i->next->name);
                strcpy(i->surname, i->next->surname);
                i->birthYear = i->next->birthYear;

                // 3. Vraćamo stare podatke od i u i->next
                strcpy(i->next->name, tempName);
                strcpy(i->next->surname, tempSurname);
                i->next->birthYear = tempYear;

                swapped = 1; // Zabilježili smo da je bilo zamjene
            }
            i = i->next;
        }
    } while (swapped); // Ponavljamo dok god je bilo promjena

    return 0;
}

int writeListToFile(Position p){
    Position q = p->next;

    FILE* file;
    file = fopen("Osobe.txt", "w");
    if (file == NULL){
        printf("Datoteka se nije uspjela otvoriti!");
        return 1;
    }

    while(q != NULL){
        fprintf(file, "%s %s %d\n", q->name, q->surname, q->birthYear);
        q = q->next;
    }

    fclose(file);
    return 0;
}

int readFromFile(Position p){

    FILE* file;
    file = fopen("Osobe.txt", "r");
    if (file == NULL){
        printf("Doslo je do pogreske prilikom otvaranja!\n");
        return 1;
    }

    char name[50];
    char surname[50];
    int birthYear;

    //fscanf vraca broj procitanih stavki
    while (fscanf(file, "%s %s %d", name, surname, &birthYear)==3){
        Person* person;
        person = (Person*) malloc (sizeof(Person));
        if (person == NULL){
            printf("Pogreska prilikom alokacije!\n");
            fclose(file);
            return 1;
        }

        strcpy(person->name, name);
        strcpy(person->surname, surname);
        person->birthYear = birthYear;

    Position last = p;
    while (last->next != NULL){
        last = last->next;
    }
    insertAfter(last, person);

    }

    fclose(file);

    return 0;
}


int main(){

    Person head;
    strcpy(head.name, "");      //strcpy(odrediste, sadrzaj koji upisujemo);
    strcpy(head.surname, "");
    head.birthYear = 0;
    head.next = NULL;


    Position p = &head;

    insertBegin("Marko ", "Markic", 1995, p);
    insertBegin("Luka ", "Lukic", 2000, p);
    insertBegin("Marin ", "Marinic", 2000, p);

    insertEnd("Petar ", "Petric", 1997, p);

    printList(p);

    Position found = findLastName("Markic", p);
    if (found){
        printf("Pronadeno: %s %s | %d\n", found->name, found->surname, found->birthYear);
    }

    deletePerson ("Lukic", p);
    printf("\nLista nakon brisanja:\n");
    printList(p);
    printf("\n");

    // TRECI ZADATAK
    printf("Treci zadatak:\n");

    insertAfterEl("Marinic", "Ante", "Antic", 2004, p);
    insertBeforeEl("Markic", "Josip", "josipovic", 2001, p);

    printList(p);

    printf("\n");
    //C 
    sortList(p);
    printList(p);

    //D
    writeListToFile(p);

    //E
    
    //brisanje el. unutar vezane liste da ne ispisuje duplo
    Position current = p->next;
    Position nextNode = NULL;

    while (current != NULL){
        nextNode = current->next;
        free (current);
        current = nextNode;
    }

    p->next = NULL; // head liste vise ne pokazuje ni na koga jer je lista prazna


    printf("Sadrzaj datoteke:\n");
    readFromFile(p);
    printList(p);

    return 0;
}



