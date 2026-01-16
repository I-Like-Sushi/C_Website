#include "../include/post_form.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char *return_value_after_amp(char *old_value) {
    char *amp = strchr(old_value, '&');
    if (amp) {
        *amp = '\0';        // terminate the first value
        return amp + 1;     // return pointer to rest of body
    }
    return NULL;            // no '&' found
}

size_t parse_form(char *body, Key_value *key_value, size_t max_fields) {
    size_t count = 0;

    char *pair = strtok(body, "&");
    while (pair && count < max_fields) {
        char *equal_char = strchr(pair, '=');
        if (equal_char) {
            *equal_char = '\0';
            key_value[count].key = pair;
            key_value[count].value = equal_char + 1;
            key_value[count].element = count;
            count++;
        }
        pair = strtok(NULL, "&");
    }

    return count;

}
