#include <stdio.h>

void drawRoof(int sirka);
void drawBox(int sirka, int vyska, int plot);
int readInputAndValidate(int *sirka, int *vyska, int *plot);

int main() {
    int sirka, vyska, plot;
    int errorCode = readInputAndValidate(&sirka, &vyska, &plot);

    if (errorCode != 0) {
        return errorCode;
    }

    drawRoof(sirka);
    drawBox(sirka, vyska, plot);
    return 0;
}

int readInputAndValidate(int *sirka, int *vyska, int *plot) {
    if (3 != scanf("%d %d %d", vyska, sirka, plot)) {
        fprintf(stderr, "Error: Chybny vstup!\n");
        return 100;
    }

    if (*plot <= 0 || *plot > *vyska) {
        fprintf(stderr, "Error: Neplatna velikost plotu!\n");
        return 103;
    }

    if ((*sirka < 3 || *sirka > 69) || (*vyska < 3 || *vyska > 69)) {
        fprintf(stderr, "Error: Vstup mimo interval!\n");
        return 101;
    }

    if (*sirka % 2 == 0) {
        fprintf(stderr, "Error: Sirka neni liche cislo!\n");
        return 102;
    }

    return 0;
}

void drawRoof(int sirka) {
    int i, j, k;
    for(i=1; i < sirka; i+=2) {
        for(j=1; j<= (sirka - i) / 2; j++)
            printf(" ");
        printf("X");
        if(i != 1) {
            for(k=1; k<=i-2; k++)
                printf(" ");
            printf("X");
        }
        printf("\n");
    }
}

void drawBox(int sirka, int vyska, int plot) {
    int i, j;
    for(i = 1; i <= vyska; i++) {
        for(j = 1; j <= sirka; j++) {
            if(i == 1 || i == vyska || j == 1 || j == sirka)
                printf("X");
            else {
                char startChar = (i % 2 == 0) ? '*' : 'o';
                printf("%c", (j % 2 == 0) ? (startChar == 'o' ? '*' : 'o') : startChar);
            }
        }

        // Draw fence if necessary
        if (i > vyska - plot) {
            printf(i == vyska || i == vyska - plot + 1 ? "-|-|-|" : " | | | ");
        }

        printf("\n");
    }
}






