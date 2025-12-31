#include "../include/edit_text_html.h"
#include "../include/get_form.h"

#include <stdio.h>

void replace_html_text(SOCKET Client, char *html_file, char *type_of_file, char *old_value, char *new_value) {

    FILE *file = fopen(html_file, "r");
    if (!file) fprintf(stderr, "Could not open file in replace_html_text.");
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(length);

    char n_buffer[4096];
    char buffer_complete[4096];

    char *buffer_beginning;
    char *buffer_end;

    if (buffer) {
        size_t bytes = fread(buffer, 1, length, file);
        buffer[bytes] = '\0';
        strcpy(n_buffer, buffer);
        buffer_beginning = strtok(n_buffer, "{");
        buffer_end = strstr(buffer, old_value);
        buffer_end += strlen(old_value);
        strcpy(buffer_complete, buffer_beginning);
        strcat(buffer_complete, new_value);
        strcat(buffer_complete, buffer_end);

        char header[256];
        int header_len = snprintf(header, sizeof(header),
                                      "HTTP/1.1 200 OK \r\n"
                                      "Content-Type: text/%s \r\n"
                                      "Content-Length: %zu \r\n"
                                      "\r\n",
                                      type_of_file, sizeof(buffer_complete));
        printf("filesize: %zu\n", sizeof(buffer_complete));

        send(Client, header, header_len, 0);
        send(Client, buffer_complete, sizeof(buffer_complete), 0);
    }

    fclose(file);
    free(buffer);
}