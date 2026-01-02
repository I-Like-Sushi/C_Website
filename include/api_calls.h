#ifndef API_CALLS_H
#define API_CALLS_H

#include <stddef.h>

size_t write_callback(void *data, size_t size, size_t nmemb, void *userdata);

size_t got_data(char *buffer, size_t item_size, size_t n_items, void *ignore_this);

#endif
