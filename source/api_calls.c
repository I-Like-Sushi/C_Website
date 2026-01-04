#include "../include/api_calls.h"

#include <stdio.h>
#include <string.h>

size_t write_callback(void *data, size_t size, size_t nmemb, void *userdata) {
    size_t total_bytes = size * nmemb;
    strncat(userdata, data, total_bytes);
    return total_bytes;
}