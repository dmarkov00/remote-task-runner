#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <string>

class Client {

public:
    Client();

    bool Connect();

    bool Send(std::string message);

    bool SendFile(std::string fileName);

    void Receive(std::string &data);

    void WriteFile(std::string content);

    bool Close();

private:
    int socketFd;
    sockaddr_in sa{};

private:

    void Socket();

    void SetupSocketData();

};


#endif
