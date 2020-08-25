#include <iostream>
#include <string>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cross_socket.h"

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        std::cout << argc << std::endl;
        return -1;
    }

    if (std::string("server").compare(std::string(argv[1])) == 0) {
        int i = 0;
        std::cout << "server!" << std::endl;
        ServerSocket server;
        if (server.Bind("127.0.0.1", 6543)) {
            std::cout << "bind error, check if the port 6543 is used!" << std::endl;
            return -1;
        }
        std::cout << "bind!" << std::endl;
        if (server.Listen(5)) {
            std::cout << "listen error" << std::endl;
            return -1;
        }
        std::cout << "listen!" << std::endl;

        while (i < 10) {

        if (server.Accept()) {
            std::cout << "accept error" << std::endl;
            return -1;
        }
        std::cout << "accept!" << std::endl;
        char buff_rcv[1025];
        if (server.Recv(buff_rcv) < 0) {
            std::cout << "recv error" << std::endl;
            server.CloseClient();
            server.Close();
            return -1;
        }
        std::cout << buff_rcv;
        std::cout << "World!" << std::endl;
        if (server.Send("World!") < 0) {
            std::cout << "send error" << std::endl;
            server.CloseClient();
            server.Close();
            return -1;
        }
        server.CloseClient();
        i++;

        }
        server.Close();
    }
    else {
        std::cout << "client!" << std::endl;
        ClientSocket client;
        if (client.Connect("127.0.0.1", 6543)) {
            std::cout << "connect error" << std::endl;
            return -1;
        }
        std::cout << "connect!" << std::endl;
        char buff_rcv[1025];
        std::cout << "Hello, ";
        if (client.Send("Hello, ") < 0) {
            std::cout << "send error" << std::endl;
            client.Close();
            return -1;
        }
        if (client.Recv(buff_rcv) < 0) {
            std::cout << "recv error" << std::endl;
            client.Close();
            return -1;
        }
        std::cout << buff_rcv;
        client.Close();
    }
}
