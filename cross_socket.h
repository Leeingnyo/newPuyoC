#ifdef _WIN32

#else

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#endif

#define BUFF_SIZE 8192

enum class ErrorCode {NO_ERROR, CREATE_ERROR, BIND_ERROR, LISTEN_ERROR, ACCEPT_ERROR, CONNECT_ERROR, RECV_ERROR, SEND_ERROR};

class Socket {
protected:
    int my_socket;
    struct sockaddr_in my_address;

    int your_socket;
    struct sockaddr_in your_address;
    unsigned int your_address_size;

    ErrorCode error_code = ErrorCode::NO_ERROR;
public:
    Socket();
    bool IsValid();
    bool IsInvalid();
    virtual int Recv(char *buffer)=0;
    virtual int Send(char *buffer)=0;
    virtual std::string Recv()=0;
    virtual int Send(std::string data)=0;
    virtual void Close();
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
