#include <stdlib.h>
#include <stdio.h>

#include "io.h"

#define DEFAULT_SIZE 100

/**************************** INTERFACE FUNCTIONS ****************************/
io_error_t read_file(char* file_name, char** content) {
    *content = (char*)calloc(DEFAULT_SIZE, sizeof(char));
    int content_size = DEFAULT_SIZE;

    FILE* file = fopen(file_name, "r");

    if (file == NULL) {
        return FILE_READ_NOT_FOUND;
    }

    int symb = fgetc(file);
    int ind = 0;

    while (symb != EOF) {
        if (ind >= content_size - 1) {
            content_size *= 2;
            *content = (char*)realloc(*content, content_size * sizeof(char));
        }
        *(*content+ind) = (char)symb;
        symb = fgetc(file);
        ind++;
    }

    return FILE_READ_SUCCESS;
}
