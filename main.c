#ifndef UNICODE
#define UNICODE
#endif

#include <Winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include <curl/curl.h>

#include "./include/get_form.h"
#include "include/edit_text_html.h"
#include "include/post_form.h"

#pragma comment(lib, "WS2_32.lib")

int main(void) {

    WSADATA data;
    int result = WSAStartup(MAKEWORD(2, 2), &data);

    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
    }

    SOCKET Server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    int bind_result = bind(Server, (struct sockaddr*)&address, sizeof(address));

    if (bind_result != 0) {
        printf("bind_result failed: %d\n", bind_result);
    }

    listen(Server, SOMAXCONN);
    printf("Server is running and listening on 127.0.0.1:8080\n");


    ///////////////////////

    // CURL *curl;
    // CURLcode curl_result;
    //
    // curl = curl_easy_init();
    //
    // if (curl == NULL) {
    //     fprintf(stderr, "HTTP request failed\n");
    //     return -1;
    // }
    //
    // curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");
    // curl_easy_setopt(curl, CURLOPT_URL, "https://reddit.com/");
    //
    //
    // curl_result = curl_easy_perform(curl);
    //
    // if (curl_result != CURLE_OK) {
    //     fprintf(stderr, "Error: %s\n", curl_easy_strerror(curl_result));
    //     return -1;
    // }
    //
    // curl_easy_cleanup(curl);

    ///////////////////////



    while (1) {
        SOCKET Client = accept(Server, NULL, NULL);
        char buffer[4096];
        int bytes = recv(Client, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
        } else {
            perror("recv failed");
        }

        // GET
        if (strncmp(buffer, "GET /homepage", 13) == 0) {
            send_txt_file(Client, "..//index.html", "html");
        } else if (strncmp(buffer, "GET /index.css", 14) == 0) {
            send_txt_file(Client, "..//index.css", "css");
        } else if (strncmp(buffer, "GET /profilePage.css", 20) == 0) {
            send_txt_file(Client, "..//routes//profilePage//profilePage.css", "css");
        } else if (strncmp(buffer, "GET /profilePage", 16) == 0) {
            printf("Buffer: %s\n", buffer);
            send_txt_file(Client, "..//routes//profilePage//profilePage.html", "html");
        } else if (strncmp(buffer, "GET /contact.css", 16) == 0) {
            send_txt_file(Client, "..//routes//contact//contact.css", "css");
        } else if (strncmp(buffer, "GET /contact-page", 17) == 0) {
            send_txt_file(Client, "..//routes//contact//contact.html", "html");
        } else if (strncmp(buffer, "GET /weather.css", 16) == 0) {
            send_txt_file(Client, "..//routes//weather//weather.css", "css");
        } else if (strncmp(buffer, "GET /weather", 12) == 0) {
            send_txt_file(Client, "..//routes//weather//weather.html", "html");
        }






        // POST
        else if (strncmp(buffer, "POST /submit", 12) == 0) {
            char *body_start = strstr(buffer, "\r\n\r\n");

            char *first_name = NULL;
            char *last_name = NULL;

            if (body_start) {
                body_start += 4;

                Key_value form[2];
                size_t max_fields = sizeof(form) / sizeof(form[0]);
                size_t count = parse_form(body_start, form, max_fields);

                first_name = form[0].value;
                last_name = form[1].value;

                printf("first_name: %s\n", first_name);
                printf("last_name: %s\n", last_name);

                for (size_t i = 0; i < count; i++) {
                    printf("%s[%zu] = %s\n", form[i].key, form[i].element, form[i].value);
                }
            }

            replace_html_text(Client, "..//routes//profilePage//profilePage.html", "html", "{{name}}", first_name);

            closesocket(Client);

        }


        /// ToDo:
        ///  1. Get the data from the contact input and put it somewhere next to the form.
        ///  2. HTTP requests.


        else if (strncmp(buffer, "GET /errorPage.css", 18) == 0) {
            send_txt_file(Client, "..//routes//errorPage//errorPage.css", "css");
        } else {
            send_txt_file(Client, "..//routes//errorPage//errorPage.html", "html");
        }


        // POST
        if (buffer, "POST /submit") {

        }
    }




    closesocket(Server);
    WSACleanup();




    return 0;
}