#ifndef POST_FORM_H
#define POST_FORM_H

#include <stddef.h>

typedef struct {
    char *key;
    char *value;
    size_t element;
} Key_value;

char *return_value_after_amp(char *old_value);

size_t parse_form(char *body, Key_value *key_value, size_t max_fields);

#endif