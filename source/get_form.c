#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <libpq-fe.h>

#include "../include/get_form.h"
#include "../include/edit_text_html.h"

void send_txt_file(int Client, const char *file_name , const char *type_of_file) {
    FILE *file = fopen(file_name, "rb");
    if (!file) {
        fprintf(stderr, "Could not open %s.%s file\n", file_name, type_of_file);
        return;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("fseek");
        fclose(file);
        return;
    }
    long size = ftell(file);
    if (size < 0) {
        perror("ftell");
        fclose(file);
        return;
    }
    rewind(file);

    char *file_body = malloc((size_t)size + 1);
    if (!file_body) {
        fprintf(stderr, "malloc failed\n");
        fclose(file);
        return;
    }

    size_t read_bytes = fread(file_body, 1, (size_t)size, file);
    file_body[read_bytes] = '\0';
    fclose(file);

    char header[256];
    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/%s; charset=utf-8\r\n"
        "Content-Length: %zu\r\n"
        "\r\n",
        type_of_file, read_bytes);

    send(Client, header, header_len, 0);
    send(Client, file_body, (int)read_bytes, 0);

    free(file_body);
    close(Client);
}


void send_image_file(int Client, const char *file_name, const char *type_of_file) {
    FILE *file = fopen(file_name, "rb");
    if (!file) fprintf(stderr, "Image file %s could not be opened", file_name);
    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    char *file_body = malloc(filesize + 1);
    size_t read_bytes = fread(file_body, 1, filesize, file);
    file_body[read_bytes] = '\0';
    fclose(file);

    char header[256];
    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK \r\n"
        "Content-Type: image/%s \r\n"
        "Content-Length: %ld\r\n"
        "\r\n",
        type_of_file, filesize);
    printf("filesize: %ld\n", filesize);
    printf("img header: %s\n", header);

    send(Client, header, header_len, 0);
    send(Client, file_body, filesize, 0);
    free(file_body);
    close(Client);
}


















