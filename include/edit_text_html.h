#ifndef EDIT_TEXT_HTML
#define EDIT_TEXT_HTML

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

void replace_in_file_to(const char *template_path,
                        const char *output_path,
                        const char *placeholder,
                        const char *replacement);

#endif