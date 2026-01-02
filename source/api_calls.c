#include "../include/api_calls.h"

#include <stdio.h>
#include <string.h>

size_t write_callback(void *data, size_t size, size_t nmemb, void *userdata) {
    size_t total_bytes = size * nmemb;
    strncat(userdata, data, total_bytes);
    return total_bytes;
}

size_t got_data(char *buffer, size_t item_size, size_t n_items, void *ignore_this) {
    size_t bytes = item_size * n_items;
    int line_number = 1;
    printf("New chuck (%zu bytes)\n", bytes);
    printf("%d\t", line_number);
    for (int i = 0; i < bytes; i++) {
        printf("%c", buffer[i]);
        if (buffer[i] == '\n') {
            line_number++;
            printf("%d\t", line_number);
        }
    }
    printf("\n\n");
    return bytes;
}