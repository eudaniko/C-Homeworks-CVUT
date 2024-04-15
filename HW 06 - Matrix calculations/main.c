#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 1000
#define MAX_NUMBERS 100
#define INPUT_ERROR 100

typedef struct {
    int rows;
    int columns;
    int **data;
} Matrix;


int read_numbers(const char *str, int *numbers, int max_numbers) {
    const char *current = str;
    char *end;
    int count = 0;

    while (*current != '\0' && count < max_numbers) {
        long int num = strtol(current, &end, 10);
        if (current == end) {
            break;
        }
        numbers[count++] = (int) num;
        current = end;
    }

    return count;
}

void get_row(char *input) {
    if (fgets(input, MAX_LENGTH, stdin) == NULL) {
        input[0] = '\0';
    } else {
        input[strcspn(input, "\n")] = '\0';
    }
}

Matrix get_matrix_size() {
    Matrix m;
    char input[MAX_LENGTH];
    int matrix_size[2];

    get_row(input);
    read_numbers(input, matrix_size, 2);

    m.rows = matrix_size[0];
    m.columns = matrix_size[1];
    return m;
}

void print_matrix(Matrix m) {
    printf("%d %d\n", m.rows, m.columns);
    for (int row = 0; row < m.rows; row++) {
        for (int column = 0; column < m.columns; column++) {
            if (column > 0) {
                printf(" ");
            }
            printf("%d", m.data[row][column]);
        }
        printf("\n");
    }
}


void input_error() {
    fprintf(stderr, "Error: Chybny vstup!\n");
    exit(INPUT_ERROR);
}

Matrix create_empty_matrix(int rows, int columns) {
    Matrix m;
    m.rows = rows;
    m.columns = columns;
    m.data = malloc(m.rows * sizeof(int *));
    for (int i = 0; i < m.rows; i++) {
        m.data[i] = malloc(m.columns * sizeof(int));
    }
    return m;
}

Matrix create_matrix() {
    Matrix m = get_matrix_size();
    m.data = malloc(m.rows * sizeof(int *));

    for (int r = 0; r < m.rows; r++) {
        m.data[r] = malloc(m.columns * sizeof(int));
        char input_row[MAX_LENGTH];
        int numbers_in_row[MAX_NUMBERS];

        get_row(input_row);
        int count = read_numbers(input_row, numbers_in_row, MAX_NUMBERS);

        if (count != m.columns) {
            input_error();
        }

        for (int c = 0; c < m.columns; c++) {
            m.data[r][c] = numbers_in_row[c];
        }
    }
    return m;
}

void free_matrix(Matrix m) {
    for (int i = 0; i < m.rows; i++) {
        free(m.data[i]);
    }
    free(m.data);
}

void add_matrices(Matrix a, Matrix b, Matrix result) {
    if (a.rows != b.rows || a.columns != b.columns) {
        input_error();
    }

    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.columns; j++) {
            result.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }
}

void subtract_matrices(Matrix a, Matrix b, Matrix result) {
    if (a.rows != b.rows || a.columns != b.columns) {
        input_error();
    }

    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.columns; j++) {
            result.data[i][j] = a.data[i][j] - b.data[i][j];
        }
    }
}

void multiply_matrices(Matrix a, Matrix b, Matrix result) {
    if (a.columns != b.rows) {
        input_error();
    }

    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < b.columns; j++) {
            result.data[i][j] = 0;
            for (int k = 0; k < a.columns; k++) {
                result.data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }
}

Matrix calculate(Matrix first_matrix, Matrix second_matrix, char operation) {
    Matrix result_matrix = create_empty_matrix(first_matrix.rows, first_matrix.columns);
    if (operation == '+' || operation == '-') {
        result_matrix = create_empty_matrix(first_matrix.rows, first_matrix.columns);
    } else if (operation == '*') {
        result_matrix = create_empty_matrix(first_matrix.rows, second_matrix.columns);
    } else {
        input_error();
    }

    if (operation == '+') {
        add_matrices(first_matrix, second_matrix, result_matrix);
    } else if (operation == '-') {
        subtract_matrices(first_matrix, second_matrix, result_matrix);
    } else if (operation == '*') {
        multiply_matrices(first_matrix, second_matrix, result_matrix);
    }
    return result_matrix;
}


int main() {
    int max_count = 4;
    Matrix *matrices = malloc(max_count * sizeof(Matrix));
    char *operations = malloc((max_count - 1) * sizeof(char));
    char input[MAX_NUMBERS];

    for (int i = 0; i < max_count; i++) {
        matrices[i] = create_matrix();
        if (i < max_count - 1) {
            fgets(input, MAX_NUMBERS, stdin);
            operations[i] = input[0];
            if (operations[i] == '\n' || operations[i] == EOF) {
                break;
            }
        }
    }

    for (int i = 0; i < max_count - 1;) {
        if (operations[i] == '*') {
            Matrix result = calculate(matrices[i], matrices[i + 1], '*');
            free_matrix(matrices[i]);
            free_matrix(matrices[i + 1]);
            matrices[i] = result;

            for (int j = i + 1; j < max_count - 1; j++) {
                matrices[j] = matrices[j + 1];
                operations[j - 1] = operations[j];
            }
            max_count--;
        } else {
            i++;
        }
    }

    Matrix result = matrices[0];
    for (int i = 1; i < max_count; i++) {
        if (operations[i - 1] == '+') {
            result = calculate(result, matrices[i], '+');
        } else if (operations[i - 1] == '-') {
            result = calculate(result, matrices[i], '-');
        }
    }

    print_matrix(result);

    for (int i = 0; i < max_count; i++) {
        free_matrix(matrices[i]);
    }
    free(matrices);
    free(operations);

    return 0;
}


