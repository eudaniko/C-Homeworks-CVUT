#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define INITIAL_STR_SIZE 10  // Initial size for strings
#define min(x, y) ((x) < (y) ? (x) : (y))  // Macro to find minimum of two values

// Function declarations
char *read_line();

int length_check(char *str1, char *str2, bool optional_mode);

char *shift_string(char *str, int shift_amount_delta);

int count_matching_chars(const char *str1, const char *str2);

int levenshtein_distance(const char *str1, const char *str2);

// Main function
int main(int argc, char *argv[]) {
    // Check if optional mode is activated
    bool optional_mode = argc > 1 && strcmp(argv[1], "-prp-optional") == 0;

    // Read input strings
    char *encoded_str = read_line();
    char *overheard_str = read_line();
    length_check(encoded_str, overheard_str, optional_mode);

    // Initialize variables for finding the best shift
    int best_shift = 0;
    int min_distance = INT_MAX;

    // Iterate through possible shifts
    for (int i = 0; i < 52; ++i) {
        char *shifted_str = shift_string(encoded_str, i);
        int distance = optional_mode ?
                       levenshtein_distance(shifted_str, overheard_str) :
                       (strlen(shifted_str) - count_matching_chars(shifted_str, overheard_str));
        // Update the best shift if a smaller distance is found
        if (distance < min_distance) {
            min_distance = distance;
            best_shift = i;
        }
        free(shifted_str);
    }

    // Output the result
    char *result_str = shift_string(encoded_str, best_shift);
    printf("%s\n", result_str);

    // Free allocated memory
    free(result_str);
    free(encoded_str);
    free(overheard_str);
    return 0;
}

// Function to calculate Levenshtein distance
int levenshtein_distance(const char *str1, const char *str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int *costs = calloc((len2 + 1), sizeof(int));

    // Initialize costs array
    for (int j = 0; j <= len2; j++) {
        costs[j] = j;
    }

    // Calculate distances
    for (int i = 1; i <= len1; i++) {
        costs[0] = i;
        int nw = i - 1;
        for (int j = 1; j <= len2; j++) {
            int cj = min(1 + min(costs[j], costs[j - 1]), str1[i - 1] == str2[j - 1] ? nw : nw + 1);
            nw = costs[j];
            costs[j] = cj;
        }
    }

    // Final distance
    int distance = costs[len2];
    free(costs);
    return distance;
}

// Function to read a line of text
char *read_line() {
    int capacity = INITIAL_STR_SIZE;
    char *str = malloc(capacity);
    int c, len = 0;
    // Read characters until newline or EOF
    while ((c = getchar()) != '\n' && c != EOF) {
        // Check for valid character range
        if (!(('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))) {
            fprintf(stderr, "Error: Chybny vstup!\n");
            exit(100);
        }
        // Resize string if necessary
        if (len == capacity - 1) {
            capacity *= 2;
            str = realloc(str, capacity);
        }
        str[len++] = c;
    }
    str[len] = '\0';
    return str;
}

// Function to check the length of input strings
int length_check(char *str1, char *str2, bool optional_mode) {
    // Skip check in optional mode
    if (optional_mode) {
        return 0;
    }
    // Check if lengths are equal
    int length_str1 = strlen(str1);
    int length_str2 = strlen(str2);
    if (length_str1 != length_str2) {
        fprintf(stderr, "Error: Chybna delka vstupu!\n");
        exit(101);
    }
    return 0;
}

// Function to apply Caesar cipher shift to a string
char *shift_string(char *str, int shift_amount_delta) {
    int str_length = strlen(str) + 1;
    char *str_shifted = malloc(str_length);
    for (int i = 0; str[i] != '\0'; ++i) {
        int changed_char = str[i];
        // Check if character is upper or lowercase letter
        bool is_large_sign = changed_char >= 'A' && changed_char <= 'Z';
        bool is_small_sign = changed_char >= 'a' && changed_char <= 'z';

        // Apply shift
        int k = 0;
        while (shift_amount_delta > k) {
            if (is_large_sign || is_small_sign) {
                changed_char += 1;
                // Wrap around the alphabet
                changed_char = (changed_char == 'Z' + 1) ? 'a' :
                               (changed_char == 'z' + 1) ? 'A' :
                               changed_char;
            }
            k += 1;
        }
        str_shifted[i] = changed_char;
    }
    str_shifted[str_length - 1] = '\0';
    return str_shifted;
}

// Function to count matching characters in two strings
int count_matching_chars(const char *str1, const char *str2) {
    int count = 0;
    int length = strlen(str2);
    // Count matching characters
    for (int i = 0; i < length; i++) {
        if (str1[i] == str2[i]) {
            count++;
        }
    }
    return count;
}

