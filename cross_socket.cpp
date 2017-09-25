#ifdef _WIN32



#else

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>

#include "cross_socket.h"

Socket::Socket() {
    my_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (my_socket == -1) {
        error_code = ErrorCode::CREATE_ERROR;
        return;
    }
}

inline bool Socket::IsValid() {
    return error_code == ErrorCode::NO_ERROR;
}
inline bool Socket::IsInvalid() {
    return error_code != ErrorCode::NO_ERROR;
}
void Socket::Close() {
    close(my_socket);
}

int ServerSocket::Bind(std::string ip_address, int port) {
    if (IsInvalid()) return -2;
    memset(&my_address, 0, sizeof(my_address));
    my_address.sin_family = AF_INET;
    my_address.sin_port = htons(port);
    my_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(my_socket, (struct sockaddr*)&my_address, sizeof(my_address)) == -1) {
        error_code = ErrorCode::BIND_ERROR;
        return -1;
    }
    return 0;
}
int ServerSocket::Listen(int backlog) {
    if (IsInvalid()) return -2;
    if (listen(my_socket, backlog) == -1) {
        error_code = ErrorCode::LISTEN_ERROR;
        return -1;
    }
    return 0;
}
int ServerSocket::Accept() {
    your_socket = accept(my_socket, (struct sockaddr*)&your_address, &your_address_size);
    if (your_socket == -1) {
        error_code = ErrorCode::ACCEPT_ERROR;
        return -1;
    }
    return 0;
}
int ServerSocket::Recv(char *buffer) {
    return recv(your_socket, buffer, BUFF_SIZE, 0);
}
int ServerSocket::Send(char *buffer) {
    return send(your_socket, buffer, strlen(buffer) + 1, 0); // with NULL
}

void ServerSocket::Close() {
    CloseClient();
    close(my_socket);
}
void ServerSocket::CloseClient() {
    close(your_socket);
}

int ClientSocket::Connect(std::string ip_address, int port) {
    memset(&your_address, 0, sizeof(your_address));
    your_address.sin_family = AF_INET;
    your_address.sin_port = htons(port);
    your_address.sin_addr.s_addr = inet_addr(ip_address.c_str());

    if (connect(my_socket, (struct sockaddr*)&your_address, sizeof(your_address)) == -1) {
        error_code = ErrorCode::CONNECT_ERROR;
        return -1;
    }
    return 0;
}
int ClientSocket::Recv(char *buffer) {
    return recv(my_socket, buffer, BUFF_SIZE, 0);
}
int ClientSocket::Send(char *buffer) {
    return send(my_socket, buffer, strlen(buffer) + 1, 0); // with NULL
}

#endif
