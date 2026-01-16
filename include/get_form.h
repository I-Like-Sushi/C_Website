#ifndef GET_FORM_H
#define GET_FORM_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void send_txt_file(int Client, const char *file, const char *type_of_file);

void send_image_file(int Client, const char *file, const char *type_of_file);

void send_index_html(int Client, const char *file, const char *type_of_file, int temperature, char *name);

#endif