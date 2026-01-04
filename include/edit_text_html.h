#ifndef EDIT_TEXT_HTML
#define EDIT_TEXT_HTML

#include <winsock2.h>
#include <stdio.h>

void replace_in_file_to(const char *template_path,
                        const char *output_path,
                        const char *placeholder,
                        const char *replacement);

#endif