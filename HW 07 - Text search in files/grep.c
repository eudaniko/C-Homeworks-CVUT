#include <stdio.h>

#define success 0
#define failed 1
#define RED "\033[31m"
#define RESET "\033[0m"
#define string_size 1024

int string_length(const char *str);

void print_string(const char *str);

int check_highlighted_option();

void print_highlighted(const char *str, const char *search, int search_len);

int search_string(char *search, FILE *file, int regular_expression, int highlited);

int main(int argc, char *argv[]) {
    FILE *file = NULL;
    int regular_expression = 0;
    int highlited = 0;
    int file_provided = 1;
    char *search = NULL;
    int search_result;

    if (argc >= 4) {
        regular_expression = (argv[1][0] == '-' && argv[1][1] == 'E');
        highlited = check_highlighted_option(argv, 1);
        if (argc == 5 && regular_expression && highlited) {
            search = argv[3];
            file = fopen(argv[4], "r");
        } else if (argc == 4 && highlited) {
            search = argv[2];
            file = fopen(argv[3], "r");
        } else if (argc == 4 && regular_expression) {
            search = argv[2];
            file = fopen(argv[3], "r");
        }

    } else if (argc == 3) {
        search = argv[1];
        file = fopen(argv[2], "r");
    } else if (argc == 2) {
        file = stdin;
        search = argv[1];
        file_provided = 0;
    } else {
        return 1;
    }

    if (file == NULL) {
        return 1;
    }

    if (search != NULL) {
        search_result = search_string(search, file, regular_expression, highlited);
    } else {
        search_result = failed;
    }

    if (file_provided && file != NULL) fclose(file);
    return search_result;
}


int string_length(const char *str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

void print_string(const char *str) {
    for (size_t i = 0; str[i] != '\n'; i++) {
        printf("%c", str[i]);
    }
    printf("\n");

}

int check_highlighted_option(char *argv[], int position) {
    return (argv[position][0] == '-' && argv[position][1] == '-' &&
            argv[position][2] == 'c' && argv[position][3] == 'o' &&
            argv[position][4] == 'l' && argv[position][5] == 'o' &&
            argv[position][6] == 'r' && argv[position][7] == '=' &&
            argv[position][8] == 'a' && argv[position][9] == 'l' &&
            argv[position][10] == 'w' && argv[position][11] == 'a' &&
            argv[position][12] == 'y' && argv[position][13] == 's');
}

void print_highlighted(const char *str, const char *search, int search_len) {
    int i = 0;
    while (str[i] != '\0') {
        int match = 1;
        for (int j = 0; j < search_len && str[i + j] != '\0'; ++j) {
            if (str[i + j] != search[j]) {
                match = 0;
                break;
            }
        }
        if (match) {
            printf(RED);
            for (int j = 0; j < search_len; ++j) {
                printf("%c", str[i + j]);
            }
            printf(RESET);
            i += search_len;
        } else {
            printf("%c", str[i]);
            ++i;
        }
    }
}

int search_string(char *search, FILE *file, int regular_expression, int highlited) {
    int search_result = failed;
    int search_len = string_length(search);
    char line[string_size];
    int match = 0;
    int regular_case_1 = 0;
    int regular_case_2 = 0;

    if (regular_expression) {
        while (fgets(line, sizeof(line), file)) {
            for (size_t i = 0; line[i] != '\n'; i++) {
                regular_case_1 = (search[match + 1] == '?' && line[i] == search[match + 2]);
                regular_case_2 = (search[match] == '?' && line[i] == search[match + 1]);
                if (line[i] == search[match] || regular_case_2 || regular_case_1) {
                    if (regular_case_1) match += 3;
                    else if (regular_case_2) match += 2;
                    else match++;
                    if (match == search_len) {
                        if (highlited) print_highlighted(line, search, search_len);
                        else print_string(line);
                        search_result = success;
                        match = 0;
                        break;
                    }
                } else if (search[match] == '+' && line[i] == search[match + 1]) {
                    match += 2;
                    if (match == search_len) {
                        if (highlited) print_highlighted(line, search, search_len);
                        else print_string(line);
                        search_result = success;
                        match = 0;
                        break;
                    }
                } else if (search[match] == '+' && line[i] != search[match + 1]) {
                    int distance = 0;
                    while (search[i - 1] == line[match + 1]) {
                        match++;
                        distance++;
                    }
                    if (line[match + 1] != search[i + 1]) {
                        match = 0;
                        continue;
                    }
                    i += distance;

                    if (match <= search_len) {
                        if (highlited) print_highlighted(line, search, search_len);
                        else print_string(line);
                        search_result = success;
                        match = 0;
                        break;
                    }
                } else if (search[match] == '*' && line[i] == search[match + 1]) {
                    match += 2;
                    if (match == search_len) {
                        if (highlited) print_highlighted(line, search, search_len);
                        else print_string(line);
                        search_result = success;
                        match = 0;
                        break;
                    }
                } else if (search[match] == '*' && line[i] != search[match + 1]) {
                    int distance = 0;
                    while (search[i - 1] == line[match + 1]) {
                        match++;
                        distance++;
                    }
                    if (line[match + 1] != search[i + 1]) {
                        match = 0;
                        continue;
                    }
                    i += distance;

                    if (match <= search_len) {
                        if (highlited) print_highlighted(line, search, search_len);
                        else print_string(line);
                        search_result = success;
                        match = 0;
                        break;
                    }
                } else if (line[i] == search[match] || (search[match + 1] == '*' && line[i] == search[match + 2])) {
                    if ((search[match + 1] == '*' && line[i] == search[match + 2])) match += 3;
                    else match++;
                    if (match == search_len) {
                        if (highlited) print_highlighted(line, search, search_len);
                        else print_string(line);
                        search_result = success;
                        match = 0;
                        break;
                    }
                } else {
                    match = 0;
                }
            }
        }
    } else {
        while (fgets(line, sizeof(line), file)) {
            for (size_t i = 0; line[i] != '\n'; i++) {
                if (line[i] == search[match]) {
                    match++;
                    if (match == search_len) {
                        if (highlited) {
                            print_highlighted(line, search, search_len);
                        } else {
                            print_string(line);
                        }
                        search_result = success;
                        match = 0;
                        break;
                    }
                } else {
                    match = 0;
                }
            }
        }
    }


    return search_result;
}
