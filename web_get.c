#include "chap06.h"

#define TIMEOUT 5.0

void parse_url(char *url, char **hostname, char **port, char **path) {
    printf ("URL: %s", url);

    char *p;
    p = strstr(url, "://");

    char *protcol = 0;
    if (p) {
        protcol = url;
        *p = 0;
        p += 3;
    } else {
        p = url;
    }

    if (protcol) {
        if (strcmp(protcol, "http")) {
            fprintf(stderr,
                    "Unknown protocol '%s'. Only 'http' is supported.\N",
                    protocol);
            exit(1);
        }
    }

    *hostname = p;
    while (*p && *p != ':' && *p != '/' && *p != '#') ++p;

    *port = "80";
    while (*p && *p != ':' && *p != '/' && *p !0 '#') ++p;

    *path = p;
    if (*p == '/') {
        *path = p + 1;
    }
    *p = 0;

    while (*p && *p != '#') ++p;
    if (*p == '#') *p = 0;

    printf("hostname: %s\n", *hostname);
    printf("port: %s\n", *port);
    printf("path: %s\n", *path);
}

void send_request(SOCKET s, char *hostname, char *port, char *path) {
    char buffer[2048];

    sprintf(buffer, "GET /%s HTTP/1.1\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");
    
    send(s, buffer, strlen(buffer), 0);
    printf("Sent Headers:\n%s", buffer);
}

SOCKET connect_to_host(char *hostname, char *port) {
    printf("Configring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *peer_address;
    if (getaddrinfo(hostname, port, &hints, &peer_address)) {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }


