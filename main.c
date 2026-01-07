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
#include <cjson/cJSON.h>

#include "./include/get_form.h"
#include "./include/edit_text_html.h"
#include "./include/post_form.h"
#include "./include/api_calls.h"

#pragma comment(lib, "WS2_32.lib")

///////////////////// Make everything one page /////////////////////

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


    /////////////////////

    CURL *curl;
    CURLcode curl_result;

    curl = curl_easy_init();

    if (curl == NULL) {
        fprintf(stderr, "HTTP request failed\n");
        return -1;
    }

    char response[8192] = {0};

    curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");

    /////////////////
    /// Todo: Get current weather at location of user.
    /////////////////

    curl_easy_setopt(curl, CURLOPT_URL,
        "https://api.open-meteo.com/v1/forecast?latitude=52.37&longitude=4.89&current_weather=true");

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    curl_result = curl_easy_perform(curl);

    if (curl_result != CURLE_OK) {
        fprintf(stderr, "Error: %s\n", curl_easy_strerror(curl_result));
        return -1;
    }

    printf("API Response:\n%s\n", response);

    curl_easy_cleanup(curl);

    cJSON *json = cJSON_Parse(response);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("Error: %s\n", error_ptr);
        }
        cJSON_Delete(json);
    }

    cJSON *current_weather = cJSON_GetObjectItemCaseSensitive(json, "current_weather");

    if (!cJSON_IsObject(current_weather)) {
        printf("Error: current_weather not found or not an object\n");
        cJSON_Delete(json);
    }

    cJSON *temperature = cJSON_GetObjectItemCaseSensitive(current_weather, "temperature");
    if (cJSON_IsNumber(temperature)) {
        printf("Current Temperature: %.2f°C\n", temperature->valuedouble);
    } else {
        printf("Error: temperature is missing or not a number\n");
    }

    cJSON_Delete(json);


    /////////////////////

    // struct sockaddr_in sa = {0};
    // socklen_t socklen =



    /////////////////////



    while (1) {
        SOCKET Client = accept(Server, NULL, NULL);
        char buffer[4096];
        int bytes = recv(Client, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
        } else {
            perror("recv failed");
        }

        char temp_str[15];

        snprintf(temp_str, sizeof(temp_str), "%.2f°C", temperature->valuedouble);

        // GET
        if (strncmp(buffer, "GET /homepage", 13) == 0) {
            replace_in_file_to("..//index.html", "..//new_html.html", "{{name}}", "Your name");
            replace_in_file_to("..//new_html.html" ,"..//new_html.html", "{{weather}}", temp_str);
            send_txt_file(Client, "..//new_html.html", "html");
        } else if (strncmp(buffer, "GET /index.css", 14) == 0) {
            send_txt_file(Client, "..//index.css", "css");
        } else if (strncmp(buffer, "GET /source/img/The_C_Programming_Language_logo.svg", 51) == 0) {
            send_image_file(Client, "..//source//img//The_C_Programming_Language_logo.svg", "svg+xml");
        }


        else if (strncmp(buffer, "GET /profilePage.css", 20) == 0) {
            send_txt_file(Client, "..//route//profilePage//profilePage.css", "css");
        } else if (strncmp(buffer, "GET /profilePage", 16) == 0) {
            send_txt_file(Client, "..//route//profilePage//profilePage.html", "html");
        } else if (strncmp(buffer, "GET /contact.css", 16) == 0) {
            send_txt_file(Client, "..//route//contact//contact.css", "css");
        } else if (strncmp(buffer, "GET /contact-page", 17) == 0) {
            send_txt_file(Client, "..//route//contact//contact.html", "html");
        } else if (strncmp(buffer, "GET /weather.css", 16) == 0) {
            send_txt_file(Client, "..//route//weather//weather.css", "css");
        } else if (strncmp(buffer, "GET /weather", 12) == 0) {
            send_txt_file(Client, "..//route//weather//weather.html", "html");
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
            }

            // replace_html_text(Client, "..//route//profilePage//profilePage.html", "html", "{{name}}", first_name);

            closesocket(Client);

        }

        else if (strncmp(buffer, "GET /errorPage.css", 18) == 0) {
            send_txt_file(Client, "..//route//errorPage//errorPage.css", "css");
        } else {
            send_txt_file(Client, "..//route//errorPage//errorPage.html", "html");
        }

    }




    closesocket(Server);
    WSACleanup();




    return 0;
}