#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX 50

int brojacRedova() {
    FILE *file;
    file = fopen("studenti.txt", "r");
    if (file == NULL){
        printf("Pogreska prilikom otvaranja datoteke");
        return 1;
    }

    int brojac = 0;
    int trenutniZnak;

    // fgetc - file get charachter
    while ((trenutniZnak = fgetc(file)) != EOF){ //EOF od engl. End of file
        if (trenutniZnak == '\n')
            brojac++;
    }

    fclose(file);
    return brojac;
}

typedef struct Studenti {
    char ime[MAX];
    char prezime[MAX];
    int bodovi;
} Student;


int main() {

    int i;

    int brojRedova = 0;
    brojRedova = brojacRedova();

    Student* studenti = NULL;
    studenti = (Student *) malloc (brojRedova * sizeof(Student));

    //ponovno citanje dokumenta jer prethodno iz funkcije vrijedi samo od { do }
    FILE *file;
    file = fopen("studenti.txt", "r");
    if (file == NULL){
        printf("Pogreska prilikom otvaranja datoteke.");
        return 1;
    }

    // ima i funkciju spremanja sadrzaja txt u RAM
    int maxBodova = 0;
    for (i=0; i < brojRedova; i++){
        if (fscanf(file, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi ) == 3){
            if (studenti[i].bodovi > maxBodova) {
                maxBodova = studenti[i].bodovi;
            }
        }
    }

    fclose(file);

    for (i=0; i < brojRedova; i++){
        float relBrBod = 0.0;
        if (maxBodova > 0){
            relBrBod = ((float)studenti[i].bodovi / maxBodova) * 100;
        }
        printf("%s\t %s\t | Bodovi: %d\t | Relativni bodovi: %.2f\n", studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, relBrBod);
    }

    free(studenti);


        return 0;
}