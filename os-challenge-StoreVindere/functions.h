#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdint.h>
#include "messages.h"

typedef struct {
    uint8_t hash[SHA256_DIGEST_LENGTH];
    uint64_t start;
    uint64_t end;
    uint8_t priority;
} Request;

int create_listening_socket(int port);
Request read_request(int client_socket);
uint64_t find_answer(Request request);
void send_answer(int client_socket, uint64_t answer);

#endif
