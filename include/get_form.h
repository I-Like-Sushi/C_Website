#ifndef GET_FORM_H
#define GET_FORM_H

#include <Winsock2.h>

void send_txt_file(SOCKET Client, const char *file, const char *type_of_file);

#endif