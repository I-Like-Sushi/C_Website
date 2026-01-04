#ifndef GET_FORM_H
#define GET_FORM_H

#include <Winsock2.h>

void send_txt_file(SOCKET Client, const char *file, const char *type_of_file);

void send_image_file(SOCKET Client, const char *file, const char *type_of_file);

void send_index_html(SOCKET Client, const char *file, const char *type_of_file, int temperature, char *name);

#endif