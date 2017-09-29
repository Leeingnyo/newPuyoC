#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#else

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SOCKET int
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)

#endif

#define BUFF_SIZE 8192

enum class ErrorCode {NO_ERR, CREATE_ERROR, BIND_ERROR, LISTEN_ERROR, ACCEPT_ERROR, CONNECT_ERROR, RECV_ERROR, SEND_ERROR};

class Socket {
protected:
    SOCKET my_socket;
    
    struct sockaddr_in my_address;

    SOCKET your_socket;
    struct sockaddr_in your_address;
    unsigned int your_address_size;

    ErrorCode error_code = ErrorCode::NO_ERR;
public:
    Socket();
    bool IsValid();
    bool IsInvalid();
    virtual int Recv(char *buffer)=0;
    virtual int Send(char *buffer)=0;
    virtual std::string Recv()=0;
    virtual int Send(std::string data)=0;
    virtual void Close();

    static int StartUp();
    static int CleanUp();
};

class ServerSocket : public Socket {
public:
    int Bind(std::string ip_address, int port);
    int Listen(int backlog);
    int Accept();
    void CloseClient();
    virtual int Recv(char *buffer);
    virtual int Send(char *buffer);
    virtual std::string Recv();
    virtual int Send(std::string data);
    virtual void Close();
};

class ClientSocket : public Socket {
public:
    int Connect(std::string ip_address, int port);
    virtual int Recv(char *buffer);
    virtual int Send(char *buffer);
    virtual std::string Recv();
    virtual int Send(std::string data);
};
