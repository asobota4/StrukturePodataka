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

    free(curr);

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

    insertEnd("Petar ", "Petric", 1997, p);

    printList(p);

    Position found = findLastName("Markic", p);
    if (found){
        printf("Pronadeno: %s %s | %d\n", found->name, found->surname, found->birthYear);
    }

    deletePerson ("Lukic", p);
    printf("\nLista nakon brisanja:\n");
    printList(p);

    return 0;
}



