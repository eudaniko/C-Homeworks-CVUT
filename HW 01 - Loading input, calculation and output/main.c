#include <stdio.h>


/* The main program */
int printResults(int firstNoun, int secondNoun) {
    printf("Desitkova soustava: %d %d\n", firstNoun, secondNoun);
    printf("Sestnactkova soustava: %x %x\n", firstNoun, secondNoun);
    printf("Soucet: %d + %d = %d\n", firstNoun, secondNoun, firstNoun + secondNoun);
    printf("Rozdil: %d - %d = %d\n", firstNoun, secondNoun, firstNoun - secondNoun);
    printf("Soucin: %d * %d = %d\n", firstNoun, secondNoun, firstNoun * secondNoun);
    if (secondNoun != 0) {
        printf("Podil: %d / %d = %d\n", firstNoun, secondNoun, firstNoun / secondNoun);
        printf("Prumer: %.1f\n", (firstNoun + secondNoun) / 2.0);
        return 0;
    } else {
        printf("Podil: %d / %d = NaN\n", firstNoun, secondNoun);
        fprintf(stderr, "Error: Vstup je mimo interval!\n");
        printf("Prumer: %.1f\n", (firstNoun + secondNoun) / 2.0);
        return 101;

    }
}

int main(int argc, char *argv[]) {
    int firstNoun, secondNoun;
    scanf("%d", &firstNoun);
    scanf("%d", &secondNoun);

    if (((-10000 <= firstNoun) && (firstNoun <= 10000)) && ((-10000 <= secondNoun) && (secondNoun <= 10000))) {
        return printResults(firstNoun, secondNoun);
    } else {
        fprintf(stderr,
                "Error: Vstup je mimo interval!\n");
        return 100;
    }
}



