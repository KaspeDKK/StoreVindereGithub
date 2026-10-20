#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#include "functions.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    int listening_socket = create_listening_socket(port);

    while (1) {
        int client_socket = accept(listening_socket, NULL, NULL); // waits for the next client

        Request request = read_request(client_socket);
        uint64_t answer = find_answer(request);
        send_answer(client_socket, answer);

        printf("%lu\n", answer);
        close(client_socket);
    }

    return 0;
}
