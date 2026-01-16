#include "../include/edit_text_html.h"
#include "../include/get_form.h"

#include <stdio.h>




#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void replace_in_file_to(const char *template_path, const char *output_path,
                        const char *placeholder, const char *replacement)
{

    FILE *fp = fopen(template_path, "rb");
    if (!fp) {
        perror("fopen (read)");
        return;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        perror("fseek");
        fclose(fp);
        return;
    }
    long size = ftell(fp);
    if (size < 0) {
        perror("ftell");
        fclose(fp);
        return;
    }
    rewind(fp);

    char *buffer = malloc((size_t)size + 1);
    if (!buffer) {
        fclose(fp);
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    size_t read_bytes = fread(buffer, 1, (size_t)size, fp);
    buffer[read_bytes] = '\0';
    fclose(fp);

    char *pos = strstr(buffer, placeholder);
    if (!pos) {
        FILE *out = fopen(output_path, "wb");
        if (!out) {
            perror("fopen (write)");
            free(buffer);
            return;
        }
        fwrite(buffer, 1, read_bytes, out);
        fclose(out);
        free(buffer);
        return;
    }

    size_t before_len      = (size_t)(pos - buffer);
    size_t placeholder_len = strlen(placeholder);
    size_t replacement_len = strlen(replacement);
    size_t after_len       = read_bytes - before_len - placeholder_len;

    size_t new_size = before_len + replacement_len + after_len;

    char *new_buffer = malloc(new_size + 1);
    if (!new_buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        free(buffer);
        return;
    }

    memcpy(new_buffer, buffer, before_len);
    memcpy(new_buffer + before_len, replacement, replacement_len);
    memcpy(new_buffer + before_len + replacement_len,
           pos + placeholder_len,
           after_len);

    new_buffer[new_size] = '\0';

    free(buffer);

    FILE *out = fopen(output_path, "wb");
    if (!out) {
        perror("fopen (write)");
        free(new_buffer);
        return;
    }

    fwrite(new_buffer, 1, new_size, out);
    fclose(out);
    free(new_buffer);
}


