#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRIME 1000000

long long ReadInput();

void CreateSieveOfEratosthenes(int primes[], int *primeSize);

void DoPrimeFactorization(int primes[], int primeSize, long long num);

int main() {
    int primes[MAX_PRIME / 10], primeSize = 0;
    CreateSieveOfEratosthenes(primes, &primeSize);

    printf("Zadejte celá čísla ukončená nulou:\n");

    while (1) {
        long long num = ReadInput();
        if (num == 0) break;

        printf("Prvociselny rozklad cisla %lld je: ", num);
        DoPrimeFactorization(primes, primeSize, num);
    }

    return 0;
}

long long ReadInput() {
    long long num;
    if (scanf("%lld", &num) != 1 || num < 0) {
        fprintf(stderr, "Error: Chybny vstup!\n");
        exit(100);
    }
    return num;
}

void CreateSieveOfEratosthenes(int primes[], int *primeSize) {
    char isPrime[MAX_PRIME];
    memset(isPrime, 1, sizeof(isPrime));

    for (int p = 2; p * p < MAX_PRIME; p++) {
        if (isPrime[p]) {
            for (int i = p * p; i < MAX_PRIME; i += p)
                isPrime[i] = 0;
        }
    }

    for (int p = 2; p < MAX_PRIME; p++) {
        if (isPrime[p]) {
            primes[*primeSize] = p;
            (*primeSize)++;
        }
    }
}

void DoPrimeFactorization(int primes[], int primeSize, long long num) {
    int firstFactor = 1; // Pro správné formátování výstupu

    if (num == 1) {
        printf("1\n");
        return;
    }

    for (int i = 0; i < primeSize && (long long) primes[i] * primes[i] <= num; i++) {
        int count = 0;
        while (num % primes[i] == 0) {
            count++;
            num /= primes[i];
        }
        if (count > 0) {
            if (!firstFactor)
                printf(" x ");
            printf("%d", primes[i]);
            if (count > 1)
                printf("^%d", count);
            firstFactor = 0;
        }
    }
    if (num > 1) {
        if (!firstFactor)
            printf(" x ");
        printf("%lld", num);
    }
    printf("\n");
}

