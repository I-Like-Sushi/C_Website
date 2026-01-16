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
#include <curl/curl.h>
#include <cJSON.h>

#include "./include/get_form.h"
#include "./include/edit_text_html.h"
#include "./include/post_form.h"
#include "./include/api_calls.h"

int main(void) {

    int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server);
        return 1;
    }

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    if (bind(server, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        close(server);
        return 1;
    }

    if (listen(server, SOMAXCONN) < 0) {
        perror("listen");
        close(server);
        return 1;
    }

    printf("Server is running and listening on 127.0.0.1:8080\n");

    CURL *curl = curl_easy_init();
    if (!curl) return -1;

    char response[8192] = {0};

    curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");
    curl_easy_setopt(curl, CURLOPT_URL,
        "https://api.open-meteo.com/v1/forecast?latitude=52.37&longitude=4.89&current_weather=true");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    if (curl_easy_perform(curl) != CURLE_OK) {
        curl_easy_cleanup(curl);
        return -1;
    }

    printf("Test");


    curl_easy_cleanup(curl);

    cJSON *json = cJSON_Parse(response);
    cJSON *current_weather = cJSON_GetObjectItem(json, "current_weather");
    cJSON *temperature = cJSON_GetObjectItem(current_weather, "temperature");

    while (1) {
        int client = accept(server, NULL, NULL);
        if (client < 0) continue;

        char buffer[4096];
        int bytes = recv(client, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            close(client);
            continue;
        }
        buffer[bytes] = '\0';

        char temp_str[16];
        snprintf(temp_str, sizeof(temp_str), "%.2f°C", temperature->valuedouble);

        if (strncmp(buffer, "GET /homepage", 13) == 0) {
            replace_in_file_to("../index.html", "../new_html.html", "{{name}}", "Your name");
            replace_in_file_to("../new_html.html", "../new_html.html", "{{weather}}", temp_str);
            send_txt_file(client, "../new_html.html", "html");
        }
        else if (strncmp(buffer, "GET /index.css", 14) == 0) {
            send_txt_file(client, "../index.css", "css");
        } else if (strncmp(buffer, "GET /c_socket_diagram_dark.svg", 30) == 0) {
            send_image_file(client, "..//source//img//C_socket_diagram_dark.svg", "svg+xml");
        }












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
            }

            // replace_html_text(Client, "..//route//profilePage//profilePage.html", "html", "{{name}}", first_name);

            close(client);

        }
        else {
            send_txt_file(client, "../route/errorPage/errorPage.html", "html");
        }
    }

    close(server);
    return 0;
}









