#include <stdio.h>
#include <stdlib.h>

int main() {
    int num;
    float numberSum = 0;
    int is_first = 1;
    float numberAmount = 0;
    float positiveNumberAmount = 0, negativeNumberAmount = 0;
    float evenNumberAmount = 0, oddNumberAmount = 0;
    int maxNum = -2147483648, minNum = 2147483647;


    while ((scanf("%d", &num) > 0)) {
        if (num > 10000 || num < -10000) {
            printf("\nError: Vstup je mimo interval!\n");
            return 100;
        }
        if (num > 0)++positiveNumberAmount;
        else if (num < 0)++negativeNumberAmount;

        if (num % 2 == 0)++evenNumberAmount;
        else ++oddNumberAmount;

        if (num < minNum) minNum = num;
        if (num > maxNum) maxNum = num;

        if (is_first) {
            printf("%d", num);
            is_first = 0;
        } else {
            printf(", %d", num);
        }
        numberSum += num;
        ++numberAmount;
    }

    printf("\nPocet cisel: %.0f\n", numberAmount);
    printf("Pocet kladnych: %.0f\n", positiveNumberAmount);
    printf("Pocet zapornych: %.0f\n", negativeNumberAmount);
    printf("Procento kladnych: %.2f\n", positiveNumberAmount / numberAmount * 100);
    printf("Procento zapornych: %.2f\n", negativeNumberAmount / numberAmount * 100);
    printf("Pocet sudych: %.0f\n", evenNumberAmount);
    printf("Pocet lichych: %.0f\n", oddNumberAmount);
    printf("Procento sudych: %.2f\n", evenNumberAmount / numberAmount * 100);
    printf("Procento lichych: %.2f\n", oddNumberAmount / numberAmount * 100);
    printf("Prumer: %.2f\n", numberSum / numberAmount);
    printf("Maximum: %d\n", maxNum);
    printf("Minimum: %d\n", minNum);
    return 0;
}
