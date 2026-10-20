#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "functions.h"

#define LONESHA256_STATIC
#include "lonesha256.h"

int create_listening_socket(int port) {
    int listening_socket = socket(AF_INET, SOCK_STREAM, 0);

    int reuse_address = 1;
    setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof reuse_address); // lets us restart without waiting for the port

    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY); // any ip, required by the challenge
    address.sin_port = htons(port);

    bind(listening_socket, (struct sockaddr *)&address, sizeof address);
    listen(listening_socket, 128); // up to 128 clients can wait in line

    return listening_socket;
}

Request read_request(int client_socket) {
    uint8_t buffer[PACKET_REQUEST_SIZE];
    recv(client_socket, buffer, PACKET_REQUEST_SIZE, 0);

    /*
    0-31 inclusive is hash
    32-39 inclusive is start
    40-47 inclusive is end
    48 is prio
    */

    Request request;
    memcpy(request.hash, buffer + PACKET_REQUEST_HASH_OFFSET, SHA256_DIGEST_LENGTH);
    memcpy(&request.start, buffer + PACKET_REQUEST_START_OFFSET, sizeof request.start);
    memcpy(&request.end, buffer + PACKET_REQUEST_END_OFFSET, sizeof request.end);
    request.priority = buffer[PACKET_REQUEST_PRIO_OFFSET];

    request.start = be64toh(request.start); // big endian on the wire
    request.end = be64toh(request.end);

    return request;
}

uint64_t find_answer(Request request) {
    uint8_t candidate_hash[SHA256_DIGEST_LENGTH];

    for (uint64_t candidate = request.start; candidate < request.end; candidate++) {
        lonesha256(candidate_hash, (const unsigned char *)&candidate, sizeof candidate); // hashed as little endian, same as the client
        if (memcmp(candidate_hash, request.hash, SHA256_DIGEST_LENGTH) == 0) {
            return candidate;
        }
    }

    return 0; // not reached, the answer is always in the range
}

void send_answer(int client_socket, uint64_t answer) {
    uint64_t answer_big_endian = htobe64(answer);
    send(client_socket, &answer_big_endian, sizeof answer_big_endian, 0);
}
