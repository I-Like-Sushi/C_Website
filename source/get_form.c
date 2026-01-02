#include <WS2tcpip.h>
#include <Windows.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

#include "../include/get_form.h"

void send_txt_file(SOCKET Client, const char *file_name , const char *type_of_file) {
    FILE *file = fopen(file_name, "rb");
    if (!file) fprintf(stderr, "Could not open %s.%s file", file_name, type_of_file);
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
                                  "Content-Type: text/%s \r\n"
                                  "Content-Length: %ld \r\n"
                                  "\r\n",
                                  type_of_file, filesize);
    printf("filesize: %ld\n", filesize);

    send(Client, header, header_len, 0);
    send(Client, file_body, filesize, 0);
    free(file_body);
    closesocket(Client);
}

void send_image_file(SOCKET Client, const char *file_name, const char *type_of_file) {
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
    closesocket(Client);
}