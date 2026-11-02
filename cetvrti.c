#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Poly* Position;

typedef struct Poly{
    int coeff;
    int power;
    Position next;
} Polynom;

int insertSorted(Position p, Position newPoly);

int insertAfter(Position p, Position newPoly){
    newPoly->next = p->next;
    p->next = newPoly;

    return 0;
}

int readFromFile(Position p, char* filename){   //arg. filename nam sluzi da mozemo koristiti funkciju za pol1 i pol2 da je univerzalna

    FILE* file;
    file = fopen(filename, "r");
    if(file == NULL){
        printf("Greska pri otvaranju datoteke!");
        return 1;
    }

    int coeff;
    int power;
    while(fscanf(file, "%d %d", &coeff, &power)==2){    // datoteku cu pisat na nacin da je prvi broj u redu koeficijent, a drugi je potencija
        Polynom* polynom;
        polynom = (Polynom*)malloc(sizeof(Polynom));

        if (polynom == NULL){
            printf("Doslo je do pogreske prilikom dinamicke alokacije!");
            fclose (file);
            return 1;
        }

        polynom->coeff = coeff;
        polynom->power = power;
        polynom->next = NULL;

        insertSorted(p, polynom);
    }

    fclose(file);
    return 0;
}

int insertSorted(Position p, Position newPoly){

    Position q = p;

    while (q->next != NULL && q->next->power > newPoly->power){
        q = q->next;
    }

    insertAfter(q, newPoly);

    return 0;
}

int printPoly(Position p){

    Position q = p->next;
    while (q != NULL){
        printf("%d %d\n", q->coeff, q->power);
        q = q->next;
    }
    return 0;
}

int addPolynoms(Position p1, Position p2, Position pResult){

    // Kopiranje prvog polinoma u rezultat
    Position q1 = p1->next;

    while (q1 != NULL){

        Polynom* polynom;
        polynom = (Polynom*)malloc(sizeof(Polynom));

        if (polynom == NULL){
            printf("Dinamicka alokacija neuspjesna!");
            return 1;
        }

        polynom->coeff = q1->coeff;
        polynom->power = q1->power;
        polynom->next = NULL;

        insertSorted(pResult, polynom);

        q1 = q1->next;
    }

    // Prolazak kroz drugi polinom
    Position q2 = p2->next;

    while (q2 != NULL){

        Position prev = pResult;
        Position qResult = pResult->next;

        // Trazenje clana s jednakom potencijom
        while (qResult != NULL && qResult->power > q2->power){
            prev = qResult;
            qResult = qResult->next;
        }

        // Ako postoji clan s jednakom potencijom
        if (qResult != NULL && qResult->power == q2->power){

            qResult->coeff += q2->coeff;

            // Ako je koeficijent jednak nuli, brisemo clan
            if (qResult->coeff == 0){
                prev->next = qResult->next;
                free(qResult);
            }
        }

        // Ako ne postoji, dodajemo novi clan
        else{

            Polynom* polynom;
            polynom = (Polynom*)malloc(sizeof(Polynom));

            if (polynom == NULL){
                printf("Dinamicka alokacija neuspjesna!");
                return 1;
            }

            polynom->coeff = q2->coeff;
            polynom->power = q2->power;
            polynom->next = NULL;

            insertSorted(pResult, polynom);
        }

        q2 = q2->next;
    }

    return 0;
}

int multiplyPolynoms(Position p1, Position p2, Position pResult){
    
    Position q1 = p1->next;
    while (q1 != NULL){

        Position q2 = p2->next;
        while (q2 != NULL){

            Polynom* polynom;
            polynom = (Polynom*)malloc(sizeof(Polynom));

            if(polynom == NULL){
                printf("Neuspjesna alokacija mem.!");
                return 1;
            }

            polynom->coeff = q1->coeff * q2->coeff; //koef. se mnoze
            polynom->power = q1->power + q2->power; //potencije se zbrajaju
            polynom->next = NULL;

            Position prev = pResult;
            Position qResult = pResult->next;

            while (qResult != NULL && qResult->power > polynom->power){
                prev = qResult;
                qResult = qResult->next;
            }

            if (qResult != NULL && qResult->power == polynom->power){
                qResult->coeff += polynom->coeff;
                free(polynom);

                if (qResult->coeff == 0){
                    prev->next = qResult->next;
                    free(qResult);
                }
            }

            else{
                insertSorted(pResult, polynom);
            }

            q2 = q2->next;
        }

        q1 = q1->next;

    }
    
    return 0;

}

int freeList(Position p){

    Position curr = p->next;

    while (curr != NULL){

        Position temp = curr;
        curr = curr->next;
        free(temp);
    }

    p->next = NULL;

    return 0;
}

int main(){

    Polynom headFirst;
    Polynom headSecond;

    headFirst.coeff = 0;
    headFirst.power = 0;
    headFirst.next = NULL;

    headSecond.coeff = 0;
    headSecond.power = 0;
    headSecond.next = NULL;

    Position p1 = &headFirst;
    Position p2 = &headSecond;

    readFromFile(p1, "polinom1.txt");
    readFromFile(p2, "polinom2.txt");

    printf("Prvi polinom:\n");
    printPoly(p1);
    printf("Drugi polinom:\n");
    printPoly(p2);

    //
    Polynom headResult;
    headResult.coeff = 0;
    headResult.power = 0;
    headResult.next = NULL;

    Position pResult = &headResult;

    printf("Zbroj polinoma:\n");
    addPolynoms(p1, p2, pResult);
    printPoly(pResult);
    //

    //
    Polynom headMultiply;
    headMultiply.coeff = 0;
    headMultiply.power = 0;
    headMultiply.next = NULL;

    Position pMultiply = &headMultiply;
    printf("Umnozak polinoma:\n");
    multiplyPolynoms(p1, p2, pMultiply);
    printPoly(pMultiply);
    //

    freeList(p1);
    freeList(p2);
    freeList(pResult);
    freeList(pMultiply);

    return 0;
}
